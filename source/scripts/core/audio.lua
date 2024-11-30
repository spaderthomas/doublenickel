function doublenickel.audio.play(sound, loop)
  if not sound then return end

  if type(sound) == 'table' then
    local handle
    local current
    for index, file in pairs(sound) do
      if index == 1 then
        current = doublenickel.audio.play(file, false)
        handle = current
      elseif index == #sound then
        current = doublenickel.audio.play_after(current, file, true)
      else
        current = doublenickel.audio.play_after(current, file, false)
      end
    end

    return handle
  else
    -- So I can store things as empty strings and have them be editable in the GUI,
    -- but they don't try to use that empty string as an actual asset path
    if #sound == 0 then return end

    if loop then
      return doublenickel.ffi.dn_audio_play_looped(sound)
    else
      return doublenickel.ffi.dn_audio_play_sound(sound)
    end
  end
end

function doublenickel.audio.play_after(after_handle, ...)
  local handle = doublenickel.audio.play(...)
  doublenickel.ffi.dn_audio_queue(after_handle, handle);

  return handle
end

function doublenickel.audio.is_playing(handle)
  if not handle then return false end
  return doublenickel.ffi.dn_audio_is_playing(handle)
end

function doublenickel.audio.play_interpolated(sound, interpolator, loop)
  local handle = doublenickel.audio.play(sound, loop)
  if not handle then return end

  doublenickel.audio.interpolate(handle, interpolator)

  return handle
end

function doublenickel.audio.interpolate(handle, interpolator)
  if not handle then return end

  if doublenickel.audio.internal.interpolating[handle] then
    -- If we were already interpolating this sound, begin the new interpolation where the old one left
    -- off to keep a smooth transition
    interpolator.start = doublenickel.audio.internal.interpolating[handle]:get_value()
  elseif not interpolator.start then
    -- If it's unspecified, use the sound's current volume
    interpolator.start = 1
  end

  interpolator:reset()
  doublenickel.audio.set_volume(handle, interpolator:get_value())
  doublenickel.audio.internal.interpolating[handle] = interpolator
end

function doublenickel.audio.stop_after_interpolate(handle)
  if not handle then return end

  if doublenickel.audio.internal.interpolating[handle] then
    doublenickel.audio.internal.interpolating[handle].stop_after_interpolate = true
  end
end

function doublenickel.audio.stop(handle)
  if handle then doublenickel.ffi.dn_audio_stop(handle) end
end

function doublenickel.audio.stop_all()
  doublenickel.ffi.dn_audio_stop_all()
end

function doublenickel.audio.defer_stop(handle, time)
  doublenickel.audio.internal.deferred_stop[handle] = {
    target = time,
    accumulated = 0
  }
end

function doublenickel.audio.pause(handle)
  return doublenickel.ffi.dn_audio_pause(handle)
end

function doublenickel.audio.unpause(handle)
  return doublenickel.ffi.dn_audio_resume(handle)
end

function doublenickel.audio.set_volume(handle, volume)
  if handle then doublenickel.ffi.dn_audio_set_volume(handle, volume) end
end

function doublenickel.audio.dn_audio_set_master_volume(volume)
  doublenickel.ffi.dn_audio_set_master_volume(volume)
end

function doublenickel.audio.dn_audio_set_master_volume_mod(volume_mod)
  local interpolation = doublenickel.audio.internal.interpolation.master_volume_mod
  interpolation:set_target(volume_mod)
  interpolation:set_start(doublenickel.audio.get_master_volume_mod())
  interpolation:reset()
end

function doublenickel.audio.set_cutoff(handle, cutoff)
  if handle then doublenickel.ffi.dn_audio_set_filter_cutoff(handle, cutoff) end
end

function doublenickel.audio.dn_audio_set_master_filter_cutoff(cutoff)
  doublenickel.ffi.dn_audio_set_master_filter_cutoff(cutoff)
end

function doublenickel.audio.get_master_cutoff()
  return doublenickel.ffi.dn_audio_get_master_filter_cutoff()
end

function doublenickel.audio.get_master_volume()
  return doublenickel.ffi.dn_audio_get_master_volume()
end

function doublenickel.audio.get_master_volume_mod()
  return doublenickel.ffi.dn_audio_get_master_volume_mod()
end

function doublenickel.audio.enable()
  doublenickel.audio.dn_audio_set_master_volume_mod(1)
end

function doublenickel.audio.disable()
  doublenickel.audio.dn_audio_set_master_volume_mod(0)
end

---------------
-- INTERNALS --
---------------
function doublenickel.audio.init()
  doublenickel.dn_log('doublenickel.audio.init')
  
  doublenickel.audio.internal = {
    deferred_stop = {},
    interpolating = {},
    play_after = {},
    interpolation = {
      master_volume_mod = doublenickel.interpolation.EaseInOut:new({ start = 1, target = 1, time = 2, exponent = 3 })
    }
  }

  if not doublenickel.is_packaged_build then
    doublenickel.audio.disable()
  end

  doublenickel.audio.dn_audio_set_master_volume(3)
end

local function update_interpolation()
  local remove = {}
  for handle, interpolator in pairs(doublenickel.audio.internal.interpolating) do
    interpolator:update()
    doublenickel.audio.set_volume(handle, interpolator:get_value())

    if interpolator:is_done() then
      table.insert(remove, handle)

      if interpolator.stop_after_interpolate then
        doublenickel.audio.stop(handle)
      end
    end
  end

  for index, handle in pairs(remove) do
    doublenickel.audio.internal.interpolating[handle] = nil
  end

  local master_volume_mod = doublenickel.audio.internal.interpolation.master_volume_mod
  master_volume_mod:update()
  doublenickel.ffi.dn_audio_set_master_volume_mod(master_volume_mod:get_value())
end

local function update_deferred_stop()
  local remove = {}
  for handle, data in pairs(doublenickel.audio.internal.deferred_stop) do
    data.accumulated = data.accumulated + doublenickel.dt
    if data.accumulated >= data.target then
      doublenickel.audio.stop(handle)
      table.insert(remove, handle)
    end
  end

  for index, handle in pairs(remove) do
    doublenickel.audio.internal.deferred_stop[handle] = nil
  end
end

local function update_play_after()
  local remove = {}
  for handle, data in pairs(doublenickel.audio.internal.play_after) do
    if not doublenickel.audio.is_playing(data.after) then
      doublenickel.audio.unpause(handle)
      table.insert(remove, handle)
    end
  end

  for index, handle in pairs(remove) do
    doublenickel.audio.internal.play_after[handle] = nil
  end
end

function doublenickel.audio.update()
  update_interpolation()
  update_deferred_stop()
  update_play_after()
end

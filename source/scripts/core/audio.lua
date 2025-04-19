function dn.audio.play(sound, loop)
  if not sound then return end

  if type(sound) == 'table' then
    local handle
    local current
    for index, file in pairs(sound) do
      if index == 1 then
        current = dn.audio.play(file, false)
        handle = current
      elseif index == #sound then
        current = dn.audio.play_after(current, file, true)
      else
        current = dn.audio.play_after(current, file, false)
      end
    end

    return handle
  else
    -- So I can store things as empty strings and have them be editable in the GUI,
    -- but they don't try to use that empty string as an actual asset path
    if #sound == 0 then return end

    if loop then
      return dn.ffi.audio_play_looped(sound)
    else
      return dn.ffi.audio_play_sound(sound)
    end
  end
end

function dn.audio.play_after(after_handle, ...)
  local handle = dn.audio.play(...)
  dn.ffi.audio_queue(after_handle, handle);

  return handle
end

function dn.audio.is_playing(handle)
  if not handle then return false end
  return dn.ffi.audio_is_playing(handle)
end

function dn.audio.play_interpolated(sound, interpolator, loop)
  local handle = dn.audio.play(sound, loop)
  if not handle then return end

  dn.audio.interpolate(handle, interpolator)

  return handle
end

function dn.audio.interpolate(handle, interpolator)
  if not handle then return end

  if dn.audio.internal.interpolating[handle] then
    -- If we were already interpolating this sound, begin the new interpolation where the old one left
    -- off to keep a smooth transition
    interpolator.start = dn.audio.internal.interpolating[handle]:get_value()
  elseif not interpolator.start then
    -- If it's unspecified, use the sound's current volume
    interpolator.start = 1
  end

  interpolator:reset()
  dn.audio.set_volume(handle, interpolator:get_value())
  dn.audio.internal.interpolating[handle] = interpolator
end

function dn.audio.stop_after_interpolate(handle)
  if not handle then return end

  if dn.audio.internal.interpolating[handle] then
    dn.audio.internal.interpolating[handle].stop_after_interpolate = true
  end
end

function dn.audio.stop(handle)
  if handle then dn.ffi.audio_stop(handle) end
end

function dn.audio.stop_all()
  dn.ffi.audio_stop_all()
end

function dn.audio.defer_stop(handle, time)
  dn.audio.internal.deferred_stop[handle] = {
    target = time,
    accumulated = 0
  }
end

function dn.audio.pause(handle)
  return dn.ffi.audio_pause(handle)
end

function dn.audio.unpause(handle)
  return dn.ffi.audio_resume(handle)
end

function dn.audio.set_volume(handle, volume)
  if handle then dn.ffi.audio_set_volume(handle, volume) end
end

function dn.audio.dn_audio_set_master_volume(volume)
  dn.ffi.audio_set_master_volume(volume)
end

function dn.audio.dn_audio_set_master_volume_mod(volume_mod)
  local interpolation = dn.audio.internal.interpolation.master_volume_mod
  interpolation:set_target(volume_mod)
  interpolation:set_start(dn.audio.get_master_volume_mod())
  interpolation:reset()
end

function dn.audio.set_cutoff(handle, cutoff)
  if handle then dn.ffi.audio_set_filter_cutoff(handle, cutoff) end
end

function dn.audio.dn_audio_set_master_filter_cutoff(cutoff)
  dn.ffi.audio_set_master_filter_cutoff(cutoff)
end

function dn.audio.get_master_cutoff()
  return dn.ffi.audio_get_master_filter_cutoff()
end

function dn.audio.get_master_volume()
  return dn.ffi.audio_get_master_volume()
end

function dn.audio.get_master_volume_mod()
  return dn.ffi.audio_get_master_volume_mod()
end

function dn.audio.enable()
  dn.audio.dn_audio_set_master_volume_mod(1)
end

function dn.audio.disable()
  dn.audio.dn_audio_set_master_volume_mod(0)
end

---------------
-- INTERNALS --
---------------
function dn.audio.init()
  dn.ffi.log('dn.audio.init')
  
  dn.audio.internal = {
    deferred_stop = {},
    interpolating = {},
    play_after = {},
    interpolation = {
      master_volume_mod = dn.interpolation.EaseInOut:new({ start = 1, target = 1, time = 2, exponent = 3 })
    }
  }

  if not dn.is_packaged_build then
    dn.audio.disable()
  end

  dn.audio.dn_audio_set_master_volume(3)
end

local function update_interpolation()
  local remove = {}
  for handle, interpolator in pairs(dn.audio.internal.interpolating) do
    interpolator:update()
    dn.audio.set_volume(handle, interpolator:get_value())

    if interpolator:is_done() then
      table.insert(remove, handle)

      if interpolator.stop_after_interpolate then
        dn.audio.stop(handle)
      end
    end
  end

  for index, handle in pairs(remove) do
    dn.audio.internal.interpolating[handle] = nil
  end

  local master_volume_mod = dn.audio.internal.interpolation.master_volume_mod
  master_volume_mod:update()
  dn.ffi.audio_set_master_volume_mod(master_volume_mod:get_value())
end

local function update_deferred_stop()
  local remove = {}
  for handle, data in pairs(dn.audio.internal.deferred_stop) do
    data.accumulated = data.accumulated + dn.dt
    if data.accumulated >= data.target then
      dn.audio.stop(handle)
      table.insert(remove, handle)
    end
  end

  for index, handle in pairs(remove) do
    dn.audio.internal.deferred_stop[handle] = nil
  end
end

local function update_play_after()
  local remove = {}
  for handle, data in pairs(dn.audio.internal.play_after) do
    if not dn.audio.is_playing(data.after) then
      dn.audio.unpause(handle)
      table.insert(remove, handle)
    end
  end

  for index, handle in pairs(remove) do
    dn.audio.internal.play_after[handle] = nil
  end
end

function dn.audio.update()
  update_interpolation()
  update_deferred_stop()
  update_play_after()
end

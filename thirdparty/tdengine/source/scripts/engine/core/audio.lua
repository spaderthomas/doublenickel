function tdengine.audio.play(sound, loop)
  if not sound then return end

  if type(sound) == 'table' then
    local handle
    local current
    for index, file in pairs(sound) do
      if index == 1 then
        current = tdengine.audio.play(file, false)
        handle = current
      elseif index == #sound then
        current = tdengine.audio.play_after(current, file, true)
      else
        current = tdengine.audio.play_after(current, file, false)
      end
    end

    return handle
  else
    -- So I can store things as empty strings and have them be editable in the GUI,
    -- but they don't try to use that empty string as an actual asset path
    if #sound == 0 then return end

    if loop then
      return tdengine.ffi.play_sound_loop(sound)
    else
      return tdengine.ffi.play_sound(sound)
    end
  end
end

function tdengine.audio.play_after(after_handle, ...)
  local handle = tdengine.audio.play(...)
  tdengine.ffi.play_sound_after(after_handle, handle);

  return handle
end

function tdengine.audio.is_playing(handle)
  if not handle then return false end
  return tdengine.ffi.is_sound_playing(handle)
end

function tdengine.audio.play_interpolated(sound, interpolator, loop)
  local handle = tdengine.audio.play(sound, loop)
  if not handle then return end

  tdengine.audio.interpolate(handle, interpolator)

  return handle
end

function tdengine.audio.interpolate(handle, interpolator)
  if not handle then return end

  if tdengine.audio.internal.interpolating[handle] then
    -- If we were already interpolating this sound, begin the new interpolation where the old one left
    -- off to keep a smooth transition
    interpolator.start = tdengine.audio.internal.interpolating[handle]:get_value()
  elseif not interpolator.start then
    -- If it's unspecified, use the sound's current volume
    interpolator.start = 1
  end

  interpolator:reset()
  tdengine.audio.set_volume(handle, interpolator:get_value())
  tdengine.audio.internal.interpolating[handle] = interpolator
end

function tdengine.audio.stop_after_interpolate(handle)
  if not handle then return end

  if tdengine.audio.internal.interpolating[handle] then
    tdengine.audio.internal.interpolating[handle].stop_after_interpolate = true
  end
end

function tdengine.audio.stop(handle)
  if handle then tdengine.ffi.stop_sound(handle) end
end

function tdengine.audio.stop_all()
  tdengine.ffi.stop_all_sounds()
end

function tdengine.audio.defer_stop(handle, time)
  tdengine.audio.internal.deferred_stop[handle] = {
    target = time,
    accumulated = 0
  }
end

function tdengine.audio.pause(handle)
  return tdengine.ffi.pause_sound(handle)
end

function tdengine.audio.unpause(handle)
  return tdengine.ffi.unpause_sound(handle)
end

function tdengine.audio.set_volume(handle, volume)
  if handle then tdengine.ffi.set_volume(handle, volume) end
end

function tdengine.audio.set_master_volume(volume)
  tdengine.ffi.set_master_volume(volume)
end

function tdengine.audio.set_master_volume_mod(volume_mod)
  local interpolation = tdengine.audio.internal.interpolation.master_volume_mod
  interpolation:set_target(volume_mod)
  interpolation:set_start(tdengine.audio.get_master_volume_mod())
  interpolation:reset()
end

function tdengine.audio.set_cutoff(handle, cutoff)
  if handle then tdengine.ffi.set_cutoff(handle, cutoff) end
end

function tdengine.audio.set_master_cutoff(cutoff)
  tdengine.ffi.set_master_cutoff(cutoff)
end

function tdengine.audio.get_master_cutoff()
  return tdengine.ffi.get_master_cutoff()
end

function tdengine.audio.get_master_volume()
  return tdengine.ffi.get_master_volume()
end

function tdengine.audio.get_master_volume_mod()
  return tdengine.ffi.get_master_volume_mod()
end

function tdengine.audio.enable()
  tdengine.audio.set_master_volume_mod(1)
end

function tdengine.audio.disable()
  tdengine.audio.set_master_volume_mod(0)
end

---------------
-- INTERNALS --
---------------
function tdengine.audio.init()
  tdengine.audio.internal = {
    deferred_stop = {},
    interpolating = {},
    play_after = {},
    interpolation = {
      master_volume_mod = tdengine.interpolation.EaseInOut:new({ start = 1, target = 1, time = 2, exponent = 3 })
    }
  }

  if not tdengine.is_packaged_build then
    tdengine.audio.disable()
  end

  tdengine.audio.set_master_volume(3)
end

local function update_interpolation()
  local remove = {}
  for handle, interpolator in pairs(tdengine.audio.internal.interpolating) do
    interpolator:update()
    tdengine.audio.set_volume(handle, interpolator:get_value())

    if interpolator:is_done() then
      table.insert(remove, handle)

      if interpolator.stop_after_interpolate then
        tdengine.audio.stop(handle)
      end
    end
  end

  for index, handle in pairs(remove) do
    tdengine.audio.internal.interpolating[handle] = nil
  end

  local master_volume_mod = tdengine.audio.internal.interpolation.master_volume_mod
  master_volume_mod:update()
  tdengine.ffi.set_master_volume_mod(master_volume_mod:get_value())
end

local function update_deferred_stop()
  local remove = {}
  for handle, data in pairs(tdengine.audio.internal.deferred_stop) do
    data.accumulated = data.accumulated + tdengine.dt
    if data.accumulated >= data.target then
      tdengine.audio.stop(handle)
      table.insert(remove, handle)
    end
  end

  for index, handle in pairs(remove) do
    tdengine.audio.internal.deferred_stop[handle] = nil
  end
end

local function update_play_after()
  local remove = {}
  for handle, data in pairs(tdengine.audio.internal.play_after) do
    if not tdengine.audio.is_playing(data.after) then
      tdengine.audio.unpause(handle)
      table.insert(remove, handle)
    end
  end

  for index, handle in pairs(remove) do
    tdengine.audio.internal.play_after[handle] = nil
  end
end

function tdengine.audio.update()
  update_interpolation()
  update_deferred_stop()
  update_play_after()
end

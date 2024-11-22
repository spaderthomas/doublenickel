Camera = tdengine.entity.define('Camera')

Camera.components = {}

Camera.editor_fields = {
  'default_state'
}

Camera.states = tdengine.enum.define(
  'CameraState',
  {
    Idle = 0,
    Interpolate = 1,
    FollowPlayer = 2,
    InterpolateToPlayer = 3
  }
)

tdengine.enum.define(
  'CameraZoomState',
  {
    Idle = 0,
    Interpolate = 1,
  }
)

function Camera:init(params)
  self.offset = tdengine.vec2()
  self.noise_offset = tdengine.vec2()


  self.enabled = true
  self.default_state = tdengine.enum.load(params.default_state) or tdengine.enums.CameraState.Idle
  self.state = self.default_state

  self.zoom = 1.0
  self.zoom_state = tdengine.enums.CameraZoomState.Idle
end

function Camera:on_new_game()
  self.offset = tdengine.vec2()
  self.noise_offset = tdengine.vec2()
end

function Camera:on_main_menu()
  self.offset = tdengine.vec2(0, -1080)
  self.noise_offset = tdengine.vec2()
end

function Camera:update()
  if not self.enabled then return end

  self:update_offset()
  self:update_zoom()
end

function Camera:update_offset()
  if self.state == tdengine.enums.CameraState.Idle then

  elseif self.state == tdengine.enums.CameraState.Interpolate then
    self:update_interpolation()
  elseif self.state == tdengine.enums.CameraState.InterpolateToPlayer then
    -- self.interpolation.target = self:get_player_offset(2) -- @refactor everything player related
    -- self:clamp_to_bounds(self.interpolation.target)
    -- self:update_interpolation()
  elseif self.state == tdengine.enums.CameraState.FollowPlayer then
    -- self:move_to_player()
    -- self:clamp_to_bounds()
  end

  self:update_sway()

  local final_offset = self.offset:add(self.noise_offset)
  final_offset.x = math.floor(final_offset.x)
  final_offset.y = math.floor(final_offset.y)

  tdengine.ffi.set_camera(final_offset:unpack())
end

function Camera:update_interpolation()
  self.interpolation:update()
  self.offset:assign(self.interpolation:get_value())
  if self.interpolation:is_done() then
    if self.interpolation.stop_after_interpolate then
      self.state = tdengine.enums.CameraState.Idle
    else
      self.state = self.default_state
    end
    self.interpolation = nil
  end
end

function Camera:update_zoom()
  if self.zoom_state == tdengine.enums.CameraZoomState.Idle then

  elseif self.zoom_state == tdengine.enums.CameraZoomState.Interpolate then
    self.zoom_interp:update()
    self.zoom = self.zoom_interp:get_value()
    if self.zoom_interp:is_done() then
      self.zoom_state = tdengine.enums.CameraZoomState.Idle
      self.zoom_interp = nil
    end
  end

  --tdengine.ffi.set_zoom(self.zoom)
end

function Camera:update_sway()
  if self.perlin then
    local t = self.perlin.speed * tdengine.elapsed_time
    local amp_max = self.perlin.amplitude
    local amp_min = self.perlin.amplitude * -1
    local nx = tdengine.ffi.perlin(t, 0, amp_min, amp_max)
    local ny = tdengine.ffi.perlin(0, t, amp_min, amp_max)

    self.perlin.interpolation:update()
    local blend = self.perlin.interpolation:get_value()
    self.noise_offset.x = tdengine.math.lerp(0, nx, blend)
    self.noise_offset.y = tdengine.math.lerp(0, ny, blend)

    self.perlin.timer:update()
    if self.perlin.timer:is_done() then
      self.hiding = true
      self.perlin.interpolation:reverse()
      self.perlin.interpolation:reset()
    end

    if self.hiding and self.perlin.interpolation:is_done() then
      self.perlin = nil
    end
  end
end

function Camera:set(position)
  self.offset = position
end

function Camera:move(delta)
  if self:is_locked() then return end

  self.offset = self.offset:add(delta)
end

function Camera:move_to_player()
  --dbg.file()
  --log.warn('Camera:move_to_player() is unimplemented')
  -- if self:is_locked() then return end

  -- local player = tdengine.find_entity('Player')
  -- if not player then return end

  -- local collider = player:find_component('Collider')

  -- local position = collider:get_position()
  -- local nx, ny = tdengine.get_native_resolution()
  -- self.offset.x = position.x - nx / 2
  -- self.offset.y = position.y - ny / 2
end

function Camera:clamp_to_bounds(offset)
  offset = offset or self.offset
  -- If not interpolating, respect the camera bounds
  local bounds = tdengine.find_entity('CameraBounds')
  if bounds and tdengine.tick then
    if bounds.lock then
      offset:assign(bounds.lock_to)
    else
      offset.x = math.min(offset.x, bounds:right())
      offset.x = math.max(offset.x, bounds:left())

      offset.y = math.min(offset.y, bounds:top())
      offset.y = math.max(offset.y, bounds:bottom())
    end
  end
end

function Camera:is_locked()
  local bounds = tdengine.find_entity('CameraBounds')
  return bounds and bounds.lock
end

function Camera:interpolate_to(position, interpolation)
  self.state = tdengine.enums.CameraState.Interpolate

  self.interpolation = interpolation
  interpolation:set_start(self.offset)
  interpolation:set_target(position)
end

-- function Camera:interpolate_to_player(interpolation)
--   interpolation = interpolation or tdengine.interpolation.SmoothDamp2:new()
--   self:interpolate_to(self:get_player_offset(2), interpolation)
--   self.state = tdengine.enums.CameraState.InterpolateToPlayer
-- end

-- function Camera:interpolate_to_dialogue(interpolation)
--   interpolation = interpolation or tdengine.interpolation.SmoothDamp2:new()
--   self:interpolate_to(self:get_player_offset(3), interpolation)
-- end

function Camera:stop_after_interpolate()
  if self.interpolation then
    self.interpolation.stop_after_interpolate = true
  end
end

function Camera:begin_sway(amplitude, speed, time)
  time = time or 60 * 60 * 60 -- Functionally forever
  self.perlin = {
    amplitude = amplitude or 100,
    speed = speed or .1,
    timer = Timer:new(time),
    interpolation = tdengine.interpolation.SmoothDamp:new({
      start = 0,
      target = 1,
      time = 1,
      exponent = 2,
      velocity = .05

    }),
    hiding = false
  }
end

function Camera:end_sway()
  if not self.perlin then return end
  if self.perlin.hiding then return end

  self.perlin.hiding = true
  self.perlin.interpolation:reverse()
  self.perlin.interpolation:reset()
end

function Camera:zoom_to(value, interpolation)
  interpolation = interpolation or tdengine.interpolation.SmoothDamp:new()

  self.zoom_state = tdengine.enums.CameraZoomState.Interpolate
  self.zoom_interp = interpolation
  self.zoom_interp:set_start(self.zoom)
  self.zoom_interp:set_target(value)
  self.zoom_interp:reset()
end

function Camera:get_player_offset(n)
  -- n = n or 2 -- Centered? Or centered in the first 2/3?

  -- local player = tdengine.find_entity('Player'):find_component('Collider')
  -- local base_position = player:get_position()
  -- local nx, ny = tdengine.get_native_resolution();
  -- return tdengine.vec2(base_position.x - nx / n, base_position.y - ny / n)
end

function Camera:set_offset(offset)
  self.offset:assign(offset)
end

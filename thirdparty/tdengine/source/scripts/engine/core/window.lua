local self = tdengine.window

tdengine.window.states = tdengine.enum.define(
  'WindowAnimationState',
  {
    Idle = 0,
    Interpolate = 1
  }
)

function tdengine.window.init()
  self.state = self.states.Idle

  self.display_mode = tdengine.ffi.get_display_mode()
  self.interpolation = {
    window_size = tdengine.interpolation.EaseInOut2:new({ time = 1, exponent = 3 })
  }
end

function tdengine.window.update()
  if self.state == self.states.Idle then

  elseif self.state == self.states.Interpolate then
    local window_size = self.interpolation.window_size:get_value()
    tdengine.ffi.set_window_size(window_size.x, window_size.y)

    self.interpolation.window_size:update()
    if self.interpolation.window_size:is_done() then
      tdengine.ffi.set_display_mode(self.display_mode)
      self.state = self.states.Idle
    end
  end

  local display_mode = tdengine.ffi.get_display_mode()
  if display_mode == tdengine.enums.DisplayMode.p1280_800 then
    local content_area       = tdengine.ffi.get_content_area()
    local ratio              = content_area.x / content_area.y
    local sixteen_nine_width = content_area.y * 16 / 9

    local delta              = sixteen_nine_width - content_area.x
    local left               = 0 + (delta / 2)
    local right              = window.content_area.x - (delta / 2)
    tdengine.ffi.set_orthographic_projection(left, right, 0, 800, -1, 1)
  end
end

function tdengine.window.animate_display_mode(display_mode)
  local was_full_screen = self.display_mode == tdengine.enums.DisplayMode.Fullscreen
  local is_full_screen = display_mode == tdengine.enums.DisplayMode.Fullscreen
  if was_full_screen or is_full_screen or tdengine.ffi.is_steam_deck() then
    tdengine.ffi.set_display_mode(display_mode)
    self.display_mode = display_mode
    self.state = self.states.Idle
    return
  end

  self.display_mode = display_mode


  local current_size = tdengine.ffi.get_content_area()
  current_size = tdengine.vec2(current_size.x, current_size.y)
  local target_size = tdengine.vec2()

  if self.display_mode == tdengine.enums.DisplayMode.p480 then
    target_size.x = 854
    target_size.y = 480
  elseif self.display_mode == tdengine.enums.DisplayMode.p720 then
    target_size.x = 1280
    target_size.y = 720
  elseif self.display_mode == tdengine.enums.DisplayMode.p1280_800 then
    target_size.x = 1280
    target_size.y = 800
  elseif self.display_mode == tdengine.enums.DisplayMode.p1080 then
    target_size.x = 1920
    target_size.y = 1080
  elseif self.display_mode == tdengine.enums.DisplayMode.p1440 then
    target_size.x = 2560
    target_size.y = 1440
  elseif self.display_mode == tdengine.enums.DisplayMode.p2160 then
    target_size.x = 3840
    target_size.y = 2160
  end

  self.interpolation.window_size:set_start(current_size)
  self.interpolation.window_size:set_target(target_size)
  self.interpolation.window_size:reset()
  self.state = self.states.Interpolate
end

function tdengine.window.get_content_area()
  return tdengine.vec2(tdengine.ffi.get_content_area())
end

function tdengine.window.get_game_area_size()
  return tdengine.vec2(tdengine.ffi.get_game_area_size())
end

function tdengine.window.set_game_area_size(size)
  tdengine.ffi.set_game_area_size(size.x, size.y)
end

function tdengine.window.set_game_area_position(position)
  tdengine.ffi.set_game_area_position(position.x, position.y)
end

function tdengine.window.get_native_resolution()
	return tdengine.vec2(tdengine.ffi.get_native_resolution())
end
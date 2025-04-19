local self = dn.window

dn.window.states = dn.enum.define(
  'WindowAnimationState',
  {
    Idle = 0,
    Interpolate = 1
  }
)

function dn.window.init()
  dn.ffi.log('dn.window.init')
  self.state = self.states.Idle

  self.display_mode = dn.ffi.window_get_display_mode()
  self.interpolation = {
    window_size = dn.interpolation.EaseInOut2:new({ time = 1, exponent = 3 })
  }
end

function dn.window.update()
  if self.state == self.states.Idle then

  elseif self.state == self.states.Interpolate then
    local window_size = self.interpolation.window_size:get_value()
    dn.unported.set_window_size(window_size.x, window_size.y)

    self.interpolation.window_size:update()
    if self.interpolation.window_size:is_done() then
      dn.ffi.window_set_display_mode(self.display_mode)
      self.state = self.states.Idle
    end
  end
end

function dn.window.animate_display_mode(display_mode)
  local was_full_screen = self.display_mode == dn.enums.DisplayMode.Fullscreen
  local is_full_screen = display_mode == dn.enums.DisplayMode.Fullscreen
  if was_full_screen or is_full_screen or dn.ffi.steam_is_deck() then
    dn.ffi.window_set_display_mode(display_mode)
    self.display_mode = display_mode
    self.state = self.states.Idle
    return
  end

  self.display_mode = display_mode


  local current_size = dn.ffi.window_get_content_area()
  current_size = dn.vec2(current_size.x, current_size.y)
  local target_size = dn.vec2()

  if self.display_mode == dn.enums.DisplayMode.p480 then
    target_size.x = 854
    target_size.y = 480
  elseif self.display_mode == dn.enums.DisplayMode.p720 then
    target_size.x = 1280
    target_size.y = 720
  elseif self.display_mode == dn.enums.DisplayMode.p1280_800 then
    target_size.x = 1280
    target_size.y = 800
  elseif self.display_mode == dn.enums.DisplayMode.p1080 then
    target_size.x = 1920
    target_size.y = 1080
  elseif self.display_mode == dn.enums.DisplayMode.p1440 then
    target_size.x = 2560
    target_size.y = 1440
  elseif self.display_mode == dn.enums.DisplayMode.p2160 then
    target_size.x = 3840
    target_size.y = 2160
  end

  self.interpolation.window_size:set_start(current_size)
  self.interpolation.window_size:set_target(target_size)
  self.interpolation.window_size:reset()
  self.state = self.states.Interpolate
end


function dn.window.get_game_area_size()
  local data = dn.ffi.coord_get()
  return dn.vec2(data.framebuffer_size.x, data.framebuffer_size.y)
end

function dn.window.set_game_area_size(size)
  dn.ffi.coord_set_framebuffer_size(size.x, size.y)
end

function dn.window.set_game_area_position(position)
  dn.ffi.coord_set_framebuffer_position(position.x, position.y)
end

function dn.window.dn_window_get_native_resolution()
	return dn.vec2(dn.ffi.window_get_native_resolution())
end
Animation = tdengine.component.define('Animation')

Animation.editor_fields = {
  'default',
  'layer',
  'scale',
  'enabled',
  'offset',
  'size',
  'opacity',
  'static_image',
}

Animation.imgui_ignore = {
  queued_animations = true,
  current = true,
  name = true,
  accumulated = true,
}

function Animation:init(params)
  params = params or {}
  self.default = params.default or 'default'
  self.animation = self.default
  self.layer = params.layer or tdengine.layers.foreground
  self.scale = params.scale or 1
  self.opacity = params.opacity or 1
  self.enabled = tdengine.math.ternary(params.enabled, true, false)
  self.offset = tdengine.vec2(params.offset)
  self.center = tdengine.math.ternary(params.center, true, false)
  self.size = tdengine.vec2(params.size) 
  self.static_image = params.static_image or nil
  self.color = params.color or nil

  self.loop = true
  self.queued_animations = tdengine.data_types.queue:new()

  -- In the editor, when we're editing animations, we don't want to lookup the animation from the
  -- canonical list. We want to use the copy we're editing and watch it change live.
  if params.data then
    self.data = params.data
  end

  self:restart()
end

function Animation:update()
  -- A static image never has to advance between frames
  if self.static_image then return end

  self.changed_frame = false

  -- Otherwise, find the animation data and see if we're done with the current frame
  self.accumulated = self.accumulated + tdengine.dt

  local data = self.data or tdengine.animation.find(self.animation)

  local current = data.frames[self.current]
  if not current then return end

  -- If this frame has a specific speed, use that. Otherwise, just use the animation's global speed.
  local target = 0
  if current.time and current.time > 0 then
    target = current.time
  else
    target = data.speed
  end

  -- If we're done with the frame, advance one frame. That could also mean looping back
  -- to the beginning, or starting the next queued animation
  if self.accumulated >= target then
    if self.loop then
      self.current = self.current == #data.frames and 1 or self.current + 1
    else
      self.current = self.current == #data.frames and self.current or self.current + 1
    end

    if self:is_done() and not self.queued_animations:is_empty() then
      local animation = self.queued_animations:pop()
      self:begin(animation)
    end

    self.accumulated = 0
    self.changed_frame = true
  end
end

function Animation:draw()
  if not self.enabled then return end

  local collider = self:get_entity():find_component('Collider')
  local image = self:get_image()
  local position = self:get_position()
  local size = self.size:copy()
  if size.x == 0 or size.y == 0 then
    size.x, size.y = tdengine.sprite_size(image)
  end

  tdengine.ffi.set_layer(self.layer)
  tdengine.ffi.set_world_space(true)

  tdengine.ffi.draw_image_ex(image, position.x, position.y, size.x, size.y, self.opacity)
end

--
-- PUBLIC API
--
function Animation:get_position()
  local collider = self:get_entity():find_component('Collider')

  if self.center then
    return collider:center_image(image):add(self.offset)
  else
    return collider:get_position():add(self.offset)
  end
end

function Animation:set_loop(loop)
  self.loop = loop
end

function Animation:queue(name)
  self.queued_animations:push(name)
end

function Animation:restart()
  self.current = 1
  self.accumulated = 0
end

function Animation:begin(name)
  self.animation = name
  self.changed_frame = true
  self.queued_animations:clear()
  self:restart()
end

function Animation:begin_if_inactive(name)
  if self.animation == name then return end
  self:begin(name)
end

function Animation:get_image()
  -- If a static image is specified, always prefer that
  if self.static_image then
    return self.static_image
  end

  -- Otherwise, look up the current frame of the current animation
  local data = self.data or tdengine.animation.find(self.animation)
  local frame = data.frames[self.current]
  frame = frame or { image = 'debug.png', time = .25 }
  return frame.image
end

function Animation:get_size()
  local image = self:get_image()
  if not image then return tdengine.vec2() end

  return tdengine.vec2(tdengine.sprite_size(image)):scale(self.scale)
end

function Animation:disable()
  self.enabled = false
end

function Animation:enable()
  self.enabled = true
end

function Animation:is_playing(name)
  return self.animation == name
end

function Animation:is_done()
  local data = self.data or tdengine.animation.find(self.animation)
  return self.current == #data.frames and not self.loop
end

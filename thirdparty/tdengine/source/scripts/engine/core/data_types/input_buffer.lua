InputBuffer = tdengine.class.define('InputBuffer')

function InputBuffer:init()
  self.key = glfw.keys.A
  self.delay = 0
  self.speed = 0
  self.time = 0
  self.callback = function() end

  self.input = ContextualInput:new()
end

function InputBuffer:update(dt)
  dt = dt or tdengine.dt

  if self.input:pressed(self.key) then
    self.time = self.delay
    self.callback()
  end

  if self.input:down(self.key) then
    self.time = self.time - dt
    if self.time <= 0 then
      self.time = self.speed
      self.callback()
    end
  end
end

function InputBuffer:set_key(key)
  self.key = key
end

function InputBuffer:set_delay(delay)
  self.delay = delay
end

function InputBuffer:set_speed(speed)
  self.speed = speed
end

function InputBuffer:set_callback(callback)
  self.callback = callback
end

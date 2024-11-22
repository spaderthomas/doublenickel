local SmoothDamp = tdengine.class.define('SmoothDamp')
tdengine.interpolation.SmoothDamp = SmoothDamp

function SmoothDamp:init(params)
  params = params or {}

  self.start = params.start or 0
  self.target = params.target or 1
  self.velocity = params.velocity or .1
  self.epsilon = params.epsilon or .01

  self:reset()
end

function SmoothDamp:update()
  local direction = self.target - self.current
  local delta = direction * self.velocity
  self.current = self.current + delta

  if self:is_done() then
    self.current = self.target
  end

  return self:is_done()
end

function SmoothDamp:reset()
  self.current = self.start
end

function SmoothDamp:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function SmoothDamp:is_done()
  return math.abs(self.target - self.current) < self.epsilon
end

function SmoothDamp:get_value()
  return self.current
end

function SmoothDamp:set_start(start)
  self.start = start
  self:reset()
end

function SmoothDamp:set_target(target)
  self.target = target
  self:reset()
end

function SmoothDamp:set_velocity(velocity)
  self.velocity = velocity
end

local SmoothDamp2 = tdengine.class.define('SmoothDamp2')
tdengine.interpolation.SmoothDamp2 = SmoothDamp2

function SmoothDamp2:init(params)
  params = params or {}

  self.start = tdengine.vec2(params.start) or tdengine.vec2()
  self.target = tdengine.vec2(params.target) or tdengine.vec2()
  self.velocity = params.velocity or .1
  self.epsilon = params.epsilon or .01

  self:reset()
end

function SmoothDamp2:update()
  local direction = self.target:subtract(self.current)
  local delta = direction:scale(self.velocity)
  self.current.x = self.current.x + delta.x
  self.current.y = self.current.y + delta.y

  if self:is_done() then
    self.current.x = self.target.x
    self.current.y = self.target.y
  end

  return self:is_done()
end

function SmoothDamp2:reset()
  self.current = tdengine.vec2(self.start)
end

function SmoothDamp2:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function SmoothDamp2:is_done()
  return self.target:distance(self.current) < self.epsilon
end

function SmoothDamp2:get_value()
  return self.current
end

function SmoothDamp2:set_start(start)
  self.start = start
  self:reset()
end

function SmoothDamp2:set_target(target)
  self.target = target
  self:reset()
end

function SmoothDamp2:set_velocity(velocity)
  self.velocity = velocity
end

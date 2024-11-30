local ConstantSpeed = tdengine.class.define('ConstantSpeed')
tdengine.interpolation.ConstantSpeed = ConstantSpeed

function ConstantSpeed:init(params)
  params = params or {}

  self.start = params.start or 0
  self.target = params.target or 1
  self.velocity = params.velocity or 10 -- Pixels per second

  self:reset()
end

function ConstantSpeed:update(dt)
  dt = dt or tdengine.dt
  local delta = self.velocity * dt
  self.accumulated = self.accumulated + delta

  if self.velocity > 0 then
    self.accumulated = math.min(self.accumulated, self.distance)
  else
    self.accumulated = math.max(self.accumulated, self.distance)
  end
  return self:is_done()
end

function ConstantSpeed:reset()
  self.accumulated = 0
  self.distance = self.target - self.start

  if self.distance < 0 and self.velocity > 0 then
    self.velocity = self.velocity * -1
  elseif self.distance > 0 and self.velocity < 0 then
    self.velocity = self.velocity * -1
  end
end

function ConstantSpeed:is_done()
  return self.accumulated == self.distance
end

function ConstantSpeed:get_value()
  return self.start + self.accumulated
end

function ConstantSpeed:set_start(start)
  self.start = start
  self:reset()
end

function ConstantSpeed:set_target(target)
  self.target = target
  self:reset()
end

local ConstantSpeed2 = tdengine.class.define('ConstantSpeed2')
tdengine.interpolation.ConstantSpeed2 = ConstantSpeed2

function ConstantSpeed2:init(params)
  params = params or {}

  self.start = params.start or tdengine.vec2()
  self.target = params.target or tdengine.vec2()
  self.velocity = params.velocity or 10

  self.x = ConstantSpeed:new({
    start = self.start.x,
    target = self.target.x,
    velocity = self.velocity
  })

  self.y = ConstantSpeed:new({
    start = self.start.y,
    target = self.target.y,
    velocity = self.velocity
  })
end

function ConstantSpeed2:update(dt)
  dt = dt or tdengine.dt
  self.x:update(dt)
  self.y:update(dt)
  return self:is_done()
end

function ConstantSpeed2:reset()
  self.x:reset()
  self.y:reset()
end

function ConstantSpeed2:is_done()
  return self.x:is_done() and self.y:is_done()
end

function ConstantSpeed2:get_value()
  return tdengine.vec2(self.x:get_value(), self.y:get_value())
end

function ConstantSpeed2:set_start(start)
  self.start = start:copy()
  self.x:set_start(start.x)
  self.y:set_start(start.y)
end

function ConstantSpeed2:set_target(target)
  self.target = target:copy()
  self.x:set_target(self.target.x)
  self.y:set_target(self.target.y)
end

function ConstantSpeed2:set_velocity(velocity)
  self.velocity = velocity
  self.x.velocity = velocity
  self.y.velocity = velocity
end

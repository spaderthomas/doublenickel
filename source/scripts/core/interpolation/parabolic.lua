local Parabolic = tdengine.class.define('Parabolic')
tdengine.interpolation.Parabolic = Parabolic

local function parabolic(t, exp)
  return 1 - math.pow(math.abs(2 * t - 1), exp)
end

function Parabolic:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2
  self.t = 0

  self:reset()
end

function Parabolic:update(dt)
  dt = dt or tdengine.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  return self:is_done()
end

function Parabolic:is_done()
  return self.accumulated >= self.time
end

function Parabolic:get_value()
  self.t = self.accumulated / self.time
  eased_t = parabolic(self.t, self.exponent)
  return self.start + (self.target - self.start) * eased_t
end

function Parabolic:reset()
  self.accumulated = 0
  self.t = 0
end

function Parabolic:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function Parabolic:set_start(start)
  self.start = start
end

function Parabolic:set_target(target)
  self.target = target
end

local Parabolic2 = tdengine.class.define('Parabolic2')
tdengine.interpolation.Parabolic2 = Parabolic2

function Parabolic2:init(params)
  params = params or {}
  self.start = tdengine.vec2(params.start)
  self.target = tdengine.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2
  self.t = 0

  self:reset()
end

function Parabolic2:update(dt)
  dt = dt or tdengine.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  return self:is_done()
end

function Parabolic2:is_done()
  return self.accumulated >= self.time
end

function Parabolic2:get_value()
  self.t = self.accumulated / self.time
  eased_t = parabolic(self.t, self.exponent)
  local delta = self.target:subtract(self.start)
  return self.start:add(delta:scale(eased_t))
end

function Parabolic2:reset()
  self.accumulated = 0
  self.t = 0
end

function Parabolic2:reverse()
  local tmp = self.start:copy()
  self.start = self.target:copy()
  self.target = tmp
end

function Parabolic2:set_start(start)
  self.start = start:copy()
end

function Parabolic2:set_target(target)
  self.target = target:copy()
end

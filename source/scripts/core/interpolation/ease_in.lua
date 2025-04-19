local function ease(t, exp)
  return math.pow(t, exp)
end

local EaseIn = dn.class.define('EaseIn')
dn.interpolation.EaseIn = EaseIn
dn.add_class_metamethod(
  EaseIn,
  '__call',
  function(_, a, b, t, exp)
    exp = exp or 2
    t = ease(t, exp)
    return dn.interpolation.Lerp(a, b, t)
  end)


function EaseIn:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2 -- The exponent controls the rate of change
  self.t = 0

  self:reset()
end

function EaseIn:update(dt)
  dt = dt or dn.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseIn:is_done()
  return self.accumulated >= self.time
end

function EaseIn:get_value()
  return dn.interpolation.EaseIn(self.start, self.target, self.t, self.exponent)
end

function EaseIn:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseIn:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function EaseIn:set_start(start)
  self.start = start
end

function EaseIn:set_target(target)
  self.target = target
end

local EaseIn2 = dn.class.define('EaseIn2')
dn.interpolation.EaseIn2 = EaseIn2
dn.add_class_metamethod(EaseIn2, '__call', function(_, a, b, t, exp)
  return dn.vec2(
    dn.interpolation.EaseIn(a.x, b.x, t, exp),
    dn.interpolation.EaseIn(a.y, b.y, t, exp)
  )
end)

function EaseIn2:init(params)
  params = params or {}
  self.start = dn.vec2(params.start)
  self.target = dn.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2
  self.t = 0

  self:reset()
end

function EaseIn2:update(dt)
  dt = dt or dn.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseIn2:is_done()
  return self.accumulated >= self.time
end

function EaseIn2:get_value()
  return dn.interpolation.EaseInOut2(self.start, self.target, self.t, self.exponent)
end

function EaseIn2:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseIn2:reverse()
  local tmp = self.start:copy()
  self.start = self.target:copy()
  self.target = tmp
end

function EaseIn2:set_start(start)
  self.start = start:copy()
end

function EaseIn2:set_target(target)
  self.target = target:copy()
end

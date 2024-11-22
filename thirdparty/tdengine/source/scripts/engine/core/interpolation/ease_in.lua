local function ease(t, exp)
  return math.pow(t, exp)
end

local EaseIn = tdengine.class.define('EaseIn')
tdengine.interpolation.EaseIn = EaseIn
tdengine.add_class_metamethod(
  EaseIn,
  '__call',
  function(_, a, b, t, exp)
    exp = exp or 2
    t = ease(t, exp)
    return tdengine.interpolation.Lerp(a, b, t)
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
  dt = dt or tdengine.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseIn:is_done()
  return self.accumulated >= self.time
end

function EaseIn:get_value()
  return tdengine.interpolation.EaseIn(self.start, self.target, self.t, self.exponent)
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

local EaseIn2 = tdengine.class.define('EaseIn2')
tdengine.interpolation.EaseIn2 = EaseIn2
tdengine.add_class_metamethod(EaseIn2, '__call', function(_, a, b, t, exp)
  return tdengine.vec2(
    tdengine.interpolation.EaseIn(a.x, b.x, t, exp),
    tdengine.interpolation.EaseIn(a.y, b.y, t, exp)
  )
end)

function EaseIn2:init(params)
  params = params or {}
  self.start = tdengine.vec2(params.start)
  self.target = tdengine.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2
  self.t = 0

  self:reset()
end

function EaseIn2:update(dt)
  dt = dt or tdengine.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseIn2:is_done()
  return self.accumulated >= self.time
end

function EaseIn2:get_value()
  return tdengine.interpolation.EaseInOut2(self.start, self.target, self.t, self.exponent)
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

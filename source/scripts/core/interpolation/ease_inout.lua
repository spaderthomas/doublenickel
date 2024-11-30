local ease = function(t, exp)
  if t < 0.5 then
    return math.pow(2, exp - 1) * math.pow(t, exp)
  else
    return 1 - math.pow(-2 * t + 2, exp) / 2
  end
end

local EaseInOut = doublenickel.class.define('EaseInOut')
doublenickel.interpolation.EaseInOut = EaseInOut
doublenickel.add_class_metamethod(
  EaseInOut,
  '__call',
  function(_, a, b, t, exp)
    exp = exp or 2
    t = ease(t, exp)
    return doublenickel.interpolation.Lerp(a, b, t)
  end)

function EaseInOut:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2
  self.t = 0

  self:reset()
end

function EaseInOut:update(dt)
  dt = dt or doublenickel.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time

  return self:is_done()
end

function EaseInOut:is_done()
  return self.accumulated >= self.time
end

function EaseInOut:get_value()
  return doublenickel.interpolation.EaseInOut(self.start, self.target, self.t, self.exponent)
end

function EaseInOut:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseInOut:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function EaseInOut:set_target(target)
  self.target = target
end

function EaseInOut:set_start(start)
  self.start = start
end

local EaseInOut2 = doublenickel.class.define('EaseInOut2')
doublenickel.interpolation.EaseInOut2 = EaseInOut2
doublenickel.add_class_metamethod(EaseInOut2, '__call', function(_, a, b, t, exp)
  return doublenickel.vec2(
    doublenickel.interpolation.EaseInOut(a.x, b.x, t, exp),
    doublenickel.interpolation.EaseInOut(a.y, b.y, t, exp)
  )
end)

function EaseInOut2:init(params)
  params = params or {}
  self.start = doublenickel.vec2(params.start)
  self.target = doublenickel.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2
  self.t = 0

  self:reset()
end

function EaseInOut2:update()
  local dt = doublenickel.dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseInOut2:is_done()
  return self.accumulated >= self.time
end

function EaseInOut2:get_value()
  return doublenickel.interpolation.EaseInOut2(self.start, self.target, self.t, self.exponent)
end

function EaseInOut2:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseInOut2:reverse()
  local tmp = self.start
  self.start = self.target:copy()
  self.target = tmp:copy()
end

function EaseInOut2:set_target(target)
  self.target = target:copy()
end

function EaseInOut2:set_start(start)
  self.start = start:copy()
end

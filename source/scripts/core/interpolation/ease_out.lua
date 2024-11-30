local function ease(t, exp)
  return 1 - math.pow(1 - t, exp)
end

local EaseOut = doublenickel.class.define('EaseOut')
doublenickel.interpolation.EaseOut = EaseOut
doublenickel.add_class_metamethod(
  EaseOut,
  '__call',
  function(_, a, b, t, exp)
    exp = exp or 2
    t = ease(t, exp)
    return doublenickel.interpolation.Lerp(a, b, t)
  end)


function EaseOut:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2 -- The exponent controls the rate of change

  self:reset()
end

function EaseOut:update(dt)
  dt = dt or doublenickel.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseOut:is_done()
  return self.accumulated >= self.time
end

function EaseOut:get_value()
  return doublenickel.interpolation.EaseOut(self.start, self.target, self.t, self.exponent)
end

function EaseOut:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseOut:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function EaseOut:set_start(start)
  self.start = start
end

function EaseOut:set_target(target)
  self.target = target
end

local EaseOut2 = doublenickel.class.define('EaseOut2')
doublenickel.interpolation.EaseOut2 = EaseOut2
doublenickel.add_class_metamethod(EaseOut2, '__call', function(_, a, b, t, exp)
  return doublenickel.vec2(
    doublenickel.interpolation.EaseOut(a.x, b.x, t, exp),
    doublenickel.interpolation.EaseOut(a.y, b.y, t, exp)
  )
end)

function EaseOut2:init(params)
  params = params or {}
  self.start = doublenickel.vec2(params.start)
  self.target = doublenickel.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.exponent = params.exponent or 2

  self:reset()
end

function EaseOut2:update(dt)
  dt = dt or doublenickel.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseOut2:is_done()
  return self.accumulated >= self.time
end

function EaseOut2:get_value()
  return doublenickel.interpolation.EaseInOut2(self.start, self.target, self.t, self.exponent)
end

function EaseOut2:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseOut2:reverse()
  local tmp = self.start
  self.start = self.target:copy()
  self.target = tmp:copy()
end

function EaseOut2:set_target(target)
  self.target = target:copy()
end

function EaseOut2:set_start(start)
  self.start = start:copy()
end

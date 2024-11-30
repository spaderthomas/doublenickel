local EaseInOutBounce = doublenickel.class.define('EaseInOutBounce')
doublenickel.interpolation.EaseInOutBounce = EaseInOutBounce

local function ease(t)
  local c1 = 1.70158
  local c2 = c1 * 1.525
  if t < 0.5 then
    return 0.5 * ((2 * t) ^ 2 * ((c2 + 1) * 2 * t - c2))
  else
    return 0.5 * (((2 * t - 2) ^ 2 * ((c2 + 1) * (2 * t - 2) + c2)) + 2)
  end
end

doublenickel.add_class_metamethod(
  EaseInOutBounce,
  '__call',
  function(_, a, b, t, exp)
    exp = exp or 2
    t = ease(t, exp)
    return doublenickel.interpolation.Lerp(a, b, t)
  end)



function EaseInOutBounce:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.t = 0

  self:reset()
end

function EaseInOutBounce:update(dt)
  dt = dt or doublenickel.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseInOutBounce:is_done()
  return self.accumulated >= self.time
end

function EaseInOutBounce:get_value()
  return doublenickel.interpolation.EaseInOutBounce(self.start, self.target, self.t)
end

function EaseInOutBounce:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseInOutBounce:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function EaseInOutBounce:set_target(target)
  self.target = target
end

function EaseInOutBounce:set_start(start)
  self.start = start
end

local EaseInOutBounce2 = doublenickel.class.define('EaseInOutBounce2')
doublenickel.interpolation.EaseInOutBounce2 = EaseInOutBounce2

doublenickel.add_class_metamethod(
  EaseInOutBounce2,
  '__call',
  function(_, a, b, t, exp)
    return doublenickel.vec2(
      doublenickel.interpolation.EaseInOutBounce(a.x, b.x, t),
      doublenickel.interpolation.EaseInOutBounce(a.y, b.y, t)
    )
  end)

function EaseInOutBounce2:init(params)
  params = params or {}
  self.start = doublenickel.vec2(params.start)
  self.target = doublenickel.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.t = 0

  self:reset()
end

function EaseInOutBounce2:update()
  local dt = doublenickel.dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  self.t = self.accumulated / self.time
  return self:is_done()
end

function EaseInOutBounce2:is_done()
  return self.accumulated >= self.time
end

function EaseInOutBounce2:get_value()
  return doublenickel.interpolation.EaseInOutBounce2(self.start, self.target, self.t)
end

function EaseInOutBounce2:reset()
  self.accumulated = 0
  self.t = 0
end

function EaseInOutBounce2:reverse()
  local tmp = self.start
  self.start = self.target:copy()
  self.target = tmp:copy()
end

function EaseInOutBounce2:set_target(target)
  self.target = target:copy()
end

function EaseInOutBounce2:set_start(start)
  self.start = start:copy()
end

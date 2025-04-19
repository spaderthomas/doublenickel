local Lerp = dn.class.define('Lerp')
dn.interpolation.Lerp = Lerp
dn.add_class_metamethod(Lerp, '__call', function(_, a, b, t)
  return (1 - t) * a + t * b
end)



function Lerp:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1

  self:reset()
end

function Lerp:update(dt)
  dt = dt or dn.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  return self:is_done()
end

function Lerp:is_done()
  return self.accumulated >= self.time
end

function Lerp:get_value()
  local t = self.accumulated / self.time
  return dn.interpolation.Lerp(self.start, self.target, t)
end

function Lerp:reset()
  self.accumulated = 0
  self.t = 0
end

function Lerp:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

function Lerp:set_target(target)
  self.target = target
end

function Lerp:set_start(start)
  self.start = start
end

local Lerp2 = dn.class.define('Lerp2')
dn.interpolation.Lerp2 = Lerp2
dn.add_class_metamethod(Lerp2, '__call', function(_, a, b, t)
  return dn.vec2(
    dn.interpolation.Lerp(a.x, b.x, t),
    dn.interpolation.Lerp(a.y, b.y, t)
  )
end)

function Lerp2:init(params)
  params = params or {}
  self.start = dn.vec2(params.start)
  self.target = dn.vec2(params.target)
  self.time = params.time or 1
  self.speed = params.speed or 1

  self:reset()
end

function Lerp2:update()
  local dt = dn.dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  return self:is_done()
end

function Lerp2:is_done()
  return self.accumulated >= self.time
end

function Lerp2:get_value()
  local t = self.accumulated / self.time
  return dn.vec2(
    (1 - t) * self.start.x + t * self.target.x,
    (1 - t) * self.start.y + t * self.target.y
  )
end

function Lerp2:reset()
  self.accumulated = 0
  self.t = 0
end

function Lerp2:reverse()
  local tmp = self.start
  self.start = self.target:copy()
  self.target = tmp:copy()
end

function Lerp2:set_target(target)
  self.target = target:copy()
end

function Lerp2:set_start(start)
  self.start = start:copy()
end

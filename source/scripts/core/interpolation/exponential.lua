local Exponential = tdengine.class.define('Exponential')
tdengine.interpolation.Exponential = Exponential

function Exponential:init(params)
  params = params or {}
  self.start = params.start or 0
  self.target = params.target or 1
  self.time = params.time or 1
  self.speed = params.speed or 1
  self.t = 0

  self:reset()
end

function Exponential:update(dt)
  dt = dt or tdengine.dt
  dt = dt / self.speed
  self.accumulated = math.min(self.accumulated + dt, self.time)
  return self:is_done()
end

function Exponential:is_done()
  return self.accumulated >= self.time
end

function Exponential:get_value()
  -- Calculate the normalized time (t) in the range [0, 1]
  self.t = self.accumulated / self.time

  -- Exponential interpolation using f(x) = A * e^(k * x)
  -- Here we want to normalize it to range [0, 1] between start and target
  local k = 5 -- Adjust k for desired steepness
  local e_kx_min = math.exp(0)
  local e_kx_max = math.exp(k)

  -- Normalize the value
  local normalized_value = (math.exp(k * self.t) - e_kx_min) / (e_kx_max - e_kx_min)

  -- Scale and offset the normalized value to fit between start and target
  return self.start + (self.target - self.start) * normalized_value
end

function Exponential:reset()
  self.accumulated = 0
  self.t = 0
  self.current = self.start
end

function Exponential:reverse()
  local tmp = self.start
  self.start = self.target
  self.target = tmp
end

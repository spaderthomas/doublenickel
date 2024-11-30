Timer = tdengine.class.define('Timer')

function Timer:init(time)
  self:begin(time)
end

function Timer:begin(t)
  self.interval = self.interval or t
  self.accumulated = 0
end

function Timer:update(dt)
  dt = dt or tdengine.dt
  self.accumulated = self.accumulated + dt
  self.last_frame = self.this_frame
  self.this_frame = self:is_expired()
  return self:is_done()
end

function Timer:is_expired()
  return self.accumulated >= self.interval
end

function Timer:is_done()
  return self.this_frame and not self.last_frame
end

function Timer:reset()
  self.accumulated = 0
end

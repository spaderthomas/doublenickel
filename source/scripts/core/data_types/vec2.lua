local vec2_mixin = {
  translate = function(self, other)
    self.x = self.x + other.x
    self.y = self.y + other.y
    return self
  end,
  add = function(self, other)
    return dn.vec2(self.x + other.x, self.y + other.y)
  end,
  update = function(self, other)
    self.x = self.x + other.x
    self.y = self.y + other.y
    return self
  end,
  subtract = function(self, other)
    if type(other) == 'table' then
      return dn.vec2(self.x - other.x, self.y - other.y)
    elseif type(other) == 'number' then
      return dn.vec2(self.x - other, self.y - other)
    end
  end,
  scale = function(self, scalar)
    return dn.vec2(self.x * scalar, self.y * scalar)
  end,
  truncate = function(self, digits)
    return dn.vec2(truncate(self.x, digits), truncate(self.y, digits))
  end,
  abs = function(self)
    return dn.vec2(math.abs(self.x), math.abs(self.y))
  end,
  equals = function(self, other, eps)
    eps = eps or dn.deq_epsilon
    return double_eq(self.x, other.x, eps) and double_eq(self.y, other.y, eps)
  end,
  clampl = function(self, scalar)
    return dn.vec2(math.max(scalar, self.x), math.max(scalar, self.y))
  end,
  clamp = function(self, low, high)
    return dn.vec2(
      dn.math.clamp(self.x, low.x, high.x),
      dn.math.clamp(self.y, low.y, high.y))
  end,
  pairwise_mult = function(self, other)
    return dn.vec2(self.x * other.x, self.y * other.y)
  end,
  distance = function(self, other)
    local d = self:subtract(other)
    return math.sqrt(d.x * d.x + d.y * d.y)
  end,
  normal = function(self)
    return dn.vec2(self.y, self.x * -1)
  end,
  normalize = function(self)
    local length = math.sqrt(self.x * self.x + self.y * self.y)
    if length > 0 then
      return dn.vec2(self.x / length, self.y / length)
    else
      return dn.vec2(self) -- Always return a copy
    end
  end,
  dot = function(self, other)
    return self.x * other.x + self.y * other.y
  end,
  length = function(self)
    return math.sqrt(self.x * self.x + self.y * self.y)
  end,
  floor = function(self)
    return dn.vec2(math.floor(self.x), math.floor(self.y))
  end,
  ceil = function(self)
    return dn.vec2(math.ceil(self.x), math.ceil(self.y))
  end,

  ----------------
  -- TYPE UTILS --
  ----------------
  unpack = function(self)
    return self.x, self.y
  end,
  assign = function(self, other)
    self.x = other.x
    self.y = other.y
  end,
  copy = function(self)
    return dn.vec2(self)
  end,
  to_ctype = function(self)
    return ffi.new('dn_vector2_t', self.x, self.y)
  end
}

dn.vec2_impl = dn.class.define('vec2_impl')
dn.vec2_impl:include(vec2_mixin)
dn.vec2 = function(x, y)
  local vec = dn.vec2_impl:new()

  if type(x) == 'table' or type(x) == 'cdata' then
    vec.x = x.x
    vec.y = x.y
    return vec
  else
    if not y and x then y = x end
    if not y and not x then y = 0 end
    x = x or 0
    vec.x = x
    vec.y = y
    return vec
  end
end

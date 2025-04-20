
function average(a, b)
  return (a + b) / 2
end

dn.deq_epsilon = .00000001
function double_eq(x, y, eps)
  eps = eps or dn.deq_epsilon
  return math.abs(x - y) < eps
end

function truncate(float, digits)
  local mult = 10 ^ digits
  return math.modf(float * mult) / mult
end

dn.op_or, dn.op_xor, dn.op_and = 1, 3, 4

function bitwise(oper, a, ...)
  -- base case 1: the parameter pack is empty. return nil to signal.
  if a == nil then
    return nil
  end

  local b = bitwise(oper, ...)

  -- base case 2: we're at the end of the parameter pack. just return yourself.
  if b == nil then
    return a
  end

  local r, m, s = 0, 2 ^ 31
  repeat
    s, a, b = a + b + m, a % m, b % m
    r, m = r + m * oper % (s - a - b), m / 2
  until m < 1
  return r
end

function ternary(cond, if_true, if_false)
  if cond then return if_true else return if_false end
end

-- Useful for when you need to switch operation -- just switch out the function
function add(x, y)
  return x + y
end

function subtract(x, y)
  return x - y
end

function dn.math.init()
  pi = 3.14159265359
  dn.math.pi = 3.14159265359
  dn.really_large_number = 2000000000
  dn.really_small_number = -2000000000
  dn.math.seed_rng()
end

function dn.math.ranged_sin(x, min, max)
  local sin = math.sin(x)
  local coefficient = (max - min) / 2
  local offset = (max + min) / 2
  return coefficient * sin + offset
end

function dn.math.ranged_cos(x, min, max)
  return dn.math.ranged_sin(x + dn.math.pi / 2, min, max)
end

function dn.math.timed_sin(speed, min, max)
  return dn.math.ranged_sin(dn.elapsed_time * speed, min, max)
end

function dn.math.random_float(min, max)
  local random = math.random()
  local range = max - min
  return min + random * range
end

function dn.math.random_int(min, max)
  return math.random(min, max)
end

function dn.math.ternary(cond, if_true, if_false)
  if cond then return if_true else return if_false end
end

function dn.math.lerp(a, b, x)
  return x * b + (1 - x) * a
end

function dn.math.clamp(x, low, high)
  return math.min(math.max(x, low), high)
end

function dn.math.snap_to_range(x, range)
  local min_distance = dn.really_large_number
  local closest = x
  for i, v in pairs(range) do
    local distance = math.abs(v - x)
    if min_distance > distance then
      min_distance = distance
      closest = v
    end
  end

  return closest
end

function dn.math.map(value, in_min, in_max, out_min, out_max)
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
end

function dn.math.rotate_point(point, center, angle)
  local s = math.sin(angle)
  local c = math.cos(angle)

  return dn.vec2(
    center.x + c * (point.x - center.x) - s * (point.y - center.y),
    center.y + s * (point.x - center.x) + c * (point.y - center.y)
  )
end

function dn.math.fmod(x, y)
  if not y then dbg() end
  local fx = x * 1.0
  local fy = y * 1.0
  return fx - (math.floor(fx / fy) * fy)
end

function dn.math.mod(x, y)
  return x - (math.floor(x / y) * y)
end

function dn.math.mod1(x, y)
  return ((x - 1) % y) + 1
end

function dn.math.seed_rng()
  math.randomseed(os.clock() * 1000000)
end

function dn.math.turns_to_rads(turns)
  return turns * 2 * dn.math.pi
end
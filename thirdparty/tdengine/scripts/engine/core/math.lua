
function average(a, b)
  return (a + b) / 2
end

tdengine.deq_epsilon = .00000001
function double_eq(x, y, eps)
  eps = eps or tdengine.deq_epsilon
  return math.abs(x - y) < eps
end

function truncate(float, digits)
  local mult = 10 ^ digits
  return math.modf(float * mult) / mult
end

tdengine.op_or, tdengine.op_xor, tdengine.op_and = 1, 3, 4

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


function tdengine.math.init()
  tdengine.math.pi = 3.14159265359
  tdengine.really_large_number = 2000000000
  tdengine.really_small_number = -2000000000
  tdengine.math.seed_rng()
end

function tdengine.math.ranged_sin(x, min, max)
  local sin = math.sin(x)
  local coefficient = (max - min) / 2
  local offset = (max + min) / 2
  return coefficient * sin + offset
end

function tdengine.math.ranged_cos(x, min, max)
  return tdengine.math.ranged_sin(x + tdengine.math.pi / 2, min, max)
end

function tdengine.math.timed_sin(speed, min, max)
  return tdengine.math.ranged_sin(tdengine.elapsed_time * speed, min, max)
end

function tdengine.math.random_float(min, max)
  local random = math.random()
  local range = max - min
  return min + random * range
end

function tdengine.math.random_int(min, max)
  return math.random(min, max)
end

function tdengine.math.ternary(cond, if_true, if_false)
  if cond then return if_true else return if_false end
end

function tdengine.math.lerp(a, b, x)
  return x * b + (1 - x) * a
end

function tdengine.math.clamp(x, low, high)
  return math.min(math.max(x, low), high)
end

function tdengine.math.snap_to_range(x, range)
  local min_distance = tdengine.really_large_number
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

function tdengine.math.map(value, in_min, in_max, out_min, out_max)
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
end

function tdengine.math.rotate_point(point, center, angle)
  local s = math.sin(angle)
  local c = math.cos(angle)

  return tdengine.vec2(
    center.x + c * (point.x - center.x) - s * (point.y - center.y),
    center.y + s * (point.x - center.x) + c * (point.y - center.y)
  )
end

function tdengine.math.fmod(x, y)
  local fx = x * 1.0
  local fy = y * 1.0
  return fx - (math.floor(fx / fy) * fy)
end

function tdengine.math.mod(x, y)
  return x - (math.floor(x / y) * y)
end

function tdengine.math.mod1(x, y)
  return ((x - 1) % y) + 1
end

function tdengine.math.seed_rng()
  math.randomseed(os.clock() * 1000000)
end

function tdengine.math.turns_to_rads(turns)
  return turns * 2 * tdengine.math.pi
end

-- Hash lookup table as defined by Ken Perlin
-- This is a randomly arranged array of all numbers from 0-255 inclusive
local permutation_raw = { 151, 160, 137, 91, 90, 15,
  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
}
local permutation = {}

for i = 0, 255 do
  permutation[i] = permutation_raw[i + 1]
  permutation[i + 256] = permutation_raw[i + 1]
end


-- Gradient function finds dot product between pseudorandom gradient vector
-- and the vector from input coordinate to a unit cube vertex
local dot_product = {
  [0x0] = function(x, y, z) return x + y end,
  [0x1] = function(x, y, z) return -x + y end,
  [0x2] = function(x, y, z) return x - y end,
  [0x3] = function(x, y, z) return -x - y end,
  [0x4] = function(x, y, z) return x + z end,
  [0x5] = function(x, y, z) return -x + z end,
  [0x6] = function(x, y, z) return x - z end,
  [0x7] = function(x, y, z) return -x - z end,
  [0x8] = function(x, y, z) return y + z end,
  [0x9] = function(x, y, z) return -y + z end,
  [0xA] = function(x, y, z) return y - z end,
  [0xB] = function(x, y, z) return -y - z end,
  [0xC] = function(x, y, z) return y + x end,
  [0xD] = function(x, y, z) return -y + z end,
  [0xE] = function(x, y, z) return y - x end,
  [0xF] = function(x, y, z) return -y - z end
}
local function grad(hash, x, y, z)
  return dot_product[bitwise(tdengine.op_and, hash, 0xF)](x, y, z)
end

-- Fade function is used to smooth final output
local function fade(t)
  return t * t * t * (t * (t * 6 - 15) + 10)
end

local function lerp(t, a, b)
  return a + t * (b - a)
end

-- Return range: [-1, 1]
function tdengine.math.perlin(x, y, vmin, vmax)
  y           = y or 0
  z           = z or 0

  -- Calculate the "unit cube" that the point asked will be located in
  local xi    = bitwise(tdengine.op_and, math.floor(x), 255)
  local yi    = bitwise(tdengine.op_and, math.floor(y), 255)
  local zi    = bitwise(tdengine.op_and, math.floor(z), 255)

  -- Next we calculate the location (from 0 to 1) in that cube
  x           = x - math.floor(x)
  y           = y - math.floor(y)
  z           = z - math.floor(z)

  -- We also fade the location to smooth the result
  local u     = fade(x)
  local v     = fade(y)
  local w     = fade(z)

  -- Hash all 8 unit cube coordinates surrounding input coordinate
  local p     = permutation
  local A, AA, AB, AAA, ABA, AAB, ABB, B, BA, BB, BAA, BBA, BAB, BBB
  A           = p[xi] + yi
  AA          = p[A] + zi
  AB          = p[A + 1] + zi
  AAA         = p[AA]
  ABA         = p[AB]
  AAB         = p[AA + 1]
  ABB         = p[AB + 1]

  B           = p[xi + 1] + yi
  BA          = p[B] + zi
  BB          = p[B + 1] + zi
  BAA         = p[BA]
  BBA         = p[BB]
  BAB         = p[BA + 1]
  BBB         = p[BB + 1]

  -- Take the weighted average between all 8 unit cube coordinates
  local noise = lerp(w,
    lerp(v,
      lerp(u,
        grad(AAA, x, y, z),
        grad(BAA, x - 1, y, z)
      ),
      lerp(u,
        grad(ABA, x, y - 1, z),
        grad(BBA, x - 1, y - 1, z)
      )
    ),
    lerp(v,
      lerp(u,
        grad(AAB, x, y, z - 1), grad(BAB, x - 1, y, z - 1)
      ),
      lerp(u,
        grad(ABB, x, y - 1, z - 1), grad(BBB, x - 1, y - 1, z - 1)
      )
    )
  )

  if vmin and vmax then noise = tdengine.math.map(noise, -1, 1, vmin, vmax) end
  return noise
end

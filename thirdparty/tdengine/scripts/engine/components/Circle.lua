ColliderCircle = tdengine.component.define('ColliderCircle')

function ColliderCircle:init(params)
  params = params or {}

  local position = params.position or tdengine.vec2(0, 0)
  self.position = tdengine.vec2(position)

  self.radius = params.radius or 100
end

function ColliderCircle:serialize()
  return {
    position = self.position,
    radius = self.radius
  }
end

-------------
-- VIRTUAL --
-------------
function ColliderCircle:get_height()
  return self.radius * 2
end

function ColliderCircle:get_width()
  return self.radius * 2
end

function ColliderCircle:get_center()
  return tdengine.vec2(self.position)
end

function ColliderCircle:get_position()
  return tdengine.vec2(self.position)
end

function ColliderCircle:get_xmin()
  return self.position.x - self.radius
end

function ColliderCircle:get_ymin()
  return self.position.y - self.radius
end

function ColliderCircle:get_xmax()
  return self.position.x + self.radius
end

function ColliderCircle:get_ymax()
  return self.position.y + self.radius
end

function ColliderCircle:move(delta)
  self.position.x = self.position.x + delta.x
  self.position.y = self.position.y + delta.y
end

function ColliderCircle:set_position(position)
  self.position.x = position.x
  self.position.y = position.y
end

function ColliderCircle:show(color)
  color = color or tdengine.colors.red
  --tdengine.ffi.draw_quad(self.position.x, self.position.y, self.radius, self.radius, color)
  tdengine.ffi.draw_circle_sdf(self.position.x, self.position.y, self.radius, tdengine.color_to_vec4(color), 1)
end

function ColliderCircle:is_point_inside(point)
  local dx = point.x - self.position.x
  local dy = point.y - self.position.y
  distance = math.sqrt(dx * dx + dy * dy)
  return distance < self.radius
end

function ColliderCircle:get_corners()
  return {}
end

function ColliderCircle:get_normals()
  return {}
end

function ColliderCircle:get_points()
  return {
    self:get_position()
  }
end

function ColliderCircle:find_sat_axes(other)
  local points = other:get_points()
  local center = self.position

  local min_distance = 10000000
  local closest = nil
  for i, point in pairs(points) do
    local distance = center:distance(point)
    if min_distance > distance then
      min_distance = distance
      closest = point
    end
  end

  local axis = {
    closest:subtract(center):normalize()
  }
  return axis
end

function ColliderCircle:project(axis)
  local projection = self.position:dot(axis)
  return projection - self.radius, projection + self.radius
end

function ColliderCircle:center_image(image)
  local sx, sy = tdengine.sprite_size(image)
  local position = tdengine.vec2()
  position.x = self.position.x - sx / 2
  position.y = self.position.y + sy / 2
  return position
end

------------
-- CIRCLE --
------------
function ColliderCircle:get_radius()
  return self.radius
end

function ColliderCircle:set_radius(radius)
  self.radius = radius
end

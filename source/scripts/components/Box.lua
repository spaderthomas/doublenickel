ColliderBox = doublenickel.component.define('ColliderBox')

function ColliderBox:init(params)
  params = params or {}
  local position = params.position or doublenickel.vec2(0, 0)
  self.position = doublenickel.vec2(position)

  local dimension = params.dimension or { x = 100, y = 100 }
  self.dimension = doublenickel.vec2(dimension)
end

function ColliderBox:serialize()
  return {
    position = self.position,
    dimension = self.dimension
  }
end

-------------
-- VIRTUAL --
-------------
function ColliderBox:get_height()
  return self.dimension.y
end

function ColliderBox:get_width()
  return self.dimension.x
end

function ColliderBox:get_center()
  return doublenickel.vec2(
    self.position.x + (self.dimension.x / 2),
    self.position.y - (self.dimension.y / 2))
end

function ColliderBox:get_position()
  return doublenickel.vec2(self.position)
end

function ColliderBox:get_xmin()
  return self.position.x
end

function ColliderBox:get_ymin()
  return self.position.y - self.dimension.y
end

function ColliderBox:get_xmax()
  return self.position.x + self.dimension.x
end

function ColliderBox:get_ymax()
  return self.position.y
end

function ColliderBox:move(delta)
  self.position.x = self.position.x + delta.x
  self.position.y = self.position.y + delta.y
end

function ColliderBox:set_position(position)
  self.position.x = position.x
  self.position.y = position.y
end

function ColliderBox:show(color)
  color = color or doublenickel.colors.idk
  if self.dbg_color then color = self.dbg_color end


  if self.base.world_space then doublenickel.ffi.set_world_space(true)() end
  if not self.base.world_space then doublenickel.ffi.end_world_space(true)() end
  doublenickel.ffi.draw_quad_l(self.position, self.dimension, color)
end

function ColliderBox:is_point_inside(point)
  local left = point.x > self.position.x
  local right = point.x < self.position.x + self.dimension.x
  local top = point.y < self.position.y
  local bottom = point.y > self.position.y - self.dimension.y

  return left and right and top and bottom
end

function ColliderBox:get_points()
  local xmin, xmax = self:get_xmin(), self:get_xmax()
  local ymin, ymax = self:get_ymin(), self:get_ymax()
  return {
    doublenickel.vec2(xmin, ymin),
    doublenickel.vec2(xmin, ymax),
    doublenickel.vec2(xmax, ymax),
    doublenickel.vec2(xmax, ymin),
  }
end

function ColliderBox:get_normals()
  return {
    doublenickel.vec2(1, 0),
    doublenickel.vec2(0, 1),
  }
end

function ColliderBox:find_sat_axes(other)
  return self:get_normals()
end

function ColliderBox:project(axis)
  local min = doublenickel.really_large_number
  local max = doublenickel.really_small_number

  local points = self:get_points()
  for i, point in pairs(points) do
    local projection = point:dot(axis)
    min = math.min(min, projection)
    max = math.max(max, projection)
  end

  return min, max
end

function ColliderBox:center_image(image)
  local sx, sy = doublenickel.sprite_size(image)
  local position = doublenickel.vec2()
  position.x = self.position.x + (self.dimension.x / 2 - sx / 2)
  position.y = self.position.y - (self.dimension.y / 2 - sy / 2)

  return position
end

------------------
-- BOX SPECIFIC --
------------------
function ColliderBox:set_dimension(dimension)
  self.dimension.x = dimension.x
  self.dimension.y = dimension.y
end

function ColliderBox:get_corners()
  local corners = {}

  local xs = {
    self:get_xmin(),
    self:get_xmax()
  }
  local ys = {
    self:get_ymin(),
    self:get_ymax()
  }

  for _, x in pairs(xs) do
    for _, y in pairs(ys) do
      table.insert(corners, doublenickel.vec2(x, y))
    end
  end

  return corners
end

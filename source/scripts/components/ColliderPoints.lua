ColliderPoints = tdengine.component.define('ColliderPoints')

function ColliderPoints:init(params)
  params = params or {}

  self.points = tdengine.data_types.array:new()

  local points = params.points or {}
  for _, point in pairs(points) do
    self.points:add(point)
  end
end

function ColliderPoints:serialize()
  return {
    points = self.points.data,
  }
end

-------------
-- VIRTUAL --
-------------
function ColliderPoints:get_height()
  log.warn('unimplemented: ColliderPoints:get_height')
  return 0
end

function ColliderPoints:get_width()
  log.warn('unimplemented: ColliderPoints:get_width')
  return 0
end

function ColliderPoints:get_center()
  log.warn('unimplemented: ColliderPoints:get_center')
  return self.points:at(1)
end

function ColliderPoints:get_position()
  log.warn('unimplemented: ColliderPoints:get_width')
  return 0
end

function ColliderPoints:get_xmin()
  log.warn('unimplemented: ColliderPoints:get_xmin')
  return 0
end

function ColliderPoints:get_ymin()
  log.warn('unimplemented: ColliderPoints:get_ymin')
  return 0
end

function ColliderPoints:get_xmax()
  log.warn('unimplemented: ColliderPoints:get_xmax')
  return 0
end

function ColliderPoints:get_ymax()
  log.warn('unimplemented: ColliderPoints:get_ymax')
  return 0
end

function ColliderPoints:move(delta)
  log.warn('unimplemented: ColliderPoints:move')
end

function ColliderPoints:set_position(position)
  log.warn('unimplemented: ColliderPoints:move')
end

function ColliderPoints:show(color)
  for index, point in self.points:iterate() do
    local next_point = self.points:at(index + 1)
    if not next_point then next_point = self.points:at(1) end

    tdengine.ffi.draw_line(point.x, point.y, next_point.x, next_point.y, 5, color)
  end
end

function ColliderPoints:is_point_inside(p)
  local sign = 0
  for i, pa in self.points:iterate() do
    local pb = self.points:at(i + 1)
    if not pb then pb = self.points:at(1) end

    -- When we cross some edge of the collider and the point, we get a vector perpendicular to both. Since they're
    -- 2D vectors, that vector must be pointing along the Z axis. We can use the sign of the Z component to figure
    -- out which side of the edge the point is on. If that sign is the same for all edges, then the point is within
    -- the collider
    --
    -- The cross product of two 3D vectors is:
    --    c.x = a.y * b.z - a.z * b.y
    --    c.y = a.z * b.x - a.x * b.z
    --    c.z = a.x * b.y - a.y * b.x
    --
    -- But since the Z component of our vectors is 0 (since we're in 2D), the only term that is nonzero is the
    -- third one. The formula below just calculates that, where a = an edge of the collider and b = the vector
    -- from the edge to the point we're checking.
    local determinant = (pb.x - pa.x) * (p.y - pa.y) - (pb.y - pa.y) * (p.x - pa.x)
    if determinant > 0 and sign < 0 then return false end
    if determinant < 0 and sign > 0 then return false end
    sign = determinant
  end

  return true
end

function ColliderPoints:get_corners()
  return {}
end

function ColliderPoints:get_normals()
  return {}
end

function ColliderPoints:get_points()
  return self.points.data
end

function ColliderPoints:find_sat_axes(other)

end

function ColliderPoints:project(axis)
  log.warn('project')
  local projection = self.position:dot(axis)
  return projection - self.radius, projection + self.radius
end

function ColliderPoints:center_image(image)
  log.warn('unimplemented: ColliderPoints:center_image')
  return tdengine.vec2()
end

ColliderCapsule = tdengine.component.define('ColliderCapsule')

function ColliderCapsule:init(params)
  params = params or {}
  self.transform = Matrix3:new(params.transform)
end

function ColliderCapsule:serialize()
  return {
    transform = self.transform:serialize()
  }
end


-------------
-- VIRTUAL --
-------------
function ColliderCapsule:get_height()
  log.warn('unimplemented')
  return 69
end

function ColliderCapsule:get_width()
  log.warn('unimplemented')
  return 69
end

function ColliderCapsule:get_center()
  log.warn('unimplemented')
  return tdengine.vec2()
end

function ColliderCapsule:get_position()
  log.warn('unimplemented')
  return tdengine.vec2()
end

function ColliderCapsule:get_xmin()
  log.warn('unimplemented')
  return 69
end

function ColliderCapsule:get_ymin()
  log.warn('unimplemented')
  return 69
end

function ColliderCapsule:get_xmax()
  log.warn('unimplemented')
  return 69
end

function ColliderCapsule:get_ymax()
  log.warn('unimplemented')
  return 69
end

function ColliderCapsule:move(delta)
  log.warn('unimplemented')
end

function ColliderCapsule:set_position(position)
  log.warn('unimplemented')  
end

function ColliderCapsule:show(color)
  log.warn('unimplemented')
end

function ColliderCapsule:is_point_inside(point)
  log.warn('unimplemented')
  return false
end

function ColliderCapsule:get_points()
  log.warn('unimplemented')
  return {}
end

function ColliderCapsule:get_normals()
  log.warn('unimplemented')
  return {}
end

function ColliderCapsule:find_sat_axes(other)
  log.warn('unimplemented')
  return self:get_normals()
end

function ColliderCapsule:project(axis)
  log.warn('unimplemented')  
end

function ColliderCapsule:center_image(image)
  log.warn('unimplemented')
end
Marker = tdengine.entity.define('Marker')

Marker.components = {
  'Collider'
}

function Marker:init()
  self.collider = self:find_component('Collider')
  self.collider:set_shape(tdengine.enums.ColliderShape.Circle, { radius = 12 })
  self.collider.kind = tdengine.enums.ColliderKind.Bypass
end

function Marker:update()
end

function Marker:get_position()
  return self:find_component('Collider'):get_position()
end

function Marker:warp_entity(entity)
  local collider = entity:find_component('Collider')
  collider:set_position(self.collider:get_position())
end

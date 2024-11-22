Interaction = tdengine.component.define('Interaction')

Interaction.editor_fields = {
  'callback',
  'align_position',
  'align_size',
  'collider',
  'align_offset'
}

function Interaction:init(params)
  params = params or {}

  -- @hack WEIRD_COLLIDER_CONSTRUCTION
  -- @refactor
  params.collider = params.collider or {}
  self.collider = add_component(nil, 'Collider', {
    shape = params.collider.shape or Collider.shapes.Circle,
    kind = tdengine.enums.ColliderKind.Static,
    impl = params.collider.impl
  })

  self.callbacks = tdengine.data_types.array:new()

  self.align_offset = tdengine.vec2()

  if params.align_position == nil then
    self.align_position = true
  else
    self.align_position = params.align_position
  end

  if params.align_size == nil then
    self.align_size = false
  else
    self.align_size = params.align_size
  end
end

function Interaction:update()
  self:check_align()
end

function Interaction:add_callback(callback)
  self.callbacks:add(callback)
end

function Interaction:on_interaction()
  for _, callback in self.callbacks:iterate() do
    callback()
  end
end

function Interaction:check_align()
  if self.align_position then self:align_position_to_entity() end
  if self.align_size then self:align_size_to_entity() end
end

function Interaction:align_size_to_entity()
  local collider = self:get_entity():find_component('Collider')
  local w = collider:get_width()
  local h = collider:get_height()
  self.collider.impl:set_radius((w + h) / 2 * .75)
end

function Interaction:align_position_to_entity()
  local collider = self:get_entity():find_component('Collider')
  local center = collider:get_center():add(self.align_offset)
  self.collider:set_position(center)
end

Collider = dn.component.define('Collider')

dn.enum.define(
	'ColliderShape',
	{
		Box = 0,
		Circle = 1,
		Capsule = 2,
	}
)

Collider.kinds = dn.enum.define(
	'ColliderKind',
	{
		Static = 0,
		Dynamic = 1,
		Bypass = 2,
		CameraBounds = 3,
		Trigger = 4,
		Interaction = 5
	}
)

Collider.editor_fields = { 
	'attached', 
	'attach_offset', 
}

function Collider:init(params)
	local shape = dn.enum.load(params.shape) or dn.enums.ColliderShape.Box
	self:set_shape(shape, params.impl)

	self.kind = dn.enum.load(params.kind) or dn.enums.ColliderKind.Static
	self.attached = params.attached or ''
	self.attach_offset = params.attach_offset or dn.vec2()

	if params.world_space ~= nil then
		self.world_space = params.world_space
	else
		self.world_space = true
	end

	local editor_callbacks = {
    on_change_field = function(field) self:on_change_field(field) end,
  }
  dn.editor.set_editor_callbacks(self, editor_callbacks)
end

function Collider:on_change_field(field)
	if field == 'shape' then
		self:set_shape(self.shape)
	end
end

function Collider:is_attached() return #self.attached > 0 end

function Collider:attach(entity_uuid, attach_offset)
	self.attached = entity_uuid
	self.attach_offset = attach_offset or dn.vec2()
	self:update()
end

function Collider:unattach()
	self.attached = nil
	self.attach_offset = dn.vec2()
	self:update()
end

function Collider:update()
	if self.attached and #self.attached > 0 then
		local entity = dn.find_entity_by_uuid(self.attached)
		if not entity then return end

		local collider = entity:find_component('Collider')
		if not collider then return end

		self:set_position(collider:get_position():add(self.attach_offset))
	end
end

function Collider:distance(other)
	local pa = self:get_position()
	local pb = other:get_position()
	return pa:distance(pb)
end

function Collider:is_world_space() return self.world_space end

function Collider:set_draw_space()
	if self:is_world_space() then
		dn.unported.set_world_space(true)()
	else
		dn.unported.end_world_space(true)()
	end
end

function Collider:set_shape(shape, params)
	-- If anything is missing from the parameters, pull it from the current collider.
	params = params or self.impl
	params = params or {}
	params.position = params.position or self.impl and self.impl.position

	self.shape = shape
	if self.shape == dn.enums.ColliderShape.Box then
		self.impl = ColliderBox:new(params)
	elseif self.shape == dn.enums.ColliderShape.Circle then
		self.impl = ColliderCircle:new(params)
	elseif self.shape == dn.enums.ColliderShape.Capsule then
		self.impl = ColliderCapsule:new(params)
	end

	self.impl.base = self
end

function Collider:set_kind(kind) self.kind = kind end

function Collider:serialize()
	local data = {}
	data.kind = self.kind
	data.shape = self.shape
	data.world_space = self.world_space
	data.impl = self.impl:serialize()

	return data
end

function Collider:make(data)
	local collider = add_component(
		nil, 'Collider', data or {
			shape = dn.enums.ColliderShape.Circle,
			world_space = false,
			impl = { position = dn.vec2(100, 100), radius = 100 }
		})
	return collider
end

-------------
-- DRAWING --
-------------
function Collider:set_color(color) self.color = color end

function Collider:show(color)
	color = color or self.color
	self.impl:show(color)
end

---------------------
-- VIRTUAL METHODS --
---------------------

-------------
-- GETTERS --
-------------
function Collider:get_height() return self.impl:get_height() end

function Collider:get_width() return self.impl:get_width() end

function Collider:get_dimension()
	return dn.vec2(self.impl:get_width(), self.impl:get_height())
end

function Collider:get_center() return self.impl:get_center() end

function Collider:get_position() return self.impl:get_position() end

function Collider:get_xmin() return self.impl:get_xmin() end

function Collider:get_xmax() return self.impl:get_xmax() end

function Collider:get_ymin() return self.impl:get_ymin() end

function Collider:get_ymax() return self.impl:get_ymax() end

function Collider:get_points() return self.impl:get_points() end

function Collider:get_normals() return self.impl:get_normals() end

function Collider:find_sat_axes(other) return self.impl:find_sat_axes(other) end

function Collider:project(axis) return self.impl:project(axis) end

function Collider:center_image(image) return self.impl:center_image(image) end

function Collider:get_impl() return self.impl end

------------
-- UPDATE --
------------
function Collider:move(delta) return self.impl:move(delta) end

function Collider:set_position(position) return self.impl:set_position(position) end

---------------
-- COLLISION --
---------------
function Collider:is_point_inside(point) return self.impl:is_point_inside(point) end

function Collider:is_cursor_inside()
	local coord = ternary(
		self:is_world_space(), dn.coordinate.world,
		dn.coordinate.game)
	local mouse = dn.vec2(dn.cursor(coord))
	return self.impl:is_point_inside(mouse)
end
CircleEditor = tdengine.class.define('CircleEditor')

CircleEditor.states = tdengine.enum.define(
	'CircleEditorStates',
	{
		Idle = 0,
		Resize = 1,
		ResizeInterpolate = 2,
		ResizeHoverWait = 3,
		ResizeHover = 4,
		ResizeHoverHide = 5,
	}
)

function CircleEditor:init(params)
	self.editor = params
	self.state = self.states.Idle

	self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.World)

	-- When you're just moving your mouse across the screen, it's really annoying to have the
	-- colliders flicker as you move over the resize band. This just adds a slight delay where
	-- if you're not hovered over the band for at least min_resize_hover_time, then it will
	-- not draw the resize widget
	self.resize = {
		click_position = tdengine.vec2(),
		min_hover_time = 0.075,
		hover_time = 0,
		select_epsilon = 8,
		old_radius = 0,
		new_radius = 0,
		collider = nil,
		interpolation = {
			size = tdengine.interpolation.EaseOut:new({ time = 1, exponent = 3 }),
			preview_alpha = tdengine.interpolation.EaseOut:new({ time = 1, exponent = 3 }),
			hover = tdengine.interpolation.EaseInOut:new({ time = .5, exponent = 3 }),
		},
		preview_alpha = .25,
		hover_alpha = .5
	}
end

function CircleEditor:update()
	if self.state == self.states.Idle then
		for collider in self:iterate_colliders() do
			if self:is_resize_hovered(collider) then
				self.resize.collider = collider
				self.resize.hover_time = 0
				self.state = self.states.ResizeHoverWait
				return
			end
		end

	elseif self.state == self.states.ResizeHoverWait then
		if not self:is_resize_hovered(self.resize.collider) then
			self.state = self.states.Idle
			return
		end

		self.resize.hover_time = self.resize.hover_time + tdengine.dt
		if self.resize.hover_time >= self.resize.min_hover_time then
			self.resize.interpolation.hover:set_start(0)
			self.resize.interpolation.hover:set_target(.5)
			self.resize.interpolation.hover:reset()

			self.state = self.states.ResizeHover
		end

	elseif self.state == self.states.ResizeHover then
		if not self:is_resize_hovered(self.resize.collider) then
			self.resize.interpolation.hover:set_start(self.resize.hover_alpha)
			self.resize.interpolation.hover:set_target(0)
			self.resize.interpolation.hover:reset()

			self.state = self.states.ResizeHoverHide
		end

		self.resize.interpolation.hover:update()
		self.resize.hover_alpha = self.resize.interpolation.hover:get_value()

		self:draw_resize_hover()

	elseif self.state == self.states.ResizeHoverHide then
		if self.resize.interpolation.hover:update() then
			self.state = self.states.Idle
		end

		self.resize.hover_alpha = self.resize.interpolation.hover:get_value()

		self:draw_resize_hover()

	elseif self.state == self.states.Resize then
		if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
			self.resize.interpolation.preview_alpha:set_start(.25)
			self.resize.interpolation.preview_alpha:set_target(0)
			self.resize.interpolation.preview_alpha:reset()

			self.resize.interpolation.size:set_start(self.resize.collider.impl.radius)
			self.resize.interpolation.size:set_target(self.resize.new_radius)
			self.resize.interpolation.size:reset()
			self.state = self.states.ResizeInterpolate
			return
		end

		local cursor = self.input:mouse()
		local center = self.resize.collider:get_position()
		local click_radius = center:distance(self.resize.click_position)
		local current_radius = center:distance(cursor)
		local radius_delta = current_radius - click_radius
		self.resize.new_radius = self.resize.old_radius + radius_delta

		self:draw_resize_preview()

	elseif self.state == self.states.ResizeInterpolate then
		if self.resize.interpolation.size:update() then
			self.state = self.states.Idle
		end

		self.resize.collider.impl:set_radius(self.resize.interpolation.size:get_value())

		self.resize.interpolation.preview_alpha:update()
		self.resize.preview_alpha = self.resize.interpolation.preview_alpha:get_value()
		self:draw_resize_preview()
	end
end

function CircleEditor:is_resize_hovered(collider)
	local circle = collider.impl
	local position = collider:get_position()
	local cursor = self.input:mouse()
	local distance = math.abs(position:distance(cursor))
	local radius = circle:get_radius()

	local greater = distance >= radius
	local less = distance <= radius + self.resize.select_epsilon

	return greater and less
end

function CircleEditor:should_draw_resize(collider)
	return self:is_resize_hovered(collider) and self.resize.hover_time > self.resize.min_hover_time
end

function CircleEditor:draw_resize_hover()
	local circle = self.resize.collider.impl

	tdengine.ffi.set_world_space(true)
	tdengine.ffi.set_layer(tdengine.editor.layers.collider_overlay)
	tdengine.ffi.draw_ring_sdf(circle.position.x, circle.position.y, circle.radius, circle.radius + self.resize.select_epsilon, tdengine.colors.spring_green:alpha(self.resize.hover_alpha):to_vec4(), 1)
end

function CircleEditor:draw_resize_preview()
	local circle = self.resize.collider.impl

	tdengine.ffi.set_world_space(true)
	tdengine.ffi.set_layer(tdengine.editor.layers.collider_overlay)
	tdengine.ffi.draw_circle_sdf(circle.position.x, circle.position.y, self.resize.new_radius, tdengine.colors.spring_green:alpha(self.resize.preview_alpha):to_vec4(), 1)
end

function CircleEditor:iterate_colliders()
	local function iterator()
		for collider in tdengine.component.iterate('Collider') do
			if collider.shape == tdengine.enums.ColliderShape.Circle then
				coroutine.yield(collider)
			end
		end
	end

  return coroutine.wrap(iterator)
end

function CircleEditor:try_consume()
	for collider in self:iterate_colliders() do
		if self:is_resize_hovered(collider) then
			self.resize.collider = collider
			self.resize.old_radius = collider.impl.radius
			self.resize.new_radius = collider.impl.radius
			self.resize.click_position = self.input:mouse()
			self.resize.preview_alpha = .25
			self.state = self.states.Resize
			return true
		end
	end

	return false
end

function CircleEditor:is_consuming_input()
	if self.state == self.states.Resize then return true end
	if self.state == self.states.ResizeInterpolate then return true end

	return false
end









---------
-- BOX --
---------
BoxEditor = tdengine.class.define('BoxEditor')

BoxEditor.states = tdengine.enum.define(
	'BoxEditorState',
	{
		Idle = 0,
		Resize = 1,
		ResizeInterpolate = 2,
		ResizeHoverWait = 3,
		ResizeHover = 4,
		ResizeHoverHide = 5,
	}
)

function BoxEditor:init(params)
	self.state = self.states.Idle
	self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.World)

	self.resize = {
		size_delta = tdengine.vec2(),
		new_size = tdengine.vec2(),
		corner_radius = 4,
		collider = nil,
		corner = tdengine.vec2(),
		corners = {},
		click_position = tdengine.vec2(),
		hover_alpha = 0.5,
		hover_time = 0,
		min_hover_time = .1,
		preview_alpha = 0.5,
		interpolation = {
			size = tdengine.interpolation.EaseOut2:new({ time = 1, exponent = 3 }),
			preview_alpha = tdengine.interpolation.EaseOut:new({ time = 1, exponent = 3 }),
			hover = tdengine.interpolation.EaseOut:new({ time = .5, exponent = 3 }),
		},
	}
end

function BoxEditor:check_for_hovered_corner()
	for collider in self:iterate_colliders() do

		local corner, corners = self:find_hovered_corner(collider)
		if corner then
			self.resize.collider = collider
			self.resize.corner = corner
			self.resize.corners = corners
			return true
		end

	end

	return false
end

function BoxEditor:interpolate_alpha_to(alpha)
	self.resize.interpolation.hover:set_start(self.resize.interpolation.hover:get_value())
	self.resize.interpolation.hover:set_target(alpha)
	self.resize.interpolation.hover:reset()
end

function BoxEditor:interpolate_preview_alpha_to(alpha)
	self.resize.interpolation.preview_alpha:set_start(self.resize.interpolation.preview_alpha:get_value())
	self.resize.interpolation.preview_alpha:set_target(alpha)
	self.resize.interpolation.preview_alpha:reset()
end

function BoxEditor:interpolate_size_to(size)
	self.resize.interpolation.size:set_start(self.resize.collider:get_dimension())
	self.resize.interpolation.size:set_target(size)
	self.resize.interpolation.size:reset()
end



function BoxEditor:update()
	if self.state == self.states.Idle then
	
		if self:check_for_hovered_corner() then
			self.resize.hover_time = 0
			self.state = self.states.ResizeHoverWait
		end

	-- RESIZE HOVER WAIT
	elseif self.state == self.states.ResizeHoverWait then
		if not self:is_corner_hovered(self.resize.corner) then
			self.state = self.states.Idle
			return
		end

		self.resize.hover_time = self.resize.hover_time + tdengine.dt
		if self.resize.hover_time >= self.resize.min_hover_time then
			self:interpolate_alpha_to(.5)
			self.state = self.states.ResizeHover
		end

	-- RESIZE HOVER
	elseif self.state == self.states.ResizeHover then
		if not self:is_corner_hovered(self.resize.corner) then
			self:interpolate_alpha_to(0)
			self.state = self.states.ResizeHoverHide
		end

		self.resize.interpolation.hover:update()
		self:draw_corners()

	-- RESIZE HOVER HIDE
elseif self.state == self.states.ResizeHoverHide then
		if self:check_for_hovered_corner() then
			self:interpolate_alpha_to(.5)
			self.state = self.states.ResizeHover
		end

		self.resize.interpolation.hover:update()
		self:draw_corners()

		if self.resize.interpolation.hover:is_done() then
			self.state = self.states.Idle
		end

-- RESIZE 
elseif self.state == self.states.Resize then
		self.resize.interpolation.preview_alpha:update()

		local mouse = self.input:mouse()
		local delta = mouse:subtract(self.resize.click_position)
		self.resize.size_delta.x = delta.x
		self.resize.size_delta.y = -delta.y
		self.resize.new_size = self.resize.collider:get_dimension():add(self.resize.size_delta)
		self:draw_resize_preview()

		if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
			self:interpolate_preview_alpha_to(0)
			self:interpolate_size_to(self.resize.new_size)
			self.state = self.states.ResizeInterpolate
			return
		end

		if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
			self.state = self.states.Idle
		end

-- RESIZE INTERPOLATE
elseif self.state == self.states.ResizeInterpolate then
		if self.resize.interpolation.size:update() then
			if self:check_for_hovered_corner() then
				self:interpolate_alpha_to(.5)
				self.state = self.states.ResizeHover
			else
				self.state = self.states.Idle
			end
		end

		local box = self.resize.collider.impl
		box.dimension:assign(self.resize.interpolation.size:get_value())

		self.resize.interpolation.preview_alpha:update()
		self:draw_resize_preview()
	end
end

function BoxEditor:iterate_colliders()
	local iterator = function()
		for collider in tdengine.component.iterate('Collider') do
			if collider.shape == tdengine.enums.ColliderShape.Box then
				coroutine.yield(collider)
			end
		end
	end

	return coroutine.wrap(iterator)
end

function BoxEditor:draw_resize_preview()
	local preview_alpha = self.resize.interpolation.preview_alpha:get_value()

	tdengine.ffi.set_world_space(true)
	tdengine.ffi.set_layer(tdengine.editor.layers.collider_overlay)

	local p = self.resize.collider:get_position()
	tdengine.ffi.draw_quad(p.x, p.y, self.resize.new_size.x, self.resize.new_size.y, tdengine.colors.spring_green:alpha(preview_alpha):to_vec4())
end

function BoxEditor:draw_corners()
	local hover_alpha = self.resize.interpolation.hover:get_value()

	tdengine.ffi.set_world_space(true)
	tdengine.ffi.set_layer(tdengine.editor.layers.collider_overlay)
	for _, corner in pairs(self.resize.corners) do
		tdengine.ffi.draw_circle_sdf(corner.x, corner.y, self.resize.corner_radius, tdengine.colors.spring_green:alpha(hover_alpha):to_vec4(), 1)
	end
end


function BoxEditor:is_corner_hovered(corner)
	local mouse = self.input:mouse()
	local distance = corner:distance(mouse)
	return distance < self.resize.corner_radius
end

function BoxEditor:find_hovered_corner(collider)
	local box = collider.impl
	local corners = box:get_corners()

	for _, corner in pairs(corners) do
		if self:is_corner_hovered(corner) then
			return corner, corners
		end
	end

	return nil, corners
end


function BoxEditor:try_consume()
	if self:check_for_hovered_corner() then
		self.resize.click_position = self.input:mouse()
		self.resize.size_delta = tdengine.vec2()
		self.resize.new_size = tdengine.vec2()
		self:interpolate_preview_alpha_to(.25)

		self.resize.interpolation.hover:set_start(0)
		self.resize.interpolation.hover:set_target(.25)
		self.resize.interpolation.hover:reset()
	
		self.state = self.states.Resize

		return true
	end

	return false
end

function BoxEditor:is_consuming_input()
	if self.state == self.states.Resize then return true end
	if self.state == self.states.ResizeInterpolate then return true end

	return false
end



CapsuleEditor = tdengine.class.define('CapsuleEditor')

CapsuleEditor.States = tdengine.enum.define(
	'CapsuleEditorState',
	{
		Idle = 0
	}
)

function CapsuleEditor:init()
	self.state = self.states.Idle
	self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.World)
end

function CapsuleEditor:iterate_colliders()
	local iterator = function()
		for collider in tdengine.component.iterate('Collider') do
			if collider.shape == tdengine.enums.ColliderShape.Capsule then
				coroutine.yield(collider)
			end
		end
	end

	return coroutine.wrap(iterator)
end


--
-- COLLIDER EDITOR
--
local ColliderEditor = tdengine.editor.define('ColliderEditor')

ColliderEditor.states = tdengine.enum.define(
	'ColliderEditorState',
	{
		Idle = 0,
		DragCollider = 1,
		Shape = 2,
	}
)

function ColliderEditor:init()
	self.__editor_controls = {
		use_light_mode = false
	}

	self.style = {
		dark = {
			selected = tdengine.colors.paynes_gray:alpha(0.5),
			hovered = tdengine.colors.paynes_gray:alpha(0.5),
			idle = tdengine.colors.cadet_gray:alpha(0.5),
		},
		light = {
			selected = tdengine.colors.spring_green:alpha(0.5),
			hovered = tdengine.colors.spring_green:alpha(0.5),
			idle = tdengine.colors.white:alpha(0.5),
		}
	}

	self.metadata = {}
	self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.World)
	self.drag_state = {}

	self.shape_editors = {
		circle = CircleEditor:new(),
		box = BoxEditor:new()
	}

	self.state = self.states.Idle
end

function ColliderEditor:update()
	self:update_shape_editors()

	if self.state == self.states.Idle then
		self:rebuild_metadata()

	elseif self.state == self.states.DragCollider then
		if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
      self.state = self.states.Idle
    end

    self.drag_state.collider:move(self.input:mouse_delta())

	elseif self.state == self.states.Shape then
		self.active_shape:update()

		if not self.active_shape:is_consuming_input() then
			self.state = self.states.Idle
		end
	end
end


function ColliderEditor:draw()
	tdengine.ffi.set_world_space(true)
	tdengine.ffi.set_layer(tdengine.editor.layers.colliders)

	for collider in tdengine.component.iterate('Collider') do
		local color = self:on_color(collider)
		collider:show(color)
	end
end

function ColliderEditor:update_shape_editors() 
	for _, shape_editor in pairs(self.shape_editors) do
		shape_editor:update()
	end
end


function ColliderEditor:rebuild_metadata()
	local colliders = tdengine.component.collect('Collider')

	local live_colliders = {}
	for _, collider in colliders:iterate() do
		-- Mark the collider as live (in a map, for easy lookup)
		live_colliders[collider.id] = true

		-- Add an entry if we haven't seen this one before
		self.metadata[collider.id] = self.metadata[collider.id] or {
			collider = collider,
			hovered = false,
			alive = true
		}
	end

	-- Remove any metadata entries that don't have a collider
	for id, _ in pairs(self.metadata) do
		if not live_colliders[id] then
			self.metadata[id] = nil
		end
	end

	-- Update for every collider
	local mouse = self.input:mouse()
	for collider in tdengine.component.iterate('Collider') do
		self.metadata[collider.id].hovered = collider:is_point_inside(mouse)
	end
end

function ColliderEditor:find_hits(position)
	local hits = tdengine.data_types.Array:new()
	for collider in tdengine.component.iterate('Collider') do
		if collider:is_point_inside(position) then
			hits:add(collider)
		end
	end

	return hits
end

function ColliderEditor:on_color(collider)
	local metadata = self.metadata[collider.id]

	local colors = self.style.dark
	if self.__editor_controls.use_light_mode then
		colors = self.style.light
	end

	if tdengine.find_entity_editor('EntitySelection'):is_collider_selected(collider) then
		return colors.selected
	elseif metadata.hovered then
		return colors.hovered
	else
		return colors.idle
	end
end


function ColliderEditor:try_consume()
	local mouse = self.input:mouse()

	for _, shape_editor in pairs(self.shape_editors) do
		if shape_editor:try_consume() then
			self.active_shape = shape_editor
			self.state = self.states.Shape
			return true
		end
	end

	local hits = self:find_hits(mouse)
	if not hits:is_empty() then
		self.drag_state.collider = hits:back()
		tdengine.find_entity_editor('EntitySelection'):select_collider(hits:back())

		self.state = self.states.DragCollider
		return true
	end


	return false
end

function ColliderEditor:is_consuming_input()
	return self.state ~= self.states.Idle
end



local editor_state = {
	idle = 'idle',
	deleting = 'deleting',
	single_select = 'single_select',
	multiselect = 'multiselect',
	separate_line_drag = 'separate_line_drag',
	separate_box_wait = 'separate_box_wait',
	separate_box_drag = 'separate_box_drag',
}

local sep_direction = {
	greater = 'greater',
	less_than = 'less_than',
}

local sep_kind = {
	x = 'x',
	y = 'y',
	box = 'box'
}

NodeEditor = tdengine.class.define('NodeEditor')

function NodeEditor:init(params)
	self.name = params.name or 'Node Editor'
	self.node_kinds = params.node_kinds or {}
	self.nodes = {}
	self.gnodes = {}
	self.camera = tdengine.vec2(0, 0)
	self.scroll_per_second = params.scroll_per_second or 3000
	self.window_position = tdengine.vec2(0, 0)
	self.window_size = tdengine.vec2(0, 0)
	self.node_padding = tdengine.vec2(8, 8)
	self.inter_node_padding = 25 -- If we automatically add a node, how far is it from the previous?
	self.canvas_rclick_pos = tdengine.vec2(0, 0)
	self.hovered = nil
	self.state = editor_state.idle
	self.separators = {}
	self.selected = {}
	self.connecting = nil
	self.disconnecting = nil
	self.deleting = nil
	self.rerouting = nil
	self.clipboard = { nodes = {}, gnodes = {} }
	self.node_queue = Queue:new(5)
	self.link_style = {
		thickness = 3,
		color = tdengine.color32(200, 200, 200, 255),
		disconnect_color = tdengine.color32(255, 0, 0, 255),
	}
	self.zoom = 1
	self.speed = {
		zoom = .25,
		node_move = 1,
	}

	self.colors = {
		background = tdengine.colors.rich_black:copy(),
		grid_line = tdengine.colors.white:alpha(.25),
		disconnect = tdengine.colors.cardinal:copy(),
		link = tdengine.colors.white:copy(),
	}

	self.style = {
		grid_thickness = 1
	}


	self.input = ContextualInput:new(tdengine.enums.InputContext.Editor, tdengine.enums.CoordinateSystem.Game)
	self.input_cache        = {}

	self.on_node_lclick     = params.on_node_lclick or function() end
	self.on_node_add        = params.on_node_add or function() end
	self.on_node_delete     = params.on_node_delete or function() end
	self.on_node_select     = params.on_node_select or function() end
	self.on_node_hover      = params.on_node_hover or function() end
	self.on_node_connect    = params.on_node_connect or function() end
	self.on_node_disconnect = params.on_node_disconnect or function() end
	self.on_node_draw       = params.on_node_draw or function() end
	self.on_node_color      = params.on_node_color or function() end
	self.on_node_context    = params.on_node_context or function() end
	self.on_node_links      = params.on_node_links or function() end
	self.on_node_paste      = params.on_node_paste or function() end
	self.on_graph_copy      = params.on_graph_copy or function() end
	self.on_node_copy       = params.on_node_copy or function() end
end

------------
-- UPDATE --
------------
function NodeEditor:update(dt)
	local remove = tdengine.data_types.array:new()
	for id, node in pairs(self.nodes) do
		if self.gnodes[id] == nil then
			remove:add(id)
		end
	end

	for id, gnode in pairs(self.gnodes) do
		if self.nodes[id] == nil then
			remove:add(id)
		end
	end

	for index, id in remove:iterate() do
		self.nodes[id] = nil
		self.gnodes[id] = nil
	end

	self:begin_canvas()

	-- @hack: This is the most expensive part of the editor, and when zoomed out definitely makes the FPS drop
	-- below acceptable. Instead of fixing that, I just skip all the expensive stuff when the game tab is
	-- open
	if not tdengine.editor.is_window_focused('Game') then
		self:calculate_visible_nodes()
		self:draw_canvas()

		self:update_input()
		self:update_translation(dt)
		self:update_zoom()
		self:update_state()
		self:update_copy_paste()
		self:update_nodes()

		self:node_context_menu()
		self:canvas_context_menu()
	end

	self:end_canvas()
end

function NodeEditor:update_input()
	self.window_size = tdengine.vec2(imgui.GetWindowSize()) 

	self.input_cache.focused = imgui.IsWindowFocused()
	self.input_cache.hovered = imgui.IsWindowHovered()
	self.input_cache.left_drag = imgui.IsMouseDragging(0, 0)
	self.input_cache.middle_drag = imgui.IsMouseDragging(2, 0)
	self.input_cache.mouse = tdengine.vec2(imgui.GetMousePos())
	self.input_cache.mouse_delta = self.input:mouse_delta()
	self.input_cache.mouse_delta.y = -self.input_cache.mouse_delta.y
	self.input_cache.mouse_drag = imgui.IsMouseDragging(0)
	self.input_cache.scroll = self.input:scroll()
end

function NodeEditor:update_nodes()
	for id, node in pairs(self.nodes) do
		local gnode = self.gnodes[id]

		-- Left click de/selects the node, plus finishes up contextual actions
		-- like connecting nodes
		if gnode.left_clicked then
			if self.input:mod_down(glfw.keys.CONTROL) then
				if self.state == editor_state.multiselect then
					self:toggle_node_selected(id)
				end
			else
				self:select_single_node(id)
				self.on_node_lclick(id)

				if self.connecting then
					local parent = self.nodes[self.connecting]
					table.insert(parent.children, id)
					self.on_node_connect(self.connecting, id)
					self.connecting = nil
				end

				if self.disconnecting then
					local parent = self.nodes[self.disconnecting]
					delete(parent.children, id)
					self.on_node_connect(self.disconnecting, id)
					self.disconnecting = nil
				end

				-- Rerouting: Pretty often, I want to put a node between two already connected nodes. Instead of having
				-- to disconnect, disconnect, reconnect, reconnect, I can just reroute. Rerouting puts a newly created
				-- node in the middle of two connected nodes.
				--
				-- I only use this for new nodes, so I don't handle any cases where the node in the middle already has
				-- children, or where this would cause multiple children.
				if self.rerouting then
					local rerouting = self.nodes[self.rerouting]

					if #rerouting.children == 1 and #node.children == 0 then
						local old_child_id = rerouting.children[1]
						local old_child = self.nodes[old_child_id]

						rerouting.children[1] = id
						node.children[1] = old_child_id

						self.on_node_connect(self.rerouting, id)
						self.on_node_connect(id, old_child_id)
					end

					self.rerouting = nil
				end
			end
		end

		-- If the node is held, apply mouse drag to move
		if gnode.active and self.input_cache.mouse_drag then
			local gnode = self.gnodes[id]
			local delta = self.input:mouse_delta():scale(1 / self.zoom)
			delta.y = -delta.y
			gnode.position.x = gnode.position.x + delta.x
			gnode.position.y = gnode.position.y + delta.y
		end

		if gnode.hovered then
			self.hovered = id
		end
	end
end

function NodeEditor:update_state()
	self.hovered = nil

	if self.state == editor_state.idle then
		-- IDLE
		self:check_multiselect_hotkeys()
	elseif self.state == editor_state.single_select then
		-- SINGLE SELECT
		if self.input:pressed(glfw.keys.DELETE) then
			self:delete_selection()
		end

		if self.input:pressed(glfw.keys.ESCAPE) then
			self:clear_selection()
			self.state = editor_state.idle
		end

		if self.input:chord_pressed(glfw.keys.ALT, glfw.keys.ENTER) then
			local size = self.node_queue:size()
			if size > 1 then
				local child = self.nodes[self.node_queue:peek_at(size)]
				local parent = self.nodes[self.node_queue:peek_at(size - 1)]
				if child and parent then
					table.insert(parent.children, self.node_queue:peek_at(size))

					local child_gnode = self.gnodes[child.uuid]
					local parent_gnode = self.gnodes[parent.uuid]
					child_gnode.position = table.deep_copy(parent_gnode.position)
					child_gnode.position.x = child_gnode.position.x + parent_gnode.size.x + self.inter_node_padding

					if self.on_node_connect then
						self.on_node_connect(parent.uuid, child.uuid)
					end
				end
			end
		end

		self:check_multiselect_hotkeys()
	elseif self.state == editor_state.deleting then
		-- DELETING
		self:delete_selection()
		self.state = editor_state.idle
	elseif self.state == editor_state.multiselect then
		-- MULTISELECT
		local stop = false

		self:check_move_nodes()
		self:check_multiselect_hotkeys()

		-- Check for deletion
		if self.input:pressed(glfw.keys.DELETE) then
			stop = true
			self:delete_selection()
		end

		-- Tab inverts the selection
		if self.input:pressed(glfw.keys.TAB) then
			for uuid, _ in pairs(self.gnodes) do
				self:toggle_node_selected(uuid)
			end
		end

		-- Exit multiselect if:
		--   1) We selected a single node
		--   2) We pressed escape
		--   3) We deleted our solection
		stop = stop or self.input:pressed(glfw.keys.ESCAPE)
		stop = stop
		if stop then
			self:end_multiselect()
			self.state = editor_state.idle
			return
		end
	elseif self.state == editor_state.separate_line_drag then
		-- SEPARATE LINE DRAG
		-- Multiselecting with a draggable, axis-aligned line
		imgui.extensions.CursorBubble()

		if self.input:pressed(glfw.keys.ESCAPE) then
			self:cancel_separator()
			return
		end

		if self.input:pressed(glfw.keys.TAB) then
			self:toggle_separator_direction()
		end

		-- Update the separator to be where the mouse is, and then update
		-- which nodes it selects
		local separator = self.separators[#self.separators]
		separator.begin = self:zoom_to_world(self.input_cache.mouse)

		self:do_multiselect()

		if self.input:pressed(glfw.keys.MOUSE_BUTTON_1) then
			self.state = editor_state.multiselect
		end
	elseif self.state == editor_state.separate_box_wait then
		-- SEPARATE BOX WAIT
		-- Wait for the user to click where the top left of the box'll be
		imgui.extensions.CursorBubble()
		if imgui.IsMouseClicked(0) then
			local separator = {
				tl = self.input_cache.mouse,
				scroll = self.camera,
				kind = sep_kind.box
			}
			table.insert(self.separators, separator)
			self.state = editor_state.separate_box_drag
		end
	elseif self.state == editor_state.separate_box_drag then
		-- SEPARATE BOX DRAG
		-- Multiselect with a box
		imgui.extensions.CursorBubble()

		if self.input:pressed(glfw.keys.ESCAPE) then
			self:cancel_separator()
			return
		end

		local separator = self.separators[#self.separators]
		separator.br = tdengine.vec2(imgui.GetMousePos())
		self:do_multiselect()

		local color = tdengine.color32(200, 255, 200, 30)
		local ds = self.camera:subtract(separator.scroll)
		local min = separator.tl:add(ds)
		imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(min.x, min.y), imgui.ImVec2(separator.br.x, separator.br.y), color, 0)

		if imgui.IsMouseClicked(0) then
			self.state = editor_state.multiselect
		end
	end
end

-------------
-- DRAWING --
-------------
function NodeEditor:draw_canvas()
	imgui.GetWindowDrawList():ChannelsSplit(3)
	self:draw_grid()
	self:draw_nodes()
	imgui.GetWindowDrawList():ChannelsMerge()
end

function NodeEditor:draw_grid()
	-- The grid is calculated in world space. I like to think of it this way. Each line on the grid refers to
	-- a canonical space in the world -- it's just a matter of where that goes on the screen. It's the same problem
	-- as drawing nodes, where we store world space and then need a location on the window to draw it.
	--
	-- So, we do all of our calculations in world space, and then just translate it to zoom space for the DrawList API
	local wx, wy = imgui.GetWindowSize()
	local grid = {
		size = 64
	}

	-- @hack: The grid just gets too dense to be useful at high zoom
	if self.zoom < .15 then
		grid.size = 256
	end

	grid.min = tdengine.vec2(
		self.camera.x % grid.size - self.camera.x - grid.size, -- Start at the closest place on the grid behind the camera
		self.camera.y % grid.size - self.camera.y - grid.size
	)
	grid.max = tdengine.vec2(
		grid.min.x + self.window_size.x / self.zoom + grid.size,
		grid.min.y + self.window_size.y / self.zoom + grid.size
	)

	for x = grid.min.x, grid.max.x, grid.size do
		local top = self:world_to_zoom(tdengine.vec2(x, grid.min.y))
		local bottom = self:world_to_zoom(tdengine.vec2(x, grid.max.y))
		imgui.GetWindowDrawList():AddLine(imgui.ImVec2(top.x, top.y), imgui.ImVec2(bottom.x, bottom.y), self.colors.grid_line:to_u32(), self.style.grid_thickness)
	end

	for y = grid.min.y, grid.max.y, grid.size do
		local left = self:world_to_zoom(tdengine.vec2(grid.min.x, y))
		local right = self:world_to_zoom(tdengine.vec2(grid.max.x, y))
		imgui.GetWindowDrawList():AddLine(imgui.ImVec2(left.x, left.y), imgui.ImVec2(right.x, right.y), self.colors.grid_line:to_u32(), self.style.grid_thickness)
	end
end

function NodeEditor:draw_nodes()
	for id, node in pairs(self.nodes) do
		-- GUI data stored separately from actual game data
		local gnode = self.gnodes[id]

		if not gnode.visible then goto continue end

		local node_rect_min = self:world_to_zoom(gnode.position)
		local node_contents_cursor = node_rect_min:add(self.node_padding:scale(self.zoom))

		-- CONTENTS
		-- We store a callback, so the owner of this node editor can call whatever ImGui stuff to fill up
		-- the node. Then, we calculate the node's final size as (contents size + top padding + bottom padding),
		-- all scaled to the correct zoom level
		--
		-- Tricky: ImGui only tells us how many pixels a thing is. But we want to use this size to calculate
		-- whether nodes are visible. We use world coordinates for that (i.e. define a viewport in terms of
		-- the world coordinates that are visible, check if node is inside). If we leave the size as pixels,
		-- then when we zoom we'll be writing (size in pixels) but reading it as (size in node units).
		--
		-- To fix this, we just convert the size to node units by undoing the zoom scale. So, if ImGui says a node
		-- is 100 pixels wide, but we're zoomed to 50%, then we know it's 200 node units.
		imgui.PushID(id)

		imgui.GetWindowDrawList():ChannelsSetCurrent(2)
		imgui.SetCursorScreenPos(node_contents_cursor:unpack())

		self:push_scaled_font()

		imgui.BeginGroup()
		imgui.Dummy(150 * self.zoom, 0)
		self.on_node_draw(id)
		imgui.EndGroup()

		local contents_size = tdengine.vec2(imgui.GetItemRectSize())
		local padding_size = self.node_padding:scale(2):scale(self.zoom)
		local pixel_size = contents_size:add(padding_size)
		local node_unit_size = pixel_size:scale(1 / self.zoom)

		gnode.size.x, gnode.size.y = node_unit_size:unpack()
		gnode.pixel_size.x, gnode.pixel_size.y = pixel_size:unpack()

		local node_rect_max = node_rect_min:add(gnode.pixel_size)

		-- INPUTS
		-- Draw a button at the size and location of the node, and then check whether it was touched.
		imgui.SetCursorScreenPos(node_rect_min:unpack())
		imgui.InvisibleButton('node', gnode.pixel_size.x, gnode.pixel_size.y)

		gnode.left_clicked = imgui.IsItemClicked(0)
		gnode.right_clicked = imgui.IsItemClicked(1)
		gnode.active = imgui.IsItemActive()

		if imgui.IsItemHovered() and not gnode.hovered then
			self.on_node_hover(id)
		end
		gnode.hovered = imgui.IsItemHovered()

		-- SLOTS
		imgui.GetWindowDrawList():ChannelsSetCurrent(1)

		local radius = 6 * self.zoom
		local segments = 5 * math.sqrt(radius)
		local ay = average(node_rect_max.y, node_rect_min.y)

		local slot_color = self.on_node_color(id) or tdengine.colors.red:copy()
		slot_color = slot_color:alpha(.75)

		local in_slot = self:input_slot(id)
		imgui.GetWindowDrawList():AddCircleFilled(imgui.ImVec2(in_slot.x, in_slot.y), radius, slot_color:to_u32(), segments)

		local out_slot = self:output_slot(id)
		imgui.GetWindowDrawList():AddCircleFilled(imgui.ImVec2(out_slot.x, out_slot.y), radius, slot_color:to_u32(), segments)

		-- BACKGROUND
		imgui.GetWindowDrawList():ChannelsSetCurrent(1)

		-- Try to draw fewer polygons at small zoom (i.e. far away), like a bootleg LOD.
		local rounding = 4 * self.zoom
		local color = self.on_node_color(id) or tdengine.colors.red:copy()
		imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(node_rect_min.x, node_rect_min.y), imgui.ImVec2(node_rect_max.x, node_rect_max.y), color:to_u32(),
			rounding)
		imgui.PopID() -- Unique node ID


		-- LINKS
		imgui.GetWindowDrawList():ChannelsSetCurrent(0)

		local output_id, output_node = id, node
		local output_slot = self:output_slot(output_id)

		local children = output_node.children
		for index, input_id in pairs(children) do
			local input_slot = self:input_slot(input_id)

			local disconnect = output_id == self.disconnecting and input_id == self.hovered

			-- Normally, use a Bezier curve. But when Bezier curves get too close,
			-- they don't look good. So if the slots are close, use a straight line.
			local slot_distance = output_slot:distance(input_slot) / self.zoom
			local bezier_min_distance = 50
			local color = ternary(disconnect, self.colors.disconnect, self.colors.link)
			if slot_distance > bezier_min_distance then
				local cp = tdengine.vec2(50 * self.zoom, 0)
				imgui.GetWindowDrawList():AddBezierCubic(
					imgui.ImVec2(output_slot.x, output_slot.y),
					imgui.ImVec2(output_slot.x + cp.x, output_slot.y),
					imgui.ImVec2(input_slot.x - cp.x, input_slot.y),
					imgui.ImVec2(input_slot.x, input_slot.y),
					color:to_u32(),
					self.link_style.thickness * self.zoom
				)
			else
				imgui.GetWindowDrawList():AddLine(
					imgui.ImVec2(output_slot.x, output_slot.y),
					imgui.ImVec2(input_slot.x, input_slot.y),
					color:to_u32(),
					self.link_style.thickness * self.zoom
				)
			end

			self.on_node_links(output_id, input_id)
		end

		self:pop_scaled_font()

		::continue::
	end

	-- If you're connecting a node to something, follow the mouse with a bezier curve
	if self.connecting then
		local p0 = self:output_slot(self.connecting)
		local cursor = tdengine.vec2(imgui.GetMousePos())

		imgui.GetWindowDrawList():AddBezierCubic(
			imgui.ImVec2(p0.x, p0.y),
			imgui.ImVec2(p0.x + 50, p0.y + 50),
			imgui.ImVec2(cursor.x - 50, cursor.y - 50),
			imgui.ImVec2(cursor.x, cursor.y),
			self.colors.link:to_u32(),
			self.link_style.thickness * self.zoom)
	end
end

function NodeEditor:push_scaled_font()
	local range = { 16, 32, 48 }

	local base_font_size = 16
	local scaled_font_size = base_font_size * self.zoom
	local clamped_font_size = tdengine.math.snap_to_range(base_font_size * self.zoom, range)
	local font = string.format('editor-%s', clamped_font_size)
	imgui.PushFont(font)

	-- @hack: The problem here is that instead of making nodes the same size (which would be smart), I calculate
	-- their size based on their contents. Because you don't have to calculate node sizes when zoomed (it just fits
	-- to its contents), this makes it really easy to do scaling...mostly.
	--
	-- The issue is that when you're very zoomed out, you pretty quickly don't have a font that is small enough. That
	-- means that the nodes will start to become relatively bigger, and will overrun the spaces between them, and look
	-- bad. The hack here is to just have ImGui render the fonts at the actual size they would be if you had like
	-- floating point font sizes. Maybe an example is better...
	--
	-- When zoom is really small, scaled_font_size will be something like 2.4. Except the smallest font we have is 3.
	-- So, to compensate, we tell ImGui to scale the font by 2.4 / 3 to get it to the correct size.
	--
	-- This makes the text look very wonky and bad. BUT, if we only do this when highly zoomed (which, if you'll remember,
	-- is the only time we _need_ to do this), then you can't tell. You actually do kind of want to do this for other
	-- zoom levels, because it keeps the relative node sizes constant, but I prefer this hack within a hack. You'll notice
	-- that the distances between nodes varies slightly with zoom level because of this.
	--if self.zoom < 0.25 then
	local diff = scaled_font_size / clamped_font_size
	imgui.SetWindowFontScale(diff)
	--end
end

function NodeEditor:pop_scaled_font()
	imgui.SetWindowFontScale(1)
	imgui.PopFont()
end

-------------------
-- CONTEXT MENUS --
-------------------
function NodeEditor:node_context_menu()
	for id, node in pairs(self.nodes) do
		local gnode = self.gnodes[id]
		if gnode.right_clicked then
			self.context_node_id = id
			imgui.OpenPopup('node_context_menu')
		end
	end

	local node = self.nodes[self.context_node_id]

	imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_WindowPadding, 8, 8)
	if imgui.BeginPopup('node_context_menu') then
		if imgui.MenuItem('Connect') then
			self.connecting = self.context_node_id
		end

		if imgui.MenuItem('Disconnect') then
			self.disconnecting = self.context_node_id
		end

		if imgui.MenuItem('Delete') then
			self:clear_selection()
			self:select_single_node(self.context_node_id)
			self.state = editor_state.deleting
		end

		if imgui.MenuItem('Reroute') then
			self.rerouting = self.context_node_id
		end

		self.on_node_context(self.context_node_id)

		imgui.EndPopup()
	else
		-- It's harmless to let this lie around, but it's more principled to clean it up
		-- whenever the context menu is not in use
		self.context_node_id = nil
	end

	imgui.PopStyleVar()
end

function NodeEditor:canvas_context_menu()
	-- Draw a context menu when the canvas is right clicked
	local rclick = self.input:right_click()
	local on_node = imgui.IsAnyItemHovered()
	if rclick and not on_node then
		self.canvas_rclick_pos = self:zoom_to_world(self.input_cache.mouse)
		imgui.OpenPopup('context_menu')
	end

	imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_WindowPadding, 8, 8)
	if imgui.BeginPopup('context_menu') then
		if imgui.BeginMenu('Add Node') then
			for i, kind in pairs(self.node_kinds) do
				if imgui.MenuItem(kind) then
					self:create_node(kind, self.canvas_rclick_pos)
				end
			end
			imgui.EndMenu()
		end

		imgui.EndPopup()
	end
	imgui.PopStyleVar()
end

------------
-- CANVAS --
------------
function NodeEditor:begin_canvas()
	local flags = 0
	flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoScrollWithMouse)
	flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoScrollbar)
	tdengine.editor.begin_window(self.name, flags)

	-- Set up the canvas
	imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_FramePadding, 1, 1)
	imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_WindowPadding, 0, 0)

	imgui.PushStyleColor(ffi.C.ImGuiCol_ChildBg, self.colors.background:to_u32())


	local flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoMove)

	tdengine.editor.begin_child('camera_region', 0, 0, flags)

	-- Reset per-frame fields
	self.window_position.x, self.window_position.y = imgui.GetCursorScreenPos()
end

function NodeEditor:end_canvas()
	tdengine.editor.end_child()

	imgui.PopStyleVar()  -- FramePadding
	imgui.PopStyleVar()  -- WindowPadding
	imgui.PopStyleColor() -- ChildBg
	tdengine.editor.end_window()
end

function NodeEditor:update_translation()
	-- If any mod keys are down, don't scroll. We use chords that contain the movement keys.
	if self.input:down(glfw.keys.LEFT_CONTROL) then return end
	if self.input:down(glfw.keys.RIGHT_CONTROL) then return end
	if self.input:down(glfw.keys.LEFT_ALT) then return end
	if self.input:down(glfw.keys.RIGHT_ALT) then return end

	local delta = tdengine.vec2()
	if self.input_cache.middle_drag then
		delta = self.input_cache.mouse_delta
		goto apply_delta
	end

	-- @hack
	if tdengine.editor.is_window_focused('Text Editor') then return end
	if not tdengine.editor.is_window_hovered(self.name) then return end

	if self.input:down(glfw.keys.W) then
		delta.y = delta.y + (self.scroll_per_second * tdengine.dt)
	end
	if self.input:down(glfw.keys.S) then
		delta.y = delta.y - (self.scroll_per_second * tdengine.dt)
	end
	if self.input:down(glfw.keys.A) then
		delta.x = delta.x + (self.scroll_per_second * tdengine.dt)
	end
	if self.input:down(glfw.keys.D) then
		delta.x = delta.x - (self.scroll_per_second * tdengine.dt)
	end
	if self.input:down(glfw.keys.LEFT_SHIFT) or self.input:down(glfw.keys.RIGHT_SHIFT) then
		delta = delta:scale(.2)
	end

	::apply_delta::
	delta = delta:scale(1 / self.zoom)
	--self.deltas.translation = self.deltas.translation:update(delta)
	self.camera = self.camera:update(delta)
end

function NodeEditor:update_zoom()
	if self.input_cache.hovered and math.abs(self.input_cache.scroll.y) > 0 then
		local zoom_min, zoom_max = .05, 3
		local zoom_delta = self.input_cache.scroll.y * self.speed.zoom
		local zoom_new = tdengine.math.clamp(
			self.zoom * (1 + zoom_delta),
			zoom_min, zoom_max)
		zoom_delta = zoom_new - self.zoom

		-- This does not make sense mathematically, I think, but it mostly works.
		local mouse = self:window_to_canvas(self.input_cache.mouse):scale(1 / self.zoom)
		local correction = mouse:scale(zoom_new):subtract(mouse:scale(self.zoom)):scale(-1)
		correction = correction:scale(1 / self.zoom)

		-- Yeah, I just can't get the camera to stop drifting when zooming out and then back in. This
		-- doesn't totally stop it but it *does* do enough to make it a lot less noticeable.
		if math.abs(zoom_delta) > 0 then
			local hack = tdengine.vec2(18, 18):scale(1 / self.zoom)
			correction = correction:add(hack)
		end

		self.camera:translate(correction)
		self.zoom = zoom_new
	end
end

-----------
-- NODES --
-----------
function NodeEditor:create_node(kind, position)
	-- Ask the thing that owns us to create the node itself
	local uuid = self.on_node_add(kind, position)

	-- Use the UUID to keep track of the node's metadata
	self.gnodes[uuid] = {
		position = tdengine.vec2(position), -- Specified in world space
		size = tdengine.vec2(),
		pixel_size = tdengine.vec2()
	}

	self:select_single_node(uuid)
	self.node_queue:push(uuid)
end

function NodeEditor:set_nodes(nodes, gnodes)
	self.nodes = nodes
	self.gnodes = gnodes

	self:clear_selection()
	self.state = editor_state.idle
	self.camera = tdengine.vec2(0, 0)
	self.zoom = 1
end

function NodeEditor:copy_node(node, gnode)
	-- If the node defines a serialize method, use it. Otherwise, just copy the table.
	local uuid = node.uuid
	self.clipboard.nodes[uuid] = node.serialize and node:serialize() or table.deep_copy(node)

	-- Translate position to window coordinates, so that C-v can put them in the
	-- same place within the window.
	local copy = table.deep_copy(gnode)
	copy.position = {
		x = gnode.position.x + self.camera.x,
		y = gnode.position.y + self.camera.y
	}
	self.clipboard.gnodes[uuid] = copy

	self.on_node_copy(uuid)
end

function NodeEditor:paste_node(node_data)
	-- First, make a clean node of the right kind. Using on_node_add lets the controller
	-- know that we're adding these nodes
	local uuid = self.on_node_add(node_data.kind)

	-- Then, fix up the data with the new UUID and deserialize into the newly-created node
	local node = self.nodes[uuid]
	node:deserialize(node_data)
	node.uuid = uuid

	-- Update the GUI node to be centered wherever the view is, plus @hack around the fact that
	-- pasting a node exactly on top of an existing node makes it impossible to select the pasted
	-- node. The hack is to offset the pasted node slightly
	local offset_hack = 25
	local gnode = table.deep_copy(self.clipboard.gnodes[node_data.uuid])
	gnode.position = {
		x = gnode.position.x - self.camera.x + offset_hack,
		y = gnode.position.y - self.camera.y + offset_hack
	}
	self.gnodes[uuid] = gnode
	return uuid
end

function NodeEditor:clear_clipboard()
	table.clear(self.clipboard.nodes)
	table.clear(self.clipboard.gnodes)
end

function NodeEditor:update_copy_paste()
	if not self.input_cache.focused then return end

	if self.input:chord_pressed(glfw.keys.CONTROL, glfw.keys.C) then
		self.on_graph_copy()
		self:clear_clipboard()
		for index, uuid in pairs(self.selected) do
			local node = self.nodes[uuid]
			local gnode = self.gnodes[uuid]
			self:copy_node(node, gnode)
		end
	end

	if self.input:chord_pressed(glfw.keys.CONTROL, glfw.keys.V) then
		-- Paste each node, marking the UUID of the pasted node
		local new_uuids = {}
		for old_uuid, node_data in pairs(self.clipboard.nodes) do
			new_uuids[old_uuid] = self:paste_node(node_data)
		end

		-- Remake links with the new nodes
		for old, new in pairs(new_uuids) do
			local children = self.nodes[new].children
			for index, old_child in pairs(children) do
				children[index] = new_uuids[old_child]
			end
		end

		-- Call into the controller
		for old, new in pairs(new_uuids) do
			local copied = self.clipboard.nodes[old]
			local pasted = self.nodes[new]
			self.on_node_paste(copied, pasted)
		end
	end
end

---------------
-- SELECTION --
---------------
function NodeEditor:is_node_in_partition(node_pos, separator)
	if separator.kind == sep_kind.x or separator.kind == sep_kind.y then
		-- These are stored as world-space coordinates
		local begin = sep
		local cmp_n = 0
		local cmp_s = 0
		if separator.kind == sep_kind.x then
			cmp_n = node_pos.x
			cmp_s = separator.begin.x
		elseif separator.kind == sep_kind.y then
			cmp_n = node_pos.y
			cmp_s = separator.begin.y
		end

		if separator.direction == sep_direction.greater then
			return cmp_n > cmp_s
		elseif separator.direction == sep_direction.less_than then
			return cmp_n < cmp_s
		end
	elseif separator.kind == sep_kind.box then
		-- These are stored as zoom-space coordinates, because I need to draw the
		-- box each frame, and the coordinates are handy to have around. I could fix
		-- this pretty easily, but I don't feel like it, and it's easy to just
		-- convert them here.
		local tl = self:zoom_to_world(separator.tl)
		local br = self:zoom_to_world(separator.br)

		local x = tl.x < node_pos.x and node_pos.x < br.x
		local y = tl.y < node_pos.y and node_pos.y < br.y
		return x and y
	end
end

function NodeEditor:cancel_separator()
	table.remove(self.separators, #self.separators)
	if #self.separators > 0 then
		self:do_multiselect()
		self.state = editor_state.multiselect
	else
		self.separators = {}
		self:clear_selection()
		self.state = editor_state.idle
	end
end

function NodeEditor:toggle_separator_direction()
	local separator = self.separators[#self.separators]
	if separator.direction == sep_direction.greater then
		separator.direction = sep_direction.less_than
	elseif separator.direction == sep_direction.less_than then
		separator.direction = sep_direction.greater
	end
end

function NodeEditor:enter_multiselect_line_mode(sep_kind)
	self.state = editor_state.separate_line_drag
	self.sep_kind = sep_kind

	local separator = {
		begin = tdengine.vec2(imgui.GetMousePos()),
		direction = sep_direction.greater,
		kind = self.sep_kind
	}
	table.insert(self.separators, separator)
end

function NodeEditor:do_multiselect()
	self:clear_selection()
	for uuid, gnode in pairs(self.gnodes) do
		for _, separator in pairs(self.separators) do
			if not self:is_node_in_partition(gnode.position, separator) then goto fail end
		end
		::pass::
		self:add_node_to_selection(uuid)
		::fail::
	end
end

function NodeEditor:end_multiselect()
	self.separators = {}
	self:clear_selection()
end

function NodeEditor:toggle_node_selected(uuid)
	if self:is_node_selected(uuid) then
		self:remove_node_from_selection(uuid)
	else
		self:add_node_to_selection(uuid)
	end
end

function NodeEditor:select_single_node(uuid)
	-- If it's already selected, noop
	local selected = self:get_selected_node()
	if selected and selected.uuid == uuid then return end

	self:clear_selection()
	self:end_multiselect()
	self:add_node_to_selection(uuid)
	self.on_node_select(uuid)
	self.state = editor_state.single_select
end

function NodeEditor:add_node_to_selection(id)
	if self:is_node_selected(id) then return end
	table.insert(self.selected, id)
end

function NodeEditor:remove_node_from_selection(id)
	delete(self.selected, id)
end

function NodeEditor:clear_selection()
	-- Keep a stable pointer for the table itself
	for i = 0, #self.selected, 1 do
		self.selected[i] = nil
	end
end

function NodeEditor:delete_selection()
	for index, uuid in pairs(self.selected) do
		self.on_node_delete(uuid)

		-- Remove all references to the deleted node in any children lists
		for id, node in pairs(self.nodes) do
			delete(node.children, uuid)
		end

		-- Remove any references in the editor's bookkeeping stuff
		if self.connecting == uuid then self.connecting = nil end
		if self.rerouting == uuid then self.rerouting = nil end
		if self.disconnecting == uuid then self.disconnecting = nil end
		self.node_queue:remove(uuid)

		-- Finally, delete node and associated GUI data
		self.gnodes[uuid] = nil
		self.nodes[uuid] = nil
	end
end

function NodeEditor:get_selected_node()
	local single_select = self.state == editor_state.single_select
	local no_selection = #self.selected == 0
	if not single_select or no_selection then return nil end

	return self.nodes[self.selected[1]]
end

function NodeEditor:is_node_selected(uuid)
	for index, sid in pairs(self.selected) do
		if uuid == sid then return true end
	end
	return false
end

function NodeEditor:find_queue_position()
	if #self.selected ~= 1 then return nil end

	local selected = self.selected[1]
	for index, uuid in self.node_queue:iterate() do
		if uuid == selected then
			return index
		end
	end

	return nil
end

--------------
-- KEYBOARD --
--------------
function NodeEditor:check_multiselect_hotkeys()
	if self.input:mod_down(glfw.keys.CONTROL) and not self.hack then
		if self.input:down(glfw.keys.X) then
			-- Ctrl + X selects nodes by splitting based on their position on X axis
			self:enter_multiselect_line_mode(sep_kind.x)
		elseif self.input:down(glfw.keys.Y) then
			-- Ctrl + Y selects nodes by splitting based on their position on Y axis
			self:enter_multiselect_line_mode(sep_kind.y)
		elseif self.input:pressed(glfw.keys.MOUSE_BUTTON_1) then
			-- Ctrl + Left Click selects nodes by making a box
			self.state = editor_state.separate_box_wait
		end
	end
end

function NodeEditor:check_move_nodes()
	-- Speed is in node units, and the farther you're zoomed out, the more node units you should go. You don't
	-- want the same speed dragging one node close up as you do when dragging a whole chunk of the graph somewhere
	-- else
	local speed = self.speed.node_move / self.zoom
	local dx, dy = 0, 0

	if self.input:down(glfw.keys.LEFT_SHIFT) or self.input:down(glfw.keys.RIGHT_SHIFT) then
		speed = speed * 4
	end

	if self.input:down(glfw.keys.RIGHT) then dx = dx + speed end
	if self.input:down(glfw.keys.LEFT) then dx = dx - speed end
	if self.input:down(glfw.keys.DOWN) then dy = dy + speed end
	if self.input:down(glfw.keys.UP) then dy = dy - speed end

	for index, uuid in pairs(self.selected) do
		local gnode = self.gnodes[uuid]
		gnode.position.x = gnode.position.x + dx
		gnode.position.y = gnode.position.y + dy
	end
end

-----------------
-- COORDINATES --
-----------------
--[[
The node editor uses four coordinate systems at various times:
(1) WINDOW
These are raw window coordinates in screen space. These are almost always for:
  - mouse events that come in as a location on the window itself
  - coordinates we give to the ImGui draw list API

(2) ZOOM
Identical to WINDOW (i.e. not relative to the canvas), except scaled such that each node unit
corresponds to the correct amount of pixels.

Zooming fundamentally means changing how much "node unit" one pixel represents. When zoom is 1,
if a node is 100 node units wide, it will cover 100 pixels when drawn. When zoom is .5, those
100 node units are only 50 pixels wide.

(3) CANVAS
Screen space coordinates, where the canvas is the screen

(4) WORLD
The world space of the world in which all of the nodes live. Practically, canvas minus camera.

--]]

function NodeEditor:window_to_canvas(window)
	window = tdengine.vec2(window)
	local world = window:subtract(self.window_position)
	return world
end

function NodeEditor:world_to_window(world)
	world = tdengine.vec2(world)
	local canvas = world:add(self.camera)
	local window = canvas:add(self.window_position)
	return window
end

function NodeEditor:world_to_zoom(world)
	local world = tdengine.vec2(world)
	local canvas = world:add(self.camera)
	local zoom = canvas:scale(self.zoom)
	local window = zoom:add(self.window_position)
	return window
end

function NodeEditor:zoom_to_world(zoom)
	local zoom = tdengine.vec2(zoom)
	local window = zoom:subtract(self.window_position)
	local canvas = window:scale(1 / self.zoom)
	local world = canvas:subtract(self.camera)
	return world
end

function NodeEditor:world_to_canvas(world)
	local world = tdengine.vec2(world)
	local canvas = world:add(self.camera)
	return canvas
end

function NodeEditor:canvas_to_window(canvas)
	canvas = tdengine.vec2(canvas)
	return canvas:add(self.window_position)
end

function NodeEditor:input_slot(id)
	local gnode = self.gnodes[id]
	local zoom = self:world_to_zoom(gnode.position)
	zoom.y = zoom.y + gnode.pixel_size.y / 2
	return zoom
end

function NodeEditor:output_slot(id)
	local gnode = self.gnodes[id]
	local slot = self:input_slot(id)
	slot.x = slot.x + gnode.pixel_size.x
	return slot
end

function NodeEditor:get_view_region()
	-- The base view region is the window size. The "unit" of the node editor is just defined to be a single
	-- pixel. So, unzoomed, if a node has a size of (100, 100), then it'll take up 100 pixels along both
	-- axes when drawn. Moreover, if the viewport is (1000, 1000), then it will be 1/10 of the viewport.
	--
	-- First, apply the zoom. If you're zoomed out to 50%, a "unit" is now only half a pixel visually. In
	-- the last example, a (100, 100) node would now be rendered at (50, 50). In addition to changing the ratio
	-- of screen pixels to "units", we also expand the viewport, so instead of (1000, 1000), we would now be
	-- looking at any node that lies within (2000, 2000)
	--
	-- Then, apply the camera. Scale before transform.
	local zoom   = self.window_size:scale(1 / self.zoom)
	local left   = 0 - self.camera.x
	local top    = 0 - self.camera.y
	local right  = zoom.x - self.camera.x
	local bottom = zoom.y - self.camera.y
	return top, bottom, left, right
end

function NodeEditor:is_node_visible(id)
	-- To determine if a node is visible, you also want to take children into account. If any child is visible,
	-- we want to draw the node so that the link doesn't visibly pop in and out
	local node = self.nodes[id]
	local gnode = self.gnodes[id]

	if self:is_node_visible_impl(gnode) then
		return true
	end

	for index, cid in pairs(node.children) do
		if self:is_node_visible_impl(self.gnodes[cid]) then
			return true
		end
	end

	return false
end

function NodeEditor:is_node_visible_impl(gnode)
	local top, bottom, left, right = self:get_view_region()

	local is_node_completely_right_of_view = right < gnode.position.x
	local is_node_completely_left_of_view = gnode.position.x + gnode.size.x < left
	local is_node_horizontally_within_view = not (is_node_completely_right_of_view or is_node_completely_left_of_view)

	local is_node_completely_above_view = gnode.position.y + gnode.size.y < top
	local is_node_completely_below_view = gnode.position.y > bottom
	local is_node_vertically_within_view = not (is_node_completely_above_view or is_node_completely_below_view)

	return is_node_horizontally_within_view and is_node_vertically_within_view
end

function NodeEditor:calculate_visible_nodes()
	for id, gnode in pairs(self.gnodes) do
		gnode.visible = self:is_node_visible(id)
	end
end

function NodeEditor:snap_to_node(id)
	local gnode = self.gnodes[id]
	local offset = tdengine.vec2(700, 400)
	self.camera = tdengine.vec2(gnode.position):scale(-1):add(offset)
end

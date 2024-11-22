local DialogueEditor = tdengine.editor.define('DialogueEditor')

function DialogueEditor:init(params)
	self.data = {}
	self.nodes = {}
	self.gnodes = {}
	self.loaded = ''
	self.queued_to_load = nil
	self.hidden = false

	local node_editor_params = {
		name = 'Dialogue Editor',
		node_kinds = tdengine.dialogue.sorted_node_kinds,
		nodes = self.nodes,
		gnodes = self.gnodes,
		on_node_add = function(i, name) return self:on_node_add(i, name) end,
		on_node_select = function(id) return self:on_node_select(id) end,
		on_node_hover = function(id) return self:on_node_hover(id) end,
		on_node_connect = function(id) return self:on_node_connect(parent, child) end,
		on_node_disconnect = function(id) return self:on_node_disconnect(parent, child) end,
		on_node_delete = function(id) return self:on_node_delete(id) end,
		on_node_draw = function(id) return self:on_node_draw(id) end,
		on_node_color = function(id) return self:on_node_color(id) end,
		on_node_context = function(id) return self:on_node_context(id) end,
		on_node_links = function(oid, iid) return self:on_node_links(oid, iid) end,
	}
	self.node_editor = NodeEditor:new(node_editor_params)

	self.metadata_editor = nil

	self.search_result = {}
	self.search_input = ''
	self.last_search_input = ''
	self.search_options = {
		exact_match = false,
		labels = true,
		kind = true,
		text = true,
		set = true,
		uuid = true,
		unlock = true,
		branch = true,
	}

	self.selected_editor = nil
	self.selected_variable = ''
	self.selected_op = ''

	self.time = 0
	self.glow = 0

  self.input = ContextualInput:new(tdengine.enums.InputContext.Editor)

	self.ids = {
		state_picker_value = '##state_picker_value',
		search = '##dialogue_editor:search',
		search_options = {
			exact_match = 'Exact Match##ded:search_options:exact',
			labels = 'Labels##ded:search_options:labels',
			kind = 'Kind##ded:search_options:kind',
			text = 'Text##ded:search_options:text',
			set = 'Set##ded:search_options:set',
			uuid = 'Uuid##ded:search_options:uuid',
			unlock = 'Unlock##ded:search_options:unlock',
			branch = 'Branch##ded:search_options:branch',
		},
	}

	self.colors = {
		branch_true = tdengine.colors.light_green:copy(),
		branch_false = tdengine.colors.cardinal:copy(),
		index_label = tdengine.colors.white:copy(),
		default = tdengine.colors.charcoal:copy(),
		highlighted = tdengine.colors.cool_gray:copy(),
		entry_point = tdengine.colors.cadet_gray:copy(),
		find_result = tdengine.colors.celadon:copy(),
		invalid = tdengine.colors.cardinal:copy(),
		nodes = {
			[tdengine.dialogue.node_kind.End] = tdengine.colors.orange:copy(),
			[tdengine.dialogue.node_kind.Set] = tdengine.colors.tyrian_purple:copy(),
			[tdengine.dialogue.node_kind.ChoiceList] = tdengine.colors.zomp:copy(),
			[tdengine.dialogue.node_kind.Label] = tdengine.colors.indian_red:copy(),
			[tdengine.dialogue.node_kind.Call] = tdengine.colors.prussian_blue:copy(),
			[tdengine.dialogue.node_kind.Jump] = tdengine.colors.prussian_blue:copy(),
			[tdengine.dialogue.node_kind.Return] = tdengine.colors.prussian_blue:copy(),
			[tdengine.dialogue.node_kind.DevMarker] = tdengine.colors.cadet_gray:copy(),
			[tdengine.dialogue.node_kind.DevNote] = tdengine.colors.cadet_gray:copy(),
		}
	}

	self.validation_errors = {}
	self.check_validation = false
	self.ignore_validation = false
	self.is_graph_dirty = false
	self.warn_on_save = false
	self.pretty_save = false

	self.popups = {
		invalid_graph = 'Invalid Graph##ded:invalid_graph',
		search = 'Search##ded:search'
	}
	self.popups_to_open = {}
	for popup, id in pairs(self.popups) do
		self.popups_to_open[id] = false
	end

	self.condition_list = imgui.extensions.DialogueConditionList:new()
	self.state_picker = imgui.extensions.StatePicker:new()
end

function DialogueEditor:update(dt)
	if self.hidden then return end
	
	self:update_timers()
	self:draw_sidebar()
	self.node_editor:update(dt)
	self:check_hotkeys()
	self:update_popups()
	self:update_search()
	self:draw_search()
	self:update_graph_validation()
	self:draw_graph_validation()
	self:load_queued_scene()
end

function DialogueEditor:update_timers()
	self.time = self.time + tdengine.dt
	self.glow = self.glow + tdengine.dt
end

function DialogueEditor:load_queued_scene()
	if self.queued_to_load then
		self:load(self.queued_to_load)
		self.queued_to_load = nil
	end
end

DialogueEditor.node_creation_hotkeys = {
	[glfw.keys.T] = tdengine.dialogue.node_kind.Text,
	[glfw.keys.L] = tdengine.dialogue.node_kind.ChoiceList,
	[glfw.keys.C] = tdengine.dialogue.node_kind.Choice,
	[glfw.keys.J] = tdengine.dialogue.node_kind.Jump,
	[glfw.keys.S] = tdengine.dialogue.node_kind.Set,
	[glfw.keys.N] = tdengine.dialogue.node_kind.Continue,
	[glfw.keys.D] = tdengine.dialogue.node_kind.DevNote,
}

function DialogueEditor:check_hotkeys()
	if not tdengine.editor.is_window_focused(self.node_editor.name) then return end

	for hotkey, node_kind in pairs(self.node_creation_hotkeys) do
		if self.input:chord_pressed(glfw.keys.ALT, hotkey) then
			local mouse = self.node_editor:zoom_to_world(self.node_editor.input_cache.mouse)
			self.node_editor:create_node(node_kind, mouse)
		end
	end
end

function DialogueEditor:draw_sidebar()
	tdengine.editor.begin_window('Dialogue Node')

	-- Selected node detail view
	self:update_text_editor()
	self:selected_detail_view()

	tdengine.editor.end_window('Node Info')
end

function DialogueEditor:selected_detail_view()
	local selected = self.node_editor:get_selected_node()
	if selected then
		local added, changed = self.selected_editor:draw()
		if added or changed then
			self:mark_graph_dirty()
		end

		if selected.kind == tdengine.dialogue.node_kind.Choice then
			tdengine.editor.begin_window('Node Info')

			local change, op = imgui.extensions.AlignedBranchCombinator(selected.unlock.combinator, self.selected_editor)
			if change then
				selected.unlock.combinator = op
			end

			if imgui.TreeNode('Unlock Conditions') then
				self.condition_list:update()
				imgui.TreePop()
			end

			tdengine.editor.end_window()
		end

		-- Set/Increment nodes: A dropdown with all the state fields in the game
		if selected.kind == tdengine.dialogue.node_kind.Set or selected.kind == tdengine.dialogue.node_kind.Increment then
			imgui.Dummy(0, 10)
			local change, state = self.state_picker:update(0)

			if change then
				selected.variable = state

				-- When a new field is selected, give a sane default value
				if tdengine.state.is_number(state) then
					selected.value = 0
				elseif tdengine.state.is_boolean(state) then
					selected.value = true
				elseif tdengine.state.is_string(state) then
					selected.value = ''
				end
			end
		end

		-- Branch nodes: A dropdown with all state fields, but they are in
		-- rows where you can composite different flags to AND together
		if selected.kind == tdengine.dialogue.node_kind.ActiveSkillCheck then
			imgui.Dummy(0, 20)
			imgui.extensions.Rolls(selected.branches)
		end

		if selected.kind == tdengine.dialogue.node_kind.Branch then
			imgui.Dummy(0, 10)
			imgui.Separator()
			imgui.Dummy(0, 10)
			local change, op = imgui.extensions.AlignedBranchCombinator(selected.combinator, self.selected_editor)
			if change then
				selected.combinator = op
			end
			self.condition_list:update()
		end
	end -- if there is a selected node
end

function DialogueEditor:update_search()
	self.last_search_input = self.search_input

	self.search_dirty = self.search_dirty or self.search_input == self.last_search_input
	if not self.search_dirty then return end
	self.search_dirty = false

	-- If we cleared out the search input, just erase all results so we're not highlighting
	-- every node in the graph
	if #self.search_input == 0 then
		self.search_result = {}
		return
	end


	self.search_result = {}

	local find = function(haystack, needle)
		if self.search_options.exact_match then
			return haystack == needle
		else
			return string.find(string.lower(haystack), string.lower(needle))
		end
	end

	local check_uuid = function(uuid, node)
		if find(uuid, self.search_input) then
			self.search_result[uuid] = node
		end
	end

	local check_kind = function(uuid, node)
		if not node.kind then return end
		if find(node.kind, self.search_input) then
			self.search_result[uuid] = node
		end
	end

	local check_branches = function(uuid, node)
		if not node.branches then return end
		for index, branch in pairs(node.branches) do
			if find(branch.variable, self.search_input) then
				self.search_result[uuid] = node
				return
			end
		end
	end

	local check_unlocks = function(uuid, node)
		if not node.unlock then return end
		for index, branch in pairs(node.unlock.branches) do
			if find(branch.variable, self.search_input) then
				self.search_result[uuid] = node
				return
			end
		end
	end

	local check_text = function(uuid, node)
		if not node.text then return end
		if find(node.text, self.search_input) then
			self.search_result[uuid] = node
		end
	end

	local check_label = function(uuid, node)
		local found = false

		if node.label then
			found = find(node.label, self.search_input) or found
		end

		if node.target then
			found = find(node.target, self.search_input) or found
		end

		if found then
			self.search_result[uuid] = node
		end
	end

	local check_state = function(uuid, node)
		if not node.variable then return end
		if find(node.variable, self.search_input) then
			self.search_result[uuid] = node
		end
	end

	for uuid, node in pairs(self.nodes) do
		if self.search_options.labels then check_label(uuid, node) end
		if self.search_options.kind then check_kind(uuid, node) end
		if self.search_options.set then check_state(uuid, node) end
		if self.search_options.branch then check_branches(uuid, node) end
		if self.search_options.unlock then check_unlocks(uuid, node) end
		if self.search_options.uuid then check_uuid(uuid, node) end
		if self.search_options.text then check_text(uuid, node) end
	end
end

function DialogueEditor:draw_search()
	tdengine.editor.begin_window('Dialogue Search')

	imgui.PushTextWrapPos(0)
	imgui.Text(
		'This text input will fuzzy search over pretty much everything in the current dialogue -- state fields used, labels, text.')
	imgui.PopTextWrapPos()

	imgui.InputText(self.ids.search, self, 'search_input')

	local option = function(key)
		if imgui.Checkbox(self.ids.search_options[key], self.search_options, key) then
			self.search_dirty = true
		end
	end

	if imgui.TreeNode('Options') then
		option('exact_match')
		option('labels')
		option('text')
		option('set')
		option('uuid')
		option('unlock')
		option('branch')

		imgui.TreePop()
	end

	local count_results = 0
	for uuid, node in pairs(self.search_result) do
		count_results = count_results + 1
	end
	imgui.Text(string.format('%d results', count_results))

	imgui.extensions.Table(self.search_result)

	tdengine.editor.end_window()
end

function DialogueEditor:update_graph_validation()
	if self.check_validation then
		self.validation_errors = validate_graph(self.nodes)
		self.check_validation = false
	end
end

function DialogueEditor:draw_graph_validation()
	local good_color = self.colors.branch_true:to_u32()
	local bad_color = self.colors.branch_false:to_u32()

	tdengine.editor.begin_window('Dialogue Info')

	local metrics = tdengine.dialogue.find_metric(self.loaded) or { nodes = 0, words = 0 }
	local scene = string.format(
		'%s (%d nodes, %d words)',
		self:full_path(),
		metrics.nodes,
		metrics.words
	)
	imgui.Text(scene)
	imgui.SameLine()

	if self.is_graph_dirty then
		local text = 'SAVED'
		imgui.extensions.TextColored(text, bad_color)
	else
		local text = 'SAVED'
		imgui.extensions.TextColored(text, good_color)
	end

	imgui.SameLine()
	imgui.Text('|')
	imgui.SameLine()

	if #self.validation_errors == 0 then
		local text = 'VALID'
		imgui.extensions.TextColored(text, good_color)
	else
		local text = 'VALID'
		imgui.extensions.TextColored(text, bad_color)
	end
	imgui.Dummy(0, 5)

	if self.metadata_editor then
		self.metadata_editor:draw()
	end

	if imgui.TreeNode('Errors') then
		local ignore = { kind = true }
		for i, err in pairs(self.validation_errors) do
			local address = table_address(err)
			local imgui_id = err.kind .. '##' .. address

			if err.node then
				if imgui.CollapsingHeader(imgui_id) then
					imgui.extensions.Table(
						err, 
						{ 
							ignore = {
								kind = true
							}
						})

					local btn_color = tdengine.color32(200, 0, 0, 255)
					local btn_label = 'Take Me There' .. '##' .. address
					imgui.PushStyleColor(ffi.C.ImGuiCol_Button, btn_color)
					if imgui.Button(btn_label) then
						self.node_editor:snap_to_node(err.node)
						self.node_editor:select_single_node(err.node)
					end
					imgui.PopStyleColor()
				end
			else
				imgui.Button(imgui_id)
			end
		end

		imgui.TreePop()
	end

	tdengine.editor.end_window()
end

function DialogueEditor:update_text_editor()
	-- Bind selected node's text to what's in the editor
	local text_editor = tdengine.find_entity_editor('TextEditor')

	local selected = self.node_editor:get_selected_node()
	if selected then
		if selected.text then
			selected.text = text_editor.text
		end

		local action = text_editor.last_action
		if action then
			self:mark_graph_dirty()
		end
	end
end

------------------------
-- SAVING AND LOADING --
------------------------
function DialogueEditor:load(name_or_path)
	local file_name = tdengine.extract_filename(name_or_path)
	if #file_name == 0 then
		log.warn('DialogueEditor:load(): Tried to pass empty string as dialogue name')
		return
	end

	self.loaded = file_name

	-- Load dialogue and GUI data from disk
	self.data = tdengine.dialogue.load(file_name)
	self.nodes = self.data.nodes
	if not self.nodes then
		self.nodes = {}
		return
	end

	local file_path = tdengine.ffi.resolve_format_path('dialogue_metadata', file_name):to_interned()
	self.gnodes = tdengine.module.read(file_path)
	for id, gnode in pairs(self.gnodes) do
		gnode.position = tdengine.vec2(gnode.position)
		gnode.size = tdengine.vec2(gnode.size)
		gnode.pixel_size = tdengine.vec2(gnode.pixel_size)
	end

	if not self.gnodes then
		self.nodes = {}
		self.gnodes = {}

		tdengine.log('no gui layout for dialogue, path = ' .. layout_path)
		return
	end

	-- Reload the main node editor with the new nodes
	self.node_editor:set_nodes(self.nodes, self.gnodes)

	-- And point the metadata editor at the new dialogue
	self.metadata_editor = imgui.extensions.TableEditor(self.data.metadata)

	local text_editor = tdengine.find_entity_editor('TextEditor')
	text_editor:set_text('')

	self.is_graph_dirty = false
	self.check_validation = true
end

function DialogueEditor:save(dialogue_name)
	if #dialogue_name == 0 then return end

	-- Open a popup if the graph is not valid + the user enabled the option to be warned when
	-- saving an invalid graph. TBH, graphs are invalid for 99% of their life -- until they're
	-- totally done, pretty much, so I don't really use this.
	local confirm = false
	confirm = confirm or #self.validation_errors > 0
	confirm = confirm and self.warn_on_save
	if confirm then
		self:open_popup(self.popups.invalid_graph)
		return false
	end

	return self:save_impl(dialogue_name)
end

function DialogueEditor:save_no_validate(dialogue_name)
	return self:save_impl(dialogue_name)
end

function DialogueEditor:save_impl(dialogue_name)
	-- Save the dialogue itself
	tdengine.dialogue.save(dialogue_name, self.data, self.pretty_save)

	-- Save out the layout data. We store extra stuff that doesn't need to be serialized in the
	-- gnodes -- the only thing we need to store is position (even size is recalculated depending
	-- on the node contents).
	local gnodes = {}
	for id, gnode in pairs(self.gnodes) do
		gnodes[id] = {
			position = gnode.position
		}
	end

	local file_path = tdengine.ffi.resolve_format_path('dialogue_metadata', dialogue_name):to_interned()
	tdengine.module.write(file_path, gnodes, tdengine.module.WriteOptions.Compact)

	-- Bookkeeping: Update the word count and mark the graph as clean
	tdengine.dialogue.update_single_metrics(dialogue_name)
	self.is_graph_dirty = false
end

function DialogueEditor:toggle_warn_on_save()
	self.warn_on_save = not self.warn_on_save
end

function DialogueEditor:new(name)
	self.validation_errors = {}
	self.data = {
		metadata = {},
		nodes = {},
		gnodes = {}
	}
	self.nodes = self.data.nodes
	self.gnodes = self.data.gnodes

	local directory = tdengine.ffi.resolve_format_path('dialogue_folder', name):to_interned()
	tdengine.ffi.remove_directory(directory)
	tdengine.ffi.create_directory(directory)
	self:save(name)
	self:load(name)
end

function DialogueEditor:delete(name)
	local directory = tdengine.ffi.resolve_format_path('dialogue_folder', name):to_interned()
	tdengine.ffi.remove_directory(directory)

	self:new('default')
end

function DialogueEditor:full_path()
	if string.len(self.loaded) > 0 then
		return self.loaded .. '.lua'
	end

	return 'no file loaded'
end

function DialogueEditor:mark_graph_dirty()
	self.is_graph_dirty = true
	self.check_validation = true
end

function DialogueEditor:open_popup(popup)
	self.popups_to_open[popup] = true
end

function DialogueEditor:update_popups()
	for id, open in pairs(self.popups_to_open) do
		if open then
			imgui.OpenPopup(id)
			self.popups_to_open[id] = false
		end
	end

	--
	-- INVALID GRAPH
	--
	imgui.SetNextWindowSize(300, 0)
	if imgui.BeginPopupModal(self.popups.invalid_graph) then
		imgui.PushTextWrapPos(0)
		imgui.Text(
			'Dialogue graph is invalid!\n\nProblems are shown in the "Errors" tab of the editor. You must fix them to continue.')
		imgui.PopTextWrapPos()

		imgui.Dummy(5, 5)
		if imgui.Button('OK', imgui.ImVec2(120, 0)) then
			imgui.CloseCurrentPopup()
		end

		imgui.SameLine()

		local bad_color = tdengine.color32(150, 0, 0, 255)
		imgui.PushStyleColor(ffi.C.ImGuiCol_Button, bad_color)
		if imgui.Button('Save Anyway', imgui.ImVec2(120, 0)) then
			-- If we were doing Save As, grab the dialogue name from that input text.
			-- Otherwise, use the currently loaded file
			local file = self.loaded
			local menu = tdengine.find_entity_editor('MainMenu')
			if menu.failed_save_as then
				file = menu.failed_save_as
				menu:reset_save_as()
			end

			self:save_no_validate(file)

			imgui.CloseCurrentPopup()
		end
		imgui.PopStyleColor()

		imgui.EndPopup()
	end
end

function DialogueEditor:does_node_use_state(node, state)
	if not state then return false end

	if node.kind == tdengine.dialogue.node_kind.Branch then
		for index, branch in pairs(node.branches) do
			if string.find(string.lower(branch.variable), string.lower(state)) then
				return true
			end
		end
	elseif node.kind == tdengine.dialogue.node_kind.Choice then
		for index, branch in pairs(node.unlock.branches) do
			if string.find(string.lower(branch.variable), string.lower(state)) then
				return true
			end
		end
	elseif node.kind == tdengine.dialogue.node_kind.Set then
		if string.find(string.lower(node.variable), string.lower(state)) then
			return true
		end
	elseif node.kind == tdengine.dialogue.node_kind.Increment then
		if string.find(string.lower(node.variable), string.lower(state)) then
			return true
		end
	end

	return false
end

function DialogueEditor:does_node_use_label(node, label)
	if not label then return false end

	if node.kind == tdengine.dialogue.node_kind.Label then
		return string.find(string.lower(node.label), string.lower(label))
	elseif node.kind == tdengine.dialogue.node_kind.Jump then
		return string.find(string.lower(node.target), string.lower(label))
	elseif node.kind == tdengine.dialogue.node_kind.Call then
		return string.find(string.lower(node.target), string.lower(label))
	end

	return false
end

function DialogueEditor:find_first_parent(child_node)
	for parent_id, parent_node in pairs(self.nodes) do
		for index, child_id in pairs(parent_node.children) do
			if child_id == child_node.uuid then
				return parent_node, index
			end
		end
	end

	return nil, nil
end

function DialogueEditor:move_node_position(parent_node, index_to_move, offset)
	local displaced = parent_node.children[index_to_move + offset]
	local mover = parent_node.children[index_to_move]
	parent_node.children[index_to_move + offset] = mover
	parent_node.children[index_to_move] = displaced
end

---------------
-- CALLBACKS --
---------------
function DialogueEditor:on_node_hover()
	self.glow = 0
end

function DialogueEditor:on_node_select(id)
	-- Clean up any editors for the previously selected node
	self.selected_editor = nil

	-- Bail early if it does not exist
	if not id then
		return
	end

	local node = self.nodes[id]

	-- Do a few bookkeeping things depending on the node type
	if node.kind == tdengine.dialogue.node_kind.Text then
		self.last_who = node.who
	elseif node.kind == tdengine.dialogue.node_kind.Choice then
		self.condition_list:set_branches(node.unlock.branches)
	elseif node.kind == tdengine.dialogue.node_kind.Set then
		self.state_picker.state = node.variable
	elseif node.kind == tdengine.dialogue.node_kind.Increment then
		self.state_picker.state = node.variable
	elseif node.kind == tdengine.dialogue.node_kind.Branch then
		self.condition_list:set_branches(node:get_branches())
	elseif node.kind == 'ChoiceList' then
		self.last_choice_list = node.uuid
	end

	-- @spader: We can move the ignored fields to the node classes themselves.
	-- Also, we should add read only fields for Professionalism.
	local editor_params = {}
	editor_params.imgui_ignore = table.shallow_copy(node.imgui_ignore)
	editor_params.imgui_ignore.children = true
	editor_params.imgui_ignore.is_entry_point = true

	if node.kind == tdengine.dialogue.node_kind.Text then
		editor_params.imgui_ignore.input = true
	elseif node.kind == tdengine.dialogue.node_kind.Choice then
		editor_params.imgui_ignore.shown = true
	elseif node.kind == tdengine.dialogue.node_kind.ChoiceList then
		editor_params.imgui_ignore.buttons = true
		editor_params.imgui_ignore.indices = true
	elseif node.kind == tdengine.dialogue.node_kind.Set then
		editor_params.imgui_ignore.variable = true
	elseif node.kind == tdengine.dialogue.node_kind.Increment then
		editor_params.imgui_ignore.variable = true
	elseif node.kind == tdengine.dialogue.node_kind.Wait then
		editor_params.imgui_ignore.accumulated = true
		editor_params.imgui_ignore.active = true
	elseif node.kind == tdengine.dialogue.node_kind.Branch then
		editor_params.imgui_ignore.branches = true
		editor_params.imgui_ignore.combinator = true
	end

	-- Create a table editor
	self.selected_editor = imgui.extensions.TableEditor(node, editor_params)

	-- Set up the text editor
	local text_editor = tdengine.find_entity_editor('TextEditor')
	text_editor:set_text(node.text)
end

function DialogueEditor:on_node_color(id)
	-- Node color processed in reverse priority order. For example, we check if it's highlighted
	-- after we check if it's a Set node, because we want the highlighted color to override the
	-- Set node color. If I was less lazy, we'd use this to figure out the node color and then
	-- adjust the saturation or something when it was highlighted.
	local node = self.nodes[id]
	local color = self.colors.default
	color = self.colors.nodes[node.kind] or color

	if node.is_entry_point then
		color = self.colors.entry_point
	end

	for i, err_node in pairs(self.validation_errors) do
		if err_node.node == node.uuid then
			local alpha = tdengine.math.ranged_sin(tdengine.elapsed_time * 8, .5, 1)
			color = self.colors.invalid:alpha(alpha)
		end
	end

	local is_active = tdengine.dialogue.controller:is_node_active(node.uuid)
	local is_find_result = self.search_result[node.uuid] or self:does_node_use_label(node, self.find_label)
	if is_find_result or is_active then
		local alpha = tdengine.math.ranged_sin(tdengine.elapsed_time * 8, 0, 1)
		color = self.colors.find_result:alpha(alpha)
	end

	local highlight = false
	highlight = highlight or id == self.node_editor.hovered
	highlight = highlight or self.node_editor:is_node_selected(node.uuid)
	if highlight then
		color = self.colors.highlighted
	end

	return color
end

function DialogueEditor:on_node_links(output_id, input_id)
	self.colors = {
		branch_true = tdengine.colors.spring_green:copy(),
		branch_false = tdengine.colors.cardinal:copy(),
		default = tdengine.colors.charcoal:copy(),
		highlighted = tdengine.colors.cool_gray:copy(),
		entry_point = tdengine.colors.cadet_gray:copy(),
		find_result = tdengine.colors.celadon:copy(),
		invalid = tdengine.colors.cardinal:copy(),
		child_index = {
			label = tdengine.colors.white:copy(),
			background = tdengine.colors.gunmetal:copy(),
		},
		nodes = {
			[tdengine.dialogue.node_kind.End] = tdengine.colors.orange:copy(),
			[tdengine.dialogue.node_kind.Set] = tdengine.colors.tyrian_purple:copy(),
			[tdengine.dialogue.node_kind.ChoiceList] = tdengine.colors.zomp:copy(),
			[tdengine.dialogue.node_kind.Label] = tdengine.colors.indian_red:copy(),
			[tdengine.dialogue.node_kind.Call] = tdengine.colors.prussian_blue:copy(),
			[tdengine.dialogue.node_kind.Jump] = tdengine.colors.prussian_blue:copy(),
			[tdengine.dialogue.node_kind.Return] = tdengine.colors.prussian_blue:copy(),
			[tdengine.dialogue.node_kind.DevMarker] = tdengine.colors.cadet_gray:copy(),
			[tdengine.dialogue.node_kind.DevNote] = tdengine.colors.cadet_gray:copy(),
		}
	}

	imgui.GetWindowDrawList():ChannelsSetCurrent(1)
	local output_slot = self.node_editor:output_slot(output_id)
	local input_slot = self.node_editor:input_slot(input_id)
	local middle = output_slot:add(input_slot):scale(.5)

	local node = self.nodes[output_id]

	local child_index = 1
	for index, child_id in pairs(node.children) do
		if child_id == input_id then
			child_index = index; break;
		end
	end

	if node:is_bifurcate() then
		-- For branch nodes, the children get a green or red dot
		local extents = tdengine.vec2(5, 5):scale(self.node_editor.zoom)
		local min = middle:subtract(extents)
		local max = middle:add(extents)
		local color = ternary(child_index == 1, self.colors.branch_true, self.colors.branch_false)
		local rounding = 50
		imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(min.x, min.y), imgui.ImVec2(max.x, max.y), color:to_u32(), rounding)
	elseif #node.children > 1 then
		-- Otherwise, we want to give the child  a number matching its index.

		-- First, draw a filled in circle halfway between the slots
		local index_glyph_offset = tdengine.vec2(10, 5)
		local min = middle:subtract(index_glyph_offset)
		local extents = tdengine.vec2(20, 20):scale(self.node_editor.zoom)
		local max = min:add(extents)
		imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(min.x, min.y), imgui.ImVec2(max.x, max.y), self.colors.child_index.background:to_u32(), 50)

		-- Then, put some text in the middle of the circle
		local text_offset = tdengine.vec2(8, 2) -- Experimental
		local text_pos = min:add(text_offset)
		imgui.SetCursorScreenPos(text_pos:unpack())

		-- And draw the child's index inside it
		imgui.PushStyleColor(ffi.C.ImGuiCol_Text, self.colors.child_index.label:to_u32())
		imgui.Text(tostring(child_index))
		imgui.PopStyleColor()
	end
	imgui.GetWindowDrawList():ChannelsSetCurrent(0)
end

function DialogueEditor:on_node_draw(id)
	local node = self.nodes[id]
	local display_name = ternary(node.who, node.who, node.kind)

	imgui.PushFont('editor-bold-32')
	imgui.Text(display_name)
	imgui.PopFont()

	if node.kind == tdengine.dialogue.node_kind.DevMarker then
		imgui.PushFont('editor-24')
	end

	imgui.PushFont('editor-16')
	imgui.Text(node:short_text())
	imgui.PopFont()

	if node.kind == tdengine.dialogue.node_kind.DevMarker then
		imgui.PopFont()
	end
end

function DialogueEditor:on_node_context(id)
	local node = self.nodes[id]
	if imgui.MenuItem('Set as entry point') then
		for i, node in pairs(self.nodes) do
			node.is_entry_point = false
		end

		node.is_entry_point = true
		self:mark_graph_dirty()
	end

	-- Move this node up or down in its parent's list of children. Since we do not track this, we have to look
	-- through every node's children until we find the parent. This does a lot of string comparisons, so
	-- it's not the fastest, but plenty fast for our purposes.
	--
	-- Just ignore the case where a node can have more than one child. And by ignore, I mean that we
	-- choose the first parent we find.
	local parent_node, index = self:find_first_parent(node)
	if (parent_node) and (index > 1) then
		if imgui.MenuItem('Move up') then
			self:move_node_position(parent_node, index, -1)
		end
	end

	if (parent_node) and (index < #parent_node.children) then
		if imgui.MenuItem('Move down') then
			self:move_node_position(parent_node, index, 1)
		end
	end

	if node.kind == tdengine.dialogue.node_kind.Return then
		if imgui.MenuItem('Jump to marker') then
			local target = find_node(node.return_to, self.nodes)
			if not target then return end -- This should never happen; validation should check missing node
			self.node_editor:snap_to_node(target.uuid)
			self.node_editor:select_single_node(target.uuid)
		end
	end

	if node.kind == tdengine.dialogue.node_kind.Switch then
		if imgui.MenuItem('Open dialogue') then
			self.queued_to_load = node.next_dialogue
		end
	end
end

function DialogueEditor:on_node_connect(parent_id, child_id)
	self:mark_graph_dirty()
end

function DialogueEditor:on_node_disconnect(parent_id, child_id)
	self:mark_graph_dirty()
end

function DialogueEditor:on_node_add(kind)
	local node = tdengine.create_node(kind)
	self.nodes[node.uuid] = node
	self:mark_graph_dirty()

	-- A little QOL thing, where when you're writing dialogue your best guess for who a new node's
	-- character is is that well it's probably whoever said the _last_ thing, so we just fill it in
	-- like that, and if we're wrong, no harm no foul, you would've had to edit it anyway.
	if node.kind == 'Text' then
		if self.last_who then
			node.who = self.last_who
		else
			node.who = ''
		end
	end

	return node.uuid
end

function DialogueEditor:on_node_delete(id)
	self:mark_graph_dirty()
end

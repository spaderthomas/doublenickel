-- UTILITIES
local types = {
	'number',
	'string',
	'bool',
	'table'
}

local function find_sorted_keys(t)
	local sorted_keys = {}
	for key, value in pairs(t) do
		table.insert(sorted_keys, key)
	end

	local compare_keys = function(ka, kb)
		local va = t[ka]
		local vb = t[kb]

		local ma = tdengine.editor.get_field_metadata(tdengine.class.get(t), ka)
		local mb = tdengine.editor.get_field_metadata(tdengine.class.get(t), kb)

		local A_FIRST = true
		local B_FIRST = false

		local is_a_cdata = type(va) == 'cdata'
		local is_b_cdata = type(vb) == 'cdata'
		if is_a_cdata and not is_b_cdata then
			return A_FIRST
		elseif not is_a_cdata and is_b_cdata then
			return B_FIRST
		end

		-- 1. Tables always come before non-tables
		local is_va_table = type(va) == 'table' and not tdengine.enum.is_enum(va)
		local is_vb_table = type(vb) == 'table' and not tdengine.enum.is_enum(vb)
		if is_va_table and not is_vb_table then
			return A_FIRST
		elseif not is_va_table and is_vb_table then
			return B_FIRST
		end

		if ma.read_only and not mb.read_only then
			return A_FIRST
		elseif not ma.read_only and mb.read_only then
			return B_FIRST
		end

		return tostring(ka) < tostring(kb)
	end

	table.sort(sorted_keys, compare_keys)

	return sorted_keys
end

--
-- TABLE
--
imgui.extensions.TableField = function(key, value, padding)
	local cursor = imgui.GetCursorPosX()

	local value_type = type(value)
	if value_type == 'string' then
		imgui.extensions.VariableName(key)
		imgui.SameLine()
		imgui.PushTextWrapPos(0)
		if padding then imgui.SetCursorPosX(cursor + padding) end
		imgui.Text(value)
		imgui.PopTextWrapPos()
	elseif value_type == 'number' then
		imgui.extensions.VariableName(key)
		imgui.SameLine()
		if padding then imgui.SetCursorPosX(cursor + padding) end
		imgui.Text(tostring(value))
	elseif value_type == 'boolean' then
		imgui.extensions.VariableName(key)
		imgui.SameLine()
		if padding then imgui.SetCursorPosX(cursor + padding) end
		imgui.Text(tostring(value))
	elseif value_type == 'table' then
		imgui.extensions.TableMenuItem(key, value)
	elseif value_type == 'cdata' then
		imgui.extensions.CType(key, value)
	elseif value_type == 'function' then
		imgui.extensions.VariableName(key)
		imgui.SameLine()
		if padding then imgui.SetCursorPosX(cursor + padding) end
		imgui.Text('<function>')
	end
end

TableOptions = tdengine.class.define('TableOptions')
function TableOptions:init()
	self.ignore_functions = false
	self.ignored_fields = {}
end

imgui.extensions.Table = function(t, options)
	options = options or TableOptions:new()

	local sorted_keys = find_sorted_keys(t)
	local padding = imgui.internal.calc_alignment(table.collect_keys(t))


	for index, k in pairs(sorted_keys) do
		local value = t[k]
		if tdengine.editor.is_ignoring_field(t, k) then goto continue end
		if options.ignored_fields[k] then goto continue end
		if options.ignore_functions and type(value) == 'function' then goto continue end

		imgui.extensions.TableField(k, t[k], padding)

		::continue::
	end
end

imgui.extensions.TableMenuItem = function(name, t)
	local address = table_address(t)
	local imgui_id = tostring(name) .. '##' .. address

	if imgui.TreeNode(imgui_id) then
		imgui.extensions.Table(t)
		imgui.TreePop()
	end
end


StructEditor = tdengine.class.define('StructEditor')

function StructEditor:init(name, struct, color)
	self.name = name
	self.struct = struct
	self.color = tdengine.colors.red:copy()

	self.colors = {
		type_column = tdengine.colors.cadet_gray:copy()
	}

	self.padding = 12
	self.columns = {
		field_name = 0,
		field_type = 0,
		widget = 0
	}

	
	local type_info = tdengine.ffi.inner_typeof(self.struct)
	for member_info in type_info:members() do
		self.columns.field_name = math.max(self.columns.field_name, imgui.CalcTextSize(member_info.name).x)
	end

	self.columns.field_type = imgui.CalcTextSize('i32*').x
	for member_info in type_info:members() do
		local inner_type = tdengine.ffi.inner_type(member_info)
		local pretty_type = tdengine.ffi.pretty_ptr(inner_type)
		self.columns.field_type = math.max(
			self.columns.field_type, 
			imgui.CalcTextSize(pretty_type).x)
	end

	for column_name in tdengine.iterator.keys(self.columns) do
		self.columns[column_name] = self.columns[column_name] + self.padding
	end
end

function StructEditor:draw()
	local label = string.format('%s##%s', self.name, tdengine.ffi.address_of(self.struct))

	local type_info = tdengine.ffi.inner_typeof(self.struct) -- For ref types

	if imgui.TreeNode(label) then
		for member_info in tdengine.ffi.sorted_members(type_info) do
			self:member(self.struct, member_info)
		end

		imgui.TreePop()
	end
end


function StructEditor:use_name_column()
	imgui.Columns(3, nil, false)
	imgui.SetColumnWidth(0, self.columns.field_name)
end

function StructEditor:use_type_column()
	imgui.NextColumn()
	imgui.SetColumnWidth(1, self.columns.field_type)
end

function StructEditor:use_widget_column()
	imgui.NextColumn()
	imgui.SetColumnWidth(2, 2000)--self.columns.widget)
end


function StructEditor:member(struct, member_info)
	local ctype = tdengine.enums.ctype

	local inner_type = member_info.type

	if ctype.float:match(inner_type.what) then
		self:CFloatMember(struct, member_info)

	elseif ctype.enum:match(inner_type.what) then
		-- p(struct[member_info.name])
		self:enum(member_info.name, tdengine.ffi.field_ptr(struct, member_info), struct[member_info.name], inner_type)

	elseif ctype.int:match(inner_type.what) and inner_type.bool then
		self:bool(member_info.name, tdengine.ffi.field_ptr(struct, member_info))

	elseif ctype.int:match(inner_type.what) then
		self:int(member_info.name, tdengine.ffi.field_ptr(struct, member_info))

	elseif ctype.struct:match(inner_type.what) then
		StructEditor:new(member_info.name, struct[member_info.name], self.color):draw()

	elseif ctype.ptr:match(inner_type.what) then
		self:pointer(member_info.name, struct[member_info.name])
	else
		-- p(member_info)
	end
end

function StructEditor:name_column(field_name)
	self:use_name_column()
	imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.editor.colors.scalar:to_u32())
	imgui.Text(field_name)
	imgui.PopStyleColor()
end

function StructEditor:type_column(pretty_type)
	self:use_type_column()
	imgui.PushStyleColor(ffi.C.ImGuiCol_Text, self.colors.type_column:to_u32())
	imgui.Text(pretty_type)
	imgui.PopStyleColor()
end

function StructEditor:opaque_ptr(field_name, type_info)
	self:name_column(field_name)
	self:type_column(type_info.name)

	self:use_widget_column()
	imgui.Text('opaque')
	imgui.Columns()

end

function StructEditor:Float(field_name, ptr)
	self:name_column(field_name)
	self:type_column(tdengine.ffi.pretty_typeof(ptr))

	self:use_widget_column()

	imgui.PushItemWidth(-1)
	local label = string.format('##%s:%s', tdengine.ffi.address_of(ptr), field_name)
	imgui.InputScalar(label, tdengine.ffi.imgui_datatypeof(ptr), ptr, nil, nil, '%.3f', 0)
	imgui.PopItemWidth()
	imgui.Columns()
end

function StructEditor:CFloatMember(struct, member_info)
	return self:Float(member_info.name, tdengine.ffi.field_ptr(struct, member_info))
end


function StructEditor:int(field_name, ptr)
	self:name_column(field_name)
	self:type_column(tdengine.ffi.pretty_typeof(ptr))

	self:use_widget_column()
	imgui.PushItemWidth(-1)
	local label = string.format('##%s:%s', tdengine.ffi.address_of(ptr), field_name)
	imgui.InputScalar(label, tdengine.ffi.imgui_datatypeof(ptr), ptr, nil, nil, '%.3f', 0)

	-- ffi.C.igInputInt(label, ptr, 0, 0, 0)
	imgui.PopItemWidth()
	imgui.Columns()
end

function StructEditor:bool(field_name, ptr)
	self:name_column(field_name)
	self:type_column(tdengine.ffi.pretty_typeof(ptr))

	self:use_widget_column()
	imgui.PushItemWidth(-1)
	local label = string.format('##%s:%s', tdengine.ffi.address_of(ptr), field_name)
	ffi.C.igCheckbox(label, ptr)

	-- ffi.C.igInputInt(label, ptr, 0, 0, 0)
	imgui.PopItemWidth()
	imgui.Columns()
end


function StructEditor:pointer(field_name, ptr)
	self:name_column(field_name)
	self:type_column(tdengine.ffi.pretty_ptrof(ptr))

	self:use_widget_column()
	if tdengine.ffi.is_opaque(ptr) then
		imgui.Text('OPAQUE')
	else
		imgui.Text(tdengine.ffi.address_of(ptr))
	end
	imgui.Columns()
end



function StructEditor:enum(field_name, ptr, current_value, enum_type)
	-- p(tdengine.ffi.typeof(ptr))
	local current_enum_info = enum_type:value(current_value)

	self:name_column(field_name)
	self:type_column(tdengine.ffi.pretty_type(enum_type))


	self:use_widget_column()
	imgui.PushItemWidth(-1)
	local label = string.format('##%s:%s', tdengine.ffi.address_of(ptr), field_name)
	if imgui.BeginCombo(label, current_enum_info.name) then
		for enum in enum_type:values() do
			if imgui.Selectable(enum.name, enum.value == ptr) then
				ptr[0] = enum.value
			end
		end

		imgui.EndCombo()
	end
	imgui.PopItemWidth()
	imgui.Columns()

end







function imgui.extensions.CArray(name, array)
	local label = string.format('%s##%s', name, tdengine.ffi.address_of(array))
	if imgui.TreeNode(label) then
		local inner_type = tdengine.ffi.inner_typeof(array)
		local element_type = inner_type.element_type

		local num_elements = inner_type.size / inner_type.element_type.size

		for i = 0, num_elements, 1 do
			imgui.extensions.CType(tostring(i), array[i])
			-- print(i)
		end
		imgui.TreePop()
	end
end

function imgui.extensions.CStruct(name, struct)
			local editor = StructEditor:new(name, struct)
	editor:draw()
end

function imgui.extensions.CType(name, value)
	local ctype = tdengine.enums.ctype

	local inner_type = tdengine.ffi.inner_typeof(value)
	if ctype.struct:match(inner_type.what) then
		imgui.extensions.CStruct(name, value)
	elseif ctype.array:match(inner_type.what) then
		imgui.extensions.CArray(name, value)
		
		-- print(num_elements)
		-- p(inner_type)
	end
end


--
-- TABLE EDITOR
--
imgui.extensions.TableEditor = function(editing, params)
	if not editing then dbg() end
	if not params then params = {} end
	local editor = {
		depth = params.depth or 0,
		enter_returns_true = params.enter_returns_true or false,
		on_end = params.on_end or nil,
		is_table_editor = true,
		key_id = tdengine.uuid_imgui(),
		value_id = tdengine.uuid_imgui(),
		type_id = tdengine.uuid_imgui(),
		selected_type = 'string',
		editing = editing,
		children = {},
		imgui_ignore = params.imgui_ignore or {},
		array_replace_name = params.array_replace_name or nil,
		draw_field_add = params.draw_field_add or false,
		child_field_add = params.child_field_add or false,
		field_add_key = '',
		field_add_value = '',
		context_menu_item = nil,
		seen = params.seen or {},
		draw = function(self) return imgui.internal.draw_table_editor(self) end,
		clear = function(self) imgui.internal.clear_table_editor(self) end,
		ignore = function(self, k) self.imgui_ignore[k] = true end,
		is_field_ignored = function(self, k)
			local ignore = false
			ignore = ignore or self.imgui_ignore[k]
			if self.editing.imgui_ignore then
				ignore = ignore or self.editing.imgui_ignore[k]
			end
			return ignore
		end,
		is_self_referential = function(self, t)
			local address = table_address(t)
			return self.seen[address]
		end,
		is_enum = function(self, t)
			return type(t) == 'table' and t.__enum
		end,
		add_child = function(self, key)
			local value = self.editing[key]
			if type(value) ~= 'table' then return end
			if self.imgui_ignore[key] then return end

			recurse = true
			recurse = recurse and not (value == self.editing)
			recurse = recurse and not self.editing.is_table_editor
			recurse = recurse and not self:is_field_ignored(key)
			recurse = recurse and not self:is_self_referential(value)
			recurse = recurse and not self:is_enum(value)
			if recurse then
				local params = imgui.internal.propagate_table_editor_params(self)
				self.children[key] = imgui.extensions.TableEditor(value, params)
			end
		end
	}

	editor.imgui_ignore.imgui_ignore = true
	editor.imgui_ignore.__type = true
	editor.imgui_ignore.__internal = true
	editor.seen[table_address(editor.editing)] = true

	-- Each child member that is a non-recursive table also gets an editor
	for key, child in pairs(editing) do
		editor:add_child(key)
	end

	return editor
end

imgui.internal.draw_table_editor = function(editor)
	-- If the user right clicked a sub-table, we show a prompt to add an entry to the sub-table.
	-- Record whether a field was added in this way to return to the user
	local field_added = false
	if editor.draw_field_add then
		print('small'); field_added = imgui.internal.draw_table_field_add(editor)
	end

	local field_changed = false
	local fields_changed = {}
	local mark_field_changed = function(field)
		field_changed = true
		table.insert(fields_changed, field)
	end

	-- Figure out ImGui stuff for alignment
	local cursor = imgui.GetCursorPosX()
	local padding = imgui.internal.calc_alignment(table.collect_keys(editor.editing))
	local open_item_context_menu = false

	local sorted_keys = find_sorted_keys(editor.editing)

	-- Display each KVP
	for _, key in ipairs(sorted_keys) do
	
		local value = editor.editing[key]
		local metadata = tdengine.editor.get_field_metadata(tdengine.class.get(editor.editing), key)

		-- Skip the variable if it's in the imgui_ignore for either the editor, or the table being edited
		local display = true
		display = display and not editor.imgui_ignore[key]
		if editor.editing.imgui_ignore then
			display = display and (not editor.editing.imgui_ignore[key])
		end

		if key == '__enum' then p(editor.editing[tdengine.editor.sentinel].ignore) end
		if tdengine.editor.is_ignoring_field(editor.editing, key) then
			display = false
		end

		-- Likewise, don't display imgui_ignore
		if key == 'imgui_ignore' then display = false end
		if key == tdengine.editor.sentinel then display = false end

		-- Assign a UNIQUE label to this entry
		local label = string.format('##%s', hash_table_entry(editor.editing, tostring(key)))

		local display_key = key
		if type(key) == 'number' and editor.array_replace_name then
			display_key = editor.array_replace_name(key, value)
		end

		-- This is a two-way binding. If ImGui says that the input box was edited, we take the value from C and put it into Lua.
		-- Otherwise, we take the value from Lua and put it into C, in case any value changes in the interpreter. This is slow -- it
		-- means we copy every string in all tables we're editing into C every frame. I can't think of a better way to do it, because
		-- there is no mechanism for triggering a callback whenever a string in Lua changes (nor would we want one) short of
		-- metatable insanity.
		if display and metadata.read_only then
			imgui.extensions.TableField(key, value, padding)
		elseif display and not metadata.read_only then
			-- local variable_name_color = imgui.internal.table_editor_depth_color(editor.depth)
			local variable_name_color = tdengine.editor.colors.scalar

			-- Strings
			if type(value) == 'string' then
				imgui.extensions.VariableName(display_key, variable_name_color)
				if imgui.IsItemClicked(1) then
					open_item_context_menu = true; editor.context_menu_item = key
				end
				imgui.SameLine()
				imgui.SetCursorPosX(cursor + padding)
				imgui.PushItemWidth(-1)

				local flags = 0
				if editor.enter_returns_true then flags = ffi.C.ImGuiInputTextFlags_EnterReturnsTrue end

				if imgui.InputText(label, editor.editing, key, flags) then
					mark_field_changed(key)
				end

				imgui.PopItemWidth()

				-- Numbers
			elseif type(value) == 'number' then
				imgui.extensions.VariableName(display_key, variable_name_color)
				if imgui.IsItemClicked(1) then
					open_item_context_menu = true; editor.context_menu_item = key
				end
				imgui.SameLine()
				imgui.SetCursorPosX(cursor + padding)

				local input_label = string.format('%s:input', label)
				local slider_label = string.format('%s:slider', label)

				imgui.PushItemWidth(100)
				if imgui.SliderFloat(slider_label, editor.editing, key, metadata.slider_min, metadata.slider_max) then
					mark_field_changed(key)
				end
				imgui.PopItemWidth()
				imgui.SameLine()

				imgui.PushItemWidth(-1)
				if imgui.InputFloat(input_label, editor.editing, key) then
					mark_field_changed(key)
				end
				imgui.PopItemWidth()

				-- Booleans
			elseif type(value) == 'boolean' then
				imgui.extensions.VariableName(display_key, variable_name_color)
				if imgui.IsItemClicked(1) then
					open_item_context_menu = true; editor.context_menu_item = key
				end
				imgui.SameLine()
				imgui.SetCursorPosX(cursor + padding)
				imgui.PushItemWidth(-1)

				if imgui.Checkbox(label, editor.editing, key) then
					mark_field_changed(key)
				end
			elseif type(value) == 'cdata' then
				imgui.extensions.CType(display_key, value)
				-- Functions, trivialls
			elseif type(value) == 'function' then
				-- @spader 2/20/23: It's useful sometimes to see whether a function member is set,
				-- but I never use this, so don't do it to avoid clutter
				if false then
					imgui.extensions.VariableName(display_key, variable_name_color)
					imgui.SameLine()
					imgui.SetCursorPosX(cursor + padding)
					imgui.PushItemWidth(-1)

					imgui.Text('function')
					imgui.PopItemWidth()
				end
			elseif type(value) == 'thread' then
				-- Colors (at this point, we know it's a table)
			elseif tdengine.is_color_like(value) then
				local picker_flags = 0

				if imgui.TreeNode(display_key .. label) then
					imgui.ColorPicker4(label, editor.editing, key, picker_flags)
					imgui.TreePop()
				end
			elseif editor:is_self_referential(value) then
				if false then
					imgui.extensions.VariableName(display_key, variable_name_color)
					imgui.SameLine()
					imgui.SetCursorPosX(cursor + padding)
					imgui.PushItemWidth(-1)

					imgui.Text('self referential')
					imgui.PopItemWidth()
				end
			elseif tdengine.enum.is_enum(value) then
				imgui.extensions.VariableName(display_key, variable_name_color)
				if imgui.IsItemClicked(1) then
					open_item_context_menu = true;
					editor.context_menu_item = key
				end
				imgui.SameLine()
				imgui.SetCursorPosX(cursor + padding)
				imgui.PushItemWidth(-1)

				if imgui.BeginCombo(label, value:to_string()) then
					local enum_data = tdengine.enum_data[value.__enum]
					local enums = {}
					for name, _ in pairs(enum_data) do
						table.insert(enums, name)
					end
					table.sort(enum_data)

					for _, enum in pairs(enums) do
						if imgui.Selectable(enum) then
							editor.editing[key] = enum_data[enum]
							mark_field_changed(key)
						end
					end
					imgui.EndCombo()
				end
				imgui.PopItemWidth()

				-- All other tables
			elseif type(value) == 'table' then
				-- If this is the first time we've seen this sub-table, make an editor for it.
				if not editor.children[key] then
					local params = imgui.internal.propagate_table_editor_params(editor)
					editor.children[key] = imgui.extensions.TableEditor(value, params)
					mark_field_changed(key)
				end
				local child = editor.children[key]
				child.editing = value

				-- Here's where we draw the tree node for the child table. If it's down, we recurse. We
				-- also keep track of whether we'll need to open the modals
				local open_context_menu = false
				local unique_treenode_id = display_key .. label
				if imgui.TreeNode(unique_treenode_id) then
					if imgui.IsItemClicked(1) then open_context_menu = true end
					sub_field_added, sub_field_changed = imgui.internal.draw_table_editor(child, seen)
					field_added = field_added or sub_field_added
					if sub_field_changed then mark_field_changed(key) end

					imgui.TreePop()
				else
					if imgui.IsItemClicked(1) then open_context_menu = true end
				end

				-- At this point, table + header are drawn. We've just got to show any popups.
				-- Show the context menu
				local open_field_editor = false
				local context_menu_id = label .. ':context_menu'
				if open_context_menu then
					imgui.OpenPopup(context_menu_id)
				end
				if imgui.BeginPopup(context_menu_id) then
					if imgui.MenuItem('Add field') then
						open_field_editor = true
					end
					imgui.EndPopup()
				end

				-- Show the aforementioned field editor modal
				local field_editor_id = label .. ':field_editor'
				if open_field_editor then
					imgui.OpenPopup(field_editor_id)
				end
				if imgui.BeginPopup(field_editor_id) then
					if imgui.internal.draw_table_field_add(child) then
						imgui.CloseCurrentPopup()
					end
					imgui.EndPopup()
				end

				::done_table::
			end
		end
	end

	-- If any variable was right clicked, we show a little context menu where you can delete it
	local item_context_menu_id = '##' .. hash_table_entry(editor.editing, editor.context_menu_item)
	if open_item_context_menu then
		imgui.OpenPopup(item_context_menu_id)
	end
	if imgui.BeginPopup(item_context_menu_id) then
		if imgui.Button('Delete') then
			editor.editing[editor.context_menu_item] = nil
			editor.context_menu_item = nil
			imgui.CloseCurrentPopup()
		end
		imgui.EndPopup()
	end

	if editor.on_done then editor.on_done() end

	for _, field in pairs(fields_changed) do
		tdengine.editor.run_editor_callback(editor.editing, 'on_change_field', field)
	end

	return field_added, field_changed
end

imgui.internal.draw_table_field_add = function(editor)
	imgui.PushItemWidth(80)
	if imgui.BeginCombo(editor.type_id, editor.selected_type) then
		for index, name in pairs(types) do
			if imgui.Selectable(name) then
				editor.selected_type = name
			end
		end
		imgui.EndCombo()
	end
	imgui.PopItemWidth()

	imgui.SameLine()
	imgui.extensions.VariableName('key')


	local enter_on_key = false
	local enter_on_value = false
	imgui.PushItemWidth(100)
	imgui.SameLine()
	if imgui.InputText(editor.key_id, editor, 'field_add_key', ffi.C.ImGuiInputTextFlags_EnterReturnsTrue) then
		enter_on_key = true
	end
	imgui.PopItemWidth()

	imgui.PushItemWidth(170)
	if editor.selected_type ~= 'table' then
		imgui.SameLine()
		imgui.extensions.VariableName('value')

		imgui.SameLine()
		if imgui.InputText(editor.value_id, editor, 'field_add_value', ffi.C.ImGuiInputTextFlags_EnterReturnsTrue) then
			enter_on_value = true
		end
	end

	if enter_on_key or enter_on_value then
		local key = editor.field_add_key
		key = tonumber(key) or key

		local value = editor.field_add_value

		editor.field_add_key = ''
		editor.field_add_value = ''

		if value == 'nil' then
			value = nil
		elseif editor.selected_type == 'number' then
			value = tonumber(value) or 0
		elseif editor.selected_type == 'string' then
			value = tostring(value) or ''
		elseif editor.selected_type == 'bool' then
			value = (value == 'true')
		elseif editor.selected_type == 'table' then
			local params = imgui.internal.propagate_table_editor_params(editor)
			editor.children[key] = imgui.extensions.TableEditor(value, params)
		end

		editor.editing[key] = value
		imgui.SetKeyboardFocusHere(-1)
	end
	imgui.PopItemWidth()

	return enter_on_key or enter_on_value
end

function imgui.internal.calc_alignment(keys)
	local max_key_size = 0
	for key in tdengine.iterator.values(keys) do
		local display_key = tostring(key)
		local key_size = imgui.CalcTextSize(display_key)
		max_key_size = math.max(max_key_size, key_size.x)
	end

	local padding = 12
	return max_key_size + padding
end

imgui.internal.table_editor_padding = function(editor)
	local keys = table.collect_keys(editor.editing)
	-- Very hacky way to line up the inputs: Figure out the largest key, then when drawing a key,
	-- use the difference in length between current key and largest key as a padding. Does not work
	-- that well, but kind of works
end

imgui.internal.table_editor_depth_color = function(depth)
	local colors = {
		tdengine.colors.celadon:copy(),
		tdengine.colors.cool_gray:copy(),
	}
	return colors[(depth % 2) + 1]
end

imgui.internal.propagate_table_editor_params = function(editor)
	local params = {}
	params.draw_field_add = editor.child_field_add
	params.depth = editor.depth + 1
	params.enter_returns_true = editor.enter_returns_true
	params.seen = table.shallow_copy(editor.seen)
	return params
end

imgui.internal.clear_table_editor = function(editor)
	editor.key_id = tdengine.uuid_imgui()
	editor.value_id = tdengine.uuid_imgui()
	editor.type_id = tdengine.uuid_imgui()
	editor.children = {}
end

--
-- WIDGETS
--
imgui.extensions.VariableName = function(name)
	imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.editor.colors.scalar:to_u32())
	imgui.Text(tostring(name))
	imgui.PopStyleColor()
	if imgui.IsItemHovered() then
		sx, sy = imgui.GetItemRectSize();
		x, y = imgui.GetItemRectMin();
		px, py = 5, 3

		imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(x - px, y - py), imgui.ImVec2(x + sx + px, y + sy + py), tdengine.editor.colors.scalar:to_u32())
	end
end

imgui.extensions.Vec2 = function(name, v)
	if not name or not v then
		print(v)
		print('vec2 missing a parameter')
		return
	end
	imgui.extensions.VariableName(name)
	imgui.SameLine()
	imgui.Text('(' .. tostring(v.x) .. ', ' .. tostring(v.y) .. ')')
end

imgui.extensions.WhitespaceSeparator = function(whitespace)
	imgui.Dummy(0, whitespace)
	imgui.Separator()
	imgui.Dummy(0, whitespace)
end

imgui.extensions.CursorBubble = function(extents, color)
	extents       = extents or tdengine.vec2(15, 15)
	color         = color or tdengine.color32(0, 255, 128, 255)

	-- Draw a bubble around the cursor, which separates the nodes
	local color   = tdengine.color32(0, 255, 128, 255)
	local extents = tdengine.vec2(15, 15)
	local middle  = tdengine.vec2(imgui.GetMousePos(0))
	local min     = middle:subtract(extents:scale(.5))
	local max     = middle:add(extents)
	imgui.GetWindowDrawList():AddRectFilled(
		imgui.ImVec2(min.x, min.y), imgui.ImVec2(max.x, max.y), color, 50)
end

imgui.extensions.TextColored = function(text, color)
	imgui.PushStyleColor(ffi.C.ImGuiCol_Text, color)
	imgui.Text(text)
	imgui.PopStyleColor()
end

function imgui.extensions.TreeNodeFont(label, font)
	imgui.PushFont(font)
	local tree_expanded = imgui.TreeNode(label)
	imgui.PopFont()

	return tree_expanded
end

function imgui.extensions.ComboBox(label, active, items, on_select, ...)
	local change = false
	if imgui.BeginCombo(label, active) then
		for index, item in pairs(items) do
			local is_item_selected = item == active;
			if imgui.Selectable(item, is_item_selected) then
				on_select(item, ...)
				change = true
			end
		end

		imgui.EndCombo()
	end

	return change
end

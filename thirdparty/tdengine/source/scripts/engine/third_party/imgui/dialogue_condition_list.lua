--------------------
-- CONDITION LIST --
--------------------
imgui.extensions.DialogueConditionList = tdengine.class.define('ImguiDialogueConditionList')

function imgui.extensions.DialogueConditionList:init()
	self.branches = nil
	self.state_pickers = {}
	self.ids = {
		branch_op = string.format('##branch_op:%s', table.address(self))
	}
end

function imgui.extensions.DialogueConditionList:set_branches(branches)
	self.branches = branches

	table.clear(self.state_pickers)
	for ib, branch in ipairs(self.branches) do
		self:add_state_picker(branch)
	end
end

function imgui.extensions.DialogueConditionList:add_state_picker(branch)
	local address = table_address(branch)
	self.state_pickers[address] = imgui.extensions.StatePicker:new(branch.variable)
end

function imgui.extensions.DialogueConditionList:remove_state_picker(branch)
	local address = table_address(branch)
	self.state_pickers[address] = nil
end

function imgui.extensions.DialogueConditionList:get_state_picker(branch)
	local address = table_address(branch)
	return self.state_pickers[address]
end

function imgui.extensions.DialogueConditionList:add_branch()
	local branch = make_default_branch()
	self:add_state_picker(branch)
	table.insert(self.branches, branch)
end

function imgui.extensions.DialogueConditionList:remove_branch(index)
	local branch = self.branches[index]
	self:remove_state_picker(branch)
	table.remove(self.branches, index)
end

function imgui.extensions.DialogueConditionList:update()
	for ib, branch in ipairs(self.branches) do
		imgui.PushID(table_address(branch))

		-- X buttons before each line, to delete the entry.
		local branch_to_delete
		local color = tdengine.color32(150, 0, 0, 255)
		local label = string.format('X##%d', ib)

		imgui.PushStyleColor(ffi.C.ImGuiCol_Button, color)

		if imgui.Button(label) then
			value_changed = true
			branch_to_delete = ib
		end
		imgui.PopStyleColor()

		imgui.SameLine()

		-- Show a state picker, so you can change what state field
		--
		-- @duplicate: This same code is in the dialogue editor, where you have a state picker and then
		-- a block to update the branch's value if it reports a change
		local state_picker = self:get_state_picker(branch)
		local change, state = state_picker:update()
		if change then
			branch.variable = state

			-- When a new field is selected, give a sane default value
			if tdengine.state.is_number(state) then
				branch.value = 0
			elseif tdengine.state.is_boolean(state) then
				branch.value = true
			elseif tdengine.state.is_string(state) then
				branch.value = ''
			end
		end
		imgui.SameLine()

		-- Show an op picker
		local valid_ops = get_valid_ops_for_value(branch.value)
		local current_op = branch.op
	
		imgui.PushItemWidth(175)
		if imgui.BeginCombo(self.ids.branch_op, tdengine.branch_op_names[current_op]) then
			for _, op_id in pairs(valid_ops) do
				local op_display_name = tdengine.branch_op_names[op_id]
				local op_selected = op_id == current_op
	
				if imgui.Selectable(op_display_name, op_selected) then
					branch.op = op_id
				end
			end
			imgui.EndCombo()
		end
	
		imgui.PopItemWidth()

		imgui.SameLine()


		-- An appropriate InputXXX, depending on the type of the value
		local label = string.format('%s:%s:%s', '##state_picker', table_address(branch), branch.variable)
		local var = index_string(tdengine.state.data, branch.variable)
		if is_number(var) then
			imgui.PushItemWidth(100)
			imgui.InputFloat(label, branch, 'value')
			imgui.PopItemWidth()
		elseif is_bool(var) then
			imgui.Checkbox(label, branch, 'value')
		elseif tdengine.state.is_string(branch.variable) then
			imgui.PushItemWidth(100)
			imgui.InputText(label, branch, 'value')
			imgui.PopItemWidth()
		end

		imgui.PopID()

		if branch_to_delete then self:remove_branch(branch_to_delete) end
	end

	-- Finally, a button to add another entry
	if imgui.Button('Add Condition') then
		self:add_branch()
	end

	return value_changed, field_combo_open, field_changed
end

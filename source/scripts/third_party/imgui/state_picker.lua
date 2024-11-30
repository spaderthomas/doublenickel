imgui.extensions.StatePicker = tdengine.class.define('ImguiStatePicker')

function imgui.extensions.StatePicker:init(state)
	self.filter = InputFilter:new()
	self.input = ContextualInput:new(tdengine.enums.InputContext.Editor, tdengine.enums.CoordinateSystem.Game)
	self.state = state or ''
	self.is_open = false
end

function imgui.extensions.StatePicker:filter_state()
	local sorted_state = tdengine.state.get_sorted_fields()
	local filtered_state = {}

	for index, state in pairs(sorted_state) do
		if self.filter:pass(state) then
			table.insert(filtered_state, state)
		end
	end

	return filtered_state
end

function imgui.extensions.StatePicker:update(width)
	--[[
	I guess this is as good of a place as any to put this. The layout code for a lot of the editor
	(e.g. how wide should this combo box be?) is very naive. In a lot of cases, and in particular
	the UI for picking rolls and conditions and lists thereof, we just hardcode widths.

	I'm sure it's possible to do this in a smarter way, but it pretty much works for me and only
	looks wrong in a few places.

	The default width of 250 is tuned for UI where you have [state] [op] [value] [X]. We want that
	to be large enough so you can see the state field, but not so large that the side menu has to be
	very wide, which eats into the game view / dialogue editor view. In fact, the editor is *highly*
	tuned to this 250 value; the default layout's left side panel is sized so that the aforementioned
	piece of UI fits *exactly*.

	That's why this function takes this stupid parameter; so that in other places (e.g. a Set node's
	detail UI), we can use full width, but in space-conscious places, we use a sane default. Zero width
	means to fill the region.
  ]] --
	width = width or 250
	if width > 0 then imgui.PushItemWidth(250) end

	local change = false
	local next_state = self.state
	self.is_open = false

	local label = '##state_picker:'
	if imgui.BeginCombo(label, self.state) then
		self.is_open = true

		-- Only update the filter if the combo box is open
		self.filter:update()
		local state = self:filter_state()
		imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.color32(0, 255, 0, 255))
		imgui.Text(self.filter.buffer)
		imgui.PopStyleColor()

		-- Show each state field in the combo box
		for i, variable in ipairs(state) do
			local item_selected = variable == self.state
			if imgui.Selectable(variable, item_selected) then
				change = true
				next_state = variable
			end
		end

		-- If there's only one choice, you can press enter to select it and close the combo box
		local is_only_one_choice = #state == 1
		if is_only_one_choice and self.input:pressed(glfw.keys.ENTER) then
			change = true
			next_state = state[1]
			imgui.CloseCurrentPopup()
		end

		imgui.EndCombo()
	end

	if not self.is_open then
		self.filter:clear()
	end

	if width > 0 then imgui.PopItemWidth() end

	self.state = next_state
	return change, next_state
end

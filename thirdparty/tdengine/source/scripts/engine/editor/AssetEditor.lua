DefaultEditor = tdengine.editor.define('DefaultEditor')

DefaultEditor.popup_kind = {
	edit = 'edit##default_editor'
}

function DefaultEditor:init()
	local popups = {
		{
			id = self.popup_kind.edit,
			window = 'Edit Default',
			callback = function() self:popup() end
		}
	}
	self.popups = Popups:new(popups)

	self.animation = {}
	self.table_editor = {}
	self:reset()
end

function DefaultEditor:update(dt)
	self.popups:update()
end

function DefaultEditor:reset()
	self.animation = {
		name = '',
		frames = {}
	}

	self.table_editor = imgui.extensions.TableEditor(self.animation)
end

function DefaultEditor:edit(name)
	local animation = tdengine.animation.find(name)

	self:reset()
	self.animation.name = name
	self.animation.frames = animation.frames
end

function DefaultEditor:popup()
	local done = false

	if imgui.BeginPopupModal('Edit Default', true) then
		self.table_editor:draw()

		-- OK
		if imgui.Button('OK', imgui.ImVec2(100, 25)) then
			if invalid then
				invalid_popup = true
			else
				done = true
			end
		end

		imgui.SameLine()

		-- Cancel
		if imgui.Button('Cancel', imgui.ImVec2(100, 25)) then
			done = true
		end

		-- Cleanup
		if done then
			self:reset()
			imgui.CloseCurrentPopup()
		end
		imgui.EndPopup()
	end
end

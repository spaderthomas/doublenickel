TextureAtlasEditor = tdengine.editor.define('TextureAtlasEditor')

TextureAtlasEditor.popup_kind = {
	edit = 'edit##texture_atlas_editor'
}

TextureAtlasEditor.ids = {
	name = 'Name##texture_atlas_editor',
	folder = '##folder:texture_atlas_editor',
}

function TextureAtlasEditor:init()
	self.popups = Popups:new({
		[self.popup_kind.edit] = {
			window = 'Texture Atlas',
			callback = function() self:create_popup() end
		}
	})

	self.directories = tdengine.data_types.array:new()

	self.atlas_name = ''
	self.folder = ''
end

function TextureAtlasEditor:update()
	self.popups:update()
end

function TextureAtlasEditor:create()
	self.atlas_name = ''
	self.directories = tdengine.data_types.array:new()
end

function TextureAtlasEditor:edit(name)
	self.atlas_name = name
	self.directories = tdengine.data_types.array:new()
	
	local atlas = tdengine.texture.find(self.atlas_name)
	for index, directory in pairs(atlas.directories) do
		self.directories:add(directory)
	end
end

InputTextField = tdengine.class.define('InputTextField')

function TextureAtlasEditor:create_popup()
	if imgui.BeginPopupModal(self.popup_kind.edit) then
		imgui.InputText(self.ids.name, self, 'atlas_name')
		local name_empty = #self.atlas_name == 0
		local name_valid = tdengine.is_extension(self.atlas_name, '.png')

		imgui.SameLine()
		imgui.PushStyleColor(ffi.C.ImGuiCol_Button, tdengine.colors32.button_red_dark)
		if imgui.Button('Delete Atlas') then
			tdengine.texture.delete(self.atlas_name)
			tdengine.texture.save()
			imgui.CloseCurrentPopup()
		end
		imgui.PopStyleColor()

		-- List all current directories in the atlas, with a button to remove
		local dirs_valid = true
		local remove = tdengine.data_types.array:new()
		for index, directory in self.directories:iterate() do
			local label = string.format('%s:%s', directory, index)

			local full_path = tdengine.ffi.resolve_format_path('image', directory):to_interned()

			imgui.Text(directory)

			imgui.SameLine()
			imgui.PushStyleColor(ffi.C.ImGuiCol_Button, tdengine.colors32.button_red)
			local delete_label = string.format('Remove##%s', label)
			if imgui.Button(delete_label) then
				remove:add(index)
			end
			imgui.PopStyleColor()

			imgui.SameLine()
			if tdengine.does_path_exist(full_path) then
				imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.colors32.button_green)
				imgui.Text('(OK)')
				imgui.PopStyleColor()
			else
				imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.colors32.button_red)
				imgui.Text('(BAD)')
				imgui.PopStyleColor()

				dirs_valid = false
			end
		end

		for _, index in remove:iterate() do
			self.directories:remove(index)
		end

		-- An input to add a directory
		imgui.InputText(self.ids.folder, self, 'folder', ffi.C.ImGuiInputTextFlags_EnterReturnsTrue)

		local path = tdengine.ffi.resolve_format_path('image', self.folder):to_interned()
		local folder_empty = #self.folder == 0
		local folder_valid = tdengine.does_path_exist(path)
		imgui.SameLine()

		if folder_valid and not folder_empty then
			imgui.PushStyleColor(ffi.C.ImGuiCol_Button, tdengine.colors32.button_green)
			folder_entered = imgui.Button('Add Folder') or folder_entered
			if folder_entered then
				self.directories:add(self.folder)
			end
			imgui.PopStyleColor()
		else
			imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.colors32.button_red)
			imgui.Text('Invalid folder')
			imgui.PopStyleColor()
		end

		imgui.Dummy(10, 10)
		if not dirs_valid then
			message = string.format('- All directories must exist under %s',
				tdengine.ffi.resolve_named_path('images'):to_interned())
			imgui.Text(message)
		end
		if name_empty then
			local message = '- Atlas name cannot be empty'
			imgui.Text(message)
		end
		if not name_valid then
			local message = '- Atlas name must be a .png'
			imgui.Text(message)
		end

		local valid = name_valid and dirs_valid and not name_empty
		if valid then
			imgui.PushStyleColor(ffi.C.ImGuiCol_Button, tdengine.colors32.button_green)
			if imgui.Button('Save') then
				local atlas = tdengine.texture.find(self.atlas_name)
				atlas.directories = table.deep_copy(self.directories.data)
				atlas.hash = 0
				atlas.mod_time = 0
				atlas.name = self.atlas_name

				tdengine.texture.save()

				self.directories:clear()
			end
			imgui.PopStyleColor()
		else
			imgui.PushStyleColor(ffi.C.ImGuiCol_Button, tdengine.colors32.button_gray)
			imgui.Button('Save')
			imgui.PopStyleColor()
		end


		imgui.SameLine()
		imgui.PushStyleColor(ffi.C.ImGuiCol_Button, tdengine.colors32.button_red)
		if imgui.Button('Cancel') then
			imgui.CloseCurrentPopup()
		end
		imgui.PopStyleColor()

		imgui.EndPopup()
	end
end

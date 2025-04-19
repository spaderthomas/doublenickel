local state = {
	idle = 'idle',
	choosing_dialogue = 'choosing_dialogue',
	choosing_state = 'choosing_state',
	choosing_scene = 'choosing_scene'
}

local MainMenu = dn.editor.define('MainMenu')

function MainMenu:init(params)
	self.open_save_layout_modal = false
	self.open_new_dialogue_modal = false
	self.open_save_dialogue_modal = false
	self.state = state.idle

	self.size = dn.vec2(6, 6)

	self.ids = {
		save_layout = '##ded:save_layout',
		save_dialogue = '##ded:save_dialogue',
		new_dialogue = '##ded:new_dialogue',
		new_state = '##ded:new_state',
		editor_full = '##main_menu:editor_full',
		editor_minimal = '##main_menu:editor_minimal',
	}

	self.saved_layout_name = ''
	self.saved_dialogue_name = ''

	self.input = ContextualInput:new(dn.enums.InputContext.Editor, dn.enums.CoordinateSystem.Game)
end

function MainMenu:update(dt)
	imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_FramePadding, self.size:unpack())
	if imgui.BeginMainMenuBar() then
		self:Window()
		self:Asset()
		self:Dialogue()
		self:Scene()

		dn.lifecycle.run_callback(dn.lifecycle.callbacks.on_main_menu)
		
		imgui.EndMainMenuBar()
	end
	imgui.PopStyleVar()

	self:update_state()
	self:show_modals()
end

function MainMenu:update_state()
	if self.state == state.choosing_dialogue then
		if imgui.file_browser_is_file_selected() then
			local dialogue_editor = dn.find_entity_editor('DialogueEditor')
			dialogue_editor:load(imgui.file_browser_get_selected_file())
			self.state = state.idle
		end
	elseif self.state == state.choosing_state then
		if imgui.file_browser_is_file_selected() then
			dn.load_save_by_path(imgui.file_browser_get_selected_file())
			self.state = state.idle
		end
	elseif self.state == state.choosing_scene then
		if imgui.file_browser_is_file_selected() then
			local path = imgui.file_browser_get_selected_file()
			local scene = dn.extract_filename(path)
			scene = dn.strip_extension(scene)

			local scene_editor = dn.find_entity_editor('SceneEditor')
			scene_editor:load(scene)

			self.state = state.idle
		end

		if self.input:pressed(glfw.keys.ESCAPE) then
			imgui.file_browser_close()
			self.state = state.idle
		end
	end
end

function MainMenu:show_modals(dt)
	-- Save layout
	if self.open_save_layout_modal then
		imgui.OpenPopup('Save Layout')
	end
	imgui.SetNextWindowSize(250, 100)
	if imgui.BeginPopupModal('Save Layout') then
		imgui.Text('Name')
		imgui.SameLine()
		imgui.InputText(self.ids.save_layout, self, 'saved_layout_name')

		imgui.Dummy(5, 5)

		if imgui.Button('Save') then
			dn.ffi.imgui_save_layout(self.saved_layout_name)
			self.saved_layout_name = ''
			imgui.CloseCurrentPopup()
		end
		imgui.SameLine()

		if imgui.Button('Cancel') then
			self.saved_layout_name = ''
			imgui.CloseCurrentPopup()
		end

		imgui.EndPopup()
	end

	-- Save dialogue
	if self.open_save_dialogue_modal then
		imgui.OpenPopup('Save Dialogue')
	end
	imgui.SetNextWindowSize(250, 100)
	if imgui.BeginPopupModal('Save Dialogue') then
		imgui.Text('Name')
		imgui.SameLine()
		imgui.InputText(self.ids.save_dialogue, self, 'saved_dialogue_name')

		imgui.Dummy(5, 5)

		if imgui.Button('Save') then
			local dialogue_editor = dn.find_entity_editor('DialogueEditor')
			local success = dialogue_editor:save(self.saved_dialogue_name)

			if success then
				dialogue_editor:load(self.saved_dialogue_name)
			else
				self.failed_save_as = self.saved_dialogue_name
			end

			-- Clean up
			self.saved_dialogue_name = ''
			imgui.CloseCurrentPopup()
		end
		imgui.SameLine()

		if imgui.Button('Cancel') then
			self.saved_dialogue_name = ''
			imgui.CloseCurrentPopup()
		end

		imgui.EndPopup()
	end

	-- New dialogue
	if self.open_new_dialogue_modal then
		imgui.OpenPopup('New Dialogue')
	end

	local size = dn.vec2(250, 100)
	dn.editor.center_next_window(size)
	imgui.SetNextWindowSize(size.x, size.y)
	if imgui.BeginPopupModal('New Dialogue') then
		imgui.Text('Name')
		imgui.SameLine()
		imgui.InputText(self.ids.new_dialogue, self, 'saved_dialogue_name')

		imgui.Dummy(5, 5)

		if imgui.Button('Save') then
			local dialogue_editor = dn.find_entity_editor('DialogueEditor')
			dialogue_editor:new(self.saved_dialogue_name)

			-- Clean up
			self.saved_dialogue_name = ''
			imgui.CloseCurrentPopup()
		end
		imgui.SameLine()

		if imgui.Button('Cancel') then
			self.saved_dialogue_name = ''
			imgui.CloseCurrentPopup()
		end

		imgui.EndPopup()
	end


	self.open_save_layout_modal   = false
	self.open_new_dialogue_modal  = false
	self.open_save_dialogue_modal = false
end

function MainMenu:Dialogue()
	if imgui.BeginMenu('Dialogue') then
		if imgui.MenuItem('New') then
			self.open_new_dialogue_modal = true
		end

		-- The hotkey entity in the editor takes care of the hotkey part
		local dialogue_editor = dn.find_entity_editor('DialogueEditor')
		if imgui.MenuItem('Open', 'Ctrl+O') then
			local directory = dn.ffi.paths_resolve('dialogues')
			imgui.file_browser_set_work_dir(directory)
			imgui.file_browser_open()
			self.state = state.choosing_dialogue
		end

		if imgui.MenuItem('Save', 'Ctrl+S') then
			dialogue_editor:save(dialogue_editor.loaded)
		end

		if imgui.MenuItem('Save As') then
			self.open_save_dialogue_modal = true
		end

		if imgui.MenuItem('Delete') then
			dialogue_editor:delete(dialogue_editor.loaded)
		end

		if imgui.BeginMenu('Options') then
			if imgui.MenuItem('Warn on Save', '', dialogue_editor.warn_on_save) then
				dialogue_editor:toggle_warn_on_save()
			end

			if imgui.MenuItem('Pretty Save', '', dialogue_editor.pretty_save) then
				dialogue_editor.pretty_save = not dialogue_editor.pretty_save
			end

			if imgui.MenuItem('Update Metrics') then
				dn.dialogue.update_all_metrics()
			end
			imgui.EndMenu()
		end

		if imgui.BeginMenu('State') then
			if imgui.MenuItem('Reset') then
				dn.state.load_file('default')
			end

			imgui.EndMenu() -- Tools
		end

		imgui.EndMenu() -- Dialogue
	end
end

function MainMenu:Window()
	if imgui.BeginMenu('Window') then
		if imgui.BeginMenu('Layout') then
			if imgui.BeginMenu('Open') then
				local layout_dir = dn.ffi.paths_resolve('layouts')
				for entry in dn.filesystem.iterate_directory(layout_dir) do
					local layout = ffi.string(dn.ffi.string_to_cstr(entry.file_path))
					if imgui.MenuItem(dn.strip_extension(layout)) then
						local file_name = dn.strip_extension(layout)
						dn.ffi.imgui_load_layout(file_name)
					end
				end

				imgui.EndMenu()
			end
   
			if imgui.MenuItem('Save As') then
				self.open_save_layout_modal = true
			end

			imgui.EndMenu() -- Load
		end

		if imgui.MenuItem('Reinit Editor') then
			dn.editor.init()
		end

		imgui.EndMenu() -- Layout
	end
end

function MainMenu:Asset()
	if imgui.BeginMenu('Asset') then
		self:AssetCreate()
		self:AssetEdit()
		self:AssetReload()
		imgui.EndMenu() -- Asset
	end
end

function MainMenu:AssetCreate()
	if imgui.BeginMenu('Create') then
		if imgui.MenuItem('Animation') then
			local editor = dn.find_entity_editor('AnimationEditor')
			editor:create('New Animation')
			editor:edit('New Animation')
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		if imgui.MenuItem('Background') then
			local editor = dn.find_entity_editor('BackgroundEditor')
			editor:create()
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		if imgui.MenuItem('Character') then
			local editor = dn.find_entity_editor('CharacterEditor')
			editor:setup_create_character()
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		if imgui.MenuItem('Texture Atlas') then
			local editor = dn.find_entity_editor('TextureAtlasEditor')
			editor:create()
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		imgui.EndMenu() -- Create
	end
end

function MainMenu:AssetEdit()
	-- Edit assets
	if imgui.BeginMenu('Edit') then
		-- Animation
		if imgui.BeginMenu('Animation') then
			local animations = {}
			for name, _ in pairs(dn.animation.data) do
				table.insert(animations, name)
			end
			table.sort(animations)

			for index, name in pairs(animations) do
				if imgui.MenuItem(name) then
					local editor = dn.find_entity_editor('AnimationEditor')
					editor:edit(name)
					editor.popups:open_popup(editor.popup_kind.edit)
				end
			end

			imgui.EndMenu() -- Animation
		end

		-- Background
		if imgui.BeginMenu('Background') then
			local backgrounds = {}
			for name, _ in pairs(dn.background.data) do
				table.insert(backgrounds, name)
			end
			table.sort(backgrounds)

			for index, name in pairs(backgrounds) do
				if imgui.MenuItem(name) then
					local editor = dn.find_entity_editor('BackgroundEditor')
					editor:edit(name)
					editor.popups:open_popup(editor.popup_kind.edit)
				end
			end

			imgui.EndMenu() -- Background
		end

		-- Character
		if imgui.BeginMenu('Character') then
			local characters = {}
			for name, _ in pairs(dn.dialogue.characters) do
				table.insert(characters, name)
			end
			table.sort(characters)

			for index, character in pairs(characters) do
				if imgui.MenuItem(character) then
					local editor = dn.find_entity_editor('CharacterEditor')
					editor:edit_character(character)
				end
			end

			imgui.EndMenu() -- Character
		end

		if imgui.BeginMenu('Texture Atlas') then
			local atlases = {}
			for name, _ in pairs(dn.texture.data.atlases) do
				table.insert(atlases, name)
			end
			table.sort(atlases)

			for index, name in pairs(atlases) do
				if imgui.MenuItem(name) then
					local editor = dn.find_entity_editor('TextureAtlasEditor')
					editor:edit(name)
					editor.popups:open_popup(editor.popup_kind.edit)
				end
			end

			imgui.EndMenu() -- Texture Atlas
		end

		imgui.EndMenu() -- Edit
	end
end

function MainMenu:AssetReload()
	if imgui.BeginMenu('Reload') then
		if imgui.MenuItem('Animations') then
			dn.animation.init()
		end

		if imgui.MenuItem('Characters') then
			dn.load_characters()
		end

		if imgui.MenuItem('Actions') then
			dn.action.init()
		end
		imgui.EndMenu()
	end
end

function MainMenu:Scene()
	if imgui.BeginMenu('Scene') then
		local collider_editor = dn.find_entity_editor('ColliderEditor').collider_editor
		local scene_editor = dn.find_entity_editor('SceneEditor')

		if imgui.BeginMenu('Open') then
			local scenes = dn.filesystem.collect_named_directory('scenes')
			for index, name in pairs(scenes) do
				scenes[index] = string.gsub(name, '.lua', '')
			end
			table.sort(scenes)

			for index, scene in pairs(scenes) do
				if imgui.MenuItem(scene) then
					scene_editor:load(scene)
				end
			end
			imgui.EndMenu()
		end

		if imgui.MenuItem('Save', 'Ctrl+S') then
			scene_editor:save()
		end

		if imgui.MenuItem('Play', 'F5', dn.tick) then
			scene_editor:toggle_play_mode()
		end

		if imgui.MenuItem('Step Mode', 'F1', dn.step) then
			dn.step = not dn.step
		end

		if imgui.BeginMenu('Persistent Entities') then
			if imgui.MenuItem('Save') then
				dn.persistent.write()
			end
			if imgui.MenuItem('Reload') then
				dn.persistent.init()
			end
			imgui.EndMenu()
		end

		imgui.EndMenu()
	end
end
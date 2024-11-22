local state = {
	idle = 'idle',
	choosing_dialogue = 'choosing_dialogue',
	choosing_state = 'choosing_state',
	choosing_scene = 'choosing_scene'
}

local MainMenu = tdengine.editor.define('MainMenu')

function MainMenu:init(params)
	self.open_save_layout_modal = false
	self.open_new_dialogue_modal = false
	self.open_save_dialogue_modal = false
	self.state = state.idle

	self.size = tdengine.vec2(6, 6)

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

	self.input = ContextualInput:new(tdengine.enums.InputContext.Editor, tdengine.enums.CoordinateSystem.Game)
end

function MainMenu:update(dt)
	imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_FramePadding, self.size:unpack())
	if imgui.BeginMainMenuBar() then
		self:Window()
		self:Asset()
		self:Dialogue()
		self:Scene()

		tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_main_menu)
		
		imgui.EndMainMenuBar()
	end
	imgui.PopStyleVar()

	self:update_state()
	self:show_modals()
end

function MainMenu:update_state()
	if self.state == state.choosing_dialogue then
		if imgui.IsAnyFileSelected() then
			local dialogue_editor = tdengine.find_entity_editor('DialogueEditor')
			dialogue_editor:load(imgui.GetSelectedFile())
			self.state = state.idle
		end
	elseif self.state == state.choosing_state then
		if imgui.IsAnyFileSelected() then
			tdengine.load_save_by_path(imgui.GetSelectedFile())
			self.state = state.idle
		end
	elseif self.state == state.choosing_scene then
		if imgui.IsAnyFileSelected() then
			local path = imgui.GetSelectedFile()
			local scene = tdengine.extract_filename(path)
			scene = tdengine.strip_extension(scene)

			local scene_editor = tdengine.find_entity_editor('SceneEditor')
			scene_editor:load(scene)

			self.state = state.idle
		end

		if self.input:pressed(glfw.keys.ESCAPE) then
			imgui.CloseFileBrowser()
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
			tdengine.ffi.save_editor_layout(self.saved_layout_name)
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
			local dialogue_editor = tdengine.find_entity_editor('DialogueEditor')
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

	local size = tdengine.vec2(250, 100)
	tdengine.editor.center_next_window(size)
	imgui.SetNextWindowSize(size.x, size.y)
	if imgui.BeginPopupModal('New Dialogue') then
		imgui.Text('Name')
		imgui.SameLine()
		imgui.InputText(self.ids.new_dialogue, self, 'saved_dialogue_name')

		imgui.Dummy(5, 5)

		if imgui.Button('Save') then
			local dialogue_editor = tdengine.find_entity_editor('DialogueEditor')
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
		local dialogue_editor = tdengine.find_entity_editor('DialogueEditor')
		if imgui.MenuItem('Open', 'Ctrl+O') then
			local directory = tdengine.ffi.resolve_named_path('dialogues'):to_interned()
			imgui.SetFileBrowserWorkDir(directory)
			imgui.OpenFileBrowser()
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
				tdengine.dialogue.update_all_metrics()
			end
			imgui.EndMenu()
		end

		if imgui.BeginMenu('State') then
			if imgui.MenuItem('Reset') then
				tdengine.state.load_file('default')
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
				local layout_dir = tdengine.ffi.resolve_named_path('layouts'):to_interned()
				local layouts = tdengine.scandir(layout_dir)
				for i, layout in pairs(layouts) do
					if imgui.MenuItem(tdengine.strip_extension(layout)) then
						local file_name = tdengine.strip_extension(layout)
						tdengine.ffi.use_editor_layout(file_name)
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
			tdengine.editor.init()
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
			local editor = tdengine.find_entity_editor('AnimationEditor')
			editor:create('New Animation')
			editor:edit('New Animation')
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		if imgui.MenuItem('Background') then
			local editor = tdengine.find_entity_editor('BackgroundEditor')
			editor:create()
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		if imgui.MenuItem('Character') then
			local editor = tdengine.find_entity_editor('CharacterEditor')
			editor:setup_create_character()
			editor.popups:open_popup(editor.popup_kind.edit)
		end

		if imgui.MenuItem('Texture Atlas') then
			local editor = tdengine.find_entity_editor('TextureAtlasEditor')
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
			for name, _ in pairs(tdengine.animation.data) do
				table.insert(animations, name)
			end
			table.sort(animations)

			for index, name in pairs(animations) do
				if imgui.MenuItem(name) then
					local editor = tdengine.find_entity_editor('AnimationEditor')
					editor:edit(name)
					editor.popups:open_popup(editor.popup_kind.edit)
				end
			end

			imgui.EndMenu() -- Animation
		end

		-- Background
		if imgui.BeginMenu('Background') then
			local backgrounds = {}
			for name, _ in pairs(tdengine.background.data) do
				table.insert(backgrounds, name)
			end
			table.sort(backgrounds)

			for index, name in pairs(backgrounds) do
				if imgui.MenuItem(name) then
					local editor = tdengine.find_entity_editor('BackgroundEditor')
					editor:edit(name)
					editor.popups:open_popup(editor.popup_kind.edit)
				end
			end

			imgui.EndMenu() -- Background
		end

		-- Character
		if imgui.BeginMenu('Character') then
			local characters = {}
			for name, _ in pairs(tdengine.dialogue.characters) do
				table.insert(characters, name)
			end
			table.sort(characters)

			for index, character in pairs(characters) do
				if imgui.MenuItem(character) then
					local editor = tdengine.find_entity_editor('CharacterEditor')
					editor:edit_character(character)
				end
			end

			imgui.EndMenu() -- Character
		end

		if imgui.BeginMenu('Texture Atlas') then
			local atlases = {}
			for name, _ in pairs(tdengine.texture.data.atlases) do
				table.insert(atlases, name)
			end
			table.sort(atlases)

			for index, name in pairs(atlases) do
				if imgui.MenuItem(name) then
					local editor = tdengine.find_entity_editor('TextureAtlasEditor')
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
			tdengine.animation.load()
		end

		if imgui.MenuItem('Characters') then
			tdengine.load_characters()
		end

		if imgui.MenuItem('Actions') then
			tdengine.action.init()
		end
		imgui.EndMenu()
	end
end

function MainMenu:Scene()
	if imgui.BeginMenu('Scene') then
		local collider_editor = tdengine.find_entity_editor('ColliderEditor').collider_editor
		local scene_editor = tdengine.find_entity_editor('SceneEditor')

		if imgui.BeginMenu('Open') then
			local scene_dir = tdengine.ffi.resolve_named_path('scenes'):to_interned()
			local scenes = tdengine.scandir(scene_dir)
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

		if imgui.MenuItem('Play', 'F5', tdengine.tick) then
			scene_editor:toggle_play_mode()
		end

		if imgui.MenuItem('Step Mode', 'F1', tdengine.step) then
			tdengine.step = not tdengine.step
		end

		if imgui.BeginMenu('Persistent Entities') then
			if imgui.MenuItem('Save') then
				tdengine.persistent.write()
			end
			if imgui.MenuItem('Reload') then
				tdengine.persistent.init()
			end
			imgui.EndMenu()
		end

		imgui.EndMenu()
	end
end
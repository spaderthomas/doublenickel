local EngineStats = tdengine.editor.define('EngineStats')

function EngineStats:init(params)
	self.ded = {
		nodes = {},
		layout_data = {},
		loaded = '',
		selected = nil,
		connecting = nil,
		disconnecting = nil,
		deleting = nil,
		scrolling = tdengine.vec2(0, 0),
		scroll_per_second = 100,
		window_position = tdengine.vec2(0, 0),
		input_id = '##ded_editor',
		text_who_id = '##ded:detail:set_entity',
		set_var_id = '##ded:detail:set_var',
		set_val_id = '##ded:detail:set_val',
		internal_id_id = '##ded:detail:set_internal_id',
		return_to_id = '##ded:detail:set_return_to',
		branch_on_id = '##ded:detail:set_branch_var',
		next_dialogue_id = '##ded:detail:next_dialogue',
		branch_val_id = '##ded:detail:set_branch_val',
		empty_name_id = '##ded:detail:set_empty_name',
		selected_editor = nil,
		selected_effect = 1
	}

	self.selected = nil
	self.entity_editor = nil
	self.frame = 0

	self.do_layout_save = false
	self.ids = {
		save_layout = '##menu:save_layout'
	}

	self.display_cursor = false

	self.fps_timer = Timer:new(1)
	self.fps = 0
	self.spf = 0

	self.volume = 1.0

	self.screen_fade = 2.0
	self.screen_fade_elapsed = 0.0
	self.screen_fade_enabled = true

	self.gui_animations = imgui.extensions.TableEditor(tdengine.gui.animation)
	self.gui_scroll = imgui.extensions.TableEditor(tdengine.gui.scroll)
	self.gui_drag = imgui.extensions.TableEditor(tdengine.gui.drag)
	self.gui_menu = imgui.extensions.TableEditor(tdengine.gui.menu)
	self.save_data = imgui.extensions.TableEditor(tdengine.scene.save_data)
	self.subsystems = {}

	self.metrics = { 
		target_fps = 0,
	}

	self.audio = {}
	self.particle_systems = {}

	self.cameras = {
		Editor = tdengine.vec2(),
		Game = tdengine.vec2(),
	}

	self.action_data = {}

	self.imgui_ignore = {
		entity_editor = true,
		imgui_ignore = true
	}

	-- Cache this, because it makes something of a difference to do this every frame
	self.named_paths = {}
	for name, path in tdengine.paths.iterate() do
		table.insert(self.named_paths, tdengine.paths.NamedPath:new(name, path:gsub('%%', '%%%%')))
	end
	table.sort(self.named_paths, function(a, b) return a.path < b.path end)
end

function EngineStats:update(dt)
	self:playground()
	self:calculate_framerate()
	self:engine_viewer()
end

function EngineStats:playground()
	if self.window then
		self.window:update()
		local x = tdengine.ffi.get_content_area()
		tdengine.ffi.set_window_size(self.window:get_value(), x.y)

		if self.window:is_done() then
			if self.window.once then
				self.window = nil
			else
				self.window:reverse()
				self.window:reset()
				self.window.once = true
			end
		end
	end
end

function EngineStats:engine_viewer()
	tdengine.editor.begin_window('Engine')

	imgui.PushFont('editor-bold-16')
	imgui.Text('User')
	imgui.PopFont()

	for name, subsystem in pairs(tdengine.subsystem.subsystems) do
		if not self.subsystems[name] then
			self.subsystems[name] = imgui.extensions.TableEditor(subsystem)
		end

		if imgui.TreeNode(name) then
			self.subsystems[name]:draw()
			imgui.TreePop()
		end
	end

	tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_engine_viewer)

	imgui.PushFont('editor-bold-16')
	imgui.Text('Engine')
	imgui.PopFont()

	if imgui.TreeNode('Time') then
		
	
		imgui.extensions.Table(self.metrics)
		imgui.TreePop()
	end

	if imgui.TreeNode('Window') then
		local main_view = tdengine.editor.find('GameViewManager'):find_main_view()
		imgui.extensions.TableField('Main View', main_view.name)
		imgui.extensions.Vec2('Content Area', tdengine.window.get_content_area())
		imgui.extensions.Vec2('Native Resolution', tdengine.window.get_native_resolution())
		imgui.extensions.Vec2('Game View', tdengine.window.get_game_area_size())
		imgui.TreePop()
	end

	if imgui.TreeNode('Input') then
		imgui.extensions.VariableName('Mode')
		imgui.SameLine()
		local device = tdengine.input.get_input_device()
		if device == tdengine.input.device_kinds.mkb then
			imgui.Text('Mouse + Keyboard')
		elseif device == tdengine.input.device_kinds.controller then
			imgui.Text('Kontroller')
		end

		imgui.extensions.VariableName('Context')
		imgui.SameLine()
		imgui.Text(tdengine.input.active_context():to_string())


		imgui.Checkbox('Display Cursor', self, 'display_cursor')

		if self.display_cursor then
			local world = tdengine.vec2(tdengine.cursor(tdengine.coordinate.world))
			tdengine.ffi.set_layer(100)
			tdengine.ffi.set_world_space(true)()
			tdengine.draw_circle_l(world, 5, tdengine.colors.red)
		end

		if imgui.TreeNode('Camera') then
			local camera = tdengine.find_entity_editor('EditorCamera')
			imgui.extensions.Vec2('Editor', camera.offset:truncate(3))

			local camera = tdengine.find_entity('Camera')
			if camera then
				imgui.extensions.Vec2('Game', camera.offset:truncate(3))
			end
			imgui.TreePop()
		end


		if imgui.TreeNode('Mouse') then
			local output = tdengine.vec2(tdengine.window.get_content_area())
			local screen = tdengine.vec2(tdengine.cursor(tdengine.coordinate.screen)):truncate(3)
			local screen_px = tdengine.vec2()
			screen_px.x = math.floor(screen.x * output.x)
			screen_px.y = math.floor(screen.y * output.y)
			imgui.extensions.Vec2('Screen        (Pixel)', screen_px)

			imgui.extensions.Vec2('Screen      (Percent)', screen)

			local window = tdengine.vec2(tdengine.cursor(tdengine.coordinate.window)):truncate(3)
			imgui.extensions.Vec2('Game Window (Percent)', window)

			local game = tdengine.vec2(tdengine.cursor(tdengine.coordinate.game)):truncate(3)
			game.x = math.floor(game.x)
			game.y = math.floor(game.y)
			imgui.extensions.Vec2('Game Window   (Pixel)', game)

			local world = tdengine.vec2(tdengine.cursor(tdengine.coordinate.world)):truncate(3)
			world.x = math.floor(world.x)
			world.y = math.floor(world.y)
			imgui.extensions.Vec2('World         (Pixel)', world)

			imgui.TreePop()
		end

		imgui.TreePop()
	end

	if imgui.TreeNode('Actions') then
		imgui.Text(string.format('Active Action Set: %s', tdengine.action.get_active_set()))
		imgui.Text(string.format('Action Set Cooldown: %d', tdengine.ffi.get_action_set_cooldown()))

		local color_active = tdengine.color32(0, 200, 0, 255)
		local color_inactive = tdengine.color32(200, 0, 0, 255)

		local action_data = tdengine.action.data or {}
		local actions = action_data and action_data.actions or {}
		if imgui.TreeNode('Action State') then
			for _, action in pairs(action_data) do
				local active = tdengine.ffi.was_digital_pressed(action)
				local color = active and color_active or color_inactive
				imgui.PushStyleColor(ffi.C.ImGuiCol_Text, color)
				imgui.Text(action)
				imgui.PopStyleColor()

				if not self.action_data[action] then self.action_data[action] = 0 end
				if active then self.action_data[action] = tdengine.frame end

				imgui.SameLine()
				imgui.Text(string.format('%d', self.action_data[action]))
			end
			imgui.TreePop()
		end

		if imgui.TreeNode('Action File') then
			imgui.extensions.Table(actions)
			imgui.TreePop()
		end

		imgui.TreePop()
	end

	if imgui.TreeNode('Audio') then
		self.audio = {
			['Master Cutoff'] = tdengine.audio.get_master_cutoff(),
			['Master Volume'] = tdengine.audio.get_master_volume(),
			['Master Volume Mod'] = tdengine.audio.get_master_volume_mod()
		}
		imgui.extensions.Table(self.audio)

		if imgui.Button('Enable Audio') then
			tdengine.audio.enable()
		end

		imgui.SameLine()
		if imgui.Button('Disable Audio') then
			tdengine.audio.disable()
		end

		imgui.SameLine()
		if imgui.Button('Stop All') then
			tdengine.audio.stop_all()
		end

		imgui.extensions.Table(tdengine.audio.internal)
		imgui.TreePop()
	end

	if imgui.TreeNode('Backgrounds') then
		imgui.extensions.Table(tdengine.background.data)
		imgui.TreePop()
	end

	if imgui.TreeNode('Callbacks') then
		local keys = {}
		for name, callback in pairs(tdengine.callback.data) do
			table.insert(keys, name)
		end
		table.sort(keys)
		for index, name in pairs(keys) do
			imgui.Text(name)
		end
		imgui.TreePop()
	end

	if imgui.TreeNode('Dialogue Metrics') then
		imgui.extensions.Table(tdengine.dialogue.metrics)
		imgui.TreePop()
	end

	if imgui.TreeNode('Enums') then
		imgui.extensions.Table(tdengine.enum_data)
		imgui.TreePop()
	end

	if imgui.TreeNode('Fonts') then
		imgui.extensions.Table(tdengine.fonts.data)
		imgui.TreePop()
	end

	if imgui.TreeNode('GPU') then
		imgui.extensions.Table(tdengine.gpus)
		imgui.TreePop()
	end


	if imgui.TreeNode('GUI') then
		if imgui.TreeNode('Animations') then
			self.gui_animations:draw()
			imgui.TreePop()
		end

		if imgui.TreeNode('Scroll') then
			self.gui_scroll:draw()
			imgui.TreePop()
		end

		if imgui.TreeNode('Menu Data') then
			self.gui_menu:draw()
			imgui.TreePop()
		end

		if imgui.TreeNode('Menu Stack') then
			imgui.extensions.Table(tdengine.gui.menu_stack)
			imgui.TreePop()
		end

		if imgui.TreeNode('Drag') then
			self.gui_drag:draw()
			imgui.TreePop()
		end

		imgui.TreePop()
	end

	if imgui.TreeNode('Images') then
		imgui.extensions.Table(tdengine.texture.data)
		imgui.TreePop()
	end

	if imgui.TreeNode('Paths') then
		if imgui.Button('Sort By Name') then
			table.sort(self.named_paths, function(a, b) return a.name < b.name end)
		end
		imgui.SameLine()
		if imgui.Button('Sort By Path') then
			table.sort(self.named_paths, function(a, b) return a.path < b.path end)
		end
		for _, named_path in pairs(self.named_paths) do
			local label = string.format('%s##editor:paths', named_path.name)

			imgui.PushStyleColor(ffi.C.ImGuiCol_Text, tdengine.colors.zomp:to_u32())
			local popped = false

			if imgui.TreeNode(label) then
				imgui.PopStyleColor()
				popped = true

				imgui.Text(named_path.path)
				imgui.TreePop()
			end
			if not popped then imgui.PopStyleColor() end
			--imgui.extensions.TableField(named_path.name, named_path.path)
		end
		imgui.TreePop()
	end

	if imgui.TreeNode('Particles') then
		if imgui.Button('Stop All') then
			tdengine.ffi.stop_all_particles()
			tdengine.ffi.lf_destroy_all()
		end

		local particle_systems = tdengine.find_entities('ParticleSystem')
		for id, particle_system in pairs(particle_systems) do
			if not self.particle_systems[particle_system.uuid] then
				self.particle_systems[particle_system.uuid] = {
					timer = Timer:new(.25),
					stats = particle_system:check_stats()
				}
			end

			local metadata = self.particle_systems[particle_system.uuid]
			metadata.timer:update()
			if metadata.timer:is_done() then
				metadata.stats = particle_system:check_stats()
				metadata.timer:reset()
			end

			local label = particle_system.uuid
			if #particle_system.tag > 0 then label = particle_system.tag end
			if imgui.TreeNode(label) then
				imgui.extensions.Table(metadata.stats)
				imgui.TreePop()
			end
		end
		imgui.TreePop()
	end



	if imgui.TreeNode('Saves') then
		self.save_data:draw()
		imgui.TreePop()
	end

	if imgui.TreeNode('Subsystems') then
		imgui.extensions.Table(tdengine.subsystem.subsystems)
		imgui.TreePop()
	end

	if imgui.TreeNode('Types') then
		local options = TableOptions:new()
		options.ignore_functions = false
		imgui.extensions.Table(tdengine.types, options)
		imgui.TreePop()
	end

	if imgui.TreeNode('Utility') then
		if imgui.Button('Show Text Input') then
			tdengine.steam.show_text_input('Title', 'Text')
		end

		if imgui.Button('Window') then
		end

		imgui.TreePop()
	end

	tdengine.editor.end_window()
end

function EngineStats:calculate_framerate()
	if self.fps_timer:update(tdengine.dt) then
		self.fps_timer:reset()

		local metrics = tdengine.time_metric.query_all()
		table.merge(metrics, self.metrics)

		self.metrics.target_fps = tdengine.ffi.get_target_fps()
		self.metrics.actual_fps = math.floor(1000.0 / self.metrics.frame.average)
	end
end

function crash()
	local x = nil
	print(x.y)
end
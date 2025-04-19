local EngineStats = doublenickel.editor.define('EngineStats')

function EngineStats:init(params)
  self.ded = {
    nodes = {},
    layout_data = {},
    loaded = '',
    selected = nil,
    connecting = nil,
    disconnecting = nil,
    deleting = nil,
    scrolling = doublenickel.vec2(0, 0),
    scroll_per_second = 100,
    window_position = doublenickel.vec2(0, 0),
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

  self.gui_animations = imgui.extensions.TableEditor(doublenickel.gui.animation)
  self.gui_scroll = imgui.extensions.TableEditor(doublenickel.gui.scroll)
  self.gui_drag = imgui.extensions.TableEditor(doublenickel.gui.drag)
  self.gui_menu = imgui.extensions.TableEditor(doublenickel.gui.menu)
  self.save_data = imgui.extensions.TableEditor(doublenickel.scene.save_data)
  self.subsystems = {}

  self.metrics = { 
    target_fps = 0,
  }

  self.audio = {}
  self.particle_systems = {}

  self.cameras = {
    Editor = doublenickel.vec2(),
    Game = doublenickel.vec2(),
  }

  self.action_data = {}

  self.imgui_ignore = {
    entity_editor = true,
    imgui_ignore = true
  }
end

function EngineStats:update(dt)
  self:playground()
  self:calculate_framerate()
  self:engine_viewer()
end

function EngineStats:playground()
  if self.window then
    self.window:update()
    local x = dn.ffi.window_get_content_area()
    dn.unported.set_window_size(self.window:get_value(), x.y)

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
  doublenickel.editor.begin_window('Engine')

  doublenickel.editor.push_font(EditorFonts.Bold)
  imgui.Text('User')
  imgui.PopFont()

  for name, subsystem in pairs(doublenickel.subsystem.subsystems) do
    if not self.subsystems[name] then
      self.subsystems[name] = imgui.extensions.TableEditor(subsystem)
    end

    if imgui.TreeNode(name) then
      self.subsystems[name]:draw()
      imgui.TreePop()
    end
  end

  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_engine_viewer)

  doublenickel.editor.push_font(EditorFonts.Bold)
  imgui.Text('Engine')
  imgui.PopFont()

  if imgui.TreeNode('Time') then
    
  
    imgui.extensions.Table(self.metrics)
    imgui.TreePop()
  end

  if imgui.TreeNode('Window') then
    local main_view = doublenickel.editor.find('GameViewManager'):find_main_view()
    imgui.extensions.TableField('Main View', main_view.name)
    imgui.extensions.Vec2('Content Area', dn.ffi.window_get_content_area(), '%d') 
    imgui.extensions.Vec2('Native Resolution', doublenickel.window.dn_window_get_native_resolution(), '%d')
    imgui.extensions.Vec2('Game View', doublenickel.window.get_game_area_size(), '%d')
    imgui.TreePop()
  end

  if imgui.TreeNode('Input') then
    imgui.extensions.VariableName('Mode')
    imgui.SameLine()
    local device = doublenickel.input.get_input_device()
    if device == InputDevice.MouseAndKeyboard then
      imgui.Text('Mouse + Keyboard')
    elseif device == InputDevice.Controller then
      imgui.Text('Kontroller')
    end

    imgui.extensions.VariableName('Context')
    imgui.SameLine()
    imgui.Text(doublenickel.input.active_context():to_string())


    imgui.Checkbox('Display Cursor', self, 'display_cursor')

    if self.display_cursor then
      local world = doublenickel.vec2(dn.ffi.input_mouse(ffi.C.DN_COORD_UNIT_WORLD))
      doublenickel.draw_circle_l(world, 5, doublenickel.colors.red)
    end

    if imgui.TreeNode('Camera') then
      local camera = doublenickel.find_entity_editor('EditorCamera')
      imgui.extensions.Vec2('Editor', camera.offset:truncate(3))

      local camera = doublenickel.find_entity('Camera')
      if camera then
        imgui.extensions.Vec2('Game', camera.offset:truncate(3))
      end
      imgui.TreePop()
    end


    if imgui.TreeNode('Mouse') then
      imgui.extensions.Vec2('Screen', dn.ffi.input_mouse(ffi.C.DN_COORD_UNIT_SCREEN), '%.3f')
      imgui.extensions.Vec2('Window', dn.ffi.input_mouse(ffi.C.DN_COORD_UNIT_WINDOW), '%.3f')
      imgui.extensions.Vec2('Game  ', dn.ffi.input_mouse(ffi.C.DN_COORD_UNIT_GAME), '%d')
      imgui.extensions.Vec2('World ', dn.ffi.input_mouse(ffi.C.DN_COORD_UNIT_WORLD), '%d')

      imgui.TreePop()
    end

    imgui.TreePop()
  end

  if imgui.TreeNode('Actions') then
    imgui.Text(string.format('Active Action Set: %s', doublenickel.action.get_active_set()))
    imgui.Text(string.format('Action Set Cooldown: %d', dn.unported.get_action_set_cooldown()))

    local color_active = doublenickel.color32(0, 200, 0, 255)
    local color_inactive = doublenickel.color32(200, 0, 0, 255)

    local action_data = doublenickel.action.data or {}
    local actions = action_data and action_data.actions or {}
    if imgui.TreeNode('Action State') then
      for _, action in pairs(action_data) do
        local active = dn.unported.was_digital_pressed(action)
        local color = active and color_active or color_inactive
        imgui.PushStyleColor(ffi.C.ImGuiCol_Text, color)
        imgui.Text(action)
        imgui.PopStyleColor()

        if not self.action_data[action] then self.action_data[action] = 0 end
        if active then self.action_data[action] = doublenickel.frame end

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
      ['Master Cutoff'] = doublenickel.audio.get_master_cutoff(),
      ['Master Volume'] = doublenickel.audio.get_master_volume(),
      ['Master Volume Mod'] = doublenickel.audio.get_master_volume_mod()
    }
    imgui.extensions.Table(self.audio)

    if imgui.Button('Enable Audio') then
      doublenickel.audio.enable()
    end

    imgui.SameLine()
    if imgui.Button('Disable Audio') then
      doublenickel.audio.disable()
    end

    imgui.SameLine()
    if imgui.Button('Stop All') then
      doublenickel.audio.stop_all()
    end

    imgui.extensions.Table(doublenickel.audio.internal)
    imgui.TreePop()
  end

  if imgui.TreeNode('Backgrounds') then
    imgui.extensions.Table(doublenickel.background.data)
    imgui.TreePop()
  end

  if imgui.TreeNode('Callbacks') then
    local keys = {}
    for name, callback in pairs(doublenickel.callback.data) do
      table.insert(keys, name)
    end
    table.sort(keys)
    for index, name in pairs(keys) do
      imgui.Text(name)
    end
    imgui.TreePop()
  end

  if imgui.TreeNode('Dialogue Metrics') then
    imgui.extensions.Table(doublenickel.dialogue.metrics)
    imgui.TreePop()
  end

  if imgui.TreeNode('Enums') then
    imgui.extensions.Table(doublenickel.enum_data)
    imgui.TreePop()
  end

  if imgui.TreeNode('Fonts') then
    imgui.extensions.Table(doublenickel.fonts.data)
    imgui.TreePop()
  end

  if imgui.TreeNode('GPU') then
    imgui.extensions.Table(doublenickel.gpu)
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
      imgui.extensions.Table(doublenickel.gui.menu_stack)
      imgui.TreePop()
    end

    if imgui.TreeNode('Drag') then
      self.gui_drag:draw()
      imgui.TreePop()
    end

    imgui.TreePop()
  end

  if imgui.TreeNode('Images') then
    imgui.extensions.Table(doublenickel.texture.data)
    imgui.TreePop()
  end

  if imgui.TreeNode('Particles') then
    if imgui.Button('Stop All') then
      dn.unported.stop_all_particles()
      dn.unported.lf_destroy_all()
    end

    local particle_systems = doublenickel.find_entities('ParticleSystem')
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
    imgui.extensions.Table(doublenickel.subsystem.subsystems)
    imgui.TreePop()
  end

  if imgui.TreeNode('Types') then
    local options = TableOptions:new()
    options.ignore_functions = false
    imgui.extensions.Table(doublenickel.types, options)
    imgui.TreePop()
  end

  if imgui.TreeNode('Utility') then
    if imgui.Button('Show Text Input') then
      dn.ffi.steam_open_text_input()
    end

    if imgui.Button('Window') then
    end

    imgui.TreePop()
  end

  doublenickel.editor.end_window()
end

function EngineStats:calculate_framerate()
  if self.fps_timer:update(doublenickel.dt) then
    self.fps_timer:reset()

    local metrics = doublenickel.time_metric.query_all()
    table.merge(metrics, self.metrics)

    self.metrics.target_fps = dn.ffi.engine_get_target_fps()
    self.metrics.actual_fps = math.floor(1000.0 / self.metrics.frame.average)
  end
end

function crash()
  local x = nil
  print(x.y)
end
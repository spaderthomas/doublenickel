local Hotkey = doublenickel.editor.define('Hotkey')
function Hotkey:init()
	self.input = ContextualInput:new(doublenickel.enums.InputContext.Editor, doublenickel.enums.CoordinateSystem.Game)

  self.game_hotkeys = {
    control = {
      [glfw.keys.S] = function() self:save_scene() end,
      [glfw.keys.O] = function() self:open_scene() end,
      [glfw.keys.R] = function() self:toggle_play_mode() end,
    },
    single = {
      [glfw.keys.F5] = function() self:toggle_play_mode() end,
    }
  }

  self.editor_hotkeys = {
    control = {
      [glfw.keys.S] = function() self:save_dialogue() end,
      [glfw.keys.N] = function() self:new_dialogue() end,
      [glfw.keys.O] = function() self:open_dialogue() end,
    },
    single = {
    }
  }
end

function Hotkey:update()
  if doublenickel.is_packaged_build then return end

  self:check_game_hotkeys()
  self:check_editor_hotkeys()
end

function Hotkey:check_game_hotkeys()
  self.input.context = doublenickel.enums.InputContext.Game
  
  self:check_control_hotkeys(self.game_hotkeys.control)
  self:check_single_hotkeys(self.game_hotkeys.single)
end

function Hotkey:check_editor_hotkeys()
  self.input.context = doublenickel.enums.InputContext.Editor

  self:check_control_hotkeys(self.editor_hotkeys.control)
  self:check_single_hotkeys(self.editor_hotkeys.single)
end

function Hotkey:check_control_hotkeys(hotkeys, channel)
  for key, fn in pairs(hotkeys) do
    if self.input:chord_pressed(glfw.keys.CONTROL, key) then
      fn()
    end
  end
end

function Hotkey:check_single_hotkeys(hotkeys, channel)
  for key, fn in pairs(hotkeys) do
    if self.input:pressed(key) then
      fn()
    end
  end
end

function Hotkey:save_dialogue()
  local dialogue_editor = doublenickel.editor.find('DialogueEditor')
  dialogue_editor:save(dialogue_editor.loaded)
end

function Hotkey:new_dialogue()
  doublenickel.editor.find('MainMenu').open_new_dialogue_modal = true
end

function Hotkey:open_dialogue()
  local directory = dn.paths_resolve('dialogues')
  imgui.SetFileBrowserPwd(directory)
  imgui.file_browser_open()

  -- @hack: I wanted to move the hoykey code out of the main menu, but I forgot that it was
  -- doing more than just drawing a menu. Not a hard fix, just not right now.
  local main_menu = doublenickel.find_entity_editor('MainMenu')
  main_menu.state = 'choosing_dialogue'
end

-----------------
-- GAME WINDOW --
-----------------
function Hotkey:save_scene()
  doublenickel.find_entity_editor('SceneEditor'):save()
end

function Hotkey:toggle_play_mode()
  doublenickel.find_entity_editor('SceneEditor'):toggle_play_mode()
end

function Hotkey:open_scene()
  local scenes = dn.paths_resolve('scenes')
  imgui.file_browser_set_work_dir(scenes)
  imgui.file_browser_open()

  -- @hack: I wanted to move the hoykey code out of the main menu, but I forgot that it was
  -- doing more than just drawing a menu. Not a hard fix, just not right now.
  local main_menu = doublenickel.editor.find('MainMenu')
  main_menu.state = 'choosing_scene'
end

function Hotkey:reset_state()
  doublenickel.state.load_file('default')
end

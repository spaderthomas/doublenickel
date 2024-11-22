local Hotkey = tdengine.editor.define('Hotkey')
function Hotkey:init()
	self.input = ContextualInput:new(tdengine.enums.InputContext.Editor, tdengine.enums.CoordinateSystem.Game)

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
  if tdengine.is_packaged_build then return end

  self:check_game_hotkeys()
  self:check_editor_hotkeys()
end

function Hotkey:check_game_hotkeys()
  self.input.context = tdengine.enums.InputContext.Game
  
  self:check_control_hotkeys(self.game_hotkeys.control)
  self:check_single_hotkeys(self.game_hotkeys.single)
end

function Hotkey:check_editor_hotkeys()
  self.input.context = tdengine.enums.InputContext.Editor

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
  local dialogue_editor = tdengine.editor.find('DialogueEditor')
  dialogue_editor:save(dialogue_editor.loaded)
end

function Hotkey:new_dialogue()
  tdengine.editor.find('MainMenu').open_new_dialogue_modal = true
end

function Hotkey:open_dialogue()
  local directory = tdengine.ffi.resolve_named_path('dialogues'):to_interned()
  imgui.SetFileBrowserPwd(directory)
  imgui.OpenFileBrowser()

  -- @hack: I wanted to move the hoykey code out of the main menu, but I forgot that it was
  -- doing more than just drawing a menu. Not a hard fix, just not right now.
  local main_menu = tdengine.find_entity_editor('MainMenu')
  main_menu.state = 'choosing_dialogue'
end

-----------------
-- GAME WINDOW --
-----------------
function Hotkey:save_scene()
  tdengine.find_entity_editor('SceneEditor'):save()
end

function Hotkey:toggle_play_mode()
  tdengine.find_entity_editor('SceneEditor'):toggle_play_mode()
end

function Hotkey:open_scene()
  local scenes = tdengine.ffi.resolve_named_path('scenes'):to_interned()
  imgui.SetFileBrowserWorkDir(scenes)
  imgui.OpenFileBrowser()

  -- @hack: I wanted to move the hoykey code out of the main menu, but I forgot that it was
  -- doing more than just drawing a menu. Not a hard fix, just not right now.
  local main_menu = tdengine.editor.find('MainMenu')
  main_menu.state = 'choosing_scene'
end

function Hotkey:reset_state()
  tdengine.state.load_file('default')
end

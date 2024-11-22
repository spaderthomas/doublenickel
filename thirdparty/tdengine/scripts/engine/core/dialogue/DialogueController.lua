DialogueController = tdengine.class.define('DialogueController')

dialogue_state = {
  err            = 'err',
  idle           = 'idle',
  beginning      = 'beginning',
  beginning_tool = 'beginning_tool',
  advancing      = 'advancing',
  processing     = 'processing',
  unload         = 'unload',
  done           = 'done',
}

function DialogueController:init(params)
  self.items = {}
  self.state = dialogue_state.idle
  self.tool = nil
end

function DialogueController:play_dialogue(dialogue)
  self:load_dialogue(dialogue)

  -- AUDIO
  tdengine.audio.play(self.data.metadata.sound_begin)

  -- INPUT
  tdengine.find_entity('GameState'):enter_state(tdengine.enums.GameMode.Dialogue) -- @refactor

  -- HISTORY
  local history = tdengine.dialogue.history
  history:clear()

  -- UI
  local layout = tdengine.find_entity('DialogueLayout') -- @refactor
  layout:show()

  -- CAMERA
  local camera = tdengine.find_entity('Camera') -- @refactor
  camera:interpolate_to_dialogue()
  camera:stop_after_interpolate()
end

function DialogueController:stop_dialogue()
  -- AUDIO
  tdengine.audio.play(self.data.metadata.sound_end)

  -- INPUT
  tdengine.find_entity('GameState'):enter_state(tdengine.enums.GameMode.Game) -- @refactor

  -- UI
  tdengine.find_entity('DialogueLayout'):hide() -- @refactor

  -- CAMERA
  local camera = tdengine.find_entity('Camera')
  camera:interpolate_to_player()

  self:mark_for_unload()
end

function DialogueController:load_dialogue(dialogue)
  if not dialogue then
    tdengine.log(string.format('controller got bad dialogue param, dialogue = %s', dialogue))
    self:update_state(dialogue_state.err)
    return
  end

  self.current_dialogue = dialogue

  -- Load the dialogue
  self.data = tdengine.dialogue.load(self.current_dialogue)
  if not self.data then
    self:update_state(dialogue_state.err)
    return
  end

  self.nodes = self.data.nodes

  -- Initialize ourself
  self.current = nil
  self.tool = nil
  self:update_state(dialogue_state.beginning)
end

function DialogueController:mark_for_unload()
  self.need_unload = true
  self.state = dialogue_state.unload
end

function DialogueController:unload_dialogue()
  self.tool = nil
  self.current_dialogue = nil
  self.data = nil
  self.nodes = nil
  self.current = nil
  self.state = dialogue_state.idle
end

function DialogueController:change_dialogue(dialogue)
  -- Load a new dialogue, but without totally reinitializing ourselves. This is used when switching between dialogues in
  -- one interaction (like for a Call)
  if not dialogue then
    tdengine.log(string.format('DialogueController::change_dialogue(): bad dialogue, name = %s', dialogue))
    self:update_state(dialogue_state.err)
    return
  end

  self.current_dialogue = dialogue

  self.data = tdengine.dialogue.load(self.current_dialogue)
  if not self.data then
    self:update_state(dialogue_state.err)
    return
  end

  self.nodes = self.data.nodes
end

function DialogueController:update()
  if self.need_unload then
    self.need_unload = false
    self:unload_dialogue()
    return
  end

  if self.state == dialogue_state.idle then
    return
  elseif self.state == dialogue_state.err then
    return
  elseif self.state == dialogue_state.beginning then
    self.current = find_entry_node(self.data.nodes)
    self:enter_current()
    self:try_advance_until_processing()
  elseif self.state == dialogue_state.beginning_tool then
    self.current = find_tool_node(self.data.nodes, self.tool)
    if not self.current then
      self:load_dialogue('misc-unusable-tool')
    else
      self:enter_current()
      self:try_advance_until_processing()
    end
  elseif self.state == dialogue_state.advancing then
    self:try_advance_until_processing()
  elseif self.state == dialogue_state.processing then
    self:process(tdengine.dt)
    self:try_advance_until_processing()
  elseif self.state == dialogue_state.done then
    self.state = dialogue_state.idle
  end
end

function DialogueController:process()
  local state = self.current:process(self.data.nodes)
  self:update_state(state)
end

function DialogueController:try_advance_until_processing()
  while self.state == dialogue_state.advancing do
    if not self.current then
      self:update_state(dialogue_state.err)
      return
    end

    self:advance_single()
    self:enter_current()
  end
end

function DialogueController:advance_single()
  local next_node = self.current:advance(self.data.nodes)
  if not next_node then tdengine.debug.open_debugger() end

  self.current = next_node
end

function DialogueController:enter_current()
  if not self.current then return end

  --tdengine.analytics.add_node(self.current.uuid) -- @refactor
  local state = self.current:enter(self.data.nodes)
  self:update_state(state)
end

function DialogueController:update_state(state)
  if state == nil then dbg() end
  if self.state == dialogue_state.unload then return end

  self.state = state
end

function DialogueController:is_node_active(uuid)
  if not self:is_active() then return false end
  if not self.current then return false end

  return self.current.uuid == uuid
end

function DialogueController:is_processing()
  return self.state == dialogue_state.processing
end

function DialogueController:is_active()
  if self.state == dialogue_state.err then return false end
  if self.state == dialogue_state.idle then return false end

  return true
end

function DialogueController:is_choice()
  if not self.current then return end
  if self.current.kind == tdengine.dialogue.node_kind.ChoiceList then return true end
  if self.current.kind == tdengine.dialogue.node_kind.Tithonus then return true end -- @refactor
  if self.current.kind == tdengine.dialogue.node_kind.ItemList then return true end

  return false
end

function DialogueController:select_choice(index)
  if not self.current then return end
  if not self:is_choice() then return end

  self.current.selected = index
end

function DialogueController:is_continue()
  if not self.current then return end
  return self.current.kind == tdengine.dialogue.node_kind.Continue
end

function DialogueController:is_end()
  if not self.current then return end
  if self.current.kind == tdengine.dialogue.node_kind.End then return true end
  return false
end

function DialogueController:is_button()
  return self:is_continue() or self:is_end()
end

function DialogueController:press_button()
  if not self:is_button() then return end
  self.current.clicked = true -- @refactor
end

function DialogueController:d()
  -- When messign with dialogues, it's common that I mess up and put the controller
  -- in a bad state. With this, I can just type d() in the debugger to fix.
  self.state = nil
end

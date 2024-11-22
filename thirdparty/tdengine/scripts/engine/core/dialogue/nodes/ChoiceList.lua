local ChoiceList = tdengine.node('ChoiceList')

ChoiceList.editor_fields = {
  'label',
}

ChoiceList.imgui_ignore = {
  history = true
}

ChoiceList.colors = {
  unselected = tdengine.color(1.00, 0.00, 0.00, 1.00),
  selected = tdengine.color(1.00, 0.10, 0.00, 0.50),
  hovered = tdengine.color(1.00, 1.00, 1.00, 1.00),
}

function ChoiceList:init()
  self.label = ''
  self.history = tdengine.dialogue.history
  self.indices = {}
end

function ChoiceList:short_text()
  return short_text(self.label)
end

function ChoiceList:enter(graph)
  self:reset()
  self.graph = graph
  self:add_visible_choices()

  return dialogue_state.processing
end

function ChoiceList:process()
  if self.selected then
    self.history:clear_choices()

    -- @hack: Unsure how I want to represent choices. I think I prefer "> whatever", because it's consistent
    -- over actions and speech, but that's not trivial to do right now and this is fine
    local node = self:find_selected_node()
    node.who = 'player'
    self.history:add_node(node)

    if not node.mute then
      tdengine.audio.play('ui_humver-003.wav')
    end

    return dialogue_state.advancing
  end

  return dialogue_state.processing
end

function ChoiceList:advance()
  return self:find_selected_node()
end

function ChoiceList:reset()
  self.count = 0
  self.selected = nil
  table.clear(self.indices)
  self.graph = nil
end

function ChoiceList:find_selected_node()
  local index = self.indices[self.selected]
  local uuid = self.children[index]
  return self.graph[uuid]
end

function ChoiceList:add_visible_choices()
  local found_any = false
  for index, uuid in pairs(self.children) do
    local child = self.graph[uuid]

    if child.shown and child.only_show_once then
      goto continue
    end

    local pass_checks, outcomes = evaluate_branches(child.unlock.branches, child.unlock.combinator)
    if not pass_checks then goto continue end

    found_any = true

    self.count = self.count + 1

    self.history:add_choice(child)
    self.indices[self.count] = index

    ::continue::
  end

  if not found_any then
    log.error('ChoiceList did not find any valid choices: uuid = %s', self.uuid)
  end
end

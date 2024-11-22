local Text = tdengine.node('Text')
Text.editor_fields = {
  'text',
  'who',
  'color',
  'color_id'
}

Text.imgui_ignore = {
  text = true,
  controller = true,
  history = true,
  layout = true,
}

function Text:init()
  self.text = ''
  self.who = ''
  self.color = tdengine.colors.white:copy()
  self.color_id = ''
end

function Text:enter(graph)
  local history = tdengine.dialogue.history
  history:add_node(self)

  return dialogue_state.advancing
end

function Text:process()
  return dialogue_state.advancing
end

function Text:advance(graph)
  return simple_advance(self, graph)
end

function Text:short_text()
  return short_text(self.text)
end

function Text:is_text()
  return true
end

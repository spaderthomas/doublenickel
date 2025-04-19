local Continue = dn.node('Continue')

Continue.editor_fields = {
  'mute'
}

function Continue:init()
  self.clicked = false
  self.mute = false
end

function Continue:short_text()
  return ''
end

function Continue:enter(graph)
  dn.dialogue.history:add_button()

  return dialogue_state.processing
end

function Continue:process(dt)
  if self.clicked then
    self.clicked = false

    return dialogue_state.advancing
  end

  return dialogue_state.processing
end

function Continue:advance(graph)
  return simple_advance(self, graph)
end

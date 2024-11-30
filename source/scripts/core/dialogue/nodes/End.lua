local End = doublenickel.node('End')

End.editor_fields = {
  'prompt'
}

function End:init()
  self.prompt = true
end

function End:short_text()
  return 'End'
end

function End:enter(graph)
  doublenickel.dialogue.history:add_button()
  return dialogue_state.processing
end

function End:process(dt)
  if self.prompt then
    if self.clicked then
      doublenickel.dialogue.stop()
      return dialogue_state.done
    end

    return dialogue_state.processing
  else
    doublenickel.dialogue.stop()
    return dialogue_state.done
  end
end

function End:advance(graph)
  return self
end

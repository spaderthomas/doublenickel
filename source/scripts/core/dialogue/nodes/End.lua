local End = dn.node('End')

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
  dn.dialogue.history:add_button()
  return dialogue_state.processing
end

function End:process(dt)
  if self.prompt then
    if self.clicked then
      dn.dialogue.stop()
      return dialogue_state.done
    end

    return dialogue_state.processing
  else
    dn.dialogue.stop()
    return dialogue_state.done
  end
end

function End:advance(graph)
  return self
end

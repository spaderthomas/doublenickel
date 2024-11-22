local ExitGame = tdengine.node('ExitGame')

function ExitGame:init()
end

function ExitGame:enter(graph)
  if tdengine.is_packaged_build then
    tdengine.ffi.set_exit_game()
  end
  return dialogue_state.processing
end

function ExitGame:process(graph)
  return dialogue_state.advancing
end

function ExitGame:advance(graph)
  return simple_advance(self, graph)
end

function ExitGame:short_text()
  return ''
end

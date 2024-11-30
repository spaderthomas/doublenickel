local Unreachable = tdengine.node('Unreachable')

function Unreachable:init()
end

function Unreachable:enter(graph)
  return dialogue_state.err
end

function Unreachable:advance(graph)
  return dialogue_state.err
end

function Unreachable:short_text()
  return ''
end

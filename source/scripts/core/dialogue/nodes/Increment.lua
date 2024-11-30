local Increment = tdengine.node('Increment')

Increment.editor_fields = {
  'variable',
  'step'
}

function Increment:init()
  self.variable = 'intro.default'
  self.step = 1
end

function Increment:enter(graph)
  tdengine.state.increment(self.variable, self.step)
  return dialogue_state.advancing
end

function Increment:advance(graph)
  return simple_advance(self, graph)
end

function Increment:short_text()
  return short_text(self.variable)
end

function Increment:uses_state()
  return true
end

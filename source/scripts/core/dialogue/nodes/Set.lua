local Set = tdengine.node('Set')

Set.editor_fields = {
  'variable',
  'value'
}

function Set:init()
  self.variable = 'intro.default'
  self.value = true
end

function Set:enter(graph)
  tdengine.state.set(self.variable, self.value)
  return dialogue_state.advancing
end

function Set:advance(graph)
  return simple_advance(self, graph)
end

function Set:short_text()
  return short_text(self.variable)
end

function Set:uses_state()
  return true
end

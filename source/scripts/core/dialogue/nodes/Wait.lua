local Wait = tdengine.node('Wait')

Wait.editor_fields = {
  'time'
}

Wait.imgui_ignore = {
  elapsed = true
}

function Wait:init()
  self.time = 0
  self.elapsed = 0
end

function Wait:enter(graph)
  self.elapsed = 0
  return dialogue_state.processing
end

function Wait:process(graph)
  self.elapsed = self.elapsed + tdengine.dt
  if self.elapsed >= self.time then
    return dialogue_state.advancing
  end

  return dialogue_state.processing
end

function Wait:advance(graph)
  return simple_advance(self, graph)
end

function Wait:short_text()
  return string.format('%.2f', self.time)
end

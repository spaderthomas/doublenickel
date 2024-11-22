local Label = tdengine.node('Label')

Label.editor_fields = {
  'label',
  'export'
}

function Label:init()
  self.label = 'DefaultLabel'
  self.export = false
end

function Label:advance(graph)
  return simple_advance(self, graph)
end

function Label:short_text()
  return short_text(self.label)
end

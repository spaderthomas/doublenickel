local SampleNode = tdengine.node('SampleNode')

SampleNode.editor_fields = {
  'field_to_save'
}

function SampleNode:init()
  self.field_to_save = 69
end

function SampleNode:advance(graph)
  return simple_advance(self, graph)
end

function SampleNode:short_text()
  return short_text(self.tool:to_string())
end

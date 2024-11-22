local DevNote = tdengine.node('DevNote')
DevNote.editor_fields = {
  'text',
  'must_be_fixed'
}

DevNote.imgui_ignore = {
  'controller',
  'history',
  'input',
  'layout',
}

function DevNote:init()
  self.text = ''
  self.must_be_fixed = false
end

function DevNote:enter(graph)
  dbg()
end

function DevNote:process()
  dbg()
end

function DevNote:advance(graph)
  dbg()
end

function DevNote:short_text()
  return short_text(self.text)
end

local DevMarker = tdengine.node('DevMarker')
DevMarker.editor_fields = {
  'text',
}

DevMarker.imgui_ignore = {
  'controller',
  'history',
  'input',
  'layout',
}

function DevMarker:init()
  self.text = ''
end

function DevMarker:enter(graph)
  dbg()
end

function DevMarker:process()
  dbg()
end

function DevMarker:advance(graph)
  dbg()
end

function DevMarker:short_text()
  return self.text
end

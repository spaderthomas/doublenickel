EditorCamera = doublenickel.editor.define('EditorCamera')

function EditorCamera:init()
  self.offset = doublenickel.vec2()
  self.enabled = true
end

function EditorCamera:update()
  if doublenickel.tick then return end
  doublenickel.ffi.set_camera(self.offset:floor():unpack())
end

function EditorCamera:set(position)
  self.offset = position
end

function EditorCamera:move(delta)
  self.offset = self.offset:add(delta)
end

function EditorCamera:set_offset(offset)
  self.offset:assign(offset)
end

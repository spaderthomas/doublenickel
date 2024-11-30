EditorCamera = tdengine.editor.define('EditorCamera')

function EditorCamera:init()
  self.offset = tdengine.vec2()
  self.enabled = true
end

function EditorCamera:update()
  if tdengine.tick then return end
  tdengine.ffi.set_camera(self.offset:floor():unpack())
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

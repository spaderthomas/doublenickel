EditorCamera = dn.editor.define('EditorCamera')

function EditorCamera:init()
  self.offset = dn.vec2()
  self.enabled = true
end

function EditorCamera:update()
  if dn.tick then return end
  -- dn.unported.set_camera(self.offset:floor():unpack())
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

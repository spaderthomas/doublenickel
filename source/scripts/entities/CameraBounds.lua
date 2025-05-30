CameraBounds = dn.entity.define('CameraBounds')

CameraBounds.components = {}

CameraBounds.editor_fields = {
  'position',
  'size',
  'lock',
  'lock_to',
}

function CameraBounds:init(params)
  self.position = params.position or dn.vec2()
  self.size = params.size or dn.vec2()
  self.lock = ternary(params.lock, true, false)
  self.lock_to = params.lock_to or dn.vec2()
  self.debug = false
end

function CameraBounds:draw()
  if self.debug then
    dn.unported.set_world_space(true)()
    dn.unported.set_layer(1000)

    dn.unported.draw_quad_l(self.position, self.size, dn.colors.green_light_trans)
  end
end

function CameraBounds:left()
  return self.position.x
end

function CameraBounds:right()
  return self.position.x + self.size.x
end

function CameraBounds:bottom()
  return self.position.y - self.size.y - 1080
end

function CameraBounds:top()
  return self.position.y - 1080
end

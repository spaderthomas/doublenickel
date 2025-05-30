-- POINT
-- A small utility for moving to the correct location when advancing by a tile
local BackgroundPoint = dn.class.define('BackgroundPoint')
function BackgroundPoint:init()
  self.base_position = dn.vec2()
  self.position = dn.vec2()
  self.width = 0
  self.zoom = 1
end

function BackgroundPoint:set_base_position(base_position)
  self.base_position = base_position:copy()
  self.position = base_position:copy()
end

function BackgroundPoint:set_width(width)
  self.width = width
end

function BackgroundPoint:advance()
  self.position.x = self.position.x + 1024 * self.zoom
  local dx = self.position.x - self.base_position.x
  if dx >= self.width * self.zoom then
    self.position.x = self.base_position.x
    self.position.y = self.position.y - 1024 * self.zoom
  end
end

-- BACKGROUND
Background = dn.entity.define('Background')
Background.components = {}
Background.editor_fields = {
  'background',
  'zoom',
  '__enabled'
}

function Background:init(params)
  self.background = params.background or 'computer'
  self.point = BackgroundPoint:new()
  self.zoom = params.zoom or 1
  self.base_position = dn.vec2()
  self.__enabled = ternary(params.__enabled, true, false)
end

function Background:draw(dt)
  if not self.__enabled then return end

  local data = dn.background.find(self.background)
  if not data then return end

  -- Reset the point. This is really just an editor thing, because if we change what image this
  -- entity displays, we need the new size; except that only changes in the editor.
  self.point:set_base_position(self.base_position)
  self.point:set_width(data.size.x)
  self.point.zoom = self.zoom

  local options = {
    --size = dn.vec2(1024 * self.zoom, 1024 * self.zoom)
    size = dn.vec2(2048 * self.zoom, 2048 * self.zoom),
    opacity = self.opacity or 1
  }

  dn.unported.set_world_space(true)
  dn.unported.set_layer(self.layer or dn.layers.background)
  for index, tile in pairs(data.tiles) do
    dn.unported.draw_image_l(tile, self.point.position, options)
    self.point:advance()
  end
end

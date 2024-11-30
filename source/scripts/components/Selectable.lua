Selectable = tdengine.component.define('Selectable')

function Selectable:init()
  self.on_hover = function() end
  self.on_unhover = function() end
  self.on_select = function() end
  self.is_visible = function() return false end
end

function Selectable:draw()

end

local EntitySelection = dn.editor.define('EntitySelection')

dn.enum.define(
	'EntitySelection',
	{
		None = 0,
		Some = 1,
	}
)

function EntitySelection:init()
  self.state = dn.enums.EntitySelection.None
  self.selection = nil
  self.context_selection = nil

  self.input = ContextualInput:new(dn.enums.InputContext.Game, dn.enums.CoordinateSystem.World)

  self.hotkeys = {
    deselect = glfw.keys.ESCAPE,
    delete = glfw.keys.DELETE
  }
end

function EntitySelection:update()
  if self.input:pressed(self.hotkeys.deselect) then
    self:deselect()
  end

  if self.input:pressed(self.hotkeys.delete) then
    self:delete()
  end

end

function EntitySelection:delete()
  if not self.state == dn.enums.EntitySelection.Some then return end

  dn.entity.destroy(self.selection.id)
  self:deselect()
end


function EntitySelection:select_entity()
end

function EntitySelection:select_collider(collider)
  if not collider then return self:deselect() end

  self.selection = collider:get_entity()
  self.state = dn.enums.EntitySelection.Some
end

function EntitySelection:deselect()
  self.selection = nil
  self.state = dn.enums.EntitySelection.None
end

function EntitySelection:is_entity_selected(entity)
  if not entity then return false end
  return self.selection == entity
end

function EntitySelection:is_collider_selected(collider)
  if not collider then return false end
  return self.selection == collider:get_entity()
end


function EntitySelection:context_select_entity(entity)
  self.context_selection = entity
end

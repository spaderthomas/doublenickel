local EntityList = doublenickel.class.define('EntityList')

doublenickel.enum.define(
  'EntityListKind',
  {
    Game = 0,
    Persistent = 1,
    Editor = 1,
  }
)

function EntityList:init(name, entities, kind)
  self.name = name
  self.entities = entities
  self.table_editors = {}

  self.ids = {
    combo_box = string.format('##%s:combo_box', table.address(self)),
    context_menu = string.format('##%s:context_menu', table.address(self))
  }

  self.popup = Popups:new({
    [self.ids.context_menu] = {
      callback = function() self:on_context_menu() end
    }
  })

  self.kind = kind
  self.selected_type = ''
end

function EntityList:on_context_menu()
  if imgui.BeginPopup(self.ids.context_menu) then
    local selection = doublenickel.find_entity_editor('EntitySelection').context_selection
   
    if self.kind == doublenickel.enums.EntityListKind.Game then
      if imgui.MenuItem('Copy') then
        doublenickel.entity.copy(selection)
      end
      if imgui.MenuItem('Delete') then
        doublenickel.entity.destroy(selection.id)
      end
      if imgui.MenuItem('Make Persistent') then
        doublenickel.entity.entities[selection.id] = nil
        doublenickel.entity.persistent_entities[selection.id] = selection
      end
    elseif self.kind == doublenickel.enums.EntityListKind.Persistent then
      if imgui.MenuItem('Delete') then
        doublenickel.entity.persistent_entities[selection.id] = nil
      end

    elseif self.kind == doublenickel.enums.EntityListKind.Editor then
    end

    imgui.EndPopup()
  end
end

function EntityList:draw()
  self.popup:update()

  doublenickel.editor.begin_window(self.name)
  self:draw_add_entity()
  self:draw_entities()
  doublenickel.editor.end_window()
end

function EntityList:draw_add_entity()
  if self.kind == doublenickel.enums.EntityListKind.Dynamic then
    imgui.extensions.ComboBox(
      self.ids.combo_box,
      self.selected_type,
      doublenickel.entity.sorted_types,
      function(entity_type) self.selected_type = entity_type end
    )

    imgui.SameLine()

    if imgui.Button('Add') then
      local entity = doublenickel.entity.create(self.selected_type)
      self.entities[entity.id] = entity
    end
  end

end

function EntityList:draw_entities()
  local sorted_ids = {}
  for id, entity in pairs(self.entities) do
    if not self.table_editors[id] then
      self.table_editors[id] = imgui.extensions.TableEditor(entity)
    end

    table.insert(sorted_ids, id)
  end

  table.sort(sorted_ids)

  for _, id in pairs(sorted_ids) do
    self:draw_entity(id)
  end
end

function EntityList:draw_entity(id)
  local entity = self.entities[id]

  local header_color = doublenickel.colors.white
  if doublenickel.find_entity_editor('EntitySelection'):is_entity_selected(entity) then
    header_color = doublenickel.colors.spring_green
  end
  imgui.PushStyleColor(ffi.C.ImGuiCol_Text, header_color:to_u32())

  local tree_expanded = imgui.TreeNode(self:build_label(entity))
  self:check_context_menu(entity)
  self:draw_metadata(entity)

  if tree_expanded then
    self.table_editors[id]:draw()
    imgui.TreePop()
  end
end

function EntityList:check_context_menu(entity)
  if imgui.IsItemClicked(1) then
    self.popup:open_popup(self.ids.context_menu)
    doublenickel.find_entity_editor('EntitySelection'):context_select_entity(entity)
  end
end

function EntityList:draw_metadata(entity)
  if not entity.tag or #entity.tag == 0 then
    imgui.PopStyleColor()
    return
  end

  local label = string.format('(%s)', entity.tag)

  imgui.SameLine()
  imgui.Text(label)
  imgui.PopStyleColor()
end

function EntityList:build_label(entity)
  local format = '%s##entity_list:%s'
  return string.format(format, entity:class(), entity.uuid)
end


--
-- SCENE EDITOR
--
local SceneEditor = doublenickel.editor.define('SceneEditor')

SceneEditor.states = doublenickel.enum.define(
  'SceneEditorState',
  {
    Idle = 0,
    DragPosition = 1,
    ColliderEditor = 2,
  }
)

function SceneEditor:init()
  self.scene_name = ''
  self.selected = nil

  self.state = self.states.Idle

  self.input = ContextualInput:new(doublenickel.enums.InputContext.Game, doublenickel.enums.CoordinateSystem.World)

  self.drag_state = {}

  self.entity_lists = {
    game = EntityList:new('Entities', doublenickel.entity.entities, doublenickel.enums.EntityListKind.Dynamic),
    persistent = EntityList:new('Persistent', doublenickel.entity.persistent_entities, doublenickel.enums.EntityListKind.Static),
    editor = EntityList:new('Editor', doublenickel.editor.entities, doublenickel.enums.EntityListKind.Static),
  }
end
  
function SceneEditor:on_editor_play()
  doublenickel.find_entity_editor('EditorCamera').enabled = false

  doublenickel.gui.reset()
  doublenickel.audio.stop_all()

  doublenickel.input.push_context(doublenickel.enums.InputContext.Game)

end

function SceneEditor:on_editor_stop()
  local camera = doublenickel.find_entity_editor('EditorCamera')
  camera.enabled = true
  camera:set_offset(doublenickel.vec2())

  doublenickel.gui.reset()
  doublenickel.audio.stop_all()
end

function SceneEditor:on_begin_frame()
  if self.state == self.states.drag_position then
    if not doublenickel.input.down(glfw.keys.MOUSE_BUTTON_1) then
      self.state = self.states.idle
    end

    local delta = self.input:mouse_delta():scale(-1)
    local camera = doublenickel.find_entity_editor('EditorCamera')
    camera:move(delta)
  end
end

function SceneEditor:draw()
  for _, entity_list in pairs(self.entity_lists) do
    entity_list:draw()
  end
end

function SceneEditor:update()
  if doublenickel.tick then return end

  local game_view = doublenickel.find_entity_editor('GameViewManager')
  if not game_view.hover then return end

  self:update_state()

end

function SceneEditor:update_state()
  if self.state == self.states.Idle then
    if self.input:left_click() then
      local collider_editor = doublenickel.find_entity_editor('ColliderEditor')
      if collider_editor:try_consume() then
        self.state = self.states.ColliderEditor
      else
        self.state = self.states.DragPosition
      end
    end

  elseif self.state == self.states.ColliderEditor then
    local collider_editor = doublenickel.find_entity_editor('ColliderEditor')
    if not collider_editor:is_consuming_input() then
      self.state = self.states.Idle
    end
 
  elseif self.state == self.states.DragPosition then
    if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
      self.state = self.states.Idle
    end

    local camera = doublenickel.find_entity_editor('EditorCamera')
    local delta = self.input:mouse_delta():scale(-1)
    camera:move(delta)

  end
end

-------------------
-- SERIALIZATION --
-------------------
function SceneEditor:load(name)
  self.scene_name = name
self:disable_play_mode()
end

function SceneEditor:save()
  if #self.scene_name == 0 then return end

  doublenickel.scene.write(doublenickel.entity.entities, self.scene_name)
end

function SceneEditor:toggle_play_mode()
  if doublenickel.tick then
    self:disable_play_mode()
  else
    self:enable_play_mode()
  end
end

function SceneEditor:enable_play_mode()
  doublenickel.scene.set_tick(true)
  doublenickel.scene.load(self.scene_name)
end

function SceneEditor:disable_play_mode()
  doublenickel.scene.set_tick(false)
  doublenickel.scene.load(self.scene_name)
end
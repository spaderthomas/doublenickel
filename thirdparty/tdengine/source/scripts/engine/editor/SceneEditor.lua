local EntityList = tdengine.class.define('EntityList')

tdengine.enum.define(
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
    local selection = tdengine.find_entity_editor('EntitySelection').context_selection
   
    if self.kind == tdengine.enums.EntityListKind.Game then
      if imgui.MenuItem('Copy') then
        tdengine.entity.copy(selection)
      end
      if imgui.MenuItem('Delete') then
        tdengine.entity.destroy(selection.id)
      end
      if imgui.MenuItem('Make Persistent') then
        tdengine.entity.entities[selection.id] = nil
        tdengine.entity.persistent_entities[selection.id] = selection
      end
    elseif self.kind == tdengine.enums.EntityListKind.Persistent then
      if imgui.MenuItem('Delete') then
        tdengine.entity.persistent_entities[selection.id] = nil
      end

    elseif self.kind == tdengine.enums.EntityListKind.Editor then
    end

    imgui.EndPopup()
  end
end

function EntityList:draw()
  self.popup:update()

  tdengine.editor.begin_window(self.name)
  self:draw_add_entity()
  self:draw_entities()
  tdengine.editor.end_window()
end

function EntityList:draw_add_entity()
  if self.kind == tdengine.enums.EntityListKind.Dynamic then
    imgui.extensions.ComboBox(
      self.ids.combo_box,
      self.selected_type,
      tdengine.entity.sorted_types,
      function(entity_type) self.selected_type = entity_type end
    )

    imgui.SameLine()

    if imgui.Button('Add') then
      local entity = tdengine.entity.create(self.selected_type)
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

  local header_color = tdengine.colors.white
  if tdengine.find_entity_editor('EntitySelection'):is_entity_selected(entity) then
    header_color = tdengine.colors.spring_green
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
    tdengine.find_entity_editor('EntitySelection'):context_select_entity(entity)
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
local SceneEditor = tdengine.editor.define('SceneEditor')

SceneEditor.states = tdengine.enum.define(
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

  self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.World)

  self.drag_state = {}

  self.entity_lists = {
    game = EntityList:new('Entities', tdengine.entity.entities, tdengine.enums.EntityListKind.Dynamic),
    persistent = EntityList:new('Persistent', tdengine.entity.persistent_entities, tdengine.enums.EntityListKind.Static),
    editor = EntityList:new('Editor', tdengine.editor.entities, tdengine.enums.EntityListKind.Static),
  }
end
  
function SceneEditor:on_editor_play()
  tdengine.find_entity_editor('EditorCamera').enabled = false

  tdengine.gui.reset()
  tdengine.audio.stop_all()

  tdengine.input.push_context(tdengine.enums.InputContext.Game)

end

function SceneEditor:on_editor_stop()
  local camera = tdengine.find_entity_editor('EditorCamera')
  camera.enabled = true
  camera:set_offset(tdengine.vec2())

  tdengine.gui.reset()
  tdengine.audio.stop_all()
end

function SceneEditor:on_begin_frame()
  if self.state == self.states.drag_position then
    if not tdengine.input.down(glfw.keys.MOUSE_BUTTON_1) then
      self.state = self.states.idle
    end

    local delta = self.input:mouse_delta():scale(-1)
    local camera = tdengine.find_entity_editor('EditorCamera')
    camera:move(delta)
  end
end

function SceneEditor:draw()
  for _, entity_list in pairs(self.entity_lists) do
    entity_list:draw()
  end
end

function SceneEditor:update()
  if tdengine.tick then return end

  local game_view = tdengine.find_entity_editor('GameViewManager')
  if not game_view.hover then return end

  self:update_state()

end

function SceneEditor:update_state()
  if self.state == self.states.Idle then
    if self.input:left_click() then
      local collider_editor = tdengine.find_entity_editor('ColliderEditor')
      if collider_editor:try_consume() then
        self.state = self.states.ColliderEditor
      else
        self.state = self.states.DragPosition
      end
    end

  elseif self.state == self.states.ColliderEditor then
    local collider_editor = tdengine.find_entity_editor('ColliderEditor')
    if not collider_editor:is_consuming_input() then
      self.state = self.states.Idle
    end
 
  elseif self.state == self.states.DragPosition then
    if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
      self.state = self.states.Idle
    end

    local camera = tdengine.find_entity_editor('EditorCamera')
    camera:move(self.input:mouse_delta():scale(-1))
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

  tdengine.scene.write(tdengine.entity.entities, self.scene_name)
end

function SceneEditor:toggle_play_mode()
  if tdengine.tick then
    self:disable_play_mode()
  else
    self:enable_play_mode()
  end
end

function SceneEditor:enable_play_mode()
  tdengine.scene.set_tick(true)
  tdengine.scene.load(self.scene_name)
end

function SceneEditor:disable_play_mode()
  tdengine.scene.set_tick(false)
  tdengine.scene.load(self.scene_name)
end
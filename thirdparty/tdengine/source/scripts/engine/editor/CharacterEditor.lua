Character = tdengine.class.define('Character')

function Character:init(params)
  params = params or {}

  self.name = params.name                           or 'jerry_garcia'
  self.display_name = params.display_name           or 'Jerry Garcia'
  self.color = tdengine.color(params.color)         or tdengine.colors.red:copy()
  self.body_color = tdengine.color(params.color)    or tdengine.colors.white:copy()
  self.font = params.font                           or 'game'
  self.portrait = params.portrait                   or ''
  self.omit_display_name = params.omit_display_name or false
end

CharacterEditor = tdengine.editor.define('CharacterEditor')

CharacterEditor.popup_kind = {
  edit = 'Edit Character##character_editor'
}

function CharacterEditor:init()
  local popups = {
    [self.popup_kind.edit] = {
      window = 'Edit Character',
      callback = function() self:edit_character_popup() end
    }
  }
  self.popups = Popups:new(popups)

  self.portrait_size = tdengine.vec2(160, 160)
  self.character = {}
  self.table_editor = {}
  self:setup_create_character()
end

function CharacterEditor:update(dt)
  self.popups:update()
end

-----------
-- SETUP --
-----------
function CharacterEditor:setup_create_character()
  self.character = Character:new()
  self.table_editor = imgui.extensions.TableEditor(self.character)
end

function CharacterEditor:edit_character(name)
  local character_data = tdengine.dialogue.characters[name]
  self.character = Character:new(character_data)
  self.table_editor = imgui.extensions.TableEditor(self.character)
  self.popups:open_popup(self.popup_kind.edit)
end

------------
-- POPUPS --
------------
function CharacterEditor:edit_character_popup()
  local popup_size = tdengine.vec2(600, 800)
	tdengine.editor.center_next_window(popup_size)

  local flags = 0
  flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoMove)
  flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoResize)
  flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoCollapse)


  if imgui.BeginPopupModal(self.popup_kind.edit, nil, flags) then
    self:begin_columns()
    self:draw_portrait()

    imgui.NextColumn()
    self:draw_name()
    self.table_editor:draw()

    self:end_columns()

    imgui.Dummy(10, 10)

    self:draw_buttons()
  end
end

function CharacterEditor:invalid_popup()
  local sx = 500
  local sy = 150
  local wx = tdengine.screen_dimensions().x
  imgui.SetCursorPosX((wx / 2) - (sx / 2))
  imgui.SetNextWindowSize(sx, sy)

  local flags = 0
  flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoMove)
  flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoResize)
  flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoCollapse)

  if imgui.BeginPopupModal('Invalid Character!', nil, flags) then
    if self.character.name == '' then
      imgui.Text('- Characters must have an internal name, but yours was empty.')
    end

    if self.character.display_name == '' and not self.character.omit_display_name then
      imgui.Text('- Characters must have a user-facing name, but yours was empty.')
    end

    imgui.Dummy(5, 5)

    if imgui.Button('OK', imgui.ImVec2(60, 30)) then
      imgui.CloseCurrentPopup()
    end

    imgui.EndPopup()
  end
end

---------------------
-- POPUP INTERNALS --
---------------------
function CharacterEditor:begin_columns()
  imgui.Columns(2, nil, false)
  imgui.SetColumnWidth(0, self.portrait_size.x)
end

function CharacterEditor:end_columns()
  imgui.Columns()
end

function CharacterEditor:draw_portrait()
  if #self.character.portrait == 0 then 
    imgui.Text('NO PORTRAIT')
  else
    imgui.GameImage(self.character.portrait, self.portrait_size:unpack())
  end

end

function CharacterEditor:draw_name()
  local draw_colored = function(text, font, color)
    imgui.PushStyleColor(ffi.C.ImGuiCol_Text, color:to_u32())
    imgui.PushFont(font)
    imgui.Text(text)
    imgui.PopFont()
    imgui.PopStyleColor()
  end

  draw_colored(self.character.display_name, 'merriweather-bold-32', self.character.color)
  draw_colored('This is what my dialogue looks like', 'merriweather', self.character.body_color)
end

function CharacterEditor:draw_buttons()
  local done = false
  local invalid = self.character_name == '' or
      (self.character.display_name == '' and not self.character.omit_display_name)
  local invalid_popup = false

  -- Press OK to save
  if imgui.Button('OK', imgui.ImVec2(100, 25)) then
    if invalid then
      invalid_popup = true
    else
      done = true

      -- Pull any data out of editor and into table and add it to the list.
      tdengine.dialogue.characters[self.character.name] = deep_copy_any(self.character)
      tdengine.module.write_to_named_path('character_info', tdengine.dialogue.characters, tdengine.module.WriteOptions.Pretty)
    end
  end

  -- If we try to save while it's invalid, we'll show a popup
  if invalid_popup then
    imgui.OpenPopup('Invalid Character!')
  end
  self:invalid_popup()

  imgui.SameLine()

  -- Press Cancel to discard
  if imgui.Button('Cancel', imgui.ImVec2(100, 25)) then
    done = true
  end

  -- Cleanup if we saved successfully
  if done then
    self:setup_create_character()
    self.popups:close_popup(self.popup_kind.edit)
  end
  imgui.EndPopup()
end

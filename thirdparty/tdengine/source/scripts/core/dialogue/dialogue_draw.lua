DialogueBoxItem = tdengine.class.define('DialogueBoxItem')

function DialogueBoxItem:init()
end

function DialogueBoxItem:init_from_node(node)
  self.text = node.text or ''
  self:set_character(node.who)

  if node.color_id and #node.color_id > 0 and tdengine.colors[node.color_id] then
    self.color = tdengine.colors[node.color_id]:copy()
  elseif self.character and self.character.body_color then
    self.color = tdengine.color(self.character.body_color)
  elseif node.color then
    self.color = tdengine.color(node.color)
  else
    self.color = tdengine.colors.white:copy()
  end
end

function DialogueBoxItem:set_text(text)
  self.text = text
end

function DialogueBoxItem:set_character(character_name)
  if not character_name then
    self:hide_character(); return
  end
  if #character_name == 0 then
    self:hide_character(); return
  end

  local character = tdengine.dialogue.characters[character_name] or tdengine.dialogue.characters.unknown
  self.character = character
  self.font = self.character.font or 'game'
end

function DialogueBoxItem:hide_character()
  self.character = nil
  self.display_speaker = false
end

function DialogueBoxItem:has_character()
  return self.character ~= nil
end

function DialogueBoxItem:build_body_options()
  local options = {
    world = false,
    font = 'merriweather-24',
    color = tdengine.color(self.color),
  }

  return options
end

function DialogueBoxItem:build_speaker_options()
  if not self.character then return {} end

  return {
    text = self.character.display_name,
    font = 'merriweather-bold-32',
    color = tdengine.color(self.character.color),
  }
end

-----------------
-- CHOICE ITEM --
-----------------
ChoiceItem = tdengine.class.define('ChoiceItem')

function ChoiceItem:init()
end

function ChoiceItem:init_from_node(node)
  self.node_uuid = node.uuid
  self.text = node.text or ''
  self.font = node.font or 'merriweather-bold-24'
  self.shown = node.shown
end

function ChoiceItem:build_options()
  return {
    font = self.font,
  }
end

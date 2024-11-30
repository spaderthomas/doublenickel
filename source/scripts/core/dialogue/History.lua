History = tdengine.class.define('History')

function History:init()
  self.unread_items = {}
  self.current_items = {}
  self.old_items = {}
  self.choices = {}

  self.was_dirty = false
  self.dirty = false
end

function History:update(dt)
  self.was_dirty = self.dirty
end

function History:add_node(node)
  local item = DialogueBoxItem:new()
  item:init_from_node(node)
  table.insert(self.unread_items, item)
  self.dirty = true
end

function History:add_choice(node)
  local item = ChoiceItem:new()
  item:init_from_node(node)
  table.insert(self.choices, item)
  self.dirty = true
end

function History:add_button()
  self.dirty = true
end

function History:clear()
  table.clear(self.unread_items)
  table.clear(self.old_items)
  table.clear(self.current_items)
  self.dirty = false
end

function History:read()
  for index, item in pairs(self.current_items) do
    table.insert(self.old_items, item)
  end
  table.clear(self.current_items)

  for index, item in pairs(self.unread_items) do
    table.insert(self.current_items, item)
  end
  table.clear(self.unread_items)

  self.dirty = false
end

function History:has_unread_items()
  return self.dirty
end

function History:clear_choices(node)
  table.clear(self.choices)
end

function History:has_choices()
  return #self.choices ~= 0
end

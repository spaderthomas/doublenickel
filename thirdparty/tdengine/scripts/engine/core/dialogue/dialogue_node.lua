function tdengine.serialize_node(self)
  local data = {}
  data.children = table.deep_copy(self.children)
  data.is_entry_point = self.is_entry_point
  data.kind = self.kind
  data.uuid = self.uuid

  if self.editor_fields then
    for _, field in pairs(self.editor_fields) do
      local value = self[field]
      if type(value) == 'table' and value.serialize then
        data[field] = value:serialize()
      elseif type(value) == 'table' and value.__enum then
        data[field] = {
          name = value:to_string(),
          __enum = value.__enum
        }
      elseif type(value) == 'function' then
        goto continue
      else
        data[field] = deep_copy_any(value)
      end

      ::continue::
    end
  end

  return data
end

function tdengine.deserialize_node(self, data)
  for key, value in pairs(data) do
    if type(value) == 'table' and value.__enum then
      self[key] = tdengine.enums[value.__enum][value.name]
    else
      self[key] = deep_copy_any(value)
    end
  end
end

local dialogue_node_mixin = {
  -- Data fields. Filled in here and in tdengine.create_node()
  kind = '',
  is_entry_point = false,
  children = {},
  uuid = '',

  -- Processing
  enter = function(self) return dialogue_state.advancing end,
  process = function(self) return dialogue_state.advancing end,
  advance = function(self, graph) dbg() end,

  -- Serialization
  serialize = function(self) return tdengine.serialize_node(self) end,
  deserialize = function(self, d) return tdengine.deserialize_node(self, d) end,

  -- Virtual methods that define the shape of the node for various editor tools
  is_text = function(self) return false end,
  is_conditional = function(self) return false end,
  is_bifurcate = function(self) return false end,
  uses_state = function(self) return false end,

  -- Other virtual methods
  short_text = function(self) return '' end,
  get_branches = function(self) return nil end,
  get_combinator = function(self) return nil end,

  -- Base node class methods
  get_child = function(self) return self.children[1] end
}

function tdengine.node(name)
  local class = tdengine.class.define(name)
  class:include(dialogue_node_mixin)

  class.imgui_ignore = {}

  -- Add to bookkeeping
  tdengine.dialogue.node_kind[name] = name
  tdengine.dialogue.node_type[name] = class

  table.clear(tdengine.dialogue.sorted_node_kinds)
  for name, _ in pairs(tdengine.dialogue.node_type) do
    table.insert(tdengine.dialogue.sorted_node_kinds, name)
  end
  table.sort(tdengine.dialogue.sorted_node_kinds)


  return class
end

function tdengine.create_node(name)
  params = params or {}

  -- Find the matching type in Lua
  class = tdengine.dialogue.node_type[name]
  if not class then
    tdengine.log(string.format("could not find node type: type = %s", name))
    return nil
  end

  -- Construct the entity with a do-nothing constructor
  local instance = class:new()
  instance.kind = name
  instance.uuid = tdengine.uuid()
  instance.children = {}


  if instance.init then
    instance:init(params)
  end
  return instance
end

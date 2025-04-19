function dn.serialize_node(self)
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

function dn.deserialize_node(self, data)
  for key, value in pairs(data) do
    if type(value) == 'table' and value.__enum then
      self[key] = dn.enums[value.__enum][value.name]
    else
      self[key] = deep_copy_any(value)
    end
  end
end

local dialogue_node_mixin = {
  -- Data fields. Filled in here and in dn.create_node()
  kind = '',
  is_entry_point = false,
  children = {},
  uuid = '',

  -- Processing
  enter = function(self) return dialogue_state.advancing end,
  process = function(self) return dialogue_state.advancing end,
  advance = function(self, graph) dbg() end,

  -- Serialization
  serialize = function(self) return dn.serialize_node(self) end,
  deserialize = function(self, d) return dn.deserialize_node(self, d) end,

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

function dn.node(name)
  local class = dn.class.define(name)
  class:include(dialogue_node_mixin)

  class.imgui_ignore = {}

  -- Add to bookkeeping
  dn.dialogue.node_kind[name] = name
  dn.dialogue.node_type[name] = class

  table.clear(dn.dialogue.sorted_node_kinds)
  for name, _ in pairs(dn.dialogue.node_type) do
    table.insert(dn.dialogue.sorted_node_kinds, name)
  end
  table.sort(dn.dialogue.sorted_node_kinds)


  return class
end

function dn.create_node(name)
  params = params or {}

  -- Find the matching type in Lua
  class = dn.dialogue.node_type[name]
  if not class then
    dn.ffi.log(string.format("could not find node type: type = %s", name))
    return nil
  end

  -- Construct the entity with a do-nothing constructor
  local instance = class:new()
  instance.kind = name
  instance.uuid = dn.uuid()
  instance.children = {}


  if instance.init then
    instance:init(params)
  end
  return instance
end

function tdengine.entity.allocate_id()
  local id = tdengine.entity.next_id
  tdengine.entity.next_id = tdengine.entity.next_id + 1
  return id
end

function add_component(entity, name, data)
  -- This is a module-private function; anyone writing an entity or component should use the API on the
  -- class itself (i.e. entity:add_component(name, data))
  data = data or {}

  local component = tdengine.component.types[name]:new(data)
  component.name = name
  component.id = tdengine.entity.allocate_id()
  component.uuid = data.uuid or tdengine.uuid()
  component.__internal = {
    entity = entity,
  }

  component.get_entity = function(self) return self.__internal.entity end
  component.if_tag = function(self, tag) return self:get_entity() and self:get_entity().tag == tag end
  component.break_on_tag = function(self, tag) if self:if_tag(tag) then dbg() end end


  if entity then
    entity.components[name] = component
  end

  return component
end

------------------
-- ENTITY CLASS --
------------------
function tdengine.entity.define(name)
  local class = tdengine.class.define(name)
  class:include({
    get_name = function(self) return self.name end,
    serialize = function(self) return tdengine.serialize_entity(self) end,
    add_component = function(self, component_name, data)
      local component = add_component(self, component_name, data)
  
      -- late_init() is usually called during entity creation, after all individual components have been
      -- initialized. But here, we're adding a component at runtime, so we need to call it directly.
      if component.late_init then component:late_init() end
    end,
    remove_component = function(self, component_name)
      self.components[component_name] = nil
    end,
    find_component = function(self, component_name) return self.components[component_name] end,
    hide = function(self) self.__internal.hidden = true end,
    show = function(self) self.__internal.hidden = false end,
    is_hidden = function(self) return self.__internal.hidden end,
    iterate_components = function(self) return tdengine.iterator.values(self.components) end      
  })
  class:include_update()
  class:set_field_metadata('id', FieldMetadata.Presets.ReadOnly)
  class:set_field_metadata('uuid', FieldMetadata.Presets.ReadOnly)

  tdengine.entity.types[name] = class

  tdengine.entity.sorted_types = {}
  for type_name, _ in pairs(tdengine.entity.types) do
    table.insert(tdengine.entity.sorted_types, type_name)
  end
  table.sort(tdengine.entity.sorted_types)

  return class
end


function tdengine.entity.iterate(name)
  if name then
    return tdengine.iterator.values(tdengine.entity.entities, function (_, entity)
      return entity.name == name
    end)
  else
    return tdengine.iterator.values(tdengine.entity.entities)
  end
end

function tdengine.entity.iterate_persistent()
  return tdengine.iterator.values(tdengine.entity.persistent_entities)
end


function tdengine.entity.iterate_staged()
  return tdengine.iterator.values(tdengine.entity.created_entities)
end

function tdengine.entity.run_update_callback(entity, callback)
  local fn = entity[callback:to_string()]
  fn(entity)

  for component in entity:iterate_components() do
    fn = component[callback:to_string()]
    fn(component)
  end
end



function tdengine.entity.create_anonymous(name, data)
  data = data or {}
  data.components = data.components or {}

  EntityType = tdengine.entity.types[name]
  if not EntityType then
    log.warn(string.format("could not find entity type: type = %s", name))
  end

  local entity = EntityType:allocate()

  local id = tdengine.entity.allocate_id()
  entity.id = id
  entity.name = name
  entity.uuid = data.uuid or tdengine.uuid()
  entity.tag = data.tag or ''
  entity.__internal = data.__internal or {
    hidden = false
  }
  entity.imgui_ignore = {
    name = true
  }

  entity.components = {}
  if EntityType.components then
    for _, component_name in pairs(EntityType.components) do
      local component_data = data.components[component_name]
      add_component(entity, component_name, component_data)
    end
  end

  if entity.init then entity:init(data) end

  for _, component in pairs(entity.components) do
    if component.late_init then component:late_init() end
  end

  return entity
end

function tdengine.entity.create(name, data)
  local entity = tdengine.entity.create_anonymous(name, data)
  tdengine.entity.created_entities[entity.id] = entity
  return entity
end

function tdengine.entity.destroy(id)
  tdengine.entity.destroyed_entities[id] = true
end

function tdengine.entity.copy(entity)
	local copied_entity = tdengine.serialize_entity(entity)

	if #copied_entity.tag > 0 then
	  copied_entity.tag = string.format('%s_Copy', copied_entity.tag)
	end

	copied_entity.uuid = tdengine.uuid()
	for component in tdengine.iterator.values(copied_entity.components) do
	  component.uuid = tdengine.uuid()
	end

  return tdengine.entity.create(copied_entity.name, copied_entity)
end

function tdengine.entity.save(entity)
  if not entity.save_fields then return end

  if entity.on_save_game then entity:on_save_game() end
  return tdengine.serialize_entity(entity)
end

function tdengine.entity.load(entity, serialized_entity)
  tdengine.deserialize_entity(entity, serialized_entity)
  if entity.on_load_game then entity:on_load_game() end
end

function tdengine.entity.clear_add_queue()
  tdengine.entity.created_entities = {}
end

function tdengine.entity.find(name)
  for entity in tdengine.entity.iterate() do
    if entity.name == name then
      return entity
      end
  end

  for entity in tdengine.entity.iterate_persistent() do
    if entity.name == name then
      return entity
      end
  end
end


--------------------
-- ENTITY UPDATES --
--------------------
function tdengine.entity.process_destruction()
  for id, _ in pairs(tdengine.entity.destroyed_entities) do
    local entity = tdengine.entity.entities[id]
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.deinit)
    tdengine.entity.entities[id] = nil
  end

  table.clear(tdengine.entity.destroyed_entities)
end

function tdengine.entity.process_addition()
  while true do
    if table.empty(tdengine.entity.created_entities) then
      break
    end
  
    for id, entity in pairs(tdengine.entity.created_entities) do
      tdengine.entity.entities[id] = entity
    end

    local added = table.shallow_copy(tdengine.entity.created_entities)
    table.clear(tdengine.entity.created_entities)

    for _, entity in pairs(added) do
      if entity.late_init then entity:late_init() end
    end

  end
end

function tdengine.entity.update()
  tdengine.entity.process_destruction()
  tdengine.entity.process_addition()
  
  if tdengine.tick then
    for entity in tdengine.entity.iterate() do
      tdengine.gpus.bind_entity(entity)
      tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.update)
    end

    for entity in tdengine.entity.iterate_persistent() do
      tdengine.gpus.bind_entity(entity)
      tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.update)
    end

    tdengine.physics.update()
    tdengine.interaction.update()
  end

  for entity in tdengine.entity.iterate() do
    tdengine.gpus.bind_entity(entity)
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.draw)
  end

  for entity in tdengine.entity.iterate_persistent() do
    tdengine.gpus.bind_entity(entity)
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.draw)
  end
end







-- bad old stuff >:(
function tdengine.find_active_entities()
  local entities = table.shallow_copy(tdengine.entity.entities)
  return entities
end

function tdengine.find_entity_impl(entities, name, id, tag, uuid)
  local entities = entities or tdengine.entity.entities
  local check_entity = function(entity)
    if name and entity.name == name then return true end
    if id and entity.id == id then return true end
    if tag and entity.tag == tag then return true end
    if uuid and entity.uuid == uuid then return true end
    return false
  end

  for id, entity in pairs(entities) do
    if tdengine.entity.destroyed_entities[id] then goto continue end
    if check_entity(entity) then return entity end
    ::continue::
  end

  for id, entity in pairs(tdengine.entity.created_entities) do
    if tdengine.entity.destroyed_entities[id] then goto continue end
    if check_entity(entity) then return entity end
    ::continue::
  end

  return nil
end

function tdengine.find_entity(name)
  return tdengine.find_entity_impl(tdengine.find_active_entities(), name, nil, nil, nil)
end

function tdengine.find_entity_by_id(id)
  return tdengine.find_entity_impl(tdengine.find_active_entities(), nil, id, nil, nil)
end

function tdengine.find_entity_by_tag(tag)
  return tdengine.find_entity_impl(tdengine.find_active_entities(), nil, nil, tag, nil)
end

function tdengine.find_entity_by_uuid(uuid)
  return tdengine.find_entity_impl(tdengine.find_active_entities(), nil, nil, nil, uuid)
end

function tdengine.iterate_entities(name)
  local function iterator()
    for id, entity in pairs(tdengine.find_active_entities()) do
      if not tdengine.entity.destroyed_entities[id] then
        if not name or entity.name == name then
          coroutine.yield(entity)
        end
      end
    end
  end

  return coroutine.wrap(iterator)
end

function tdengine.find_entities(name)
  local found = {}
  for entity in tdengine.iterate_entities(name) do
    table.insert(found, entity)
  end

  return found
end

function tdengine.find_entity_staged(name)
  return tdengine.find_entity_impl(tdengine.entity.created_entities, name, nil, nil, nil)
end

function tdengine.find_entity_by_id_staged(id)
  return tdengine.find_entity_impl(tdengine.entity.created_entities, nil, id, nil, nil)
end

function tdengine.find_entity_by_tag_staged(tag)
  return tdengine.find_entity_impl(tdengine.entity.created_entities, nil, nil, tag, nil)
end

function tdengine.find_entity_by_uuid_staged(uuid)
  return tdengine.find_entity_impl(tdengine.entity.created_entities, nil, nil, nil, uuid)
end

function tdengine.iterate_entities_staged(name)
  local function iterator()
    for id, entity in pairs(tdengine.entity.created_entities) do
      if not tdengine.entity.destroyed_entities[id] then
        if not name or entity.name == name then
          coroutine.yield(entity)
        end
      end
    end
  end

  return coroutine.wrap(iterator)
end

function tdengine.find_entities_staged(name)
  local found = {}
  for entity in tdengine.iterate_entities_staged(name) do
    table.insert(found, entity)
  end

  return found
end

function tdengine.find_entity_editor(name)
  for index, entity in pairs(tdengine.editor.entities) do
    if entity:is_instance_of(name) then return entity end
  end
end

function tdengine.find_entity_by_id_editor(id)
  return tdengine.find_entity_impl(tdengine.editor.entities, nil, id, nil, nil)
end

function tdengine.find_entity_by_tag_editor(tag)
  return tdengine.find_entity_impl(tdengine.editor.entities, nil, nil, tag, nil)
end

function tdengine.find_entity_by_uuid_editor(uuid)
  return tdengine.find_entity_impl(tdengine.editor.entities, nil, nil, nil, uuid)
end

function tdengine.find_all_components(name)
  local components = {}

  for id, entity in pairs(tdengine.entity.entities) do
    local component = entity:find_component(name)
    if component then table.insert(components, component) end
  end

  return components
end
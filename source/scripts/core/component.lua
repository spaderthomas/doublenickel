function tdengine.component.define(name)
  local class = tdengine.class.define(name)
  class:include_update()
  class:set_field_metadata('id', FieldMetadata.Presets.ReadOnly)
  class:set_field_metadata('uuid', FieldMetadata.Presets.ReadOnly)

  tdengine.component.types[name] = class

  return class
end

function tdengine.component.iterate(name)
  local iterator = function()
    for entity in tdengine.entity.iterate() do
      local component = entity:find_component(name)
      if component then
        coroutine.yield(component)
      end
    end
  end

  return coroutine.wrap(iterator)
end

function tdengine.component.collect(name) 
  local components = tdengine.data_types.Array:new()
  for component in tdengine.component.iterate(name) do
    components:add(component)
  end

  return components
end
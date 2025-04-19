function dn.component.define(name)
  local class = dn.class.define(name)
  class:include_update()
  class:set_field_metadata('id', FieldMetadata.Presets.ReadOnly)
  class:set_field_metadata('uuid', FieldMetadata.Presets.ReadOnly)

  dn.component.types[name] = class

  return class
end

function dn.component.iterate(name)
  local iterator = function()
    for entity in dn.entity.iterate() do
      local component = entity:find_component(name)
      if component then
        coroutine.yield(component)
      end
    end
  end

  return coroutine.wrap(iterator)
end

function dn.component.collect(name) 
  local components = dn.data_types.Array:new()
  for component in dn.component.iterate(name) do
    components:add(component)
  end

  return components
end
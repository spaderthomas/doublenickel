function doublenickel.component.define(name)
  local class = doublenickel.class.define(name)
  class:include_update()
  class:set_field_metadata('id', FieldMetadata.Presets.ReadOnly)
  class:set_field_metadata('uuid', FieldMetadata.Presets.ReadOnly)

  doublenickel.component.types[name] = class

  return class
end

function doublenickel.component.iterate(name)
  local iterator = function()
    for entity in doublenickel.entity.iterate() do
      local component = entity:find_component(name)
      if component then
        coroutine.yield(component)
      end
    end
  end

  return coroutine.wrap(iterator)
end

function doublenickel.component.collect(name) 
  local components = doublenickel.data_types.Array:new()
  for component in doublenickel.component.iterate(name) do
    components:add(component)
  end

  return components
end
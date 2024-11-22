function tdengine.persistent.init()
  for entity in tdengine.entity.iterate_persistent() do
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.stop)
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.deinit)
  end

  table.clear(tdengine.entity.persistent_entities)

  local scene = tdengine.scene.read('persistent')
  for _, entity_data in pairs(scene) do
    local entity = tdengine.entity.create_anonymous(entity_data.name, entity_data)
    tdengine.entity.persistent_entities[entity.id] = entity
  end

  for entity in tdengine.entity.iterate_persistent() do
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.late_init)
    tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.play)
  end
end

function tdengine.persistent.write()
  tdengine.scene.write(tdengine.entity.persistent_entities, 'persistent')
end
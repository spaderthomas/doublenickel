function doublenickel.persistent.init()
  doublenickel.dn_log('doublenickel.persistent.init')
  for entity in doublenickel.entity.iterate_persistent() do
    doublenickel.entity.run_update_callback(entity, doublenickel.lifecycle.update_callbacks.stop)
    doublenickel.entity.run_update_callback(entity, doublenickel.lifecycle.update_callbacks.deinit)
  end

  table.clear(doublenickel.entity.persistent_entities)

  local scene = doublenickel.scene.read('persistent')
  for _, entity_data in pairs(scene) do
    local entity = doublenickel.entity.create_anonymous(entity_data.name, entity_data)
    doublenickel.entity.persistent_entities[entity.id] = entity
  end

  for entity in doublenickel.entity.iterate_persistent() do
    doublenickel.entity.run_update_callback(entity, doublenickel.lifecycle.update_callbacks.late_init)
    doublenickel.entity.run_update_callback(entity, doublenickel.lifecycle.update_callbacks.play)
  end
end

function doublenickel.persistent.write()
  doublenickel.scene.write(doublenickel.entity.persistent_entities, 'persistent')
end
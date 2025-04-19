function dn.persistent.init()
  dn.ffi.log('dn.persistent.init')
  for entity in dn.entity.iterate_persistent() do
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.stop)
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.deinit)
  end

  table.clear(dn.entity.persistent_entities)

  local scene = dn.scene.read('persistent')
  for _, entity_data in pairs(scene) do
    local entity = dn.entity.create_anonymous(entity_data.name, entity_data)
    dn.entity.persistent_entities[entity.id] = entity
  end

  for entity in dn.entity.iterate_persistent() do
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.late_init)
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.play)
  end
end

function dn.persistent.write()
  dn.scene.write(dn.entity.persistent_entities, 'persistent')
end
local self = dn.scene

function dn.scene.init()
  self.snapshots = {}
  self.internal = {}
end

function dn.scene.configure(config)
  self.config = config

  if config.directory then
    dn.ffi.paths_add_ex(dn.paths.USER_SCENES, config.directory)
    dn.ffi.paths_add_ex(dn.paths.USER_SCENE, config.directory .. '/%s.lua')
  end

  if config.persistent then
    self.reload_persistent_scene()
  end
end

function dn.scene.update()
  if self.internal.queued_scene then
    self.internal.current_scene = self.internal.queued_scene
    self.internal.queued_scene = nil

    for entity in dn.entity.iterate() do
      dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.stop)
    end

    for entity in dn.entity.iterate() do
      dn.entity.destroy(entity.id)
    end

    for entity in dn.entity.iterate_staged() do
      dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.play)
    end
  end

  if self.internal.next_tick ~= dn.tick then
    dn.tick = self.internal.next_tick

    local callback = dn.tick and dn.lifecycle.callbacks.on_editor_play or dn.lifecycle.callbacks.on_editor_stop
    dn.lifecycle.run_callback(callback)
  end
end

function dn.scene.find(file_name)
  if dn.ffi.paths_is_registered(dn.paths.USER_SCENE) then
    local file_path = dn.ffi.paths_resolve_format(dn.paths.USER_SCENE, file_name)

    if dn.ffi.os_does_path_exist(file_path) then
      return file_path
    end
  end

  return dn.ffi.paths_resolve_format(dn.paths.SCENE, file_name)
end

function dn.scene.read(file_name)
  return dn.module.read(dn.scene.find(file_name))
end

function dn.scene.write(scene, file_name)
  local serialized_entities = {}
  for _, entity in pairs(scene) do
    local serialized_entity = dn.serialize_entity(entity)
    serialized_entities[entity.uuid] = serialized_entity
  end
  
  dn.module.write(dn.scene.find(file_name), serialized_entities, dn.module.WriteOptions.Pretty)
end

function dn.scene.load(scene_name)
  self.internal.queued_scene = scene_name

  dn.entity.clear_add_queue()

  local scene = dn.scene.read(scene_name)
  for _, entity_data in pairs(scene) do
    dn.entity.create(entity_data.name, entity_data)
  end
end

function dn.scene.set_tick(next_tick)
  self.internal.next_tick = next_tick
end

function dn.scene.populate_snapshots(save)
end

function dn.scene.apply_snapshot()
end

function dn.scene.find_snapshot(scene)
  if not self.snapshots[scene] then
    self.snapshots[scene] = {}
  end

  return self.snapshots[scene]
end

function dn.scene.reload_persistent_scene()
  dn.ffi.trace('dn.scene.reload_persistent_scene')

  for entity in dn.entity.iterate_persistent() do
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.stop)
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.deinit)
  end

  table.clear(dn.entity.persistent_entities)

  local scene = dn.scene.read(dn.scene.config.persistent)
  for _, entity_data in pairs(scene) do
    local entity = dn.entity.create_anonymous(entity_data.name, entity_data)
    dn.entity.persistent_entities[entity.id] = entity
  end

  for entity in dn.entity.iterate_persistent() do
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.late_init)
    dn.entity.run_update_callback(entity, dn.lifecycle.update_callbacks.play)
  end
end

function dn.scene.save_persistent_entities()
  dn.scene.write(dn.entity.persistent_entities, dn.scene.config.persistent)
end
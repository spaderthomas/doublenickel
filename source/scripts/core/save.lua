function dn.save.init()
  dn.ffi.log('dn.save.init')
  local directory = dn.ffi.paths_resolve('saves')
  -- log.info('Initializing save directory; directory = %s', directory)
  dn.ffi.log('Initializing save directory; directory = %s', directory)

  dn.ffi.os_create_directory(dn.ffi.paths_resolve('saves'))
  dn.ffi.os_create_directory(dn.ffi.paths_resolve('screenshots'))
end

function dn.save.create()
  save_name = os.date('%Y-%m-%d-%H-%M-%S', os.time())

  local save = {
    date = os.date('*t'),
    state = dn.state.data,
    scene = dn.current_scene,
  }

  local file_path = dn.ffi.paths_resolve_format('save', save_name)
  dn.module.write(file_path, save, dn.module.WriteOptions.Pretty)

  dn.ffi.log(string.format('Created save file; file_path = %s', file_path))
end

function dn.save.read(file_name)
  file_name = dn.strip_extension(file_name)
  local file_path = dn.ffi.paths_resolve_format('save', file_name)
  return dn.module.read(file_path)
end

function dn.save.list()
  local saves = dn.filesystem.collect_named_directory('saves')
  for index in dn.iterator.keys(saves) do
    saves[index] = dn.save.read(saves[index])
  end

  return saves
end

function dn.save.count()
  local save_dir = dn.ffi.paths_resolve('saves')
  -- local entries = dn.ffi.os_scan_directory(save_dir)
  local entries = dn.ffi.os_scan_directory(save_dir)
  return entries.count
end

function dn.save.get_save_name(data)
  return os.date('%Y-%m-%d-%H-%M-%S', os.time(data.date))
end

function dn.save.get_display_name(data)
  return os.date('%B %d, %Y at %H:%M:%S', os.time(data.date))
end

function dn.save.get_screenshot_file(data)
  return string.format('%s.png', dn.save.get_save_name(data))
end



local self = dn.scene

function dn.scene.init()
  self.snapshots = {}
  self.internal = {}
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

function dn.scene.read(file_name)
  local file_path = dn.ffi.paths_resolve_format('scene', file_name)
  return dn.module.read(file_path)
end

function dn.scene.write(scene, file_name)
  local serialized_entities = {}
  for _, entity in pairs(scene) do
    local serialized_entity = dn.serialize_entity(entity)
    serialized_entities[entity.uuid] = serialized_entity
  end

  local file_path = dn.ffi.paths_resolve_format('scene', file_name)
  dn.module.write(file_path, serialized_entities, dn.module.WriteOptions.Pretty)
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

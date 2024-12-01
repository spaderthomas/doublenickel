function doublenickel.save.init()
  doublenickel.dn_log('doublenickel.save.init')
  local directory = dn.paths_resolve('saves'):to_interned()
  -- log.info('Initializing save directory; directory = %s', directory)
  doublenickel.dn_log('Initializing save directory; directory = %s', directory)

  dn.os_create_directory(dn.paths_resolve('saves'):to_interned())
  dn.os_create_directory(dn.paths_resolve('screenshots'):to_interned())
end

function doublenickel.save.create()
  save_name = os.date('%Y-%m-%d-%H-%M-%S', os.time())

  local save = {
    date = os.date('*t'),
    state = doublenickel.state.data,
    scene = doublenickel.current_scene,
  }

  local file_path = dn.paths_resolve_format('save', save_name):to_interned()
  doublenickel.module.write(file_path, save, doublenickel.module.WriteOptions.Pretty)

  doublenickel.dn_log(string.format('Created save file; file_path = %s', file_path))
end

function doublenickel.save.read(file_name)
  file_name = doublenickel.strip_extension(file_name)
  local file_path = dn.paths_resolve_format('save', file_name):to_interned()
  return doublenickel.module.read(file_path)
end

function doublenickel.save.list()
  local saves = doublenickel.filesystem.collect_named_directory('saves')
  for index in doublenickel.iterator.keys(saves) do
    saves[index] = doublenickel.save.read(saves[index])
  end

  return saves
end

function doublenickel.save.count()
  local save_dir = dn.paths_resolve('saves'):to_interned()
  -- local entries = dn.os_scan_directory(save_dir)
  local entries = dn.os_scan_directory(save_dir)
  return entries.count
end

function doublenickel.save.get_save_name(data)
  return os.date('%Y-%m-%d-%H-%M-%S', os.time(data.date))
end

function doublenickel.save.get_display_name(data)
  return os.date('%B %d, %Y at %H:%M:%S', os.time(data.date))
end

function doublenickel.save.get_screenshot_file(data)
  return string.format('%s.png', doublenickel.save.get_save_name(data))
end



local self = doublenickel.scene

function doublenickel.scene.init()
  self.snapshots = {}
  self.internal = {}
end




function doublenickel.scene.update()
  if self.internal.queued_scene then
    self.internal.current_scene = self.internal.queued_scene
    self.internal.queued_scene = nil

    for entity in doublenickel.entity.iterate() do
      doublenickel.entity.run_update_callback(entity, doublenickel.lifecycle.update_callbacks.stop)
    end

    for entity in doublenickel.entity.iterate() do
      doublenickel.entity.destroy(entity.id)
    end

    for entity in doublenickel.entity.iterate_staged() do
      doublenickel.entity.run_update_callback(entity, doublenickel.lifecycle.update_callbacks.play)
    end
  end

  if self.internal.next_tick ~= doublenickel.tick then
    doublenickel.tick = self.internal.next_tick

    local callback = doublenickel.tick and doublenickel.lifecycle.callbacks.on_editor_play or doublenickel.lifecycle.callbacks.on_editor_stop
    doublenickel.lifecycle.run_callback(callback)
  end
end

function doublenickel.scene.read(file_name)
  local file_path = dn.paths_resolve_format('scene', file_name):to_interned()
  return doublenickel.module.read(file_path)
end

function doublenickel.scene.write(scene, file_name)
  local serialized_entities = {}
  for _, entity in pairs(scene) do
    local serialized_entity = doublenickel.serialize_entity(entity)
    serialized_entities[entity.uuid] = serialized_entity
  end

  local file_path = dn.paths_resolve_format('scene', file_name):to_interned()
  doublenickel.module.write(file_path, serialized_entities, doublenickel.module.WriteOptions.Pretty)
end



function doublenickel.scene.load(scene_name)
  self.internal.queued_scene = scene_name

  doublenickel.entity.clear_add_queue()

  local scene = doublenickel.scene.read(scene_name)
  for _, entity_data in pairs(scene) do
    doublenickel.entity.create(entity_data.name, entity_data)
  end
end


function doublenickel.scene.set_tick(next_tick)
  self.internal.next_tick = next_tick
end



function doublenickel.scene.populate_snapshots(save)
end

function doublenickel.scene.apply_snapshot()
end

function doublenickel.scene.find_snapshot(scene)
  if not self.snapshots[scene] then
    self.snapshots[scene] = {}
  end

  return self.snapshots[scene]
end

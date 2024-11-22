function tdengine.save.init()
  local directory = tdengine.ffi.resolve_named_path('saves'):to_interned()
  log.info('Initializing save directory; directory = %s', directory)

  tdengine.ffi.create_named_directory('saves')
  tdengine.ffi.create_named_directory('screenshots')
end

function tdengine.save.create()
  save_name = os.date('%Y-%m-%d-%H-%M-%S', os.time())

  local save = {
    date = os.date('*t'),
    state = tdengine.state.data,
    scene = tdengine.current_scene,
  }

  local file_path = tdengine.ffi.resolve_format_path('save', save_name):to_interned()
  tdengine.module.write(file_path, save, tdengine.module.WriteOptions.Pretty)

  tdengine.log(string.format('Created save file; file_path = %s', file_path))
end

function tdengine.save.read(file_name)
  file_name = tdengine.strip_extension(file_name)
  local file_path = tdengine.ffi.resolve_format_path('save', file_name):to_interned()
  return tdengine.module.read(file_path)
end

function tdengine.save.list()
  local saves = {}

  local save_dir = tdengine.ffi.resolve_named_path('saves'):to_interned()
  local files = tdengine.scandir(save_dir)
  for index, file_name in pairs(files) do
    table.insert(saves, tdengine.save.open(file_name))
  end

  return saves
end

function tdengine.save.count()
  local save_dir = tdengine.ffi.resolve_named_path('saves'):to_interned()
  local files = tdengine.scandir(save_dir)
  return #files
end

function tdengine.save.get_save_name(data)
  return os.date('%Y-%m-%d-%H-%M-%S', os.time(data.date))
end

function tdengine.save.get_display_name(data)
  return os.date('%B %d, %Y at %H:%M:%S', os.time(data.date))
end

function tdengine.save.get_screenshot_file(data)
  return string.format('%s.png', tdengine.save.get_save_name(data))
end



local self = tdengine.scene

function tdengine.scene.init()
  self.snapshots = {}
  self.internal = {}
end




function tdengine.scene.update()
  if self.internal.queued_scene then
    self.internal.current_scene = self.internal.queued_scene
    self.internal.queued_scene = nil

    for entity in tdengine.entity.iterate() do
      tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.stop)
    end

    for entity in tdengine.entity.iterate() do
      tdengine.entity.destroy(entity.id)
    end

    for entity in tdengine.entity.iterate_staged() do
      tdengine.entity.run_update_callback(entity, tdengine.lifecycle.update_callbacks.play)
    end
  end

  if self.internal.next_tick ~= tdengine.tick then
    tdengine.tick = self.internal.next_tick

    local callback = tdengine.tick and tdengine.lifecycle.callbacks.on_editor_play or tdengine.lifecycle.callbacks.on_editor_stop
    tdengine.lifecycle.run_callback(callback)
  end
end

function tdengine.scene.read(file_name)
  local file_path = tdengine.ffi.resolve_format_path('scene', file_name):to_interned()
  return tdengine.module.read(file_path)
end

function tdengine.scene.write(scene, file_name)
  local serialized_entities = {}
  for _, entity in pairs(scene) do
    local serialized_entity = tdengine.serialize_entity(entity)
    serialized_entities[entity.uuid] = serialized_entity
  end

  local file_path = tdengine.ffi.resolve_format_path('scene', file_name):to_interned()
  tdengine.module.write(file_path, serialized_entities, tdengine.module.WriteOptions.Pretty)
end



function tdengine.scene.load(scene_name)
  self.internal.queued_scene = scene_name

  tdengine.entity.clear_add_queue()

  local scene = tdengine.scene.read(scene_name)
  for _, entity_data in pairs(scene) do
    tdengine.entity.create(entity_data.name, entity_data)
  end
end


function tdengine.scene.set_tick(next_tick)
  self.internal.next_tick = next_tick
end



function tdengine.scene.populate_snapshots(save)
end

function tdengine.scene.apply_snapshot()
end

function tdengine.scene.find_snapshot(scene)
  if not self.snapshots[scene] then
    self.snapshots[scene] = {}
  end

  return self.snapshots[scene]
end

function dn.save.init()
end

function dn.save.configure(config)
  if not config then
    dn.ffi.warn('dn.save.configure', 'No configuration was provided for save files')
    return
  end

  if config.directory then
    dn.ffi.paths_add_ex(dn.paths.USER_SAVES, config.directory)
    dn.ffi.paths_add_ex(dn.paths.USER_SAVE, config.directory .. '/%s.lua')

    dn.ffi.os_create_directory(dn.ffi.paths_resolve(dn.paths.USER_SAVES))
  end
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
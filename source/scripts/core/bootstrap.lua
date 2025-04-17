ffi = require('ffi')
bit = require('bit')

function doublenickel.handle_error()
  local stack_trace = debug.traceback()
  stack_trace = stack_trace:gsub('stack traceback:\n', '')
  stack_trace = stack_trace:gsub('\t', '  ')

  -- The stack trace contains absolute paths, which are just hard to read. Also, if the path is long, it is
  -- shortened with "...". Remove the absolute part of the path, including the "..."
  local install_dir = dn.paths_resolve('dn_install')
  local escaped_install = install_dir:gsub('%.', '%%.')
  local last_path_element = install_dir:match("([^/]+)$")
  local pattern = '%.%.%.(.*)/' .. last_path_element

  -- Replace the full path first
  stack_trace = stack_trace:gsub(escaped_install, '')

  -- Then replace any possible shortened versions with ...
  local shortened_path_pattern = '[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+'
  stack_trace = stack_trace:gsub(pattern, '')

  local trace_message = string.format('stack trace:\n%s', stack_trace)
  dn.log(trace_message)

  doublenickel.debug.open_debugger(1)
end

function doublenickel.init_phase_0()
  doublenickel.types = {}
  doublenickel.class = {}
  doublenickel.lifecycle = {}

  doublenickel.entity = {}
  doublenickel.entity.entities = {}
  doublenickel.entity.created_entities = {}
  doublenickel.entity.destroyed_entities = {}
  doublenickel.entity.persistent_entities = {}
  doublenickel.entity.types = {}
  doublenickel.entity.next_id = 1
  doublenickel.persistent = {}

  doublenickel.component = {}
  doublenickel.component.types = {}

  doublenickel.internal = {}
  doublenickel.internal.enum_metatable = {}

  doublenickel.debug = {}

  doublenickel.constants = {}
  doublenickel.enum = {}
  doublenickel.enums = {}
  doublenickel.enum_data = {}

  doublenickel.editor = {}
  doublenickel.editor.types = {}
  doublenickel.editor.sentinel = '__editor'

  doublenickel.save = {}

  doublenickel.state = {}
  doublenickel.state.data = {}

  doublenickel.path_constants = {}

  doublenickel.quests = {}

  doublenickel.scene = {}
  doublenickel.scene.save_data = {}
  doublenickel.current_scene = nil
  doublenickel.queued_scene = nil

  doublenickel.callback = {}
  doublenickel.callback.data = {}

  doublenickel.data_types = {}

  doublenickel.dialogue = {}
  doublenickel.dialogue.node_type = {}
  doublenickel.dialogue.node_kind = {}
  doublenickel.dialogue.sorted_node_kinds = {}
  doublenickel.dialogue.metrics = {
    words = 0,
    nodes = 0,
    dialogues = {}
  }
  doublenickel.dialogue.cache = {}
  doublenickel.dialogue.characters = {}

  doublenickel.audio = {}

  doublenickel.animation = {}
  doublenickel.animation.data = {}

  doublenickel.texture = {}
  doublenickel.texture.data = {}

  doublenickel.background = {}
  doublenickel.background.data = {}

  doublenickel.input = {}
  doublenickel.input.data = {}

  doublenickel.physics = {}
  doublenickel.physics.requests = {}
  doublenickel.physics.debug = false

  doublenickel.interaction = {}
  doublenickel.interaction.callbacks = {}
  doublenickel.interaction.check_flag = false

  doublenickel.interpolation = {}

  doublenickel.gui = {}
  doublenickel.gui.animation = {}
  doublenickel.gui.scroll = {}
  doublenickel.gui.drag = {}
  doublenickel.gui.menu = {}

  doublenickel.window = {}

  doublenickel.action = {}
  doublenickel.action.event_kind = {}

  doublenickel.analytics = {}

  doublenickel.fonts = {}

  doublenickel.module = {}

  doublenickel.paths = {}

  doublenickel.shaders = {}

  doublenickel.gpu = {}

  doublenickel.app = {}

  doublenickel.iterator = {}

  doublenickel.time_metric = {}

  doublenickel.subsystem = {}
  doublenickel.subsystem.types = {}

  doublenickel.math = {}

  doublenickel.filesystem = {}

  doublenickel.asset = {}

  doublenickel.draw = {}
  doublenickel.draw.internal = {}

  doublenickel.dt = 0
  doublenickel.elapsed_time = 0
  doublenickel.frame = 0

  doublenickel.tick = doublenickel.is_packaged_build
  doublenickel.next_tick = doublenickel.tick

  imgui = {}
  imgui.extensions = {}
  imgui.internal = {}

  doublenickel.ffi = {}
  dn = {}


  -- Bootstrap the FFI, so that we can resolve paths
  ffi.cdef([[const char* dn_paths_resolve_bootstrap(const char* name);]])
  local file_path = ffi.string(ffi.C.dn_paths_resolve_bootstrap('dn_ffi_h'))
  local dn_ffi_h, err = io.open(file_path)
  if not dn_ffi_h then
    print(string.format('Failed to load dn.h from %s', file_path))
    print(err)
    return
  end
  ffi.cdef(dn_ffi_h:read('*a'))
  
  -- Run a sanity check on the compiled size of all types and what the parser inside LuaJIT generates.
  -- This is nothing more than a sanity check; i.e. we don't do anything besides bail violently if
  -- we see a mismatch.
  --
  -- The only reason this is here is because I *have* seen MSVC generate a different size than what 
  -- the FFI reports. See the types in the header which are force aligned to 16 bytes.
  local type_mismatch = false
  local type_infos = ffi.C.dn_app_query_types()
  for i = 0, type_infos.count - 1 do
    local type_info = type_infos.data[i]
    local ffi_size = ffi.sizeof(ffi.string(type_info.name))
    
    if ffi_size ~= type_info.size then
      type_mismatch = true
      print(string.format(
        'Found type mismatch; type = %s, ffi_size = %d, compiled_size = %d',
        ffi.string(type_info.name),
        ffi_size, type_info.size)
      )
    end
  end
  if type_mismatch then 
    ffi.C.dn_app_set_exit_game() -- Technically useless, since the game will explode without bootstrapping
  end

  -- Bootstrap the engine paths, so we can load the rest of the scripts
  local function collect_paths(paths, full_parent)
    local collected_paths = {}
    if not paths then return collected_paths end
    
    full_parent = full_parent or ''

    for name, data in pairs(paths) do
      local full_path = ''
      if type(data) == 'string' then
        full_path = full_parent .. data
        goto done
      else 
        full_path = full_parent .. data.path
      end
  
      if data.children then
        local child_paths = collect_paths(data.children, full_path .. '/')
        for index, path in pairs(child_paths) do
          table.insert(collected_paths, path)
        end
      end

      ::done::
      table.insert(collected_paths, {
        name = name,
        path = full_path
      })
    end
  
    return collected_paths
  end
  local file_path = ffi.C.dn_paths_resolve_cstr('dn_paths')
  local file_path = ffi.string(file_path.data, file_path.len)
  local path_info = dofile(file_path)

  local dn_install_paths = collect_paths(path_info.dn_install)
  for index, path in pairs(dn_install_paths) do
    ffi.C.dn_paths_add_engine_subpath(
      ffi.new('dn_string_t', #path.name, ffi.cast('u8*', path.name)),
      ffi.new('dn_string_t', #path.path, ffi.cast('u8*', path.path))
    )
  end

  local app_paths = collect_paths(path_info.app_paths)
  for index, path in pairs(app_paths) do
    ffi.C.dn_paths_add_app_subpath(
      ffi.new('dn_string_t', #path.name, ffi.cast('u8*', path.name)),
      ffi.new('dn_string_t', #path.path, ffi.cast('u8*', path.path))
    )
  end

  local write_paths = collect_paths(path_info.write_paths)
  for index, path in pairs(write_paths) do
    ffi.C.dn_paths_add_write_subpath(
      ffi.new('dn_string_t', #path.name, ffi.cast('u8*', path.name)),
      ffi.new('dn_string_t', #path.path, ffi.cast('u8*', path.path))
    )
  end

  -- We need a couple of files to even be able to load other files (since they
  -- define classes and enums and such). Order matters here. I consider this to be
  -- the Lua equivalent of the ordered includes in main.cpp
  --
  -- We technically load these files twice, but all files are idempotent...
  local loader = {
    'reflect.lua',
    'enum.lua',
    'class.lua',
    'math.lua'
  }

  for _, file_name in pairs(loader) do
    local file_path = ffi.C.dn_paths_resolve_format_cstr('engine_script', file_name)
    dofile(ffi.string(file_path.data, file_path.len))
  end


end

function doublenickel.init_phase_1()
  doublenickel.enum.init()
  doublenickel.ffi.init()
  doublenickel.paths.init()
  doublenickel.math.init()
  doublenickel.time_metric.init()
  doublenickel.input.init()
  doublenickel.gpu.init()
  doublenickel.state.init()
  doublenickel.animation.load()
  doublenickel.texture.load()
  doublenickel.background.load()
  doublenickel.dialogue.init()
  -- doublenickel.audio.init()
  -- doublenickel.gui.init()
  doublenickel.scene.init()
  doublenickel.asset.init()
end

function doublenickel.init_phase_2()
  doublenickel.subsystem.init()
  doublenickel.app = doublenickel.subsystem.find('App')

  doublenickel.dn_log('App:on_init_game')
  doublenickel.app:on_init_game()

  doublenickel.window.init()
  -- doublenickel.save.init()
  doublenickel.editor.init()
  doublenickel.persistent.init()

  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_start_game)
end

function doublenickel.dn_log(...)
  print(string.format(...))
end
ffi = require('ffi')
bit = require('bit')

function dn.handle_error()
  local stack_trace = debug.traceback()
  stack_trace = stack_trace:gsub('stack traceback:\n', '')
  stack_trace = stack_trace:gsub('\t', '  ')

  -- The stack trace contains absolute paths, which are just hard to read. Also, if the path is long, it is
  -- shortened with "...". Remove the absolute part of the path, including the "..."
  local install_dir = dn.ffi.paths_resolve('dn_install')
  local escaped_install = install_dir:gsub('%.', '%%.')
  local last_path_element = install_dir:match("([^/]+)$")
  local pattern = '%.%.%.(.*)/' .. last_path_element

  -- Replace the full path first
  stack_trace = stack_trace:gsub(escaped_install, '')

  -- Then replace any possible shortened versions with ...
  local shortened_path_pattern = '[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+'
  stack_trace = stack_trace:gsub(pattern, '')

  local trace_message = string.format('stack trace:\n%s', stack_trace)
  dn.ffi.log(trace_message)

  dn.debug.open_debugger(1)
end

function dn.init_phase_0()
  dn.types = {}
  dn.class = {}
  dn.lifecycle = {}

  dn.entity = {}
  dn.entity.entities = {}
  dn.entity.created_entities = {}
  dn.entity.destroyed_entities = {}
  dn.entity.persistent_entities = {}
  dn.entity.types = {}
  dn.entity.next_id = 1

  dn.component = {}
  dn.component.types = {}

  dn.internal = {}
  dn.internal.enum_metatable = {}

  dn.debug = {}

  dn.constants = {}
  dn.enum = {}
  dn.enums = {}
  dn.enum_data = {}

  dn.editor = {}
  dn.editor.types = {}
  dn.editor.sentinel = '__editor'

  dn.save = {}

  dn.state = {}
  dn.state.data = {}

  dn.path_constants = {}

  dn.quests = {}

  dn.scene = {}
  dn.scene.save_data = {}
  dn.current_scene = nil
  dn.queued_scene = nil

  dn.callback = {}
  dn.callback.data = {}

  dn.data_types = {}

  dn.dialogue = {}
  dn.dialogue.node_type = {}
  dn.dialogue.node_kind = {}
  dn.dialogue.sorted_node_kinds = {}
  dn.dialogue.metrics = {
    words = 0,
    nodes = 0,
    dialogues = {}
  }
  dn.dialogue.cache = {}
  dn.dialogue.characters = {}

  dn.audio = {}

  dn.animation = {}
  dn.animation.data = {}

  dn.texture = {}
  dn.texture.data = {}

  dn.background = {}
  dn.background.data = {}

  dn.input = {}
  dn.input.data = {}

  dn.physics = {}
  dn.physics.requests = {}
  dn.physics.debug = false

  dn.interaction = {}
  dn.interaction.callbacks = {}
  dn.interaction.check_flag = false

  dn.interpolation = {}

  dn.gui = {}
  dn.gui.animation = {}
  dn.gui.scroll = {}
  dn.gui.drag = {}
  dn.gui.menu = {}

  dn.window = {}

  dn.action = {}
  dn.action.event_kind = {}

  dn.analytics = {}

  dn.fonts = {}

  dn.module = {}

  dn.paths = {}

  dn.shaders = {}

  dn.gpu = {}

  dn.app = {}

  dn.iterator = {}

  dn.time_metric = {}

  dn.subsystem = {}
  dn.subsystem.types = {}

  dn.math = {}

  dn.filesystem = {}

  dn.asset = {}

  dn.draw = {}
  dn.draw.internal = {}

  dn.layout = {}

  dn.dt = 0
  dn.elapsed_time = 0
  dn.frame = 0

  dn.tick = dn.is_packaged_build
  dn.next_tick = dn.tick

  imgui = {}
  imgui.extensions = {}
  imgui.internal = {}

  dn.ffi = {}
  dn.unported = {}
  dn.reflection = {}
  dn.paths.USER_LAYOUTS = "dn_user_layouts"
  dn.paths.USER_LAYOUT = "dn_user_layout"
  dn.paths.LAYOUTS = "dn_layouts"
  dn.paths.LAYOUT = "dn_layout"
  dn.paths.USER_SCENES = "dn_user_scenes"
  dn.paths.USER_SCENE = "dn_user_scene"
  dn.paths.SCENES = "dn_scenes"
  dn.paths.SCENE = "dn_scene"
  dn.paths.USER_SAVES = "dn_user_saves"
  dn.paths.USER_SAVE = "dn_user_save"

  -- Bootstrap the FFI, so that we can resolve paths
  ffi.cdef([[const char* dn_paths_resolve_bootstrap(const char* name);]])
  local file_path = ffi.string(ffi.C.dn_paths_resolve_bootstrap('dn_ffi_h'))
  local dn_ffi_h, err = io.open(file_path)
  if not dn_ffi_h then
    print(string.format('Failed to load dn.ffi.h from %s', file_path))
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
      assert(false, string.format(
        'Found type mismatch; type = %s, ffi_size = %d, compiled_size = %d',
        ffi.string(type_info.name),
        ffi_size, type_info.size))
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

  local dn_install_paths = collect_paths(path_info.dn_engine)
  for index, path in pairs(dn_install_paths) do
    ffi.C.dn_paths_add_engine_subpath(
      ffi.new('dn_string_t', #path.name, ffi.cast('u8*', path.name)),
      ffi.new('dn_string_t', #path.path, ffi.cast('u8*', path.path))
    )
  end

  local app_paths = collect_paths(path_info.dn_app)
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

function dn.init_phase_1()
  dn.enum.init()
  dn.ffi.init()
  dn.paths.init()
  dn.math.init()
  dn.time_metric.init()
  dn.input.init()
  dn.gpu.init()
  dn.asset.init()
  dn.scene.init()
  dn.save.init()
  dn.layout.init()

  dn.state.init()
  dn.animation.init()
  dn.texture.init()
  dn.background.init()
  dn.dialogue.init()
  -- dn.audio.init()
  dn.gui.init()
  dn.editor.init()
  dn.window.init()
end

function dn.init_phase_2()
  dn.subsystem.init()
  app = dn.subsystem.find('App')
  app:configure()
end
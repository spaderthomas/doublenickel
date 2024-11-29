#ifndef DN_LUA_H
#define DN_LUA_H

#define DEFER_POP(l) defer { lua_pop(l, 1); };

typedef lua_State* dn_lua_interpreter_t;

typedef struct {
  dn_lua_interpreter_t state;
  FileMonitor* file_monitor;
  Array<char*, 32> script_dirs;
} dn_lua_t;
dn_lua_t dn_lua;

DN_IMP void        dn_lua_init();
DN_IMP void        dn_lua_init_game();
DN_IMP void        dn_lua_update();
DN_IMP i32         dn_lua_handle_error(lua_State* l);
DN_IMP void        dn_lua_script_dir(const char* path);
DN_IMP void        dn_lua_script_named_dir(const char* name);
DN_IMP bool        dn_lua_script_file(const char* path);
DN_IMP void        dn_lua_dump_stack();
DN_IMP const char* dn_lua_format_file_load_error(const char* error);
DN_IMP i32         dn_lua_format_file_load_error(dn_lua_interpreter_t lua);
DN_API void        dn_lua_add_dir(const char* directory);
#endif



#ifdef DN_LUA_IMPLEMENTATION
void dn_lua_init() {
    // This is the best way I figured out to check whether LuaJIT was compiled with GC64; this function is 
  // only stubbed out on GC32. On GC64, it'll actually try to use the first parameter (an allocator) and
  // crash since it's a nullptr
  //dn_lua.state = lua_newstate(nullptr, nullptr);
  
  dn_lua.state = luaL_newstate();

  arr_init(&dn_lua.script_dirs);
  
  auto events = DN_FILE_CHANGE_EVENT_ADDED | DN_FILE_CHANGE_EVENT_MODIFIED | DN_FILE_CHANGE_EVENT_REMOVED;
  auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
    // We only watch directories, so there's no need to remove a file from the watch
    // list when it's removed
    if (event->events & DN_FILE_CHANGE_EVENT_REMOVED) return;

    // Any other files we happen to create should be skipped
    if (!path_util::is_lua(event->file_path)) return;
    
    auto stripped_path = dn_paths_strip("install", event->file_path);
    dn_log("Hotloading script: %s", stripped_path);
    dn_lua_script_file(event->file_path);
  };

  dn_lua.file_monitor = dn_file_monitors_add();
  dn_lua.file_monitor->init(on_file_event, events, &dn_lua);

  // Basic Lua bootstrapping. Don't load any game scripts here. This is called before
  // we load all the backend systems, because this populates options that those systems
  // might use. 
  auto l = dn_lua.state;

  luaL_openlibs(l);

  // PHASE BOOTSTRAP:
  // Define the main table where all game data and functions go
  lua_newtable(l);
  lua_setglobal(l, "tdengine");

  // Define a variable so we can ~ifdef in Lua
  lua_getglobal(l, "tdengine");
  DEFER_POP(l);
  lua_pushstring(l, "is_packaged_build");
#if defined(FM_EDITOR)
  lua_pushboolean(l, false);
#else
  lua_pushboolean(l, true);
#endif
  lua_settable(l, -3);

  // PHASE 0:
  // Load the bootstrap file, which creates tables for each module and loads the
  // most basic packages needed to load the rest of the code (e.g. classes and enums)
  auto bootstrap = dn_paths_resolve("bootstrap");
  dn_lua_script_file(bootstrap);

  lua_pushstring(l, "init_phase_0");
  lua_gettable(l, -2);
  auto result = lua_pcall(l, 0, 0, 0);
  if (result) {
    const char* error = lua_tostring(l, -1);
    dn_log("init_phase_0(): error = %s", error);
    exit(0);
  }
  
  // PHASE 1:
  // With the base tables created, we can now do things like define classes
  // and entity types. In this phase, load the core engine packages and then
  // any static engine data
  dn_lua_script_named_dir("engine_libs");
  dn_lua_script_named_dir("engine_core");
  dn_lua_script_named_dir("engine_editor");

  lua_pushstring(l, "init_phase_1");
  lua_gettable(l, -2);
  result = lua_pcall(l, 0, 0, 0);
  if (result) {
    const char* error = lua_tostring(l, -1);
    dn_log("init_phase_1(): error = %s", error);
    exit(0);
  }
}

void dn_lua_init_game() {
  auto l = dn_lua.state;

  // PHASE 2:
  // Lua itself has been initialized, and we've loaded in other assets our scripts
  // may use (shaders, fonts, etc). The last step is to load the game scripts and
  // configure the game itself through Lua
  arr_push(&dn_lua.script_dirs, dn_paths_resolve_ex("engine_components", &dn_allocators.standard));
  arr_push(&dn_lua.script_dirs, dn_paths_resolve_ex("engine_editor", &dn_allocators.standard));
  arr_push(&dn_lua.script_dirs, dn_paths_resolve_ex("engine_entities", &dn_allocators.standard));
  arr_push(&dn_lua.script_dirs, dn_paths_resolve_ex("app", &dn_allocators.standard));

  arr_for(dn_lua.script_dirs, directory) {
    dn_lua_script_dir(*directory);
  }
  
  // All scripts are loaded. We can start the game.
  lua_getglobal(l, "tdengine");
  DEFER_POP(l);
  lua_pushstring(l, "init_phase_2");
  lua_gettable(l, -2);
  auto result = lua_pcall(l, 0, 0, 0);
  if (result) {
    const char* error = lua_tostring(l, -1);
    dn_log("init_phase_2(): error = %s", error);
    exit(0);
  }
}

void dn_lua_update() {
  dn_lua_interpreter_t l = dn_lua.state;
  
  lua_pushcfunction(l, &dn_lua_handle_error);
  DEFER_POP(l);
  
  lua_getglobal(l, "tdengine");
  DEFER_POP(l);
  lua_pushstring(l, "update_game");
  lua_gettable(l, -2);

  lua_pushnumber(l, engine.dt);

  /*
    Call in protected mode. If there is an error in this frame's update, it will call
    our error handler, which will:
    1. Print out the error message from Lua
    2. Print out the stack trace
    3. Open a debugger session.
    
    There's no need to handle the error at the callsite, because the above is pretty much
    everything we need.
  */
  lua_pcall(l, 1, 0, -4);
}

i32 dn_lua_handle_error(dn_lua_interpreter_t l) {
  // Forward to a function in Lua that formats the error and opens the debugger
  const char* error = lua_tostring(l, -1);
  lua_pop(l, 1);
  
  lua_getfield(l, LUA_GLOBALSINDEX, "tdengine");
  lua_getfield(l, -1, "handle_error");

  lua_pushstring(l, error);
  lua_call(l, 1, 1);

  return 0;
}

const char* dn_lua_format_file_load_error(const char* error) {
  static char buffer [2048];
  const char* fmt = "  %s";
  snprintf(&buffer[0], 2048, fmt, error);
  
  return &buffer[0];
}

i32 dn_lua_format_file_load_error_l(dn_lua_interpreter_t l) {
  const char* error = lua_tostring(l, 1);
  error = dn_lua_format_file_load_error(error);
  
  lua_pop(l, 1);
  lua_pushstring(l, error);
  return 1;
}

bool dn_lua_script_file(const char* file_path) {
  if (!path_util::is_lua(file_path)) return true;

  dn_lua_interpreter_t l = dn_lua.state;
  i32 initial_stack_size = lua_gettop(l);

  dn_log_flags(DN_LOG_FLAG_FILE, "scripting file: file_path = %s", file_path);

  lua_pushcfunction(l, &dn_lua_format_file_load_error_l);
  
  bool result = luaL_loadfile(l, file_path);

  // In all error cases, do not return early.
  if (result) {
    // There's a syntax error in the file. Since loadfile doesn't call the
    // function we put on the stack, format the message manually.
    const char* error = lua_tostring(l, -1);
    error = dn_lua_format_file_load_error(error);
    
    dn_log("error scripting file; file = %s", file_path);
    dn_log(error);
    lua_pop(l, 2);
    goto check_stack;
  }
  else {
    // The chunk compiled OK. Run it.
    result = lua_pcall(l, 0, 0, -2);
    
    if (result) {
      // There was a runtime error running the chunk.
      const char* error = lua_tostring(l, -1);
      dn_log("error running file; file = %s", file_path);
      dn_log(error);
      lua_pop(l, 2);
      goto check_stack;
    }

    // The chunk loaded successfully!
    lua_pop(l, 1);
    goto check_stack;
  }

 check_stack:
  i32 final_stack_size = lua_gettop(l);
  assert(initial_stack_size == final_stack_size);
  return !result;
}

void dn_lua_script_named_dir(const char* name) {
  auto path = dn_paths_resolve(name);
  dn_lua_script_dir(path);
}

void dn_lua_script_dir(const char* path) {
  std::error_code error;
  if (!std::filesystem::exists(path, error)) return;

  dn_log_flags(DN_LOG_FLAG_FILE, "Loading scripts from directory; path = %s", path);

  dn_lua.file_monitor->add_directory(path);

  struct DirectoryEntry {
    dn_tstring_t path;
    bool is_directory;
    bool is_regular_file;
    bool occupied = false;
  };
  
  Array<DirectoryEntry, 256> directory_entries;
  arr_init(&directory_entries);
  
  for (auto it = directory_iterator(path); it != directory_iterator(); it++) {
    auto entry = arr_push(&directory_entries);
    entry->occupied = true;

    auto dir_path = it->path().string();
    entry->path = dn_string_copy(dir_path, &dn_allocators.bump);
    normalize_path(entry->path);

    entry->is_regular_file = dn_os_is_regular_file(entry->path);
    entry->is_directory = dn_os_is_directory(entry->path);
  }

  auto compare_subpaths = [](const void* va, const void* vb) {
    constexpr i32 A_FIRST = -1;
    constexpr i32 B_FIRST = 1;

    auto a = (DirectoryEntry*)va;
    auto b = (DirectoryEntry*)vb;

    if (a->is_directory && !b->is_directory) return B_FIRST;
    if (b->is_directory && !a->is_directory) return A_FIRST;

    // Otherwise, sort alphabetically to keep the sort stable
    auto pa = a->path;
    auto pb = b->path;
    
    i32 i = 0;
    i32 sa = strlen(pa);
    i32 sb = strlen(pb);
    while (true) {
      if (i >= sa) return A_FIRST;
      if (i >= sb) return B_FIRST;

      auto ca = pa[i];
      auto cb = pb[i];
      i++;
      
      if (ca == cb) continue;
      if (ca > cb) return B_FIRST;
      if (cb > ca) return A_FIRST;
    }

    return A_FIRST;
  };
  qsort(directory_entries.data, directory_entries.size, sizeof(DirectoryEntry), compare_subpaths);

  arr_for(directory_entries, entry) {
    // Make sure the new file is a Lua script
    if (entry->is_regular_file) {
      dn_lua_script_file(entry->path);
    }
    else if (entry->is_directory) {
      dn_lua_script_dir(entry->path);
    }
  } 
}

void dn_lua_dump_stack () {
  dn_lua_interpreter_t l = dn_lua.state;

  dn_log("LUA STACK");
  int top = lua_gettop(l);
  for (i32 i = 1; i <= top; i++) {
    dn_log("%d\t%s\t", i, luaL_typename(l,i));
    switch (lua_type(l, i)) {
    case LUA_TNUMBER:
      dn_log("%g", lua_tonumber(l,i));
      break;
    case LUA_TSTRING:
      dn_log("%s", lua_tostring(l,i));
      break;
    case LUA_TBOOLEAN:
      dn_log("%s", (lua_toboolean(l, i) ? "true" : "false"));
      break;
    case LUA_TNIL:
      dn_log("%s", "nil");
      break;
    default:
      dn_log("%p", lua_topointer(l,i));
      break;
    }
  }
}

void dn_lua_add_dir(const char* directory) {
  auto copy = dn_string_copy(directory, &dn_allocators.standard);
  arr_push(&dn_lua.script_dirs, copy);
}
#endif
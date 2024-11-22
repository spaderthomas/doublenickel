void init_lua() {
	get_lua().init_phase_0();
	get_lua().init_phase_1();
}

void init_scripts() {
	get_lua().init_phase_2();

}

void update_game() {
	auto& lua = get_lua();
	auto l = lua.state;
	
	lua_pushcfunction(l, &handle_error);
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

inline void my_panic(std::optional<std::string> message) {
	if (message) {
		printf("%s\n", message.value().c_str());
	}
};

i32 handle_error(lua_State* l) {
	// Forward to a function in Lua that formats the error and opens the debugger
	const char* error = lua_tostring(l, -1);
	lua_pop(l, 1);
	
	lua_getfield(l, LUA_GLOBALSINDEX, "tdengine");
	lua_getfield(l, -1, "handle_error");

	lua_pushstring(l, error);
	lua_call(l, 1, 1);

	// This is really important! ImGui crashes if, at the end of the frame, it detects that you missed an End() or
	// a TreePop() (or lots of other things). This is obviously very annoying, because it means a typo in a script
	// will crash the game.
	//
	// To get around this, whenever we mess up in a script, just go ahead and rebalance all of ImGui's internal
	// stacks. You'll probably get a half-finished frame or a frame of garbage, but it's a *lot* better than crashing.
	//ImGui::ErrorCheckEndFrameRecover(nullptr, nullptr);
	return 0;
}

const char* format_file_load_error(const char* error) {
	static char buffer [2048];
	const char* fmt = "  %s";
	snprintf(&buffer[0], 2048, fmt, error);
	
	return &buffer[0];
}

i32 format_file_load_error(lua_State *l) {
	const char* error = lua_tostring(l, 1);
	error = format_file_load_error(error);
	
	lua_pop(l, 1);
	lua_pushstring(l, error);
	return 1;
}


LuaState::LuaState() {
	// This is the best way I figured out to check whether LuaJIT was compiled with GC64; this function is 
	// only stubbed out on GC32. On GC64, it'll actually try to use the first parameter (an allocator) and
	// crash since it's a nullptr
	//this->state = lua_newstate(nullptr, nullptr);
	
	this->state = luaL_newstate();

	arr_init(&script_dirs, max_script_dirs);
	
	auto events = FileChangeEvent::Added | FileChangeEvent::Modified | FileChangeEvent::Removed;
	auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
		// We only watch directories, so there's no need to remove a file from the watch
		// list when it's removed
		if (enum_any(event->events & FileChangeEvent::Removed)) return;

		// Any other files we happen to create should be skipped
		if (!path_util::is_lua(event->file_path)) return;
		
		auto manager = (LuaState*)userdata;

		auto stripped_path = strip_named_path("install", event->file_path);
		tdns_log.write("Hotloading script: %s", stripped_path);
		manager->script_file(event->file_path);
	};

	this->file_monitor = arr_push(&file_monitors);
	this->file_monitor->init(on_file_event, events, this);
}

LuaState& get_lua() {
	static LuaState lua;
	return lua;
}

bool LuaState::script_file(string file_path) {
	if (!path_util::is_lua(file_path)) return true;

	auto l = state;
	i32 initial_stack_size = lua_gettop(l);

	tdns_log.write(Log_Flags::File, "scripting file: file_path = %s", file_path);

	lua_pushcfunction(l, &format_file_load_error);
	
	bool result = luaL_loadfile(l, file_path);

	// In all error cases, do not return early.
	if (result) {
		// There's a syntax error in the file. Since loadfile doesn't call the
		// function we put on the stack, format the message manually.
		const char* error = lua_tostring(l, -1);
		error = format_file_load_error(error);
		
		tdns_log.write("error scripting file; file = %s", file_path);
		tdns_log.write(error);
		lua_pop(l, 2);
		goto check_stack;
	}
	else {
		// The chunk compiled OK. Run it.
		result = lua_pcall(l, 0, 0, -2);
		
		if (result) {
			// There was a runtime error running the chunk.
			const char* error = lua_tostring(l, -1);
			tdns_log.write("error running file; file = %s", file_path);
			tdns_log.write(error);
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

void LuaState::script_named_dir(const_string name) {
	auto path = resolve_named_path(name);
	script_dir(path);
}

void LuaState::script_dir(string path) {
	tdns_log.write(Log_Flags::File, "Loading scripts from directory; path = %s", path);

	this->file_monitor->add_directory(path);

	struct DirectoryEntry {
		tstring path;
		bool is_directory;
		bool is_regular_file;
		bool occupied = false;
	};
	
	constexpr u32 MAX_SCRIPT_ITEMS_PER_DIR = 256;
	Array<DirectoryEntry> directory_entries;
	arr_init(&directory_entries, MAX_SCRIPT_ITEMS_PER_DIR);
	
	for (auto it = directory_iterator(path); it != directory_iterator(); it++) {
		auto entry = arr_push(&directory_entries);
		entry->occupied = true;

		auto dir_path = it->path().string();
		entry->path = copy_string(dir_path, &bump_allocator);
		normalize_path(entry->path);

		entry->is_regular_file = std::filesystem::is_regular_file(it->status());
		entry->is_directory = std::filesystem::is_directory(it->status());
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
			script_file(entry->path);
		}
		else if (entry->is_directory) {
			script_dir(entry->path);
		}
	}	
}

void LuaState::dump_stack () {
	lua_State* l = state;

	printf("LUA STACK\n");
	int top = lua_gettop(l);
	for (i32 i = 1; i <= top; i++) {
		printf("%d\t%s\t", i, luaL_typename(l,i));
		switch (lua_type(l, i)) {
		case LUA_TNUMBER:
			printf("%g\n",lua_tonumber(l,i));
			break;
		case LUA_TSTRING:
			printf("%s\n",lua_tostring(l,i));
			break;
		case LUA_TBOOLEAN:
			printf("%s\n", (lua_toboolean(l, i) ? "true" : "false"));
			break;
		case LUA_TNIL:
			printf("%s\n", "nil");
			break;
		default:
			printf("%p\n",lua_topointer(l,i));
			break;
		}
	}

	printf("\n");
}


void LuaState::init_phase_0() {
	// Basic Lua bootstrapping. Don't load any game scripts here. This is called before
	// we load all the backend systems, because this populates options that those systems
	// might use. 
	auto& lua_manager = get_lua();
	auto l = lua_manager.state;

	luaL_openlibs(l);

	// Give those paths to Lua

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
	// Create the fundamental tables that the engine uses
	auto bootstrap = resolve_named_path("bootstrap");
	lua_manager.script_file(bootstrap);

	lua_pushstring(l, "init_phase_0");
	lua_gettable(l, -2);
	auto result = lua_pcall(l, 0, 0, 0);
	if (result) {
		const char* error = lua_tostring(l, -1);
		tdns_log.write("init_phase_0(): error = %s", error);
		exit(0);
	}
	
	// Bind all C functions, constants, enums, &c.
	register_api();
	register_enums();
	register_constants();

	// PHASE 1:
	// With the base tables created, we can now do things like define classes
	// and entity types. In this phase, load the core engine packages and then
	// any static engine data
	lua_manager.script_named_dir("engine_libs");
	lua_manager.script_named_dir("engine_core");
	lua_manager.script_named_dir("engine_editor");

	lua_pushstring(l, "init_phase_1");
	lua_gettable(l, -2);
	result = lua_pcall(l, 0, 0, 0);
	if (result) {
		const char* error = lua_tostring(l, -1);
		tdns_log.write("init_phase_1(): error = %s", error);
		exit(0);
	}
}

void LuaState::init_phase_1() {
}

void LuaState::init_phase_2() {
	auto& lua_manager = get_lua();
	auto l = lua_manager.state;

	// PHASE 2:
	// Lua itself has been initialized, and we've loaded in other assets our scripts
	// may use (shaders, fonts, etc). The last step is to load the game scripts and
	// configure the game itself through Lua
	arr_push(&script_dirs, resolve_named_path_ex("engine_components", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("engine_editor", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("engine_entities", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("core", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("components", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("dialogue", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("editor", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("entities", &standard_allocator));
	arr_push(&script_dirs, resolve_named_path_ex("subsystems", &standard_allocator));

	arr_for(lua_manager.script_dirs, directory) {
		lua_manager.script_dir(*directory);
	}
	
	// All scripts are loaded. We can start the game.
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "init_phase_2");
	lua_gettable(l, -2);
	auto result = lua_pcall(l, 0, 0, 0);
	if (result) {
		const char* error = lua_tostring(l, -1);
		tdns_log.write("init_phase_2(): error = %s", error);
		exit(0);
	}
}



// FFI
void add_script_directory(const char* directory) {
	auto& lua = get_lua();
	auto copy = copy_string(directory, &standard_allocator);
	arr_push(&lua.script_dirs, copy);
}


////////////////////
// PARSER HELPERS //
////////////////////
void LuaState::parse_string(i32 index, const char** value) {
	if (!lua_isstring(state, index)) return;
	*value = lua_tostring(state, index);
}

void LuaState::parse_string(i32 index, char** value) {
	if (!lua_isstring(state, index)) return;
	const char* cvalue = lua_tostring(state, index);
	strncpy(*value, cvalue, MAX_PATH_LEN);
}

void LuaState::parse_string(const char* key, const char** value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	parse_string(-1, value);
	lua_pop(state, 1);
}

void LuaState::parse_string(const char* key, char** value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	parse_string(-1, value);
	lua_pop(state, 1);
}

void LuaState::parse_bool(const char* key, bool* value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	if (lua_isboolean(state, -1)) *value = lua_toboolean(state, -1);
	
	lua_pop(state, 1);
}

void LuaState::parse_bool(i32 index, bool* value) {
	if (lua_isboolean(state, index)) *value = lua_toboolean(state, index);
}

void LuaState::parse_int32(const char* key, i32* value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	parse_int32(-1, value);
	lua_pop(state, 1);
}

void LuaState::parse_int32(i32 index, i32* value) {
	if (lua_isnumber(state, index)) *value = lua_tonumber(state, index);
}

void LuaState::parse_uint32(const char* key, u32* value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	parse_uint32(-1, value);
	lua_pop(state, 1);
}

void LuaState::parse_uint32(i32 index, u32* value) {
	if (lua_isnumber(state, index)) *value = lua_tonumber(state, index);
}

void LuaState::parse_float(const char* key, float32* value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	parse_float(-1, value);
	lua_pop(state, 1);
}

void LuaState::parse_float(i32 index, float32* value) {
	if (lua_isnumber(state, index)) *value = lua_tonumber(state, index);
}

void LuaState::parse_float64(const char* key, float64* value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	parse_float64(-1, value);
	lua_pop(state, 1);
}

void LuaState::parse_float64(i32 index, float64* value) {
	if (lua_isnumber(state, index)) *value = lua_tonumber(state, index);
}

void LuaState::parse_vec2(Vector2* vec) {
	parse_float("x", &vec->x);
	parse_float("y", &vec->y);
}

void LuaState::parse_vec2(const char* name, Vector2* vec) {
	lua_pushstring(state, name);
	lua_gettable(state, -2);
	DEFER_POP(state);
	if (!lua_istable(state, -1)) return;
	
	parse_vec2(vec);
}

void LuaState::parse_vec2(i32 index, Vector2* vec) {
	lua_pushvalue(state, index);
	DEFER_POP(state);
	if (!lua_istable(state, -1)) return;
	
	parse_vec2(vec);
}

void LuaState::parse_color(Vector4* color) {
	parse_float("r", &color->r);
	parse_float("g", &color->g);
	parse_float("b", &color->b);
	parse_float("a", &color->a);
}

void LuaState::parse_color(const char* name, Vector4* color) {
	lua_pushstring(state, name);
	lua_gettable(state, -2);
	if (!lua_istable(state, -1)) return;
	
	parse_color(color);
	lua_pop(state, 1);
}

void LuaState::parse_color(i32 index, Vector4* color) {
	lua_pushvalue(state, index);
	if (!lua_istable(state, -1)) return;
	
	parse_color(color);
	lua_pop(state, 1);
}

template<typename T>
void LuaState::parse_enum(const char* key, T* value) {
	lua_pushstring(state, key);
	lua_gettable(state, -2);
	if (!lua_isnumber(state, -1)) return;

	i32 integer_value = lua_tonumber(state, -1);
	*value = (T)integer_value;
	lua_pop(state, 1);
}

void add_named_path_ex(const char* name, const char* absolute_path) {
	if (named_paths.contains(name)) {
		auto& existing_path = named_paths.at(name);
		tdns_log.write("Tried to add named path, but name was already registered; name = %s, existing_path = %s, new_path = %s", name, existing_path.c_str(), absolute_path);
	}
	
	named_paths[name] = absolute_path;
}

void add_named_subpath(const char* name, const char* parent_name, const char* relative_path) {
	if (!name) return;
	if (!parent_name) return;
	if (!relative_path) return;
	
	auto parent_path = resolve_named_path(parent_name);
	auto absolute_path = bump_allocator.alloc_path();
	snprintf(absolute_path, TD_MAX_PATH_LEN, "%s/%s", parent_path, relative_path);
	
	add_named_path_ex(name, absolute_path);
}

void add_install_path(const char* name, const char* relative_path) {
	add_named_subpath(name, "install", relative_path);
}

void add_engine_path(const char* name, const char* relative_path) {
	add_named_subpath(name, "engine", relative_path);
}

void add_write_path(const char* name, const char* relative_path) {
	add_named_subpath(name, "write", relative_path);
}

tstring strip_named_path(const char* name, const char* absolute_path) {
	auto named_path = resolve_named_path_ex(name, &bump_allocator);
	auto named_path_len = strlen(named_path);
	auto absolute_path_len = strlen(absolute_path);
	
	auto stripped_path = copy_string(absolute_path, absolute_path_len, &bump_allocator);
	
	u32 num_strip_chars = std::min(named_path_len, absolute_path_len);
	u32 num_stripped = 0;
	for (u32 i = 0; i < num_strip_chars; i++) {
		if (stripped_path[i] != named_path[i]) break;
		num_stripped++;
	}

	return stripped_path + num_stripped;
}

tstring resolve_named_path(const char* name) {
	return resolve_named_path_ex(name, &bump_allocator);
}

string resolve_named_path_ex(const char* name, MemoryAllocator* allocator) {
	if (!name) return nullptr;
	
	if (!named_paths.contains(name)) {
		tdns_log.write("Tried to find named path, but name was not registered; name = %s", name);
		return nullptr;
	}

	auto& path = named_paths[name];
	return copy_string(path.c_str(), path.length(), allocator);		
}

tstring resolve_format_path(const char* name, const char* file_name) {
	return resolve_format_path_ex(name, file_name, &bump_allocator);
}

string resolve_format_path_ex(const char* name, const char* file_name, MemoryAllocator* allocator) {
	if (!name) return nullptr;
	if (!file_name) return nullptr;
	
	if (!named_paths.contains(name)) {
		tdns_log.write("Tried to find named path, but name was not registered; name = %s", name);
		return nullptr;
	}

	auto& path = named_paths[name];
	
	auto resolved_path = allocator->alloc_path();
	snprintf(resolved_path, TD_MAX_PATH_LEN, path.c_str(), file_name);
	return resolved_path;
}

NamedPathResult find_all_named_paths() {
	Array<NamedPath> collected_paths;
	arr_init(&collected_paths, named_paths.size(), &bump_allocator);

	for (auto& [name, path] : named_paths) {
		auto collected_path = arr_push(&collected_paths);
		collected_path->name = copy_string(name, &bump_allocator);
		collected_path->path = copy_string(path, &bump_allocator);
	}

	return {
		.data = collected_paths.data,
		.size = static_cast<u32>(collected_paths.size)
	};
}

tstring build_path(const char* relative_path) {
	auto executable_file = bump_allocator.alloc_path();
	GetModuleFileNameA(NULL, executable_file, TD_MAX_PATH_LEN);

	std::filesystem::path executable_filepath = std::filesystem::path(executable_file);
	auto executable_dir = executable_filepath.parent_path() / relative_path;
	auto canonical_path = std::filesystem::canonical(executable_dir);
	auto canonical_dir = canonical_path.string();

	auto normalized_dir = copy_string(canonical_dir, &bump_allocator);
	for (u32 i = 0; i < canonical_dir.size(); i++) {
		if (normalized_dir[i] == '\\') {
			normalized_dir[i] = '/';
		}
	}

	return normalized_dir;
}

void set_install_paths() {
	add_named_path_ex("install", build_path(app.install_path));
	add_named_path_ex("engine", build_path(app.engine_path));
	add_named_path_ex("app", build_path(app.app_path));
}

void set_write_path() {	
#if defined(FM_EDITOR)
	add_named_path_ex("write", build_path(app.write_path));
#else
	auto appdata_dir = bump_allocator.alloc_path();

	// In release mode, we have to write to an OS-approved directory. 
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appdata_dir);
	
	// Normalize
	for (int32 i = 0; i < TD_MAX_PATH_LEN; i++) {
		if (appdata_dir[i] == 0) break;
		if (appdata_dir[i] == '\\') appdata_dir[i] = '/';
	}
	add_named_path_ex("write", appdata_dir);
#endif
}

void init_paths() {
	set_install_paths();
	set_write_path();

	add_install_path("log", "tdengine.log");
	add_engine_path("bootstrap", "source/scripts/engine/core/bootstrap.lua");
	add_engine_path("engine_paths", "source/scripts/engine/data/paths.lua");
}

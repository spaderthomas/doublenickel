void add_named_path_ex(const_string name, const_string absolute_path) {
	if (named_paths.contains(name)) {
		auto& existing_path = named_paths.at(name);
		tdns_log.write("Tried to add named path, but name was already registered; name = %s, existing_path = %s, new_path = %s", name, existing_path.c_str(), absolute_path);
	}
	
	named_paths[name] = absolute_path;
}

void add_named_subpath(const_string name, const_string base_path, const_string relative_path) {
	if (!name) return;
	if (!base_path) return;
	if (!relative_path) return;
	
	auto absolute_path = bump_allocator.alloc_path();
	snprintf(absolute_path, MAX_PATH_LEN, "%s/%s", base_path, relative_path);
	
	add_named_path_ex(name, absolute_path);
}

void add_install_path(const_string name, const_string relative_path) {
	auto root = resolve_named_path("install");
	add_named_subpath(name, root, relative_path);
}

void add_write_path(const_string name, const_string relative_path) {
	auto write = resolve_named_path("write");
	add_named_subpath(name, write, relative_path);
}

tstring strip_named_path(const_string name, const_string absolute_path) {
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

tstring resolve_named_path(const_string name) {
	return resolve_named_path_ex(name, &bump_allocator);
}

string resolve_named_path_ex(const_string name, MemoryAllocator* allocator) {
	if (!name) return nullptr;
	
	if (!named_paths.contains(name)) {
		tdns_log.write("Tried to find named path, but name was not registered; name = %s", name);
		return nullptr;
	}

	auto& path = named_paths[name];
	return copy_string(path.c_str(), path.length(), allocator);		
}

tstring resolve_format_path(const_string name, const_string file_name) {
	return resolve_format_path_ex(name, file_name, &bump_allocator);
}

string resolve_format_path_ex(const_string name, const_string file_name, MemoryAllocator* allocator) {
	if (!name) return nullptr;
	if (!file_name) return nullptr;
	
	if (!named_paths.contains(name)) {
		tdns_log.write("Tried to find named path, but name was not registered; name = %s", name);
		return nullptr;
	}

	auto& path = named_paths[name];
	
	auto resolved_path = allocator->alloc_path();
	snprintf(resolved_path, MAX_PATH_LEN, path.c_str(), file_name);
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

void set_install_path(const_string root_path) {
	add_named_path_ex("install", root_path);
}

void set_write_path(const_string data_path) {
	add_named_path_ex("write", data_path);
}

void find_install_path() {
	auto install_dir = bump_allocator.alloc_path();
	
	GetModuleFileNameA(NULL, install_dir, MAX_PATH_LEN);
	int32 len = strlen(install_dir);

	// Normalize
	for (int32 i = 0; i < len; i++) {
		if (install_dir[i] == '\\') install_dir[i] = '/';
	}

#if defined(FM_EDITOR)
	// Remove executable name AND build directory
	int32 removed = 0;
	for (int32 i = len - 1; i > 0; i--) {
		if (install_dir[i] == '/') removed++;
		install_dir[i] = 0;
		if (removed == 5) break;
	}
#else
	// We're running a packaged build, which means the executable is at the top level.
	for (int32 i = len - 1; i > 0; i--) {
		if (install_dir[i] == '/') { install_dir[i] = 0; break; }
		install_dir[i] = 0;
	}
#endif

	set_install_path(install_dir);
}

void find_write_path() {	
#if defined(FM_EDITOR)
	// In debug mode, just make the AppData directory the same as
	// the repository.
	auto root = resolve_named_path("install");
	auto appdata_dir = bump_allocator.alloc_path();
	snprintf(appdata_dir, MAX_PATH_LEN, "%s/%s", root, "scripts/user/data");
#else
	auto appdata_dir = bump_allocator.alloc_path();

	// In release mode, we have to write to an OS-approved directory. 
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appdata_dir);
	
	// Normalize
	for (int32 i = 0; i < MAX_PATH_LEN; i++) {
		if (appdata_dir[i] == 0) break;
		if (appdata_dir[i] == '\\') appdata_dir[i] = '/';
	}
#endif
	
	set_write_path(appdata_dir);
}

void init_paths() {
	find_install_path();
	find_write_path();

	add_install_path("log", "tdengine.log");
	add_install_path("bootstrap", "scripts/engine/core/bootstrap.lua");
	add_install_path("engine_paths", "scripts/engine/data/paths.lua");
}

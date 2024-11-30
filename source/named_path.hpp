#ifndef DN_NAMED_PATH_H
#define DN_NAMED_PATH_H

typedef struct {
  const char* name;
  const char* path;
} dn_named_path_t;

typedef struct {
  dn_named_path_t* data;
  u32 size;
} dn_named_path_result_t;

typedef struct {
  std::unordered_map<std::string, std::string> entries;
} dn_paths_t;
dn_paths_t dn_paths;

DN_API dn_named_path_result_t dn_paths_find_all();
DN_API void                   dn_paths_add_install_subpath(const char* name, const char* relative_path);
DN_API void                   dn_paths_add_write_subpath(const char* name, const char* relative_path);
DN_API void                   dn_paths_add_engine_subpath(const char* name, const char* relative_path);
DN_API void                   dn_paths_add_subpath(const char* name, const char* parent_name, const char* relative_path);
DN_API dn_tstring_t           dn_paths_resolve(const char* name);
DN_API dn_tstring_t           dn_paths_resolve_format(const char* name, const char* file_name);
DN_API dn_tstring_t           dn_paths_strip(const char* name, const char* absolute_path);
DN_API void                   dn_paths_add_ex(const char* name, const char* absolute_path);
DN_API char*                  dn_paths_resolve_ex(const char* name, dn_allocator_t* allocator);
DN_API char*                  dn_paths_resolve_format_ex(const char* name, const char* file_name, dn_allocator_t* allocator);
DN_IMP void                   dn_paths_init();
DN_IMP void                   _dn_paths_build_root_path();
DN_IMP void                   _dn_paths_set_install_roots();
DN_IMP void                   _dn_paths_set_write_path();
#endif

#ifdef DN_NAMED_PATH_IMPLEMENTATION
void dn_paths_add_ex(const char* name, const char* absolute_path) {
  if (dn_paths.entries.contains(name)) {
    auto& existing_path = dn_paths.entries.at(name);
    dn_log("Tried to add named path, but name was already registered; name = %s, existing_path = %s, new_path = %s", name, existing_path.c_str(), absolute_path);
  }
  
  dn_paths.entries[name] = absolute_path;
}

void dn_paths_add_subpath(const char* name, const char* parent_name, const char* relative_path) {
  if (!name) return;
  if (!parent_name) return;
  if (!relative_path) return;
  
  auto parent_path = dn_paths_resolve(parent_name);
  auto absolute_path = dn::allocator::alloc<char>(&dn_allocators.bump, DN_MAX_PATH_LEN);
  snprintf(absolute_path, DN_MAX_PATH_LEN, "%s/%s", parent_path, relative_path);
  
  dn_paths_add_ex(name, absolute_path);
}

void dn_paths_add_install_subpath(const char* name, const char* relative_path) {
  dn_paths_add_subpath(name, "install", relative_path);
}

void dn_paths_add_engine_subpath(const char* name, const char* relative_path) {
  dn_paths_add_subpath(name, "engine", relative_path);
}

void dn_paths_add_write_subpath(const char* name, const char* relative_path) {
  dn_paths_add_subpath(name, "write", relative_path);
}

dn_tstring_t dn_paths_strip(const char* name, const char* absolute_path) {
  auto named_path = dn_paths_resolve_ex(name, &dn_allocators.bump);
  auto named_path_len = strlen(named_path);
  auto absolute_path_len = strlen(absolute_path);
  
  auto stripped_path = dn_string_copy(absolute_path, absolute_path_len, &dn_allocators.bump);
  
  u32 num_strip_chars = std::min(named_path_len, absolute_path_len);
  u32 num_stripped = 0;
  for (u32 i = 0; i < num_strip_chars; i++) {
    if (stripped_path[i] != named_path[i]) break;
    num_stripped++;
  }

  return stripped_path + num_stripped;
}

dn_tstring_t dn_paths_resolve(const char* name) {
  return dn_paths_resolve_ex(name, &dn_allocators.bump);
}

char* dn_paths_resolve_ex(const char* name, dn_allocator_t* allocator) {
  if (!name) return nullptr;
  
  if (!dn_paths.entries.contains(name)) {
    dn_log("Tried to find named path, but name was not registered; name = %s", name);
    return nullptr;
  }

  auto& path = dn_paths.entries[name];
  return dn_string_copy(path.c_str(), path.length(), allocator);   
}

dn_tstring_t dn_paths_resolve_format(const char* name, const char* file_name) {
  return dn_paths_resolve_format_ex(name, file_name, &dn_allocators.bump);
}

char* dn_paths_resolve_format_ex(const char* name, const char* file_name, dn_allocator_t* allocator) {
  if (!name) return nullptr;
  if (!file_name) return nullptr;
  
  if (!dn_paths.entries.contains(name)) {
    dn_log("Tried to find named path, but name was not registered; name = %s", name);
    return nullptr;
  }

  auto& path = dn_paths.entries[name];
  
  auto resolved_path = dn::allocator::alloc<char>(allocator, DN_MAX_PATH_LEN);
  snprintf(resolved_path, DN_MAX_PATH_LEN, path.c_str(), file_name);
  return resolved_path;
}

dn_named_path_result_t dn_paths_find_all() {
  dn_array_t<dn_named_path_t> collected_paths;
  dn_array_init(&collected_paths, dn_paths.entries.size(), &dn_allocators.bump);

  for (auto& [name, path] : dn_paths.entries) {
    auto collected_path = dn_array_push(&collected_paths);
    collected_path->name = dn_string_copy(name, &dn_allocators.bump);
    collected_path->path = dn_string_copy(path, &dn_allocators.bump);
  }

  return {
    .data = collected_paths.data,
    .size = static_cast<u32>(collected_paths.size)
  };
}

dn_tstring_t _dn_paths_build_root_path(const char* relative_path) {
  auto executable_file = dn::allocator::alloc<char>(&dn_allocators.bump, DN_MAX_PATH_LEN);
  GetModuleFileNameA(NULL, executable_file, DN_MAX_PATH_LEN);

  std::error_code error;
  std::filesystem::path executable_filepath = std::filesystem::path(executable_file);
  auto executable_dir = executable_filepath.parent_path() / relative_path;
  auto canonical_path = std::filesystem::canonical(executable_dir, error);
  auto canonical_dir = canonical_path.string();

  auto normalized_dir = dn_string_copy(canonical_dir, &dn_allocators.bump);
  for (u32 i = 0; i < canonical_dir.size(); i++) {
    if (normalized_dir[i] == '\\') {
      normalized_dir[i] = '/';
    }
  }

  return normalized_dir;
}

void _dn_paths_set_install_roots() {
  dn_paths_add_ex("install", _dn_paths_build_root_path(dn_app.install_path));
  dn_paths_add_ex("engine", _dn_paths_build_root_path(dn_app.engine_path));
  dn_paths_add_ex("app", _dn_paths_build_root_path(dn_app.app_path));
}

void _dn_paths_set_write_path() { 
#if defined(DN_EDITOR)
  dn_paths_add_ex("write", _dn_paths_build_root_path(dn_app.write_path));
#else
  auto appdata_dir = dn::allocator::alloc<char>(&dn_allocators.bump, DN_MAX_PATH_LEN);

  // In release mode, we have to write to an OS-approved directory. 
  SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appdata_dir);
  
  // Normalize
  for (int32 i = 0; i < DN_MAX_PATH_LEN; i++) {
    if (appdata_dir[i] == 0) break;
    if (appdata_dir[i] == '\\') appdata_dir[i] = '/';
  }
  dn_paths_add_ex("write", appdata_dir);
#endif
}

void dn_paths_init() {
  _dn_paths_set_install_roots();
  _dn_paths_set_write_path();

  dn_paths_add_install_subpath("dn_log", "doublenickel.log");
  dn_paths_add_engine_subpath("dn_bootstrap", "source/scripts/core/bootstrap.lua");
  dn_paths_add_engine_subpath("dn_install", "source/scripts/data/paths.lua");
}
#endif
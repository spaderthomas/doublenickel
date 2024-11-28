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

DN_API dn_named_path_result_t dn_paths_find_all();
DN_API void dn_paths_add_install_subpath(const char* name, const char* relative_path);
DN_API void dn_paths_add_write_subpath(const char* name, const char* relative_path);
DN_API void dn_paths_add_engine_subpath(const char* name, const char* relative_path);
DN_API void dn_paths_add_subpath(const char* name, const char* parent_name, const char* relative_path);
DN_API tstring dn_paths_resolve(const char* name);
DN_API tstring dn_paths_resolve_format(const char* name, const char* file_name);
DN_API tstring dn_paths_strip(const char* name, const char* absolute_path);

void dn_paths_add_ex(const char* name, const char* absolute_path);
string dn_paths_resolve_ex(const char* name, dn_allocator_t* allocator);
string dn_paths_resolve_format_ex(const char* name, const char* file_name, dn_allocator_t* allocator);

std::unordered_map<std::string, std::string> named_paths;
#endif

#ifdef DN_NAMED_PATH_IMPLEMENTATION
void dn_paths_add_ex(const char* name, const char* absolute_path) {
  if (named_paths.contains(name)) {
    auto& existing_path = named_paths.at(name);
    dn_log("Tried to add named path, but name was already registered; name = %s, existing_path = %s, new_path = %s", name, existing_path.c_str(), absolute_path);
  }
  
  named_paths[name] = absolute_path;
}

void dn_paths_add_subpath(const char* name, const char* parent_name, const char* relative_path) {
  if (!name) return;
  if (!parent_name) return;
  if (!relative_path) return;
  
  auto parent_path = dn_paths_resolve(parent_name);
  auto absolute_path = bump_allocator.alloc_path();
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

tstring dn_paths_strip(const char* name, const char* absolute_path) {
  auto named_path = dn_paths_resolve_ex(name, &bump_allocator);
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

tstring dn_paths_resolve(const char* name) {
  return dn_paths_resolve_ex(name, &bump_allocator);
}

string dn_paths_resolve_ex(const char* name, dn_allocator_t* allocator) {
  if (!name) return nullptr;
  
  if (!named_paths.contains(name)) {
    dn_log("Tried to find named path, but name was not registered; name = %s", name);
    return nullptr;
  }

  auto& path = named_paths[name];
  return copy_string(path.c_str(), path.length(), allocator);   
}

tstring dn_paths_resolve_format(const char* name, const char* file_name) {
  return dn_paths_resolve_format_ex(name, file_name, &bump_allocator);
}

string dn_paths_resolve_format_ex(const char* name, const char* file_name, dn_allocator_t* allocator) {
  if (!name) return nullptr;
  if (!file_name) return nullptr;
  
  if (!named_paths.contains(name)) {
    dn_log("Tried to find named path, but name was not registered; name = %s", name);
    return nullptr;
  }

  auto& path = named_paths[name];
  
  auto resolved_path = allocator->alloc_path();
  snprintf(resolved_path, DN_MAX_PATH_LEN, path.c_str(), file_name);
  return resolved_path;
}

dn_named_path_result_t dn_paths_find_all() {
  Array<dn_named_path_t> collected_paths;
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
  GetModuleFileNameA(NULL, executable_file, DN_MAX_PATH_LEN);

  std::error_code error;
  std::filesystem::path executable_filepath = std::filesystem::path(executable_file);
  auto executable_dir = executable_filepath.parent_path() / relative_path;
  auto canonical_path = std::filesystem::canonical(executable_dir, error);
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
  dn_paths_add_ex("install", build_path(dn_app.install_path));
  dn_paths_add_ex("engine", build_path(dn_app.engine_path));
  dn_paths_add_ex("app", build_path(dn_app.app_path));
}

void set_write_path() { 
#if defined(FM_EDITOR)
  dn_paths_add_ex("write", build_path(dn_app.write_path));
#else
  auto appdata_dir = bump_allocator.alloc_path();

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

void init_paths() {
  set_install_paths();
  set_write_path();

  dn_paths_add_install_subpath("log", "tdengine.log");
  dn_paths_add_engine_subpath("bootstrap", "source/scripts/engine/core/bootstrap.lua");
  dn_paths_add_engine_subpath("engine_paths", "source/scripts/engine/data/paths.lua");
}
#endif
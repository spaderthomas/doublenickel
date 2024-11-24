struct dn_named_path_t {
	const char* name;
	const char* path;
};

struct dn_named_path_result_t {
	dn_named_path_t* data;
	u32 size;
};

FM_LUA_EXPORT dn_named_path_result_t dn_paths_find_all();
FM_LUA_EXPORT void dn_paths_add_install_subpath(const char* name, const char* relative_path);
FM_LUA_EXPORT void dn_paths_add_write_subpath(const char* name, const char* relative_path);
FM_LUA_EXPORT void dn_paths_add_engine_subpath(const char* name, const char* relative_path);
FM_LUA_EXPORT void dn_paths_add_subpath(const char* name, const char* parent_name, const char* relative_path);
FM_LUA_EXPORT tstring dn_paths_resolve(const char* name);
FM_LUA_EXPORT tstring dn_paths_resolve_format(const char* name, const char* file_name);
FM_LUA_EXPORT tstring dn_paths_strip(const char* name, const char* absolute_path);

void dn_paths_add_ex(const char* name, const char* absolute_path);
string dn_paths_resolve_ex(const char* name, MemoryAllocator* allocator);
string dn_paths_resolve_format_ex(const char* name, const char* file_name, MemoryAllocator* allocator);

std::unordered_map<std::string, std::string> named_paths;

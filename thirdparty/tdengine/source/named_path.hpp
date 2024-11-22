struct NamedPath {
	const char* name;
	const char* path;
};

struct NamedPathResult {
	NamedPath* data;
	u32 size;
};

FM_LUA_EXPORT NamedPathResult find_all_named_paths();
FM_LUA_EXPORT void add_install_path(const char* name, const char* relative_path);
FM_LUA_EXPORT void add_write_path(const char* name, const char* relative_path);
FM_LUA_EXPORT void add_engine_path(const char* name, const char* relative_path);
FM_LUA_EXPORT void add_named_subpath(const char* name, const char* parent_name, const char* relative_path);
FM_LUA_EXPORT tstring resolve_named_path(const char* name);
FM_LUA_EXPORT tstring resolve_format_path(const char* name, const char* file_name);

void add_named_path_ex(const char* name, const char* absolute_path);
string resolve_named_path_ex(const char* name, MemoryAllocator* allocator);
string resolve_format_path_ex(const char* name, const char* file_name, MemoryAllocator* allocator);
tstring strip_named_path(const char* name, const char* absolute_path);

std::unordered_map<std::string, std::string> named_paths;

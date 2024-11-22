struct NamedPath {
	const_string name;
	const_string path;
};

struct NamedPathResult {
	NamedPath* data;
	u32 size;
};

FM_LUA_EXPORT NamedPathResult find_all_named_paths();
FM_LUA_EXPORT void add_install_path(const_string name, const_string relative_path);
FM_LUA_EXPORT void add_write_path(const_string name, const_string relative_path);
FM_LUA_EXPORT tstring resolve_named_path(const_string name);
FM_LUA_EXPORT tstring resolve_format_path(const_string name, const_string file_name);

void add_named_subpath(const_string name, const_string base_path, const_string relative_path);
void add_named_path_ex(const_string name, const_string absolute_path);
string resolve_named_path_ex(const_string name, MemoryAllocator* allocator);
string resolve_format_path_ex(const_string name, const_string file_name, MemoryAllocator* allocator);
tstring strip_named_path(const_string name, const_string absolute_path);

std::unordered_map<std::string, std::string> named_paths;

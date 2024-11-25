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

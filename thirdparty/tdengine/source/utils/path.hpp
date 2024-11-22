void normalize_path(char* str);
void normalize_path(std::string& str);
bool is_alphanumeric(std::string& str);
bool is_valid_filename(std::string& str);
bool is_png(std::string& asset_path);
bool is_lua(std::string& path);
bool is_luac(std::string& path);
tstring extract_file_name(const char* full_path);
bool is_directory(char* path);
char* wide_to_utf8(uint16* path, uint32 length);

namespace path_util {
	bool is_lua(std::string_view str);
}

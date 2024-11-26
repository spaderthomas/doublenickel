#ifndef DN_PATH_H
#define DN_PATH_H

#define DN_MAX_PATH_LEN 256
typedef char dn_path_t [DN_MAX_PATH_LEN];

void normalize_path(char* str);
void normalize_path(std::string& str);
bool is_png(std::string& asset_path);
tstring extract_file_name(const char* full_path);
char* wide_to_utf8(uint16* path, uint32 length);

namespace path_util {
	bool is_lua(std::string_view str);
}


DN_API void dn_path_join(dn_path_t path, const char* a, const char* b);
#endif

#ifdef DN_PATH_IMPLEMENTATION
DN_API void dn_path_join(dn_path_t path, const char* a, const char* b) {
	snprintf(path, DN_MAX_PATH_LEN, "%s/%s", a, b);
}


namespace path_util {
	bool is_lua(std::string_view str) {
		if (str.size() < 5) return false;
		std::string_view extension = str.substr(str.size() - 4, 4);
		return !extension.compare(".lua");
	}
}

void normalize_path(char* str) {
	u32 i = 0;
	while (true) {
		if (str[i] == 0) break;
		if (str[i] == '\\') {
			str[i] = '/';
		}
		i++;
	}
}

void normalize_path(std::string& str) {
	string_replace(str, "\\", "/");
}


	
// @hack I'm sure there are PNG headers I could try parsing, but this works!
bool is_png(std::string& asset_path) {
	if (asset_path.size() < 5) { return false; } // "x.png" is the shortest name
	std::string should_be_png_extension = asset_path.substr(asset_path.size() - 4, 4);
	if (should_be_png_extension.compare(".png")) return false;
	return true;
}

tstring extract_file_name(const char* full_path) {
	auto size = strlen(full_path);
	auto index = size - 1;
	while (true) {
		if (index < 0) break;
		if (full_path[index] == '/') { index += 1; break; }
		index -= 1;
	}
	
	return copy_string(full_path + index, &bump_allocator);
}

tstring wide_to_utf8(u16* string, u32 length) {
	tstring utf8 = bump_allocator.alloc<char>(length + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)string, length, utf8, length, NULL, NULL);
	return utf8;
}
#endif
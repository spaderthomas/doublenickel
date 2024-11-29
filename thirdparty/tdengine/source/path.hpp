#ifndef DN_PATH_H
#define DN_PATH_H

#define DN_MAX_PATH_LEN 256
typedef char dn_path_t [DN_MAX_PATH_LEN];

DN_API void dn_path_normalize_cstr(char* str);
DN_IMP void dn_path_normalize_std(std::string& str);
DN_API void dn_path_from_cstr(dn_path_t path, const char* a);
DN_API void dn_path_join(dn_path_t path, const char* a, const char* b);
DN_IMP bool dn_path_match_extension(std::string_view str, dn_string_t extension);
bool is_png(std::string& asset_path);
dn_tstring_t extract_file_name(const char* full_path);
char* wide_to_utf8(uint16* path, uint32 length);

namespace path_util {
	bool is_lua(std::string_view str);
}


#endif

#ifdef DN_PATH_IMPLEMENTATION
void dn_path_from_cstr(dn_path_t path, const char* a) {
	snprintf(path, DN_MAX_PATH_LEN, "%s", a);
}

void dn_path_join(dn_path_t path, const char* a, const char* b) {
	snprintf(path, DN_MAX_PATH_LEN, "%s/%s", a, b);
}

bool dn_path_match_extension(std::string_view str, dn_string_t extension) {
	if (str.size() < extension.len) return false;

	std::string_view extension_view = str.substr(str.size() - extension.len, extension.len);
	return !extension_view.compare(dn_string_to_cstr(extension, &dn_allocators.bump));
}


namespace path_util {
	bool is_lua(std::string_view str) {
		if (str.size() < 5) return false;
		std::string_view extension = str.substr(str.size() - 4, 4);
		return !extension.compare(".lua");
	}
}

void dn_path_normalize_cstr(char* str) {
	u32 i = 0;
	while (true) {
		if (str[i] == 0) break;
		if (str[i] == '\\') {
			str[i] = '/';
		}
		i++;
	}
}

void dn_path_normalize_std(std::string& str) {
	size_t start_pos = 0;
	while((start_pos = str.find("\\", start_pos)) != std::string::npos) {
			str.replace(start_pos, 1, "/");
			start_pos++;
	}
}


	
// @hack I'm sure there are PNG headers I could try parsing, but this works!
bool is_png(std::string& asset_path) {
	if (asset_path.size() < 5) { return false; } // "x.png" is the shortest name
	std::string should_be_png_extension = asset_path.substr(asset_path.size() - 4, 4);
	if (should_be_png_extension.compare(".png")) return false;
	return true;
}

dn_tstring_t extract_file_name(const char* full_path) {
	auto size = strlen(full_path);
	auto index = size - 1;
	while (true) {
		if (index < 0) break;
		if (full_path[index] == '/') { index += 1; break; }
		index -= 1;
	}
	
	return dn_string_copy(full_path + index, &dn_allocators.bump);
}

dn_tstring_t wide_to_utf8(u16* string, u32 length) {
	dn_tstring_t utf8 = dn::allocator::alloc<char>(&dn_allocators.bump, length + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)string, length, utf8, length, NULL, NULL);
	return utf8;
}
#endif

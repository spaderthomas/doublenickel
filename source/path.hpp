#ifndef DN_PATH_H
#define DN_PATH_H

#define DN_MAX_PATH_LEN 256
typedef char dn_path_t [DN_MAX_PATH_LEN];

DN_API void        dn_path_normalize_cstr(char* str);
DN_IMP void        dn_path_normalize_std(std::string& str);
DN_API void        dn_path_from_cstr(dn_path_t path, const char* a);
DN_API dn_string_t dn_path_strip_extension(const char* file_name);
DN_API dn_string_t dn_path_extract_file_name(const char* full_path);
DN_API char*       dn_string_16_to_8(u16* str, u32 length);
DN_IMP bool        dn_path_is_extension(std::string_view str, dn_string_t extension);

namespace path_util {
	bool is_lua(std::string_view str);
}


#endif

#ifdef DN_PATH_IMPLEMENTATION
void dn_path_from_cstr(dn_path_t path, const char* a) {
	snprintf(path, DN_MAX_PATH_LEN, "%s", a);
}

bool dn_path_is_extension(std::string_view str, dn_string_t extension) {
	if (str.size() < extension.len) return false;

	std::string_view extension_view = str.substr(str.size() - extension.len, extension.len);
	return !extension_view.compare(dn_string_to_cstr_ex(extension, &dn_allocators.bump));
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

dn_string_t dn_path_extract_file_name(const char* full_path) {
	auto size = strlen(full_path);
	auto index = size - 1;
	while (true) {
		if (index < 0) break;
		if (full_path[index] == '/') { index += 1; break; }
		index -= 1;
	}
	
	return {
		.data = (u8*)dn_cstr_copy(full_path + index, &dn_allocators.bump),
		.len = (u32)(size - index)
	};
}

char* dn_string_16_to_8(u16* str, u32 length) {
	dn_tstring_t utf8 = dn::allocator::alloc<char>(&dn_allocators.bump, length + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)str, length, utf8, length, NULL, NULL);
	return utf8;
}


dn_string_t dn_path_strip_extension(const char* file_name) {
	auto stripped = dn_cstr_copy(file_name, &dn_allocators.bump);
	
	auto length = strlen(stripped);
	auto period = length;
	for (int i = length; i >= 0; i--) {
		if (stripped[i] == '.') {
			length = i;
			break;
		}
	}

	return { .data = (u8*)stripped, .len = (u32)length };
}
#endif

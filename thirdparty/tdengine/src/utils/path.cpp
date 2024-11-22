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

bool is_alphanumeric(std::string& str) {
	auto is_numeric = [](char c) -> bool { return c >= '0' && c <= '9'; };
	auto is_alpha = [](char c) -> bool { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); };
	
	for (u32 ichar = 0; ichar < str.size(); ichar++) {
		char c = str.at(ichar);
		if (!(is_numeric(c) || is_alpha(c))) {
			return false;
		}
	}
	
	return true;
}

// Allowing alphanumerics, underscores, and periods
bool is_valid_filename(std::string& str) {
	auto is_numeric = [](char c) -> bool { return c >= '0' && c <= '9'; };
	auto is_alpha = [](char c) -> bool { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); };
	auto is_misc = [](char c) -> bool { return (c == '_') || c == '.'; };
	
	for (u32 ichar = 0; ichar < str.size(); ichar++) {
		char c = str.at(ichar);
		if (!(is_numeric(c) || is_alpha(c) || is_misc(c))) {
			return false;
		}
	}
	
	return true;
}

	
// @hack I'm sure there are PNG headers I could try parsing, but this works!
bool is_png(std::string& asset_path) {
	if (asset_path.size() < 5) { return false; } // "x.png" is the shortest name
	std::string should_be_png_extension = asset_path.substr(asset_path.size() - 4, 4);
	if (should_be_png_extension.compare(".png")) return false;
	return true;
}

bool is_lua(std::string& path) {
	if (path.size() < 5) { return false; } // "x.lua" is the shortest name
 	std::string should_be_tds_extension = path.substr(path.size() - 4, 4);
	if (should_be_tds_extension.compare(".lua")) return false;
	return true;
}

bool is_luac(std::string& path) {
	if (path.size() < 6) { return false; }
 	std::string should_be_tds_extension = path.substr(path.size() - 5, 5);
	if (should_be_tds_extension.compare(".luac")) return false;
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

bool is_directory(char* path) {
	auto attribute = GetFileAttributesA(path);
	if (attribute == INVALID_FILE_ATTRIBUTES) return false;
	return attribute & FILE_ATTRIBUTE_DIRECTORY;
}

tstring wide_to_utf8(u16* string, u32 length) {
	tstring utf8 = bump_allocator.alloc<char>(length + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)string, length, utf8, length, NULL, NULL);
	return utf8;
}

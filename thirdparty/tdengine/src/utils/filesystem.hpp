float64 file_mod_time(const char* file_path) {
	std::error_code error;
	auto file_mod_time = std::filesystem::last_write_time(file_path, error);
	auto file_mod_time_s = std::chrono::time_point_cast<std::chrono::seconds>(file_mod_time);
	double file_mod_time_epoch = file_mod_time_s.time_since_epoch().count();
	
	return file_mod_time_epoch;
}

tstring strip_extension(const char* file_name) {
	auto stripped = copy_string(file_name, &bump_allocator);
	
	auto length = strlen(stripped);
	auto period = length;
	for (int i = length; i >= 0; i--) {
		if (stripped[i] == '.') {
			length = i;
			break;
		}
	}

	for (int i = period; i < length; i++) {
		stripped[i] = 0;
	}

	return stripped;
}

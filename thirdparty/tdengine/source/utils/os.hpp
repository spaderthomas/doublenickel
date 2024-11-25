#ifndef DN_OS_H
#define DN_OS_H
struct dn_os_date_time_t {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;
};

DN_API bool dn_os_does_path_exist(const char* path);
DN_API bool dn_os_is_regular_file(const char* path);
DN_API bool dn_os_is_directory(const char* path);
DN_API void dn_os_remove_directory(const char* path);
DN_API void dn_os_create_directory(const char* path);
DN_API dn_os_date_time_t dn_os_get_date_time();
#endif

#ifdef DN_OS_IMPLEMENTATION
bool dn_os_does_path_exist(const char* path) {
	return std::filesystem::exists(path);
}

bool dn_os_is_regular_file(const char* path) {
	auto attribute = GetFileAttributesA(path);
	if (attribute == INVALID_FILE_ATTRIBUTES) return false;
	return !(attribute & FILE_ATTRIBUTE_DIRECTORY);
}

bool dn_os_is_directory(const char* path) {
	auto attribute = GetFileAttributesA(path);
	if (attribute == INVALID_FILE_ATTRIBUTES) return false;
	return attribute & FILE_ATTRIBUTE_DIRECTORY;
}

void dn_os_remove_directory(const char* path) {
	std::filesystem::remove_all(path);
}

void dn_os_create_directory(const char* path) {
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directories(path);
	}
}


dn_os_date_time_t dn_os_get_date_time() {
  dn_os_date_time_t date_time;

  // Get the current time point
  auto now = std::chrono::system_clock::now();

  // Convert it to time_t to extract calendar time
  std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

  // Convert to tm structure for local time
  std::tm local_time = *std::localtime(&now_time_t);

  // Extract milliseconds from the current time point
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()) % 1000;

  // Fill in the dn_os_date_time_t struct
  date_time.year = local_time.tm_year + 1900;  // tm_year is years since 1900
  date_time.month = local_time.tm_mon + 1;     // tm_mon is 0-based (January is 0)
  date_time.day = local_time.tm_mday;
  date_time.hour = local_time.tm_hour;
  date_time.minute = local_time.tm_min;
  date_time.second = local_time.tm_sec;
  date_time.millisecond = static_cast<int>(milliseconds.count());

  return date_time;
}

#endif
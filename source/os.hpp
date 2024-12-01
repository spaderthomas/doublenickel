#ifndef DN_OS_H
#define DN_OS_H
typedef enum {
  DN_OS_FILE_ATTR_NONE = 0,
  DN_OS_FILE_ATTR_REGULAR_FILE = 1,
  DN_OS_FILE_ATTR_DIRECTORY = 2,
} dn_os_file_attr_t;

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int millisecond;
} dn_os_date_time_t;

typedef struct {
  dn_string_t file_path;
  dn_string_t file_name;
  dn_os_file_attr_t attributes;
} dn_os_directory_entry_t;

typedef struct {
  dn_os_directory_entry_t* data;
  u32 count;
} dn_os_directory_entry_list_t;

DN_API bool                         dn_os_does_path_exist(dn_string_t path);
DN_API bool                         dn_os_is_regular_file(dn_string_t path);
DN_API bool                         dn_os_is_directory(dn_string_t path);
DN_API void                         dn_os_create_directory(dn_string_t path);
DN_API void                         dn_os_remove_directory(dn_string_t path);
DN_API void                         dn_os_create_file(dn_string_t path);
DN_API void                         dn_os_remove_file(dn_string_t path);
DN_API dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path);
DN_API dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path);
DN_API dn_os_date_time_t            dn_os_get_date_time();
DN_API f64                          dn_os_file_mod_time(const char* path);
DN_API void                         dn_os_memory_copy(const void* source, void* dest, u32 num_bytes);
DN_API bool                         dn_os_is_memory_equal(const void* a, const void* b, size_t len);
DN_API void                         dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
DN_API void                         dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
DN_API void                         dn_os_zero_memory(void* buffer, u32 buffer_size);
DN_IMP dn_os_file_attr_t            dn_os_winapi_attr_to_dn_attr(u32 attr);


#endif

#ifdef DN_OS_IMPLEMENTATION
bool dn_os_does_path_exist(dn_string_t path) {
  std::error_code error;
  return std::filesystem::exists(dn_string_to_cstr(path), error);
}

bool dn_os_is_regular_file(dn_string_t path) {
  auto attribute = GetFileAttributesA(dn_string_to_cstr(path));
  if (attribute == INVALID_FILE_ATTRIBUTES) return false;
  return !(attribute & FILE_ATTRIBUTE_DIRECTORY);
}

bool dn_os_is_directory(dn_string_t path) {
  auto attribute = GetFileAttributesA(dn_string_to_cstr(path));
  if (attribute == INVALID_FILE_ATTRIBUTES) return false;
  return attribute & FILE_ATTRIBUTE_DIRECTORY;
}

void dn_os_remove_directory(dn_string_t path) {
  std::error_code error;
  std::filesystem::remove_all(dn_string_to_cstr(path), error);
}

void dn_os_create_directory(dn_string_t path) {
  std::error_code error;
  if (!std::filesystem::exists(dn_string_to_cstr(path), error)) {
    std::filesystem::create_directories(dn_string_to_cstr(path), error);
  }
}

void dn_os_create_file(dn_string_t path) {
  auto handle = CreateFileA(dn_string_to_cstr(path), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  CloseHandle(handle);
}

void dn_os_remove_file(dn_string_t path) {
  DeleteFileA(dn_string_to_cstr(path));
}

dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path) {
  if (!dn_os_is_directory(path) || !dn_os_does_path_exist(path)) {
    return dn_zero_initialize();
  }

  dn_dynamic_array<dn_os_directory_entry_t> entries;
  dn::dynamic_array::init(&entries, &dn_allocators.bump);

  dn_path_t glob = dn_zero_initialize();
	// @string
  snprintf(glob, DN_MAX_PATH_LEN, "%s/*", dn_string_to_cstr(path)); 

  WIN32_FIND_DATA find_data;
  auto handle = FindFirstFile(glob, &find_data);
  if (handle == INVALID_HANDLE_VALUE) {
    return dn_zero_initialize();
  }

  do {
    if (!strcmp(find_data.cFileName, ".")) continue;
    if (!strcmp(find_data.cFileName, "..")) continue;
    
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump };
    dn_string_builder_append(&builder, path);
    dn_string_builder_append_cstr(&builder, "/");
    dn_string_builder_append_cstr(&builder, find_data.cFileName);
    dn_string_t file_path = dn_string_builder_write(&builder);
    dn::dynamic_array::push(&entries, {
      .file_path = file_path,
      .file_name = dn_string_copy_cstr(find_data.cFileName, &dn_allocators.bump),
      .attributes = dn_os_winapi_attr_to_dn_attr(GetFileAttributesA(dn_string_builder_write_cstr(&builder))),
    });
  } while (FindNextFile(handle, &find_data));

  FindClose(handle);

  return {
    .data = (dn_os_directory_entry_t*)entries.data,
    .count = entries.size
  };
}

dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path) {
  if (!dn_os_is_directory(path) || !dn_os_does_path_exist(path)) {
    return dn_zero_initialize();
  }

  // A queue of directories and child directories to scan
  dn_dynamic_array<dn_os_directory_entry_t> directory_queue;
  dn::dynamic_array::init(&directory_queue, &dn_allocators.bump);
  dn::dynamic_array::push(&directory_queue, {
    .file_path = path,
    .file_name = dn_zero_initialize(),
    .attributes = DN_OS_FILE_ATTR_DIRECTORY
  });

  // The final list of all files
  dn_dynamic_array<dn_os_directory_entry_t> result;
  dn::dynamic_array::init(&result, &dn_allocators.bump);

  // Loop through the directory queue and process all files in that directory; child directories will
  // push more directories onto the queue
  u32 process_index = 0;
  do {
    auto directory = dn::dynamic_array::at(&directory_queue, process_index);
    auto directory_entries = dn_os_scan_directory(directory->file_path);
    dn_for(i, directory_entries.count) {
      dn_os_directory_entry_t& entry = directory_entries.data[i];
      if (entry.attributes & DN_OS_FILE_ATTR_DIRECTORY) {
        dn::dynamic_array::push(&directory_queue, entry);
      }
      else if (entry.attributes & DN_OS_FILE_ATTR_REGULAR_FILE) {
        dn::dynamic_array::push(&result, entry);
      }
      else {
        DN_UNREACHABLE();
      }
    }
  } while (directory_queue.size > ++process_index);

  return {
    .data = (dn_os_directory_entry_t*)result.data,
    .count = result.size
  };
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

f64 dn_os_file_mod_time(const char* file_path) {
  std::error_code error;
  auto file_mod_time = std::filesystem::last_write_time(file_path, error);
  auto file_mod_time_s = std::chrono::time_point_cast<std::chrono::seconds>(file_mod_time);
  double file_mod_time_epoch = file_mod_time_s.time_since_epoch().count();
  
  return file_mod_time_epoch;
}

dn_os_file_attr_t dn_os_winapi_attr_to_dn_attr(u32 attr) {
  u32 result = DN_OS_FILE_ATTR_NONE;
  if ( (attr & FILE_ATTRIBUTE_DIRECTORY)) result |= DN_OS_FILE_ATTR_DIRECTORY;
  if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) result |= DN_OS_FILE_ATTR_REGULAR_FILE;
  return (dn_os_file_attr_t)result;
}

bool dn_os_is_memory_equal(const void* a, const void* b, size_t len) {
    return !memcmp(a, b, len);
}

void dn_os_memory_copy(const void* source, void* dest, u32 num_bytes) {
    std::memcpy(dest, source, num_bytes);
}

void dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size) {
  u8* current_byte = (u8*)buffer;

  int i = 0;
  while (true) {
    if (i + fill_size > buffer_size) return;
    memcpy(current_byte + i, (u8*)fill, fill_size);
    i += fill_size;
  }
}

void dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill) {
  dn_os_fill_memory(buffer, buffer_size, &fill, sizeof(u8));
}

void dn_os_zero_memory(void* buffer, u32 buffer_size) {
  dn_os_fill_memory_u8(buffer, buffer_size, 0);
}
#endif
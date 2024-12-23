#ifndef DN_FILE_MONITOR_H
#define DN_FILE_MONITOR_H

typedef enum {
	DN_FILE_CHANGE_EVENT_NONE = 0,
	DN_FILE_CHANGE_EVENT_ADDED = 1 << 0,
	DN_FILE_CHANGE_EVENT_MODIFIED = 1 << 1,
	DN_FILE_CHANGE_EVENT_REMOVED = 1 << 2,
} dn_file_change_event_t;
DEFINE_ENUM_FLAG_OPERATORS(dn_file_change_event_t);

struct FileChange {
	char* file_path;
	char* file_name;
	dn_file_change_event_t events;
	f32 time;
};

struct FileMonitor;
typedef void(*FileChangeCallback)(FileMonitor*, FileChange*, void*);

struct FileMonitor {
	struct DirectoryInfo {
		char* path;
		OVERLAPPED overlapped;
		HANDLE handle;
		void* notify_information;
		i32 bytes_returned;
	};

	struct CacheEntry {
		dn_hash_t hash;
		f64 last_event_time = 0;
	};

	static constexpr i32 BUFFER_SIZE = 4092;
	
	FileChangeCallback callback;
	dn_file_change_event_t events_to_watch;
	void* userdata;
	f64 debounce_time = .1;
	dn_array_t<DirectoryInfo, 128> directory_infos;
	dn_array_t<FileChange, 16> changes;
	dn_array_t<CacheEntry, 512> cache;

	void init(FileChangeCallback callback, dn_file_change_event_t events, void* userdata);
	bool add_directory(const char* path);
	bool add_file(const char* file_path);
	void process_changes();
	void issue_one_read(DirectoryInfo* info);
	void emit_changes();
	void add_change(char* file_path, dn_string_t file_name, dn_file_change_event_t events);
	bool check_cache(char* file_path, f64 time);
	CacheEntry* find_cache_entry(char* file_path);
};

typedef struct {
	dn_array_t<FileMonitor, 64> monitors;
} dn_file_monitors_t;
dn_file_monitors_t dn_file_monitors;


DN_IMP void         dn_file_monitors_init();
DN_IMP void         dn_file_monitors_update();
DN_API FileMonitor* dn_file_monitors_add();

#endif

#ifdef DN_FILE_MONITOR_IMPLEMENTATION
void FileMonitor::init(FileChangeCallback callback, dn_file_change_event_t events, void* userdata) {
	this->callback = callback;
	this->events_to_watch = events;
	this->userdata = userdata;
	dn_array_init(&this->directory_infos);
	dn_array_init(&this->changes);
	dn_array_init(&this->cache);
}

bool FileMonitor::add_directory(const char* directory_path) {
#if defined(DN_EDITOR)
	dn_log_flags(DN_LOG_FLAG_FILE, "%s: added %s", __func__, directory_path);
				   
	auto event = CreateEventW(NULL, false, false, NULL);
	if (!event) return false;
	
	auto handle = CreateFileA(directory_path, FILE_LIST_DIRECTORY,
							  FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
							  NULL,
							  OPEN_EXISTING,
							  FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
							  NULL);
	
	if (handle == INVALID_HANDLE_VALUE) {
		CloseHandle(event);
		return false;
	}

	auto info = dn_array_push(&directory_infos);
	info->overlapped.hEvent = event;
	info->overlapped.Offset = 0;
	info->handle = handle;
	info->path = (char*)malloc(DN_MAX_PATH_LEN * sizeof(char));
	strncpy(info->path, directory_path, DN_MAX_PATH_LEN);
	info->notify_information = calloc(BUFFER_SIZE, 1);

	issue_one_read(info);
#endif

	return true;
}

bool FileMonitor::add_file(const char* file_path) {
    dn_log_flags(DN_LOG_FLAG_FILE, "%s: added %s", __func__, file_path);
    
    // Create an event for overlapped I/O
    auto event = CreateEventW(NULL, false, false, NULL);
    if (!event) return false;
    
    // Open the file for monitoring
    auto handle = CreateFileA(file_path, FILE_GENERIC_READ,
                              FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
                              NULL,
                              OPEN_EXISTING,
                              FILE_FLAG_OVERLAPPED,
                              NULL);
    
    if (handle == INVALID_HANDLE_VALUE) {
        CloseHandle(event);
        return false;
    }

    // Push the file info into the monitoring list
    auto info = dn_array_push(&directory_infos);
    info->overlapped.hEvent = event;
    info->overlapped.Offset = 0;
    info->handle = handle;
    info->path = (char*)malloc(DN_MAX_PATH_LEN * sizeof(char));
    strncpy(info->path, file_path, DN_MAX_PATH_LEN);
    info->notify_information = calloc(BUFFER_SIZE, 1);

    // Issue the first read to monitor changes
    issue_one_read(info);

    return true;
}

void FileMonitor::issue_one_read(DirectoryInfo* info) {
	DN_ASSERT(info->handle != INVALID_HANDLE_VALUE);

	i32 notify_filter = 0;
	if (this->events_to_watch & (DN_FILE_CHANGE_EVENT_ADDED | DN_FILE_CHANGE_EVENT_REMOVED)) {
		notify_filter |= FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_CREATION;
	}
	if (this->events_to_watch & DN_FILE_CHANGE_EVENT_MODIFIED) {
		notify_filter |= FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE;
	}

	info->bytes_returned = 0;

	auto success = ReadDirectoryChangesW(info->handle, info->notify_information, BUFFER_SIZE, true, notify_filter, NULL, &info->overlapped, NULL);
	//if (!success) log_something();
}

void FileMonitor::process_changes() {
	dn_array_for(this->directory_infos, info) {
		DN_ASSERT(info->handle != INVALID_HANDLE_VALUE);

		if (!HasOverlappedIoCompleted(&info->overlapped)) continue;

		i32 bytes_written = 0;
		bool success = GetOverlappedResult(info->handle, &info->overlapped, (LPDWORD) & bytes_written, false);
		if (!success || bytes_written == 0) break;

		auto notify = (FILE_NOTIFY_INFORMATION*)info->notify_information;
		while (true) {
			// Parse this notification
			dn_file_change_event_t events = DN_FILE_CHANGE_EVENT_NONE;
			if (notify->Action == FILE_ACTION_MODIFIED) {
				events = DN_FILE_CHANGE_EVENT_MODIFIED;
			}
			else if (notify->Action == FILE_ACTION_ADDED) {
				events = DN_FILE_CHANGE_EVENT_ADDED;
			}
			else if (notify->Action == FILE_ACTION_REMOVED) {
				events = DN_FILE_CHANGE_EVENT_REMOVED;
			}
			else if (notify->Action == FILE_ACTION_RENAMED_OLD_NAME) {
				
			}
			else if (notify->Action == FILE_ACTION_RENAMED_NEW_NAME) {

			}
			else {
				continue;
			}

			// Construct the full path
			char* full_path = dn::allocator::alloc<char>(&dn_allocators.bump, DN_MAX_PATH_LEN);
			char* partial_path = dn_string_16_to_8((u16*)&notify->FileName[0], notify->FileNameLength / 2);
			snprintf(full_path, DN_MAX_PATH_LEN, "%s/%s", info->path, partial_path);
			dn_path_normalize_cstr(full_path);
			dn_string_t file_name = dn_path_extract_file_name(full_path);

			this->add_change(full_path, file_name, events);

			// Advance to the next notification
			if (notify->NextEntryOffset == 0) break;
			notify = (FILE_NOTIFY_INFORMATION*)((char*)notify + notify->NextEntryOffset);
		}

		issue_one_read(info);
	}

	emit_changes();
}

void FileMonitor::add_change(char* file_path, dn_string_t file_name, dn_file_change_event_t events) {
	auto time = glfwGetTime();

	// We don't care about directory updates. They're annoying and hard to understand
	// on Windows, and file updates give us everything we need.
	if (dn_os_is_directory(dn_string_literal(file_path))) return;

	// Exclude some annoying files
	if (file_name.data) {
		if (file_name.data[0] == '.' && file_name.data[1] == '#') return;
		if (file_name.data[0] ==  '#') return;
	}

	// We need to debounce duplicate changes. Here's a good explanation of why Windows
	// is incapable of telling us that a file changed in a sane way:
	// https://stackoverflow.com/a/14040978/6847023
	if (!check_cache(file_path, time)) return;

	dn_array_for(this->changes, change) {
		if (!strcmp(change->file_path, file_path)) {
			// De-duplicate this change
			change->events |= events;
			change->time = time;
			return;
		}
	}

	auto change = dn_array_push(&this->changes);
	change->file_path = file_path;
	change->file_name = dn_string_to_cstr_ex(file_name, &dn_allocators.bump);
	change->events = events;
	change->time = time;
}


void FileMonitor::emit_changes() {
	dn_array_for(this->changes, change) {
		this->callback(this, change, this->userdata);
		}

	dn_array_clear(&this->changes);
}

FileMonitor::CacheEntry* FileMonitor::find_cache_entry(char* file_path) {
	dn_hash_t file_hash = dn_hash_cstr_dumb(file_path);
	
	CacheEntry* found = nullptr;
	dn_array_for(this->cache, entry) {
		if (entry->hash == file_hash) {
			found = entry;
			break;
		}
	}

	if (!found) {
		found = dn_array_push(&this->cache);
		found->hash = dn_hash_cstr_dumb(file_path);
	}
	
	return found;
}

bool FileMonitor::check_cache(char* file_path, f64 time) {
	auto entry = this->find_cache_entry(file_path);
	auto delta = time - entry->last_event_time;
	entry->last_event_time = time;

	return delta > this->debounce_time;
}



void dn_file_monitors_init() {
	dn_log("%s", __func__);
	dn_array_init(&dn_file_monitors.monitors);
}

void dn_file_monitors_update() {
	dn_array_for(dn_file_monitors.monitors, monitor) {
		monitor->process_changes();
	}
}

FileMonitor* dn_file_monitors_add() {
	return dn_array_push(&dn_file_monitors.monitors);
}
#endif
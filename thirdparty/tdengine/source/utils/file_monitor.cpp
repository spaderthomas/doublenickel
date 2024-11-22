void FileMonitor::init(FileChangeCallback callback, FileChangeEvent events, void* userdata) {
	this->callback = callback;
	this->events_to_watch = events;
	this->userdata = userdata;
	arr_init(&this->directory_infos, 128);
	arr_init(&this->changes, 16);
	arr_init(&this->cache, 512);
}

bool FileMonitor::add_directory(const char* directory_path) {
#if defined(FM_EDITOR)
	tdns_log.write(Log_Flags::File, "%s: added %s", __func__, directory_path);
				   
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

	auto info = arr_push(&directory_infos);
	info->overlapped.hEvent = event;
	info->overlapped.Offset = 0;
	info->handle = handle;
	info->path = (char*)malloc(MAX_PATH_LEN * sizeof(char));
	strncpy(info->path, directory_path, MAX_PATH_LEN);
	info->notify_information = calloc(BUFFER_SIZE, 1);

	issue_one_read(info);
#endif

	return true;
}

bool FileMonitor::add_file(const char* file_path) {
    tdns_log.write(Log_Flags::File, "%s: added %s", __func__, file_path);
    
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
    auto info = arr_push(&directory_infos);
    info->overlapped.hEvent = event;
    info->overlapped.Offset = 0;
    info->handle = handle;
    info->path = (char*)malloc(MAX_PATH_LEN * sizeof(char));
    strncpy(info->path, file_path, MAX_PATH_LEN);
    info->notify_information = calloc(BUFFER_SIZE, 1);

    // Issue the first read to monitor changes
    issue_one_read(info);

    return true;
}

void FileMonitor::issue_one_read(DirectoryInfo* info) {
	fm_assert(info->handle != INVALID_HANDLE_VALUE);

	int32 notify_filter = 0;
	if (enum_any(this->events_to_watch & (FileChangeEvent::Added | FileChangeEvent::Removed))) {
		notify_filter |= FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_CREATION;
	}
	if (enum_any(this->events_to_watch & FileChangeEvent::Modified)) {
		notify_filter |= FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE;
	}

	info->bytes_returned = 0;

	auto success = ReadDirectoryChangesW(info->handle, info->notify_information, BUFFER_SIZE, true, notify_filter, NULL, &info->overlapped, NULL);
	//if (!success) log_something();
}

void FileMonitor::process_changes() {
	arr_for(this->directory_infos, info) {
		fm_assert(info->handle != INVALID_HANDLE_VALUE);

		if (!HasOverlappedIoCompleted(&info->overlapped)) continue;

		int32 bytes_written = 0;
		bool success = GetOverlappedResult(info->handle, &info->overlapped, (LPDWORD) & bytes_written, false);
		if (!success || bytes_written == 0) break;

		auto notify = (FILE_NOTIFY_INFORMATION*)info->notify_information;
		while (true) {
			// Parse this notification
			FileChangeEvent events = FileChangeEvent::None;
			if (notify->Action == FILE_ACTION_MODIFIED) {
				events = FileChangeEvent::Modified;
			}
			else if (notify->Action == FILE_ACTION_ADDED) {
				events = FileChangeEvent::Added;
			}
			else if (notify->Action == FILE_ACTION_REMOVED) {
				events = FileChangeEvent::Removed;
			}
			else if (notify->Action == FILE_ACTION_RENAMED_OLD_NAME) {
				
			}
			else if (notify->Action == FILE_ACTION_RENAMED_NEW_NAME) {

			}
			else {
				continue;
			}

			// Construct the full path
			char* full_path = bump_allocator.alloc_path();
			char* partial_path = wide_to_utf8((uint16*)&notify->FileName[0], notify->FileNameLength / 2);
			snprintf(full_path, MAX_PATH_LEN, "%s/%s", info->path, partial_path);
			normalize_path(full_path);
			char* file_name = extract_file_name(full_path);

			this->add_change(full_path, file_name, events);

			// Advance to the next notification
			if (notify->NextEntryOffset == 0) break;
			notify = (FILE_NOTIFY_INFORMATION*)((char*)notify + notify->NextEntryOffset);
		}

		issue_one_read(info);
	}

	emit_changes();
}

void FileMonitor::add_change(char* file_path, char* file_name, FileChangeEvent events) {
	auto time = glfwGetTime();

	// We don't care about directory updates. They're annoying and hard to understand
	// on Windows, and file updates give us everything we need.
	if (is_directory(file_path)) return;

	// Exclude some annoying files
	if (file_name) {
		if (file_name[0] == '.' && file_name[1] == '#') return;
		if (file_name[0] ==  '#') return;
	}

	// We need to debounce duplicate changes. Here's a good explanation of why Windows
	// is incapable of telling us that a file changed in a sane way:
	// https://stackoverflow.com/a/14040978/6847023
	if (!check_cache(file_path, time)) return;

	arr_for(this->changes, change) {
		if (!strcmp(change->file_path, file_path)) {
			// De-duplicate this change
			change->events |= events;
			change->time = time;
			return;
		}
	}

	auto change = arr_push(&this->changes);
	change->file_path = file_path;
	change->file_name = file_name;
	change->events = events;
	change->time = time;
}


void FileMonitor::emit_changes() {
	arr_for(this->changes, change) {
		this->callback(this, change, this->userdata);
		}

	arr_clear(&this->changes);
}

FileMonitor::CacheEntry* FileMonitor::find_cache_entry(char* file_path) {
	hash_t file_hash = hash_label(file_path);
	
	CacheEntry* found = nullptr;
	arr_for(this->cache, entry) {
		if (entry->hash == file_hash) {
			found = entry;
			break;
		}
	}

	if (!found) {
		found = arr_push(&this->cache);
		found->hash = hash_label(file_path);
	}
	
	return found;
}

bool FileMonitor::check_cache(char* file_path, float64 time) {
	auto entry = this->find_cache_entry(file_path);
	auto delta = time - entry->last_event_time;
	entry->last_event_time = time;

	return delta > this->debounce_time;
}



void init_file_monitors() {
	tdns_log.write(Log_Flags::File, "initializing file monitors");
	arr_init(&file_monitors, 64);
}

void update_file_monitors() {
	arr_for(file_monitors, monitor) {
		monitor->process_changes();
	}
}

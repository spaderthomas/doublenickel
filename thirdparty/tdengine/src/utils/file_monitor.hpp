enum class FileChangeEvent {
	None              = 0,
	Added             = 1 << 0,
	Modified          = 1 << 1,
	Removed           = 1 << 2,
};
DEFINE_ENUM_FLAG_OPERATORS(FileChangeEvent)

struct FileChange {
	char* file_path;
	char* file_name;
	FileChangeEvent events;
	float32 time;
};

struct FileMonitor;
typedef void(*FileChangeCallback)(FileMonitor*, FileChange*, void*);

struct FileMonitor {
	struct DirectoryInfo {
		char* path;
		OVERLAPPED overlapped;
		HANDLE handle;
		void* notify_information;
		int32 bytes_returned;
	};

	struct CacheEntry {
		hash_t hash;
		float64 last_event_time = 0;
	};

	static constexpr int32 BUFFER_SIZE = 4092;
	
	FileChangeCallback callback;
	FileChangeEvent events_to_watch;
	void* userdata;
	float64 debounce_time = .1;
	Array<DirectoryInfo> directory_infos;
	Array<FileChange> changes;
	Array<CacheEntry> cache;

	void init(FileChangeCallback callback, FileChangeEvent events, void* userdata);
	bool add_directory(const char* path);
	bool add_file(const char* file_path);
	void process_changes();
	void issue_one_read(DirectoryInfo* info);
	void emit_changes();
	void add_change(char* file_path, char* file_name, FileChangeEvent events);
	bool check_cache(char* file_path, float64 time);
	CacheEntry* find_cache_entry(char* file_path);
};

Array<FileMonitor> file_monitors;

void init_file_monitors();
void update_file_monitors();

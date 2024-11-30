#ifndef DN_LOG_H
#define DN_LOG_H
typedef enum {
	DN_LOG_FLAG_CONSOLE = 1,
	DN_LOG_FLAG_FILE = 2,
	DN_LOG_FLAG_DEFAULT = 3,
} dn_log_flags_t;

#define DN_LOGGER_MESSAGE_BUFFER_SIZE 4096
#define DN_LOGGER_PREAMBLE_BUFFER_SIZE 512
typedef struct {
	char* file_path;
	char message_buffer [DN_LOGGER_MESSAGE_BUFFER_SIZE];
	char preamble_buffer [DN_LOGGER_PREAMBLE_BUFFER_SIZE];
} dn_log_t;
dn_log_t dn_logger;

DN_API void dn_log(const char* fmt, ...);
DN_API void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...);
DN_IMP void dn_log_v(dn_log_flags_t flags, const char* fmt, va_list fmt_args);
DN_IMP void dn_log_zero();
DN_IMP void dn_log_init();
#endif

#ifdef DN_LOG_IMPLEMENTATION
void dn_log_init() {
	dn_logger = dn_zero_initialize();
	dn_logger.file_path = dn_paths_resolve_ex("dn_log", &dn_allocators.standard);
	
	FILE* file = fopen(dn_logger.file_path, "w");
	if (!file) {
		printf("Could not open log file during initialization; file_path = %s", dn_logger.file_path);
		return;
	}
	
	fclose(file);

	// We have to initialize paths before we can create the log file, so make sure that as soon as the
	// file is setup that we log the base directories we're running out of
	dn_log("install directory = %s", dn_paths_resolve("install"));
	dn_log("write directory = %s", dn_paths_resolve("write"));
}

void dn_log(const char* fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	dn_log_v(DN_LOG_FLAG_DEFAULT, fmt, fmt_args);
	va_end(fmt_args);
}

void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	dn_log_v(flags, fmt, fmt_args);
	va_end(fmt_args);
}

void dn_log_v(dn_log_flags_t flags, const char* fmt, va_list fmt_args) {
	uint64 ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto sec_since_epoch = std::time_t(ms_since_epoch / 1000);
	auto time_info = std::localtime(&sec_since_epoch);

	snprintf(dn_logger.preamble_buffer, DN_LOGGER_PREAMBLE_BUFFER_SIZE, "[%04d-%02d-%02d %02d:%02d:%02d.%03lld]",
			 1900 + time_info->tm_year, 1 + time_info->tm_mon, time_info->tm_mday,
			 time_info->tm_hour, time_info->tm_min, time_info->tm_sec, ms_since_epoch % 1000);
	
	vsnprintf(&dn_logger.message_buffer[0], DN_LOGGER_MESSAGE_BUFFER_SIZE, fmt, fmt_args);
	
	if (flags & DN_LOG_FLAG_CONSOLE) { 
		printf("%s %s\n", dn_logger.preamble_buffer, dn_logger.message_buffer); 
	}
	if (flags & DN_LOG_FLAG_FILE && dn_logger.file_path) { 
		auto file = fopen(dn_logger.file_path, "a");
		fprintf(file, "%s %s\n", dn_logger.preamble_buffer, dn_logger.message_buffer); 
		fclose(file);
	}

	dn_log_zero();
}

void dn_log_zero() {
	memset(&dn_logger.preamble_buffer[0], 0, DN_LOGGER_PREAMBLE_BUFFER_SIZE);
	memset(&dn_logger.message_buffer[0], 0, DN_LOGGER_MESSAGE_BUFFER_SIZE);
}

#endif
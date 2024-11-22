void Log::zero_buffer() {
	memset(&preamble_buffer[0], 0, preamble_buffer_size);
	memset(&message_buffer[0], 0, message_buffer_size);
}

void Log::write(const char* fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	write_impl(Log_Flags::Default, fmt, fmt_args);
	va_end(fmt_args);
}

void Log::write(uint8_t flags, const char* fmt, ...) {
	va_list fmt_args;
	va_start(fmt_args, fmt);
	write_impl(flags, fmt, fmt_args);
	va_end(fmt_args);
}

void Log::write_impl(uint8_t flags, const char* fmt, va_list fmt_args) {
	uint64 ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	auto sec_since_epoch = std::time_t(ms_since_epoch / 1000);
	auto time_info = std::localtime(&sec_since_epoch);

	snprintf(preamble_buffer, preamble_buffer_size, "[%04d-%02d-%02d %02d:%02d:%02d.%03lld]",
			 1900 + time_info->tm_year, 1 + time_info->tm_mon, time_info->tm_mday,
			 time_info->tm_hour, time_info->tm_min, time_info->tm_sec, ms_since_epoch % 1000);
	
	vsnprintf(&message_buffer[0], message_buffer_size, fmt, fmt_args);
	
	if (flags & Log_Flags::Console) { 
		printf("%s %s\n", preamble_buffer, message_buffer); 
	}
	if (flags & Log_Flags::File && file_path) { 
		auto file = fopen(file_path, "a");
		fprintf(file, "%s %s\n", preamble_buffer, message_buffer); 
		fclose(file);
	}

	zero_buffer();
}

void init_log() {
	tdns_log.file_path = resolve_named_path_ex("log", &standard_allocator);
	
	FILE* file = fopen(tdns_log.file_path, "w");
	if (!file) {
		printf("Could not open log file during initialization; file_path = %s", tdns_log.file_path);
		return;
	}
	
	fclose(file);
	tdns_log.zero_buffer();

	// We have to initialize paths before we can create the log file, so make sure that as soon as the
	// file is setup that we log the base directories we're running out of
	tdns_log.write("install directory = %s", resolve_named_path("install"));
	tdns_log.write("write directory = %s", resolve_named_path("write"));
}

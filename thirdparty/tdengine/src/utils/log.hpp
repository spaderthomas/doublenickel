namespace Log_Flags {
	uint8_t Console = 1;
	uint8_t File    = 2;
	uint8_t Default = 3;
};

struct Log {
	static constexpr int32 message_buffer_size = 2048;
	static constexpr int32 preamble_buffer_size = 256;
	
	char* file_path;
	char message_buffer [message_buffer_size];
	char preamble_buffer [preamble_buffer_size];
	
	void write(const char* fmt, ...);
	void write(uint8_t flags, const char* fmt, ...);
	void write_fmt(const char* fmt, ...);
	void write_impl(uint8_t flags, const char* fmt, va_list fmt_args);
	void zero_buffer();
};

Log tdns_log;

void init_log();

struct TimeMetric {
	RingBuffer<double> queue;
	double time_begin;

	void init();
	void begin();
	void end();
	void busy_wait(double target);
	void sleep_wait(double target);
	double get_average();
	double get_last();
	double get_largest();
	double get_smallest();
};

std::unordered_map<std::string, TimeMetric> time_metrics;

void init_time();
void update_time();

FM_LUA_EXPORT void set_target_fps(double fps);
FM_LUA_EXPORT double get_target_fps();

FM_LUA_EXPORT void tm_add(const char* name);
FM_LUA_EXPORT void tm_begin(const char* name);
FM_LUA_EXPORT void tm_end(const char* name);
FM_LUA_EXPORT double tm_average(const char* name);
FM_LUA_EXPORT double tm_last(const char* name);
FM_LUA_EXPORT double tm_largest(const char* name);
FM_LUA_EXPORT double tm_smallest(const char* name);

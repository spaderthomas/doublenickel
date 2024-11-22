void init_time() {
	set_target_fps(144);
	tm_add("frame");
}

void update_time() {
	auto& frame_timer = time_metrics["frame"];
	frame_timer.end();
	frame_timer.busy_wait(engine.dt);
}

void TimeMetric::init() {
	rb_init(&this->queue, 64);
}

void TimeMetric::begin() {
	this->time_begin = glfwGetTime();
}

void TimeMetric::end() {
	auto time_end = glfwGetTime();
	auto delta = time_end - this->time_begin;
	rb_push_overwrite(&this->queue, delta);
}

float64 TimeMetric::get_average() {
	if (!queue.size) return 0;
	
	float64 total = 0;
	rb_for(queue, entry) {
		total += **entry;
	}

	return total / queue.size;
}

float64 TimeMetric::get_last() {
	if (queue.size) return *rb_back(&queue);
	return 0;
}

double TimeMetric::get_largest() {
	double max_entry = 0;
	rb_for(queue, entry) {
		max_entry = std::max(max_entry, **entry);
	}

	return max_entry;
}

double TimeMetric::get_smallest() {
	double min_entry = std::numeric_limits<double>::max();
	rb_for(queue, entry) {
		min_entry = std::min(min_entry, **entry);
	}

	return min_entry;
}


void TimeMetric::sleep_wait(float64 target) {
	while (true) {
		float64 delta = glfwGetTime() - this->time_begin;
		if (delta >= target) break;

		double remaining_time = target - delta;
		if (remaining_time > 0) {
			std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(remaining_time * 1e6)));
		}
	}
}

void TimeMetric::busy_wait(float64 target) {
	while (true) {
		auto delta = glfwGetTime() - this->time_begin;
		if (delta >= target) break;
	}
}

void set_target_fps(float64 fps) {
	engine.target_fps = fps;
	engine.dt = 1.f / fps;
}

double get_target_fps() {
	return engine.target_fps;
}

void tm_add(const char* name) {
	TimeMetric time_metric;
	time_metric.init();
	time_metrics[name] = time_metric;
}

void tm_begin(const char* name) {
	auto& time_metric = time_metrics[name];
	time_metric.begin();
}

void tm_end(const char* name) {
	auto& time_metric = time_metrics[name];
	time_metric.end();
}

double tm_average(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_average();
}

double tm_last(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_last();
}

double tm_largest(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_largest();
}

double tm_smallest(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_smallest();
}

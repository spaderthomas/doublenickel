#ifndef DN_TIME_METRICS_H
#define DN_TIME_METRICS_H
struct dn_time_metric_t {
	dn_ring_buffer_t<double> queue;
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

std::unordered_map<std::string, dn_time_metric_t> time_metrics;

void dn_time_metrics_init();
void dn_time_metrics_update();


DN_API void dn_time_metric_add(const char* name);
DN_API void dn_time_metric_begin(const char* name);
DN_API void dn_time_metric_end(const char* name);
DN_API double dn_time_metric_average(const char* name);
DN_API double dn_time_metric_last(const char* name);
DN_API double dn_time_metric_largest(const char* name);
DN_API double dn_time_metric_smallest(const char* name);
#endif



#ifdef DN_TIME_METRICS_IMPLEMENTATION
void dn_time_metrics_init() {
	dn_time_metric_add("frame");
}

void dn_time_metrics_update() {
	auto& frame_timer = time_metrics["frame"];
	frame_timer.end();
	frame_timer.busy_wait(engine.dt);
}

void dn_time_metric_t::init() {
	dn_ring_buffer_init(&this->queue, 64);
}

void dn_time_metric_t::begin() {
	this->time_begin = glfwGetTime();
}

void dn_time_metric_t::end() {
	auto time_end = glfwGetTime();
	auto delta = time_end - this->time_begin;
	dn_ring_buffer_push_overwrite(&this->queue, delta);
}

f64 dn_time_metric_t::get_average() {
	if (!queue.size) return 0;
	
	f64 total = 0;
	dn_ring_buffer_for(queue, entry) {
		total += **entry;
	}

	return total / queue.size;
}

f64 dn_time_metric_t::get_last() {
	if (queue.size) return *dn_ring_buffer_back(&queue);
	return 0;
}

double dn_time_metric_t::get_largest() {
	double max_entry = 0;
	dn_ring_buffer_for(queue, entry) {
		max_entry = std::max(max_entry, **entry);
	}

	return max_entry;
}

double dn_time_metric_t::get_smallest() {
	double min_entry = std::numeric_limits<double>::max();
	dn_ring_buffer_for(queue, entry) {
		min_entry = std::min(min_entry, **entry);
	}

	return min_entry;
}


void dn_time_metric_t::sleep_wait(f64 target) {
	while (true) {
		f64 delta = glfwGetTime() - this->time_begin;
		if (delta >= target) break;

		double remaining_time = target - delta;
		if (remaining_time > 0) {
			std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(remaining_time * 1e6)));
		}
	}
}

void dn_time_metric_t::busy_wait(f64 target) {
	while (true) {
		auto delta = glfwGetTime() - this->time_begin;
		if (delta >= target) break;
	}
}

void dn_time_metric_add(const char* name) {
	dn_time_metric_t time_metric;
	time_metric.init();
	time_metrics[name] = time_metric;
}

void dn_time_metric_begin(const char* name) {
	auto& time_metric = time_metrics[name];
	time_metric.begin();
}

void dn_time_metric_end(const char* name) {
	auto& time_metric = time_metrics[name];
	time_metric.end();
}

double dn_time_metric_average(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_average();
}

double dn_time_metric_last(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_last();
}

double dn_time_metric_largest(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_largest();
}

double dn_time_metric_smallest(const char* name) {
	auto& time_metric = time_metrics[name];
	return time_metric.get_smallest();
}
#endif
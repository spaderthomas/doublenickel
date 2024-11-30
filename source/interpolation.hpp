///////////////
// UTILITIES //
///////////////
enum class InterpolationFn {
	Linear,
	SmoothDamp,
};

float interpolate_linear(float a, float b, float t) {
	return ((1 - t) * a) + (t * b);
}

dn_vector2_t interpolate_linear2(dn_vector2_t a, dn_vector2_t b, float t) {
	return { interpolate_linear(a.x, b.x, t), interpolate_linear(a.y, b.y, t) };
}


//////////////////
// INTERPOLATOR //
//////////////////
struct Interpolator {
	float start;
	float target;
	float progress = 0.f;
	
	float speed = 1.f;
	float epsilon = 0.01f;
	InterpolationFn function = InterpolationFn::Linear;

	void update();
	void reset();
	bool is_done();
	float get_value();

	void set_start(float start);
	void set_target(float target);
	void set_speed(float speed);
	void set_duration(float duration);
};

void Interpolator::update() {
	this->progress += engine.dt * this->speed;
	this->progress = std::min(this->progress, 1.f);
}

void Interpolator::reset() {
	this->progress = 0.f;
}

bool Interpolator::is_done() {
	return this->progress == 1.f;
}

float Interpolator::get_value() {
	if (this->function == InterpolationFn::Linear) {
		return interpolate_linear(this->start, this->target, this->progress);
	}

	return interpolate_linear(this->start, this->target, this->progress);
}

void Interpolator::set_start(float start) {
	this->start = start;
}

void Interpolator::set_target(float target) {
	this->target = target;
}

void Interpolator::set_speed(float speed) {
	this->speed = speed;
}

void Interpolator::set_duration(float duration) {
	this->speed = 1.f / duration ;
}


////////////////////
// INTERPOLATOR 2 //
////////////////////
struct Interpolator2 {
	dn_vector2_t start;
	dn_vector2_t target;
	float progress = 0.f;
	
	float speed = 1.f;
	float epsilon = 0.01f;
	InterpolationFn function = InterpolationFn::Linear;

	void update();
	void reset();
	bool is_done();
	dn_vector2_t get_value();

	void set_start(dn_vector2_t start);
	void set_target(dn_vector2_t target);
	void set_speed(float speed);
	void set_duration(float duration);
};

void Interpolator2::update() {
	this->progress += engine.dt * this->speed;
	this->progress = std::min(this->progress, 1.f);
}

void Interpolator2::reset() {
	this->progress = 0.f;
}

bool Interpolator2::is_done() {
	return this->progress == 1.f;
}

dn_vector2_t Interpolator2::get_value() {
	if (this->function == InterpolationFn::Linear) {
		return interpolate_linear2(this->start, this->target, this->progress);
	}
	
	return interpolate_linear2(this->start, this->target, this->progress);
}

void Interpolator2::set_start(dn_vector2_t start) {
	this->start = start;
}

void Interpolator2::set_target(dn_vector2_t target) {
	this->target = target;
}

void Interpolator2::set_speed(float speed) {
	this->speed = speed;
}

void Interpolator2::set_duration(float duration) {
	this->speed = 1.f / duration ;
}

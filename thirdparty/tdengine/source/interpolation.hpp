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

Vector2 interpolate_linear2(Vector2 a, Vector2 b, float t) {
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
	Vector2 start;
	Vector2 target;
	float progress = 0.f;
	
	float speed = 1.f;
	float epsilon = 0.01f;
	InterpolationFn function = InterpolationFn::Linear;

	void update();
	void reset();
	bool is_done();
	Vector2 get_value();

	void set_start(Vector2 start);
	void set_target(Vector2 target);
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

Vector2 Interpolator2::get_value() {
	if (this->function == InterpolationFn::Linear) {
		return interpolate_linear2(this->start, this->target, this->progress);
	}
	
	return interpolate_linear2(this->start, this->target, this->progress);
}

void Interpolator2::set_start(Vector2 start) {
	this->start = start;
}

void Interpolator2::set_target(Vector2 target) {
	this->target = target;
}

void Interpolator2::set_speed(float speed) {
	this->speed = speed;
}

void Interpolator2::set_duration(float duration) {
	this->speed = 1.f / duration ;
}

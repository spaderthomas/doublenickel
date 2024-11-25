#ifndef DN_ENGINE_H
#define DN_ENGINE_H
struct Engine {
	float32 target_fps;
	float32 dt;
	float32 elapsed_time;
	int32 frame = 0;
	bool exit_game = false;

	bool steam = false;
};
Engine engine;

void dn_engine_init();
void dn_engine_update();

DN_API void dn_engine_set_exit_game();
DN_API const char* dn_engine_get_game_hash();
DN_API void dn_engine_set_target_fps(double fps);
DN_API double dn_engine_get_target_fps();
DN_API bool dn_engine_exceeded_frame_time();
DN_API bool dn_engine_should_exit();
#endif



#ifdef DN_ENGINE_IMPLEMENTATION
void dn_engine_init() {
	dn_engine_set_target_fps(60);
}

void dn_engine_update() {
	dn_time_metric_begin("frame");

	// Clearly this is not representative of elapsed real time
	engine.elapsed_time += engine.dt;
	engine.frame++;
}

void dn_engine_set_target_fps(float64 fps) {
	engine.target_fps = fps;
	engine.dt = 1.f / fps;
}


double dn_engine_get_target_fps() {
	return engine.target_fps;
}

void dn_engine_set_exit_game() {
	engine.exit_game = true;
}

const char* dn_engine_get_game_hash() {
	return "hehe";
	// return GIT_HASH;
}

bool dn_engine_should_exit() {
	bool done = false;
	done |= (bool)glfwWindowShouldClose(window.handle);
	done |= engine.exit_game;
	return done;
}

bool dn_engine_exceeded_frame_time() {
	auto& frame_timer = time_metrics["frame"];
	auto now = glfwGetTime();
	double elapsed = (now - frame_timer.time_begin);
	return elapsed >= engine.dt;
}

#endif
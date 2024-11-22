void update_frame() {
	tm_begin("frame");

	// Clearly this is not representative of elapsed real time
	engine.elapsed_time += engine.dt;
	engine.frame++;
}

bool is_game_done() {
	bool done = false;
	done |= (bool)glfwWindowShouldClose(window.handle);
	done |= engine.exit_game;
	return done;
}

bool exceeded_frame_time() {
	auto& frame_timer = time_metrics["frame"];
	auto now = glfwGetTime();
	double elapsed = (now - frame_timer.time_begin);
	return elapsed >= engine.dt;
}

void set_exit_game() {
	engine.exit_game = true;
}

const char* get_game_hash() {
	return "hehe";
	// return GIT_HASH;
}

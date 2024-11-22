struct Engine {
	float32 target_fps;
	float32 dt;
	float32 elapsed_time;
	int32 frame = 0;
	bool exit_game = false;

	bool steam = false;
};
Engine engine;

bool is_game_done();
bool exceeded_frame_time();

FM_LUA_EXPORT void set_exit_game();
FM_LUA_EXPORT const char* get_game_hash();

#include "libs.hpp"

#include "utils/types.hpp"
#include "utils/assert.hpp"
#include "utils/error.hpp"
#include "utils/macros.hpp"
#include "utils/log.hpp"
#include "utils/enum.hpp"
#include "utils/array.hpp"
#include "utils/fixed_array.hpp"
#include "utils/memory.hpp"
#include "utils/stack_array.hpp"
#include "utils/ring_buffer.hpp"
#include "utils/string.hpp"
#include "utils/arena.hpp"
#include "utils/path.hpp"
#include "utils/vector.hpp"
#include "utils/quad.hpp"
#include "utils/colors.hpp"
#include "utils/defer.hpp"
#include "utils/coordinate.hpp"
#include "utils/utils.hpp"
#include "utils/file_monitor.hpp"
#include "utils/filesystem.hpp"
#include "utils/time_function.hpp"
#include "utils/noise.hpp"
#include "utils/dyn_array.hpp"
#include "utils/hash.hpp"
#include "imgui/imgui_extensions.hpp"
#include "lua.hpp"
#include "engine.hpp"
#include "time_metrics.hpp"
#include "interpolation.hpp"
#include "window.hpp"
#include "input.hpp"
#include "font.hpp"
#include "image.hpp"
#include "background.hpp"
#include "shader.hpp"
#include "fluid.hpp"
#include "text.hpp"
#include "draw.hpp"
#include "audio.hpp"
#include "api.hpp"
#include "action.hpp"
#include "particle.hpp"
#include "buffers.hpp"
#include "steam.hpp"
#include "asset.hpp"
#include "named_path.hpp"
#include "graphics.hpp"
#include "sdf.hpp"

#include "action.cpp" // HALF
#include "api.cpp"
#include "asset.cpp"
#include "audio.cpp"
#include "background.cpp" // INVERT (I need something to load large images though, in general)
#include "draw.cpp"
#include "engine.cpp"
#include "font.cpp"
#include "image.cpp" // HALF (Screenshots should be reworked, probably? I'm referencing a named path when I initialize)
#include "input.cpp"
#include "fluid.cpp" // GAME
#define GRAPHICS_IMPLEMENTATION
#include "graphics.hpp"
#include "lua.cpp"
#include "named_path.cpp"
#include "particle.cpp"
#define SDF_IMPLEMENTATION
#include "sdf.hpp"
#include "shader.cpp"
#include "steam.cpp"
#include "text.cpp"
#include "time_metrics.cpp"
#include "window.cpp"
#include "imgui/imgui_extensions.cpp"
#include "utils/array.cpp"
#include "utils/coordinate.cpp"
#define DYNAMIC_ARRAY_IMPLEMENTATION
#include "utils/dyn_array.hpp"
#include "utils/file_monitor.cpp"
#define FIXED_ARRAY_IMPLEMENTATION
#include "utils/fixed_array.hpp"
#include "utils/log.cpp"
#include "utils/memory.cpp"
#include "utils/path.cpp"
#include "utils/noise.cpp"
#include "utils/string.cpp"
#include "test.hpp"

int main() {
	init_allocators();
	run_tests();
	init_random();
	init_paths();
	init_log();
	init_time();
	init_steam();
	init_file_monitors();
	init_assets();
	init_buffers();
	init_lua();
	init_actions();
	init_audio();
	init_scripts();

	while(!is_game_done()) {
		update_frame();
		update_steam();
		update_allocators();
		update_file_monitors();
		update_assets();
		update_imgui();
		update_input();
		update_actions();
		update_game();
		update_time();
	}

	shutdown_audio();
	shutdown_steam();
	shutdown_imgui();
	shutdown_glfw();

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return main();
}

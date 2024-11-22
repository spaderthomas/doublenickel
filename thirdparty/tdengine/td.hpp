#ifndef TD_H
#define TD_H
#include "external/include/libs.hpp"

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
#include "source/lua.hpp"
#include "source/engine.hpp"
#include "source/time_metrics.hpp"
#include "source/interpolation.hpp"
#include "source/window.hpp"
#include "source/input.hpp"
#include "source/font.hpp"
#include "source/image.hpp"
#include "source/background.hpp"
#include "source/shader.hpp"
#include "source/fluid.hpp"
#include "source/text.hpp"
#include "source/draw.hpp"
#include "source/audio.hpp"
#include "source/api.hpp"
#include "source/action.hpp"
#include "source/particle.hpp"
#include "source/buffers.hpp"
#include "source/steam.hpp"
#include "source/asset.hpp"
#include "source/named_path.hpp"
#include "source/graphics.hpp"
#include "source/sdf.hpp"
#endif

#ifdef TD_IMPLEMENTATION
#include "glad.c"
#include "imgui/imgui_compilation_unit.hpp"

#include "source/action.cpp" // HALF
#include "source/api.cpp"
#include "source/asset.cpp"
#include "source/audio.cpp"
#include "source/background.cpp" // INVERT (I need something to load large images though, in general)
#include "source/draw.cpp"
#include "source/engine.cpp"
#include "source/font.cpp"
#include "source/image.cpp" // HALF (Screenshots should be reworked, probably? I'm referencing a named path when I initialize)
#include "source/input.cpp"
#include "source/fluid.cpp" // GAME
#define GRAPHICS_IMPLEMENTATION
#include "source/graphics.hpp"
#include "source/lua.cpp"
#include "source/named_path.cpp"
#include "source/particle.cpp"
#define SDF_IMPLEMENTATION
#include "source/sdf.hpp"
#include "source/shader.cpp"
#include "source/steam.cpp"
#include "source/text.cpp"
#include "source/time_metrics.cpp"
#include "source/window.cpp"
#include "source/imgui/imgui_extensions.cpp"
#include "source/utils/array.cpp"
#include "source/utils/coordinate.cpp"
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


int td_main() {
	init_allocators();
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

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
// 	return main();
// }
#endif

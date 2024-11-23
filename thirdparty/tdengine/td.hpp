#ifndef TD_H
#define TD_H
#include "external/include/libs.hpp"

#include "source/utils/types.hpp"
#include "source/utils/assert.hpp"
#include "source/utils/error.hpp"
#include "source/utils/macros.hpp"
#include "source/utils/log.hpp"
#include "source/utils/enum.hpp"
#include "source/utils/array.hpp"
#include "source/utils/fixed_array.hpp"
#include "source/utils/memory.hpp"
#include "source/utils/stack_array.hpp"
#include "source/utils/ring_buffer.hpp"
#include "source/utils/string.hpp"
#include "source/utils/arena.hpp"
#include "source/utils/path.hpp"
#include "source/utils/vector.hpp"
#include "source/utils/quad.hpp"
#include "source/utils/colors.hpp"
#include "source/utils/defer.hpp"
#include "source/utils/coordinate.hpp"
#include "source/utils/utils.hpp"
#include "source/utils/file_monitor.hpp"
#include "source/utils/filesystem.hpp"
#include "source/utils/time_function.hpp"
#include "source/utils/noise.hpp"
#include "source/utils/dyn_array.hpp"
#include "source/utils/hash.hpp"
#include "source/utils/preprocessor.hpp"
#include "source/imgui/imgui_extensions.hpp"
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
#include "source/app.hpp"
#endif

#ifdef TD_IMPLEMENTATION
#include "source/glad.c"
#include "source/imgui/imgui_compilation_unit.cpp"

#include "source/action.cpp" // HALF
#include "source/api.cpp"
#define APP_IMPLEMENTATION
#include "source/app.hpp"
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
#define COORDINATE_IMPLEMENTATION
#include "source/utils/coordinate.hpp"
#define DYNAMIC_ARRAY_IMPLEMENTATION
#include "source/utils/dyn_array.hpp"
#include "source/utils/file_monitor.cpp"
#define FIXED_ARRAY_IMPLEMENTATION
#include "source/utils/fixed_array.hpp"
#include "source/utils/log.cpp"
#include "source/utils/memory.cpp"
#include "source/utils/path.cpp"
#define PREPROCESSOR_IMPLEMENTATION
#include "source/utils/preprocessor.hpp"
#include "source/utils/noise.cpp"
#include "source/utils/string.cpp"


int td_main(TdAppDescriptor app) {
	init_allocators();
	init_random();
	init_app(app);
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

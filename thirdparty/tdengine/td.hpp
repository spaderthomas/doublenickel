#ifndef TD_H
#define TD_H
#include "external/include/libs.hpp"

#include "source/utils/types.hpp"
#include "source/utils/assert.hpp"
#include "source/utils/error.hpp"
#include "source/utils/macros.hpp"
#include "source/utils/log.hpp"
#include "source/utils/enum.hpp"
#include "source/utils/os.hpp"
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
#include "source/imgui/dn_imgui.hpp"
#include "source/lua.hpp"
#include "source/engine.hpp"
#include "source/time_metrics.hpp"
#include "source/interpolation.hpp"
#include "source/window.hpp"
#include "source/input.hpp"
#include "source/font.hpp"
#include "source/image.hpp"
#include "source/background.hpp"
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
#define DN_AUDIO_IMPLEMENTATION
#include "source/audio.hpp"
#include "source/background.cpp" // INVERT (I need something to load large images though, in general)
#include "source/draw.cpp"
#define DN_ENGINE_IMPLEMENTATION
#include "source/engine.hpp"
#define DN_FONT_IMPLEMENTATION
#include "source/font.hpp"
#include "source/image.cpp" // HALF (Screenshots should be reworked, probably? I'm referencing a named path when I initialize)
#define DN_INPUT_IMPLEMENTATION
#include "source/input.hpp"
#include "source/fluid.cpp" // GAME
#define GRAPHICS_IMPLEMENTATION
#include "source/graphics.hpp"
#include "source/lua.cpp"
#include "source/named_path.cpp"
#include "source/particle.cpp"
#define SDF_IMPLEMENTATION
#include "source/sdf.hpp"
#include "source/steam.cpp"
#include "source/text.cpp"
#define DN_TIME_METRICS_IMPLEMENTATION
#include "source/time_metrics.hpp"
#define DN_WINDOW_IMPLEMENTATION
#include "source/window.hpp"
#define DN_IMGUI_IMPLEMENTATION
#include "source/imgui/dn_imgui.hpp"
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
#define DN_OS_IMPLEMENTATION
#include "source/utils/os.hpp"
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
	dn_engine_init();
	dn_input_init();
	dn_time_metrics_init();
	dn_font_init();
	init_steam();
	init_file_monitors();
	init_assets();
	init_buffers();
	init_lua();
	init_actions();
	dn_audio_init();
	init_scripts();

	while(!dn_engine_should_exit()) {
		dn_engine_update();
		update_steam();
		update_allocators();
		update_file_monitors();
		update_assets();
		dn_imgui_update();
		dn_input_update();
		update_actions();
		update_game();
		dn_time_metrics_update();
	}

	dn_audio_shutdown();
	shutdown_steam();
	dn_imgui_shutdown();
	dn_window_shutdown();

	return 0;
}

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
// 	return main();
// }
#endif

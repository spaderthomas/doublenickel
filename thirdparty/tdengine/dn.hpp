#ifndef DN_HPP
#define DN_HPP
#include "external/include/libs.hpp"

#include "source/utils/types.hpp"
#include "source/utils/assert.hpp"
#include "source/utils/macros.hpp"
#include "source/utils/path.hpp"
#include "source/utils/log.hpp"
#include "source/utils/os.hpp"
#include "source/utils/array.hpp"
#include "source/utils/fixed_array.hpp"
#include "source/utils/memory.hpp"
#include "source/utils/stack_array.hpp"
#include "source/utils/ring_buffer.hpp"
#include "source/utils/string.hpp"
#include "source/utils/arena.hpp"
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
#include "source/utils/dynamic_array.hpp"
#include "source/utils/hash.hpp"
#include "source/utils/preprocessor.hpp"
#include "source/imgui/dn_imgui.hpp"
#include "source/dn_lua.hpp"
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
#include "source/action.hpp"
#include "source/particle.hpp"
#include "source/buffers.hpp"
#include "source/dn_steam.hpp"
#include "source/asset.hpp"
#include "source/named_path.hpp"
#include "source/graphics.hpp"
#include "source/sdf.hpp"
#include "source/app.hpp"
#endif

#ifdef DN_IMPLEMENTATION
#include "source/glad.c"
#include "source/imgui/imgui_compilation_unit.cpp"

#define APP_IMPLEMENTATION
#include "source/app.hpp"
#define DN_AUDIO_IMPLEMENTATION
#include "source/audio.hpp"
#define DN_ASSET_IMPLEMENTATION
#include "source/asset.hpp"
#define DN_ENGINE_IMPLEMENTATION
#include "source/engine.hpp"
#define DN_FONT_IMPLEMENTATION
#include "source/font.hpp"
#define DN_INPUT_IMPLEMENTATION
#include "source/input.hpp"
#define GRAPHICS_IMPLEMENTATION
#include "source/graphics.hpp"
#define DN_LUA_IMPLEMENTATION
#include "source/dn_lua.hpp"
#define DN_NAMED_PATH_IMPLEMENTATION
#include "source/named_path.hpp"
#define DN_SDF_IMPLEMENTATION
#include "source/sdf.hpp"
#define DN_STEAM_IMPLEMENTATION
#include "source/dn_steam.hpp"
#define DN_TIME_METRICS_IMPLEMENTATION
#include "source/time_metrics.hpp"
#define DN_WINDOW_IMPLEMENTATION
#include "source/window.hpp"
#define DN_IMGUI_IMPLEMENTATION
#include "source/imgui/dn_imgui.hpp"
#define COORDINATE_IMPLEMENTATION
#include "source/utils/coordinate.hpp"
#define DN_DYNAMIC_ARRAY_IMPLEMENTATION
#include "source/utils/dynamic_array.hpp"
#define DN_FILE_MONITOR_IMPLEMENTATION
#include "source/utils/file_monitor.hpp"
#define DN_FIXED_ARRAY_IMPLEMENTATION
#include "source/utils/fixed_array.hpp"
#define DN_LOG_IMPLEMENTATION
#include "source/utils/log.hpp"
#define DN_MEMORY_IMPLEMENTATION
#include "source/utils/memory.hpp"
#define DN_NOISE_IMPLEMENTATION
#include "source/utils/noise.hpp"
#define DN_OS_IMPLEMENTATION
#include "source/utils/os.hpp"
#define DN_PATH_IMPLEMENTATION
#include "source/utils/path.hpp"
#define DN_PREPROCESSOR_IMPLEMENTATION
#include "source/utils/preprocessor.hpp"
#define DN_STRING_IMPLEMENTATION
#include "source/utils/string.hpp"


#include "source/utils/array.cpp"
#include "source/text.cpp"
#include "source/draw.cpp"
#include "source/background.cpp" // INVERT (I need something to load large images though, in general)
#include "source/image.cpp" // HALF (Screenshots should be reworked, probably? I'm referencing a named path when I initialize)
#include "source/fluid.cpp" // GAME
#include "source/action.cpp" // HALF

// I should probably nuke this, because the particle system should be a GPU implementation
#include "source/particle.cpp"

int dn_main(dn_app_descriptor_t app) {
  dn_allocators_init();
  init_random();
  dn_app_init(app);
  init_paths();
  dn_log_init();
  dn_engine_init();
  dn_input_init();
  dn_time_metrics_init();
  dn_file_monitors_init();
  init_buffers();
  dn_lua_init();
  init_actions();
  dn_lua_init_game();

  while(!dn_engine_should_exit()) {
    dn_engine_update();
    dn_allocators_update();
    dn_file_monitors_update();
    dn_assets_update();
    dn_imgui_update();
    dn_input_update();
    update_actions();
    dn_lua_update();
    dn_time_metrics_update();
  }

  dn_audio_shutdown();
  dn_steam_shutdown();
  dn_imgui_shutdown();
  dn_window_shutdown();

  return 0;
}

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//  return main();
// }
#endif

#ifndef DN_HPP
#define DN_HPP
#include "external/include/dn_external.hpp"

#include "source/types.hpp"
#include "source/macros.hpp"
#include "source/memory.hpp"
#include "source/string.hpp"
#include "source/path.hpp"
#include "source/log.hpp"
#include "source/os.hpp"
#include "source/array.hpp"
#include "source/fixed_array.hpp"
#include "source/dynamic_array.hpp"
#include "source/ring_buffer.hpp"
#include "source/arena.hpp"
#include "source/colors.hpp"
#include "source/coordinate.hpp"
#include "source/math.hpp"
#include "source/file_monitor.hpp"
#include "source/time_function.hpp"
#include "source/noise.hpp"
#include "source/hash.hpp"
#include "source/preprocessor.hpp"
#include "source/dn_imgui.hpp"
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
#include "source/audio.hpp"
#include "source/action.hpp"
#include "source/particle.hpp"
#include "source/dn_steam.hpp"
#include "source/asset.hpp"
#include "source/named_path.hpp"
#include "source/graphics.hpp"
#include "source/sdf.hpp"
#include "source/app.hpp"
#include "source/test.hpp"
#endif

#ifdef DN_IMPLEMENTATION
#include "external/source/dn_external.cpp"

#define DN_APP_IMPLEMENTATION
#include "source/app.hpp"
#define DN_GEN_ARENA_IMPLEMENTATION
#include "source/arena.hpp"
#define DN_ARRAY_IMPLEMENTATION
#include "source/array.hpp"
#define DN_ASSET_IMPLEMENTATION
#include "source/asset.hpp"
#define DN_AUDIO_IMPLEMENTATION
#include "source/audio.hpp"
#define DN_COORDINATE_IMPLEMENTATION
#include "source/coordinate.hpp"
#define DN_DYNAMIC_ARRAY_IMPLEMENTATION
#include "source/dynamic_array.hpp"
#define DN_ENGINE_IMPLEMENTATION
#include "source/engine.hpp"
#define DN_FILE_MONITOR_IMPLEMENTATION
#include "source/file_monitor.hpp"
#define DN_FIXED_ARRAY_IMPLEMENTATION
#include "source/fixed_array.hpp"
#define DN_FONT_IMPLEMENTATION
#include "source/font.hpp"
#define DN_INPUT_IMPLEMENTATION
#include "source/input.hpp"
#define DN_GRAPHICS_IMPLEMENTATION
#include "source/graphics.hpp"
#define DN_HASH_IMPLEMENTATION
#include "source/hash.hpp"
#define DN_IMAGE_IMPLEMENTATION
#include "source/image.hpp"
#define DN_IMGUI_IMPLEMENTATION
#include "source/dn_imgui.hpp"
#define DN_LOG_IMPLEMENTATION
#include "source/log.hpp"
#define DN_LUA_IMPLEMENTATION
#include "source/dn_lua.hpp"
#define DN_MATH_IMPLEMENTATION
#include "source/math.hpp"
#define DN_MEMORY_IMPLEMENTATION
#include "source/memory.hpp"
#define DN_NAMED_PATH_IMPLEMENTATION
#include "source/named_path.hpp"
#define DN_NOISE_IMPLEMENTATION
#include "source/noise.hpp"
#define DN_OS_IMPLEMENTATION
#include "source/os.hpp"
#define DN_PATH_IMPLEMENTATION
#include "source/path.hpp"
#define DN_PREPROCESSOR_IMPLEMENTATION
#include "source/preprocessor.hpp"
#define DN_SDF_IMPLEMENTATION
#include "source/sdf.hpp"
#define DN_STEAM_IMPLEMENTATION
#include "source/dn_steam.hpp"
#define DN_STRING_IMPLEMENTATION
#include "source/string.hpp"
#define DN_TEST_IMPLEMENTATION
#include "source/test.hpp"
#define DN_TIME_METRICS_IMPLEMENTATION
#include "source/time_metrics.hpp"
#define DN_WINDOW_IMPLEMENTATION
#include "source/window.hpp"


#include "source/text.cpp"
#include "source/background.cpp" // INVERT (I need something to load large images though, in general)
#include "source/fluid.cpp" // GAME
#include "source/action.cpp" // HALF

// I should probably nuke this, because the particle system should be a GPU implementation
#include "source/particle.cpp"
#endif

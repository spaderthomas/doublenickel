// Platform includes
#define NOMINMAX
#include "windows.h"
#include "shlobj.h"

// Library includes
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "handmade/HandmadeMath.h"
typedef HMM_Mat4 Matrix4;
typedef HMM_Mat3 Matrix3;
typedef HMM_Mat2 Matrix2;

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_NONE
#include "stb/stb_include.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_libs/dr_wav.h"

extern "C" {
  #include "freetype/ft2build.h"
  #include "freetype/freetype.h"
  #include "freetype/ftsizes.h"
}

#include "lua/lua.hpp"
extern "C" {
	#include "lua/lauxlib.h"
}

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imfilebrowser.h"
#include "imgui/cimgui.hpp"

#define SOKOL_IMPL
#include "sokol/sokol_audio.h"
#include "sokol/sokol_log.h"

#include "steam/steam_api.h"

// STL
#include <algorithm>
#include <array>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <ctime>
#include <codecvt>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <map>
#include <math.h>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <regex>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <string_view>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std::filesystem;
using namespace std::chrono;

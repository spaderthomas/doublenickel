// action.hpp
// file_monitor.hpp
// fluid.hpp
// interpolation.hpp
// particle.hpp
// preprocessor.hpp
// sdf.hpp
// text.hpp

// dn_asset_registry
// dn_images
// dn_audio
// dn_backgrounds
// dn_screenshots
// dn_atlases
// dn_gpu
// dn_font
// dn_noise
// dn_steam

#ifndef DN_BUILD_FFI
  #include <assert.h>
  #include <float.h>
  #include <inttypes.h>
  // #include <stdatomic.h>
  #include <stdint.h>
  // #include <sys/time.h> @fix
  #include <stdint.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <stddef.h>
  #include <time.h>

  // Platform includes
  #define NOMINMAX
  #include "windows.h"
  #include "shlobj.h"

  #include "KHR/khrplatform.h"

  // Library includes
  #include "glad/glad.h"
  #include "GLFW/glfw3.h"

  #ifndef DN_MATH_BACKEND_CUSTOM
    #include "handmade/HandmadeMath.h"
  #endif

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

  #include "freetype/ft2build.h"
  #include "freetype/freetype.h"
  #include "freetype/ftsizes.h"

  #include "lua/lua.h"
  #include "lua/lauxlib.h"
  #include "lua/lualib.h"
  #include "lua/luajit.h"

  #define SOKOL_IMPL
  #include "sokol/sokol_audio.h"
  #include "sokol/sokol_log.h"

  #define CIMGUI_USE_GLFW
  #define CIMGUI_USE_OPENGL3
  #define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
  #include "cimgui/cimgui.h"
  #include "cimgui/cimgui_impl.h"

  #include "gunslinger/gs.h"

  // #include "steam/steam_api.h"
#else    
  typedef enum gs_hash_table_entry_state
  {
      GS_HASH_TABLE_ENTRY_INACTIVE = 0x00,
      GS_HASH_TABLE_ENTRY_ACTIVE = 0x01
  } gs_hash_table_entry_state;

  #define __gs_hash_table_entry(__HMK, __HMV)\
      struct\
      {\
          __HMK key;\
          __HMV val;\
          gs_hash_table_entry_state state;\
      }

  #define gs_hash_table(__HMK, __HMV)\
      struct {\
          __gs_hash_table_entry(__HMK, __HMV)* data;\
          __HMK tmp_key;\
          __HMV tmp_val;\
          size_t stride;\
          size_t klpvl;\
          size_t tmp_idx;\
      }*
#endif


//  ████████╗██╗   ██╗██████╗ ███████╗███████╗
//  ╚══██╔══╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔════╝
//     ██║    ╚████╔╝ ██████╔╝█████╗  ███████╗
//     ██║     ╚██╔╝  ██╔═══╝ ██╔══╝  ╚════██║
//     ██║      ██║   ██║     ███████╗███████║
//     ╚═╝      ╚═╝   ╚═╝     ╚══════╝╚══════╝
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define DN_F64_MAX DBL_MAX

typedef u64 dn_hash_t;
typedef u32 dn_error_t;

#define DN_ASSET_NAME_LEN 64
typedef char dn_asset_name_t [DN_ASSET_NAME_LEN];

#ifndef DN_BUILD_FFI
  typedef HANDLE dn_win32_handle_t;
  typedef DWORD dn_win32_dword_t;
  typedef WIN32_FIND_DATA dn_win32_find_data_t;
  typedef CRITICAL_SECTION dn_win32_critical_section_t;
#else
  #define DN_WIN32_UNKNOWN_SIZE_STRUCT void*

  typedef void* dn_win32_handle_t;
  typedef u32 dn_win32_dword_t;
  typedef DN_WIN32_UNKNOWN_SIZE_STRUCT dn_win32_find_data_t;
  typedef DN_WIN32_UNKNOWN_SIZE_STRUCT dn_win32_critical_section_t;
#endif

//  ███╗   ███╗ █████╗  ██████╗██████╗  ██████╗ ███████╗
//  ████╗ ████║██╔══██╗██╔════╝██╔══██╗██╔═══██╗██╔════╝
//  ██╔████╔██║███████║██║     ██████╔╝██║   ██║███████╗
//  ██║╚██╔╝██║██╔══██║██║     ██╔══██╗██║   ██║╚════██║
//  ██║ ╚═╝ ██║██║  ██║╚██████╗██║  ██║╚██████╔╝███████║
//  ╚═╝     ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝
#define dn_quad_literal(top, bottom, left, right) { \
  { left, top },     \
  { left, bottom },  \
  { right, bottom }, \
                     \
  { left, top },     \
  { right, bottom }, \
  { right, top }     \
}

#define dn_max(a, b) (a) > (b) ? (a) : (b)
#define dn_min(a, b) (a) > (b) ? (b) : (a)

#define dn_arr_len(carray) (sizeof((carray)) / sizeof((carray)[0]))

#define dn_for(it, n) for (u32 it = 0; it < (n); it++)
#define dn_for_arr(arr, it) for (u32 it = 0; it < dn_arr_len(arr); it++)

#define dn_macro_str(x) #x
#define _dn_macro_cat(x, y) x##y
#define dn_macro_cat(x, y) _dn_macro_cat(x, y)
#define dn_unique_name() dn_macro_cat(__dn_unique_name__, __LINE__)

#define dn_typedef_fn(return_type, name, ...) typedef return_type(*name)(__VA_ARGS__)

#define DN_ASSERT(condition) assert(condition)
#define DN_SOFT_ASSERT(condition) DN_ASSERT(condition) // For things you want to crash in development, but fix before release (as opposed to assers which are left in release builds and, say, logged)
#define DN_UNTESTED() DN_ASSERT(false)
#define DN_UNREACHABLE() DN_ASSERT(false)
#define DN_UNREACHABLE_MESSAGE(message) DN_ASSERT(false && (message))
// #define DN_BROKEN() DN_ASSERT(false)
#define DN_BROKEN() dn_log("DN_BROKEN(%s)", __func__)
#define DN_BREAK() DebugBreak()

#define dn_zero_initialize() { 0 }
#define dn_zero_struct(t) (t){ 0 }
#define dn_zero_return(t) { t __dn_zero_return = dn_zero_initialize(); return __dn_zero_return; }
#define dn_type_name(t) (#t)

#define dn_enum_flags(t) DEFINE_ENUM_FLAG_OPERATORS(t)

#define DN_FMT_U64 PRIu64

#define dn_ptr_lval(type, rvalue) (&(type){ rvalue })
#define dn_arr_lval(type, ...) ((type []){ __VA_ARGS__ })

#define dn_swap(t, a, b) { t dn_unique_name() = (a); (a) = (b); (b) = dn_unique_name(); }

#define DN_QSORT_A_FIRST -1
#define DN_QSORT_B_FIRST 1
#define DN_QSORT_EQUAL 0

#define gs_hash_table_for(_HT, it) for (gs_hash_table_iter it = 0; gs_hash_table_iter_valid((_HT), it); gs_hash_table_iter_advance((_HT), it))

#define dn_align(n) __declspec(align(n))

#ifndef DN_BUILD_FFI
  #define DN_API __declspec(dllexport)
  #define DN_IMP
  #define DN_ATOMIC
#else
  #define DN_API
  #define DN_IMP  @DN_FFI_CANARY
  #define DN_ATOMIC
#endif


//  ███╗   ███╗ █████╗ ████████╗██╗  ██╗
//  ████╗ ████║██╔══██╗╚══██╔══╝██║  ██║
//  ██╔████╔██║███████║   ██║   ███████║
//  ██║╚██╔╝██║██╔══██║   ██║   ██╔══██║
//  ██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║
//  ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝
#if defined(DN_MATH_BACKEND_CUSTOM) || defined(DN_BUILD_FFI)
  typedef union {
    struct {
      float x;
      float y;
    };

    struct {
      float r;
      float g;
    };
  } dn_vector2_t;

  typedef union {
    struct {
      float x;
      float y;
      float z;
    };

    struct {
      float r;
      float g;
      float b;
    };
  } dn_vector3_t;

  typedef union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };
    struct {
      float r;
      float g;
      float b;
      float a;
    };
  } dn_vector4_t;

  typedef struct { dn_vector4_t entries [4]; } dn_matrix4_t;
  typedef struct { dn_vector3_t entries [3]; } dn_matrix3_t;
  typedef struct { dn_vector2_t entries [2]; } dn_matrix2_t;
#else
  typedef HMM_Vec2 dn_vector2_t;
  typedef HMM_Vec3 dn_vector3_t;
  typedef HMM_Vec4 dn_vector4_t;
  typedef HMM_Mat4 dn_matrix4_t;
  typedef HMM_Mat3 dn_matrix3_t;
  typedef HMM_Mat2 dn_matrix2_t;
#endif

typedef struct {
  s32 x;
  s32 y;
} dn_vector2i_t;

DN_API f32          dn_math_step(f32 edge, f32 x);
DN_API f32          dn_math_lerp(f32 a, f32 b, f32 t);
DN_API dn_vector4_t dn_math_lerp4(dn_vector4_t a, dn_vector4_t b, f32 t);
DN_API dn_vector4_t dn_math_abs4(dn_vector4_t v);
DN_API dn_vector4_t dn_math_fract4(dn_vector4_t v);
DN_API f32          dn_math_clamp(f32 value, f32 lower, f32 upper);
DN_API dn_vector4_t dn_math_clamp4(dn_vector4_t v, f32 vmin, f32 vmax);
DN_API u32          dn_math_ceilf(f32 f);
DN_API u32          dn_math_ceil_divide(u32 a, u32 b);
DN_API f32          dn_random_f32_slow(f32 vmin, f32 vmax);
DN_API s32          dn_random_i32(s32 vmin, s32 vmax);
DN_IMP void         dn_random_init();


// ███╗   ██╗ ██████╗ ██╗███████╗███████╗
// ████╗  ██║██╔═══██╗██║██╔════╝██╔════╝
// ██╔██╗ ██║██║   ██║██║███████╗█████╗  
// ██║╚██╗██║██║   ██║██║╚════██║██╔══╝  
// ██║ ╚████║╚██████╔╝██║███████║███████╗
// ╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚══════╝╚══════╝
typedef struct {
  struct {
    u32 perlin;
    u32 chaoric;
  } textures;
} dn_noise_t;
dn_noise_t dn_noise;

#ifndef DN_BUILD_FFI
static const int dn_perlin_permutation_raw[256] = {
  151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,
  129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
  49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

static int dn_perlin_permutation[512];
#endif

DN_IMP void dn_noise_init();
DN_API f64 dn_noise_perlin(f64 x, f64 y);
DN_API f64 dn_noise_perlin_scaled(f64 x, f64 y, f64 vmin, f64 vmax);
DN_API f64 dn_noise_chaotic(f64 x, f64 y);
DN_API f64 dn_noise_chaotic_scaled(f64 x, f64 y, f64 vmin, f64 vmax);


//   ██████╗ ██████╗ ██╗      ██████╗ ██████╗ ███████╗
//  ██╔════╝██╔═══██╗██║     ██╔═══██╗██╔══██╗██╔════╝
//  ██║     ██║   ██║██║     ██║   ██║██████╔╝███████╗
//  ██║     ██║   ██║██║     ██║   ██║██╔══██╗╚════██║
//  ╚██████╗╚██████╔╝███████╗╚██████╔╝██║  ██║███████║
//   ╚═════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝
typedef dn_vector4_t dn_color_t;
#define dn_rgb_255(red, green, blue) { .r = (red) / 255.f, .g = (green) / 255.f, .b = (blue) / 255.f, .a = 1.0 }
#define dn_rgb_01(red, green, blue) { .r = red, .g = green, .b = blue, .a = 1.0 }

typedef struct {
  dn_color_t indian_red;
  dn_color_t tyrian_purple;
  dn_color_t cardinal;
  dn_color_t celadon;
  dn_color_t spring_green;
  dn_color_t mindaro;
  dn_color_t light_green;
  dn_color_t zomp;
  dn_color_t midnight_green;
  dn_color_t prussian_blue;
  dn_color_t orange;
  dn_color_t sunglow;
  dn_color_t selective_yellow;
  dn_color_t gunmetal;
  dn_color_t paynes_gray;
  dn_color_t cadet_gray;
  dn_color_t charcoal;
  dn_color_t cool_gray;
  dn_color_t cream;
  dn_color_t misty_rose;
  dn_color_t taupe;
  dn_color_t dark_green;
  dn_color_t rich_black;
  dn_color_t white;
} dn_colors_t;

dn_color_t dn_color_rgb_to_hsv(dn_color_t color);
dn_color_t dn_color_hsv_to_rgb(dn_color_t color);

#ifndef DN_BUILD_FFI
dn_colors_t dn_colors = {
  .indian_red       = dn_rgb_255(180, 101, 111), // RED
  .tyrian_purple    = dn_rgb_255(95,  26,  55),
  .cardinal         = dn_rgb_255(194, 37,  50),
  .celadon          = dn_rgb_255(183, 227, 204), // GREEN
  .spring_green     = dn_rgb_255(89,  255, 160),
  .mindaro          = dn_rgb_255(188, 231, 132),
  .light_green      = dn_rgb_255(161, 239, 139),
  .zomp             = dn_rgb_255(99,  160, 136),
  .midnight_green   = dn_rgb_255(25,  83,  95),
  .prussian_blue    = dn_rgb_255(16,  43,  63),  // BLUE
  .orange           = dn_rgb_255(249, 166, 32),  // ORANGE
  .sunglow          = dn_rgb_255(255, 209, 102),
  .selective_yellow = dn_rgb_255(250, 188, 42),
  .gunmetal         = dn_rgb_255(43,  61,  65),  // GRAY
  .paynes_gray      = dn_rgb_255(76,  95,  107),
  .cadet_gray       = dn_rgb_255(131, 160, 160),
  .charcoal         = dn_rgb_255(64,  67,  78),
  .cool_gray        = dn_rgb_255(140, 148, 173),
  .cream            = dn_rgb_255(245, 255, 198),  // OFF WHITE
  .misty_rose       = dn_rgb_255(255, 227, 227),
  .taupe            = dn_rgb_255(68,  53,  39),   // BROWN
  .dark_green       = dn_rgb_255(4,   27,  21),   // BLACK
  .rich_black       = dn_rgb_255(4,   10,  15),
  .white            = dn_rgb_255(255, 255, 255),
};
#endif

//  ███╗   ███╗███████╗███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗
//  ████╗ ████║██╔════╝████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝
//  ██╔████╔██║█████╗  ██╔████╔██║██║   ██║██████╔╝ ╚████╔╝
//  ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝
//  ██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║
//  ╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝
typedef enum {
  DN_ALLOCATOR_MODE_ALLOC,
  DN_ALLOCATOR_MODE_FREE,
  DN_ALLOCATOR_MODE_RESIZE,
} dn_allocator_mode_t;

struct dn_allocator_t;

dn_typedef_fn(
  void*,
  dn_alloc_fn_t,
  struct dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* previous_allocation
);

typedef struct dn_allocator_t {
  dn_alloc_fn_t on_alloc;
} dn_allocator_t;

typedef struct {
  dn_allocator_t allocator;
  u8* buffer;
  u32 capacity;
  u32 bytes_used;
  gs_hash_table(u32, u32) allocations;
} dn_bump_allocator_t;

typedef struct {
  dn_allocator_t allocator;
} dn_standard_allocator_t;

typedef struct {
  dn_standard_allocator_t standard;
  dn_bump_allocator_t bump;
  gs_hash_table(dn_hash_t, dn_allocator_t*) allocators;
} dn_allocators_t;
dn_allocators_t dn_allocators;

DN_API void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
DN_API dn_allocator_t*  dn_allocator_find(const char* name);
DN_API void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
DN_API void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
DN_API void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);
DN_IMP void             dn_bump_allocator_init(dn_bump_allocator_t* allocator, u32 capacity);
DN_IMP void             dn_bump_allocator_clear(dn_bump_allocator_t* allocator);
DN_IMP void*            dn_bump_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* buffer);
DN_IMP void             dn_standard_allocator_init(dn_standard_allocator_t* allocator);
DN_IMP void*            dn_standard_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* buffer);
DN_IMP void             dn_allocators_init();
DN_IMP void             dn_allocators_update();


//  ███████╗████████╗██████╗ ██╗███╗   ██╗ ██████╗
//  ██╔════╝╚══██╔══╝██╔══██╗██║████╗  ██║██╔════╝
//  ███████╗   ██║   ██████╔╝██║██╔██╗ ██║██║  ███╗
//  ╚════██║   ██║   ██╔══██╗██║██║╚██╗██║██║   ██║
//  ███████║   ██║   ██║  ██║██║██║ ╚████║╚██████╔╝
//  ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝
typedef struct {
  u32 len;
  u8* data;
} dn_string_t;

// dn_string_buffer_t is for defining fixed size, stack allocated strings that have a length attached. Then, you
// can use dn_str_buffer_view() or dn_str_buffer_view_ptr() to turn it into a dn_string_t and use the normal
// string APIs.
#define dn_str_buffer_t(n) struct { u32 len; u8 data [n]; }
#define dn_str_buffer_capacity(buffer) (sizeof((buffer)->data))
#define dn_str_buffer_view(buffer) (dn_string_t) { .len = (buffer).len, .data = (buffer).data }
#define dn_str_buffer_view_ptr(buffer) ((dn_string_t*)(buffer))
#define dn_string_copy_to_str_buffer(str, buffer) { dn_string_copy_to((str), (buffer)->data, dn_str_buffer_capacity(buffer)); (buffer)->len = (str).len; }

typedef char* dn_tstring_t;
#define DN_MAX_PATH_LEN 256
typedef char dn_path_t [DN_MAX_PATH_LEN];

typedef struct {
  u8* data;
  u32 count;
  u32 capacity;
} dn_string_builder_buffer_t;

typedef struct {
  dn_string_builder_buffer_t buffer;
  dn_allocator_t* allocator;
} dn_string_builder_t;

#define dn_string_cstr(s) (dn_string_t){ .data = (u8*)(s), .len = strlen(s)}
#define dn_string_literal(s) (dn_string_t){ .data = (u8*)(s), .len = sizeof(s) - 1}
#define dn_tstring_builder() (dn_string_builder_t){ .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator }

DN_API void        dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity);
DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);

DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator);
DN_IMP char*       dn_cstr_copy_n(const char* str, u32 length, dn_allocator_t* allocator);
DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator);
DN_API void        dn_cstr_copy_to(const char* str, char* buffer, u32 buffer_length);
DN_API void        dn_cstr_copy_to_n(const char* str, u32 length, char* buffer, u32 buffer_length);
DN_API bool        dn_cstr_equal(const char* a, const char* b);
DN_API u32         dn_cstr_len(const char* str);

DN_API char*       dn_string_to_cstr(dn_string_t str);
DN_API char*       dn_string_to_double_null_terminated(dn_string_t str);
DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy_cstr_n(const char* str, u32 length, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
DN_API void        dn_string_copy_to_str(dn_string_t str, dn_string_t* dest, u32 capacity);
DN_API void        dn_string_copy_to(dn_string_t str, u8* buffer, u32 capacity);
DN_API dn_string_t dn_string_alloc(u32 capacity, dn_allocator_t* allocator);
DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);
DN_API s32         dn_string_sort_kernel_alphabetical(const void* a, const void* b);
DN_API s32         dn_string_compare_alphabetical(dn_string_t a, dn_string_t b);
DN_API bool        dn_string_valid(dn_string_t str);


//   ██████╗ ███████╗
//  ██╔═══██╗██╔════╝
//  ██║   ██║███████╗
//  ██║   ██║╚════██║
//  ╚██████╔╝███████║
//   ╚═════╝ ╚══════╝
typedef enum {
  DN_OS_FILE_ATTR_NONE = 0,
  DN_OS_FILE_ATTR_REGULAR_FILE = 1,
  DN_OS_FILE_ATTR_DIRECTORY = 2,
} dn_os_file_attr_t;

typedef struct {
  s32 year;
  s32 month;
  s32 day;
  s32 hour;
  s32 minute;
  s32 second;
  s32 millisecond;
} dn_os_date_time_t;

typedef struct {
  dn_string_t file_path;
  dn_string_t file_name;
  dn_os_file_attr_t attributes;
} dn_os_directory_entry_t;

typedef struct {
  dn_os_directory_entry_t* data;
  u32 count;
} dn_os_directory_entry_list_t;

#ifndef DN_BUILD_FFI
typedef FILE* dn_os_file_t;
#else
typedef void* dn_os_file_t;
#endif

DN_API bool                         dn_os_does_path_exist(dn_string_t path);
DN_API bool                         dn_os_is_regular_file(dn_string_t path);
DN_API bool                         dn_os_is_directory(dn_string_t path);
DN_API void                         dn_os_create_directory(dn_string_t path);
DN_API void                         dn_os_remove_directory(dn_string_t path);
DN_API void                         dn_os_create_file(dn_string_t path);
DN_API void                         dn_os_remove_file(dn_string_t path);
DN_API dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path);
DN_API dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path);
DN_API dn_os_date_time_t            dn_os_get_date_time();
DN_API f64                          dn_os_file_mod_time(dn_string_t path);
DN_API void                         dn_os_normalize_path(dn_string_t path);
DN_API dn_string_t                  dn_os_parent_path(dn_string_t path, dn_allocator_t* allocator);
DN_API dn_string_t                  dn_os_path_extension(dn_string_t path);
DN_API void                         dn_os_memory_copy(const void* source, void* dest, u32 num_bytes);
DN_API bool                         dn_os_is_memory_equal(const void* a, const void* b, size_t len);
DN_API void                         dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
DN_API void                         dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
DN_API void                         dn_os_zero_memory(void* buffer, u32 buffer_size);
DN_API void                         dn_os_sleep_ms(f64 ms);
DN_API u64                          dn_os_time_since_epoch();
DN_IMP dn_os_file_attr_t            dn_os_winapi_attr_to_dn_attr(u32 attr);

#define dn_os_arr_copy(source, dest) { static_assert(sizeof(source) == sizeof(dest), "dn_os_arr_copy expects two C arrays of the same size"); dn_os_memory_copy(source, dest, sizeof(dest)); }


// ████████╗██╗  ██╗██████╗ ███████╗ █████╗ ██████╗ ██╗███╗   ██╗ ██████╗ 
// ╚══██╔══╝██║  ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██║████╗  ██║██╔════╝ 
//    ██║   ███████║██████╔╝█████╗  ███████║██║  ██║██║██╔██╗ ██║██║  ███╗
//    ██║   ██╔══██║██╔══██╗██╔══╝  ██╔══██║██║  ██║██║██║╚██╗██║██║   ██║
//    ██║   ██║  ██║██║  ██║███████╗██║  ██║██████╔╝██║██║ ╚████║╚██████╔╝
//    ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
///////////
// MUTEX //
///////////
typedef enum {
  DN_MUTEX_ATTR_NONE = 0,
  DN_MUTEX_ATTR_RECURSIVE = 1
} dn_mutex_attr_t;

typedef struct {
  dn_mutex_attr_t attrs;
  bool initialized;
  dn_win32_critical_section_t handle;
} dn_mutex_t;

void dn_mutex_init(dn_mutex_t* mutex);
void dn_mutex_lock(dn_mutex_t* mutex);
void dn_mutex_unlock(dn_mutex_t* mutex);

////////////
// THREAD //
////////////
dn_typedef_fn(void*, dn_thread_fn_t, void*);

typedef struct {
  dn_win32_handle_t handle;
  dn_win32_dword_t id;
  dn_thread_fn_t fn;
} dn_thread_t;

void dn_thread_init(dn_thread_t* thread, dn_thread_fn_t fn, void* argument);
void dn_thread_join(dn_thread_t* thread);

////////////////////////
// CONDITION VARIABLE //
////////////////////////
typedef struct {
  s32 dummy;  
} dn_condition_variable_t;


//  ██╗  ██╗ █████╗ ███████╗██╗  ██╗██╗███╗   ██╗ ██████╗
//  ██║  ██║██╔══██╗██╔════╝██║  ██║██║████╗  ██║██╔════╝
//  ███████║███████║███████╗███████║██║██╔██╗ ██║██║  ███╗
//  ██╔══██║██╔══██║╚════██║██╔══██║██║██║╚██╗██║██║   ██║
//  ██║  ██║██║  ██║███████║██║  ██║██║██║ ╚████║╚██████╔╝
//  ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝
// Thank you to John Jackson's framework Gunslinger, from which I took much inspiration but also
// ripped this piece of hashing code off wholesale.
#define GS_SIZE_T_BITS  ((sizeof(size_t)) * 8)
#define GS_SIPHASH_C_ROUNDS 1
#define GS_SIPHASH_D_ROUNDS 1
#define gs_rotate_left(__V, __N)   (((__V) << (__N)) | ((__V) >> (GS_SIZE_T_BITS - (__N))))
#define gs_rotate_right(__V, __N)  (((__V) >> (__N)) | ((__V) << (GS_SIZE_T_BITS - (__N))))

#define DN_HASH_SEED 0x9de341c9

dn_hash_t dn_hash_siphash_bytes(const void *p, size_t len, size_t seed);
dn_hash_t dn_hash_bytes(const void *p, size_t len);
dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed);
dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b);
dn_hash_t dn_hash_cstr_dumb(const char* str);
dn_hash_t dn_hash_str_dumb(dn_string_t str);
dn_hash_t dn_hash_string(dn_string_t str);
void      dn_hash_encode_hex(char* destination, const char* data, size_t len);
void      dn_hash_encode_base64(char* destination, const char* source, size_t len);

#define dn_hash_type(t) dn_hash_string_dumb((const char*)(#t))


//   ██████╗ ██████╗ ███╗   ██╗████████╗ █████╗ ██╗███╗   ██╗███████╗██████╗ ███████╗
//  ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██║████╗  ██║██╔════╝██╔══██╗██╔════╝
//  ██║     ██║   ██║██╔██╗ ██║   ██║   ███████║██║██╔██╗ ██║█████╗  ██████╔╝███████╗
//  ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██║██║██║╚██╗██║██╔══╝  ██╔══██╗╚════██║
//  ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║██║██║ ╚████║███████╗██║  ██║███████║
//   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚══════╝
/////////////////
// FIXED ARRAY //
/////////////////
typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_fixed_array_t;

#define dn_fixed_array(t, n) dn_fixed_array_t
#define dn_fixed_array_for(arr, it, t) for (t* it = (t*)arr.data; (it - (t*)arr.data) < arr.size; it++)
#define DN_FIXED_ARRAY_RUNTIME_SIZE

DN_API void dn_fixed_array_init(dn_fixed_array_t* fixed_array, u32 capacity, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_fixed_array_push(dn_fixed_array_t* fixed_array, void* data, u32 count);
DN_API u8*  dn_fixed_array_reserve(dn_fixed_array_t* fixed_array, u32 count);
DN_API void dn_fixed_array_clear(dn_fixed_array_t* fixed_array);
DN_API u32  dn_fixed_array_byte_size(dn_fixed_array_t* fixed_array);
DN_API u8*  dn_fixed_array_at(dn_fixed_array_t* fixed_array, u32 index);

///////////////////
// DYNAMIC ARRAY //
///////////////////
typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_dynamic_array_t;

#define dn_dynamic_array(t) dn_dynamic_array_t

DN_API void dn_dynamic_array_init(dn_dynamic_array_t* dynamic_array, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data);
DN_API u8*  dn_dynamic_array_push_n(dn_dynamic_array_t* dynamic_array, void* data, u32 count);
DN_API u8*  dn_dynamic_array_reserve(dn_dynamic_array_t* dynamic_array, u32 count);
DN_API void dn_dynamic_array_clear(dn_dynamic_array_t* dynamic_array);
DN_API u32  dn_dynamic_array_byte_size(dn_dynamic_array_t* dynamic_array);
DN_API u8*  dn_dynamic_array_at(dn_dynamic_array_t* dynamic_array, u32 index);
DN_API void dn_dynamic_array_grow(dn_dynamic_array_t* dynamic_array, u32 capacity);

//////////
// POOL //
//////////
#define DN_POOL_NO_FREE_SLOT -1

typedef struct {
  u32 index;
  u32 generation;
} dn_pool_handle_t;

typedef struct {
  s32 next_free;
  u32 generation;
  bool occupied;
} dn_pool_slot_t;

typedef struct {
  u8* data;
  dn_pool_slot_t* slots;
  u32 element_size;
  u32 capacity;
  s32 free_list;
} dn_pool_t;

typedef struct {
  u32 index;
  dn_pool_t* pool;
} dn_pool_iterator_t;

#define dn_pool(t) dn_pool_t

DN_API void               dn_pool_init(dn_pool_t* pool, u32 capacity, u32 element_size);
DN_API dn_pool_handle_t   dn_pool_insert(dn_pool_t* pool, void* value);
DN_API dn_pool_handle_t   dn_pool_reserve(dn_pool_t* pool);
DN_API void               dn_pool_remove(dn_pool_t* pool, dn_pool_handle_t handle);
DN_API bool               dn_pool_contains(dn_pool_t* pool, dn_pool_handle_t handle);
DN_API void               dn_pool_clear(dn_pool_t* pool);
DN_API dn_pool_handle_t   dn_pool_invalid_handle();
DN_API bool               dn_pool_is_handle_valid(dn_pool_handle_t handle);
DN_API bool               dn_pool_slot_has_next_free(dn_pool_slot_t* slot);
DN_API bool               dn_pool_slot_is_match(dn_pool_slot_t* slot, dn_pool_handle_t handle);
DN_API dn_pool_iterator_t dn_pool_iterator_init(dn_pool_t* pool);
DN_API void               dn_pool_iterator_next(dn_pool_iterator_t* it);
DN_API bool               dn_pool_iterator_done(dn_pool_iterator_t* it);
#define            dn_pool_at(POOL, HANDLE, T) ((T*)((POOL)->data + ((HANDLE).index * (POOL)->element_size)))
#define            dn_pool_at_i(POOL, INDEX, T) ((T*)((POOL)->data + ((INDEX) * (POOL)->element_size)))
#define            dn_pool_for(POOL, IT) for (dn_pool_iterator_t IT = dn_pool_iterator_init((POOL)); !dn_pool_iterator_done(&(IT)); dn_pool_iterator_next(&(IT)))
#define            dn_pool_it(POOL, IT, T) dn_pool_at_i(POOL, (IT).index, T)

/////////////////
// RING BUFFER //
/////////////////
typedef struct {
  u8* data;
  u32 element_size;
  u32 head;
  u32 size;
  u32 capacity;
} dn_ring_buffer_t;

#define dn_ring_buffer(t) dn_ring_buffer_t

typedef struct {
  u32 index;
  bool reverse;
  dn_ring_buffer_t* buffer;
} dn_ring_buffer_iterator_t;

DN_API void*                     dn_ring_buffer_at(dn_ring_buffer_t* buffer, u32 index);
DN_API void                      dn_ring_buffer_init(dn_ring_buffer_t* buffer, u32 capacity, u32 element_size);
DN_API void*                     dn_ring_buffer_back(dn_ring_buffer_t* buffer);
DN_API void*                     dn_ring_buffer_push(dn_ring_buffer_t* buffer, void* data);
DN_API void*                     dn_ring_buffer_push_zero(dn_ring_buffer_t* buffer);
DN_API void*                     dn_ring_buffer_push_overwrite(dn_ring_buffer_t* buffer, void* data);
DN_API void*                     dn_ring_buffer_push_overwrite_zero(dn_ring_buffer_t* buffer);
DN_API void*                     dn_ring_buffer_pop(dn_ring_buffer_t* buffer);
DN_API u32                       dn_ring_buffer_bytes(dn_ring_buffer_t* buffer);
DN_API void                      dn_ring_buffer_clear(dn_ring_buffer_t* buffer);
DN_API bool                      dn_ring_buffer_is_full(dn_ring_buffer_t* buffer);
DN_API bool                      dn_ring_buffer_is_empty(dn_ring_buffer_t* buffer);
DN_API void*                     dn_ring_buffer_iter_deref(dn_ring_buffer_iterator_t* it);
DN_API void                      dn_ring_buffer_iter_next(dn_ring_buffer_iterator_t* it);
DN_API void                      dn_ring_buffer_iter_prev(dn_ring_buffer_iterator_t* it);
DN_API bool                      dn_ring_buffer_iter_done(dn_ring_buffer_iterator_t* it);
DN_API dn_ring_buffer_iterator_t dn_ring_buffer_iter(dn_ring_buffer_t* buffer);
DN_API dn_ring_buffer_iterator_t dn_ring_buffer_riter(dn_ring_buffer_t* buffer);

#define dn_ring_buffer_for(rb, it)  for (dn_ring_buffer_iterator_t (it) = dn_ring_buffer_iter((&(rb)));  !dn_ring_buffer_iter_done(&(it)); dn_ring_buffer_iter_next(&(it)))
#define dn_ring_buffer_rfor(rb, it) for (dn_ring_buffer_iterator_t (it) = dn_ring_buffer_riter((&(rb))); !dn_ring_buffer_iter_done(&(it)); dn_ring_buffer_iter_prev(&(it)))
#define dn_rb_it(it, t) (t*)dn_ring_buffer_iter_deref(&(it))



// ████████╗██╗███╗   ███╗███████╗██████╗ ███████╗
// ╚══██╔══╝██║████╗ ████║██╔════╝██╔══██╗██╔════╝
//    ██║   ██║██╔████╔██║█████╗  ██████╔╝███████╗
//    ██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗╚════██║
//    ██║   ██║██║ ╚═╝ ██║███████╗██║  ██║███████║
//    ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝
typedef struct {
  dn_ring_buffer(f64) queue;
  f64 time_begin;
} dn_time_metric_t;

typedef struct {
  gs_hash_table(dn_hash_t, dn_time_metric_t) metrics;
} dn_time_metrics_t;
dn_time_metrics_t dn_time_metrics;

DN_API void              dn_time_metric_init(dn_time_metric_t* metric);
DN_API void              dn_time_metric_begin_ex(dn_time_metric_t* metric);
DN_API void              dn_time_metric_end_ex(dn_time_metric_t* metric);
DN_API f64               dn_time_metric_average(dn_time_metric_t* metric);
DN_API f64               dn_time_metric_last(dn_time_metric_t* metric);
DN_API f64               dn_time_metric_largest(dn_time_metric_t* metric);
DN_API f64               dn_time_metric_smallest(dn_time_metric_t* metric);
DN_API void              dn_time_metric_busy_wait(dn_time_metric_t* metric, f64 target);
DN_API void              dn_time_metric_sleep_wait(dn_time_metric_t* metric, f64 target);
DN_API void              dn_time_metric_begin(dn_string_t name);
DN_API void              dn_time_metric_end(dn_string_t name);
DN_API dn_time_metric_t* dn_time_metrics_find(dn_string_t name);
DN_API void              dn_time_metrics_add(dn_string_t name);
DN_IMP void              dn_time_metrics_init();
DN_IMP void              dn_time_metrics_update();


//  ██╗      ██████╗  ██████╗
//  ██║     ██╔═══██╗██╔════╝
//  ██║     ██║   ██║██║  ███╗
//  ██║     ██║   ██║██║   ██║
//  ███████╗╚██████╔╝╚██████╔╝
//  ╚══════╝ ╚═════╝  ╚═════╝
typedef enum {
  DN_LOG_FLAG_CONSOLE = 1,
  DN_LOG_FLAG_FILE = 2,
  DN_LOG_FLAG_DEFAULT = 3,
} dn_log_flags_t;

typedef enum {
  DN_LOG_LEVEL_INFO = 1,
  DN_LOG_LEVEL_TRACE = 2,
} dn_log_level_t;

#define DN_LOGGER_MESSAGE_BUFFER_SIZE 4096
#define DN_LOGGER_PREAMBLE_BUFFER_SIZE 512
typedef struct {
  char message_buffer [DN_LOGGER_MESSAGE_BUFFER_SIZE];
  char preamble_buffer [DN_LOGGER_PREAMBLE_BUFFER_SIZE];
  u32 level;
} dn_log_t;
dn_log_t dn_logger;

DN_API void dn_log(const char* fmt, ...);
DN_API void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...);
DN_API void dn_log_builder(dn_string_builder_t builder);
DN_API void dn_log_str(dn_string_t message);
DN_IMP void dn_log_v(dn_log_flags_t flags, const char* fmt, va_list fmt_args);
DN_IMP void dn_log_format_str(dn_string_t fmt, ...);
DN_IMP void dn_log_flush(dn_log_flags_t flags);
DN_IMP void dn_log_build_preamble();
DN_IMP void dn_log_zero();
DN_IMP void dn_log_init();
#define DN_LOG(fmt, ...) dn_log("%s: " fmt, __func__, __VA_ARGS__)
#define DN_LOG_2(message) dn_log("%s: " message, __func__)
#define DN_LOG_FN() dn_log("%s", __func__)


//  ██████╗  █████╗ ████████╗██╗  ██╗███████╗
//  ██╔══██╗██╔══██╗╚══██╔══╝██║  ██║██╔════╝
//  ██████╔╝███████║   ██║   ███████║███████╗
//  ██╔═══╝ ██╔══██║   ██║   ██╔══██║╚════██║
//  ██║     ██║  ██║   ██║   ██║  ██║███████║
//  ╚═╝     ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚══════╝
typedef struct {
  dn_string_t name;
  dn_string_t path;
} dn_named_path_t;

typedef struct {
  dn_named_path_t* data;
  u32 size;
} dn_named_path_result_t;

typedef struct {
  dn_string_t engine;
  dn_string_t install;
  dn_string_t write;
} dn_path_config_t;

typedef struct {
  gs_hash_table(dn_hash_t, dn_string_t) entries;
} dn_paths_t;
dn_paths_t dn_paths;

#define DN_USER_PATH_LAYOUTS dn_string_literal("layouts")
#define DN_USER_PATH_LAYOUT  dn_string_literal("layout")
#define DN_PATH_LAYOUTS      dn_string_literal("dn_layouts")
#define DN_PATH_LAYOUT       dn_string_literal("dn_layout")

DN_API dn_named_path_result_t dn_paths_find_all();
DN_API void                   dn_paths_add_install_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_write_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_engine_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_app_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_subpath(dn_string_t name, dn_string_t parent_name, dn_string_t relative_path);
DN_API dn_string_t            dn_paths_resolve(dn_string_t name);
DN_API dn_string_t            dn_paths_resolve_format(dn_string_t name, dn_string_t file_name);
DN_API dn_string_t            dn_paths_resolve_cstr(const char* name);
DN_API const char*            dn_paths_resolve_bootstrap(const char* name);
DN_API dn_string_t            dn_paths_resolve_ex(dn_string_t name, dn_allocator_t* allocator);
DN_API dn_string_t            dn_paths_resolve_format_cstr(const char* name, const char* file_name);
DN_API dn_string_t            dn_paths_resolve_format_ex(dn_string_t name, dn_string_t file_name, dn_allocator_t* allocator);
DN_API dn_string_t            dn_paths_strip(dn_string_t name, dn_string_t absolute_path);
DN_API void                   dn_paths_add_ex(dn_string_t name, dn_string_t absolute_path);
DN_IMP void                   dn_paths_init();
DN_IMP dn_string_t            _dn_paths_build_root_path();
DN_IMP void                   _dn_paths_set_install_roots(dn_path_config_t config);
DN_IMP void                   _dn_paths_set_write_path(dn_path_config_t config);


//  █████╗ ██╗   ██╗██████╗ ██╗ ██████╗ 
// ██╔══██╗██║   ██║██╔══██╗██║██╔═══██╗
// ███████║██║   ██║██║  ██║██║██║   ██║
// ██╔══██║██║   ██║██║  ██║██║██║   ██║
// ██║  ██║╚██████╔╝██████╔╝██║╚██████╔╝
// ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝                                      
typedef enum {
  DN_AUDIO_FILTER_MODE_FIRST_ORDER = 0,
  DN_AUDIO_FILTER_MODE_BUTTERWORTH = 1,
} dn_audio_filter_mode_t;

typedef struct {
  dn_audio_filter_mode_t mode;
  bool enabled;
  f32 cutoff_frequency;
  f32 cutoff_alpha;
  f32 a0, a1, a2, b1, b2;
  f32 input_history [2];
  f32 output_history [2];
} dn_low_pass_filter_t;

typedef struct {
  f32 threshold;
  f32 ratio;
  f32 attack_time;
  f32 release_time;
  bool enabled;
} dn_compressor_t;

typedef dn_pool_handle_t dn_audio_info_handle_t;
typedef struct {
  dn_asset_name_t name;
  dn_hash_t hash;
  u32 num_channels;
  u32 sample_rate;
  u64 num_frames;
  u32 num_samples;
  f32* samples;
  u32 generation;
  double file_mod_time;
} dn_audio_info_t;

typedef dn_pool_handle_t dn_audio_instance_handle_t;
typedef struct {
  dn_audio_info_handle_t info;
  dn_audio_instance_handle_t next;
  u32 next_sample;
  bool loop;
  f32 volume;
  dn_low_pass_filter_t filter;
  bool paused;
  s32 sample_buffer_offset;
  s32 samples_from_next;
  
  bool occupied;
  u32 generation;
} dn_audio_instance_t;

typedef struct {
  dn_path_t* dirs;
  u32 num_dirs;
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  f32 sample_frequency;
  f32 master_volume;
  f32 master_volume_mod;
} dn_audio_config_t;

#define DN_AUDIO_MAX_SOUNDS 1024
#define DN_AUDIO_MAX_INSTANCES 128
typedef struct {
  dn_mutex_t mutex;
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  f32 sample_frequency;
  f32 master_volume;
  f32 master_volume_mod;
  // FileMonitor* file_monitor;
  dn_fixed_array(f32, DN_FIXED_ARRAY_RUNTIME_SIZE) sample_buffer;
  dn_pool(dn_audio_info_t) sounds;
  dn_pool(dn_audio_instance_t) instances;
} dn_audio_t;
dn_audio_t dn_audio;

DN_API void                       dn_audio_set_compressor_threshold(f32 t);
DN_API void                       dn_audio_set_compressor_ratio(f32 v);
DN_API void                       dn_audio_set_compressor_attack(f32 v);
DN_API void                       dn_audio_set_compressor_release(f32 v);
DN_API void                       dn_audio_set_sample_rate(f32 v);
DN_API f32                        dn_audio_get_master_volume();
DN_API void                       dn_audio_set_master_volume(f32 v);
DN_API f32                        dn_audio_get_master_volume_mod();
DN_API void                       dn_audio_set_master_volume_mod(f32 v);
DN_API f32                        dn_audio_get_master_filter_cutoff();
DN_API void                       dn_audio_set_master_filter_cutoff(f32 v);
DN_API void                       dn_audio_set_master_filter_cutoff_enabled(bool enabled);
DN_API void                       dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode);
DN_API void                       dn_audio_set_volume(dn_audio_instance_handle_t handle, f32 volume);
DN_API void                       dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, f32 cutoff);
DN_API void                       dn_audio_set_filter_mode(dn_audio_instance_handle_t handle, dn_audio_filter_mode_t mode);
DN_API void                       dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled);
DN_API dn_audio_instance_handle_t dn_audio_play_sound(const char* name);
DN_API dn_audio_instance_handle_t dn_audio_play_looped(const char* name);
DN_API void                       dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next);
DN_API void                       dn_audio_stop(dn_audio_instance_handle_t handle);
DN_API void                       dn_audio_stop_all();
DN_API void                       dn_audio_pause(dn_audio_instance_handle_t handle);
DN_API void                       dn_audio_resume(dn_audio_instance_handle_t handle);
DN_API bool                       dn_audio_is_playing(dn_audio_instance_handle_t handle);
DN_API bool                       dn_audio_is_any_playing();
DN_API void                       dn_audio_load(const char* file_path, const char* file_name);
DN_API void                       dn_audio_load_dir(const char* path);
DN_API void                       dn_low_pass_filter_set_mode(dn_low_pass_filter_t* filter, dn_audio_filter_mode_t mode);
DN_API void                       dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, f32 cutoff);
DN_API f32                        dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, f32 input);
DN_API dn_audio_config_t          dn_audio_config_default();
DN_API void                       dn_audio_init(dn_audio_config_t config);
DN_IMP void                       dn_audio_update(f32* buffer, int frames_requested, int num_channels);
DN_IMP void                       dn_audio_shutdown();
DN_IMP dn_audio_info_t*           dn_audio_find(dn_string_t name);
DN_IMP dn_audio_info_t*           dn_audio_find_no_default(dn_string_t name);
DN_IMP dn_audio_info_t*           dn_audio_resolve(dn_audio_info_handle_t handle);
DN_IMP dn_audio_instance_t*       dn_audio_resolve_instance(dn_audio_instance_handle_t handle);
DN_IMP dn_audio_instance_handle_t dn_audio_reserve();
DN_IMP dn_audio_instance_handle_t dn_audio_play_sound_ex(dn_audio_info_t* sound, bool loop);
DN_IMP void                       dn_audio_stop_ex(dn_audio_instance_t* active_sound);



// ██╗███╗   ███╗ █████╗  ██████╗ ███████╗███████╗
// ██║████╗ ████║██╔══██╗██╔════╝ ██╔════╝██╔════╝
// ██║██╔████╔██║███████║██║  ███╗█████╗  ███████╗
// ██║██║╚██╔╝██║██╔══██║██║   ██║██╔══╝  ╚════██║
// ██║██║ ╚═╝ ██║██║  ██║╚██████╔╝███████╗███████║
// ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝
typedef struct {
  dn_hash_t hash;
  u32 handle;
  u32 width;
  u32 height;
  u32 channels;
} dn_texture_t;

DN_API void          dn_texture_init(s32 width, s32 height, s32 channels);
DN_API void          dn_texture_load_to_gpu(u32* data);
DN_API void          dn_texture_unload_from_gpu();
DN_API dn_texture_t* dn_images_find_texture_by_name(dn_string_t name);
DN_API dn_texture_t* dn_images_find_texture_by_hash(dn_hash_t hash);

typedef struct {
  dn_string_t file_path;
  dn_hash_t hash;
  dn_hash_t texture;
  dn_vector2_t* uv;
  dn_vector2i_t size;
} dn_sprite_t;

DN_API dn_sprite_t* dn_images_find_sprite(const char* name);
DN_API dn_sprite_t* dn_images_find_sprite_no_default(const char* name);


typedef struct {
  s32 id;
  dn_sprite_t* sprite;
  u32* data;
  s32 channels;
} dn_atlas_pack_item_t;

void dn_atlas_pack_item_init(dn_atlas_pack_item_t* item);

#define DN_MAX_DIRECTORIES_PER_ATLAS 32
#define DN_MAX_IMAGES_PER_ATLAS 256
#define DN_TEXTURE_ATLAS_SIZE 2048

typedef struct {
  char name [64];
  double cfg_mod_time;
  double mod_time;
  dn_hash_t cfg_files_hash;
  dn_hash_t files_hash;
  bool high_priority;
  // dn_array_t<char*> directories;
  dn_texture_t* texture;

  // Data used for generation. These are initialized to temporary storage, so
  // don't expect them to be valid in other cases.
  // dn_array_t<RectPackId> ids;
  // dn_array_t<stbrp_rect> rects;
  // dn_array_t<stbrp_node> nodes;
  // dn_array_t<u32> buffer;

  bool need_async_build;
  bool need_async_load;
  bool need_gl_init;
  bool need_config_update;

  // FileMonitor* file_monitor;
} dn_atlas_t;

typedef struct {
  dn_dynamic_array(dn_string_t) files;
} dn_atlas_bucket_t;

typedef struct {
  dn_string_t* dirs;
  u32 num_dirs;
} dn_image_config_t;

typedef struct {
  dn_mutex_t mutex;
  dn_mutex_t config_mutex;

  dn_fixed_array(dn_vector2_t, 65536) uv_data;
  dn_fixed_array(dn_texture_t, 256) textures;
  dn_fixed_array(dn_sprite_t, 1024) sprites;
  dn_fixed_array(dn_atlas_t, 64) atlases;
  dn_fixed_array(dn_atlas_bucket_t, 16) buckets;
} dn_images_t;
dn_images_t dn_images;

void                  dn_atlases_init();
void                  dn_atlas_deinit();
void                  dn_atlas_set_name(const char* name);
void                  dn_atlas_build_sync();
void                  dn_atlas_load_to_memory();
bool                  dn_atlas_is_dirty();
void                  dn_atlas_calc_hash_and_mod_time();  
void                  dn_atlas_build_from_config();
void                  dn_atlas_build_from_source();
void                  dn_atlas_write_to_config();
void                  dn_atlas_write_to_png();
void                  dn_atlas_load_to_gpu();
void                  dn_atlas_unload_from_gpu();
void                  dn_atlas_watch_files();
void                  dn_atlas_on_file_add(const char* file_path);
void                  dn_atlas_on_file_change(const char* file_path);
dn_atlas_pack_item_t* dn_atlas_find_pack_item(s32 id);
void                  dn_image_init_atlases();
dn_sprite_t*          dn_image_alloc_sprite();
dn_texture_t*         dn_image_alloc_texture();
dn_vector2_t*         dn_image_alloc_uvs();
void                  dn_image_build_sprite_from_file(const char* id, const char* file_path);
void                  dn_image_build_sprite_from_data(const char* id, unsigned char* data, s32 width, s32 height, s32 channels);
void                  dn_image_build_sprite_ex(dn_sprite_t* sprite, const char* id, unsigned char* data, s32 width, s32 height, s32 channels);
DN_IMP void           dn_images_init(dn_image_config_t config);
DN_IMP void           dn_atlas_bucket_init(dn_atlas_bucket_t* bucket);
DN_IMP u32            dn_atlas_calc_bucket(dn_string_t path);
DN_API void           dn_screenshots_init();
DN_API void           dn_screenshot_take();
DN_API void           dn_screenshot_write(const char* file_name);


// ██████╗  █████╗  ██████╗██╗  ██╗ ██████╗ ██████╗  ██████╗ ██╗   ██╗███╗   ██╗██████╗ ███████╗
// ██╔══██╗██╔══██╗██╔════╝██║ ██╔╝██╔════╝ ██╔══██╗██╔═══██╗██║   ██║████╗  ██║██╔══██╗██╔════╝
// ██████╔╝███████║██║     █████╔╝ ██║  ███╗██████╔╝██║   ██║██║   ██║██╔██╗ ██║██║  ██║███████╗
// ██╔══██╗██╔══██║██║     ██╔═██╗ ██║   ██║██╔══██╗██║   ██║██║   ██║██║╚██╗██║██║  ██║╚════██║
// ██████╔╝██║  ██║╚██████╗██║  ██╗╚██████╔╝██║  ██║╚██████╔╝╚██████╔╝██║ ╚████║██████╔╝███████║
// ╚═════╝ ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚═════╝ ╚══════╝
#define DN_BACKGROUND_TILE_SIZE 2048

typedef struct {
  dn_texture_t* texture;
  u32* data;
} dn_background_loaded_tile_t;

typedef struct {
  char* name;
  char* tile_output_folder;
  char* tile_output_full_path;
  char* source_image;
  char* source_image_full_path;
  // dn_array_t<char*, 64> tiles;
  // dn_array_t<char*, 64> tile_full_paths;
  // dn_array_t<dn_vector2i_t, 64> tile_positions;
  s32 width;
  s32 height;
  s32 channels;
  u32* data;
  bool high_priority;

  f64 mod_time;
  f64 filesystem_mod_time;

  bool need_async_build;
  bool need_async_load;
  bool need_config_update;

  // dn_array_t<LoadedTile> loaded_tiles;
  bool gpu_ready;
  bool gpu_done;
  int gpu_load_index;
} dn_background_t;

typedef struct {
  dn_dynamic_array(dn_background_t) backgrounds;
} dn_backgrounds_t;


typedef struct {
  u32 current_tile;
  dn_mutex_t mutex;
} dn_background_tile_processor_shared_t;

typedef struct {
  u32* source_data;
  u32* tile_data;
  dn_thread_t thread;

} dn_background_tile_processor_t;

void dn_background_tile_processor_init(u32* source_data);
void dn_background_tile_processor_deinit();
void dn_background_tile_processor_process(dn_background_t* background);
void dn_background_init(dn_background_t* background);
void dn_background_deinit(dn_background_t* background);
void dn_background_load_paths(dn_background_t* background);
void dn_background_set_source_image_size(dn_background_t* background, s32 width, s32 height);
void dn_background_set_source_data(dn_background_t* background, u32* data);
bool dn_background_add_tile(dn_background_t* background);
void dn_background_add_tiles(dn_background_t* background);
bool dn_background_is_dirty(dn_background_t* background);
void dn_background_build_from_source(dn_background_t* background);
void dn_background_load_to_gpu(dn_background_t* background);
void dn_background_load_one_to_gpu(dn_background_t* background);
void dn_background_load_tiles(dn_background_t* background);
void dn_background_update_config(dn_background_t* background);
void dn_backgrounds_init();


//  █████╗ ███████╗███████╗███████╗████████╗███████╗
// ██╔══██╗██╔════╝██╔════╝██╔════╝╚══██╔══╝██╔════╝
// ███████║███████╗███████╗█████╗     ██║   ███████╗
// ██╔══██║╚════██║╚════██║██╔══╝     ██║   ╚════██║
// ██║  ██║███████║███████║███████╗   ██║   ███████║
// ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝   ╚═╝   ╚══════╝
typedef enum {
  DN_ASSET_KIND_BACKGROUND,
  DN_ASSET_KIND_ATLAS,
  DN_ASSET_KIND_USER,
} dn_asset_kind_t;

typedef enum {
  DN_ASSET_COMPLETION_STATUS_RUNNING,
  DN_ASSET_COMPLETION_STATUS_DONE,
} dn_asset_completion_status_t;

typedef void* dn_asset_data_t;

typedef struct {
  dn_asset_name_t name;
  dn_asset_data_t data;
} dn_asset_t;

////////////
// LOADER //
////////////
typedef struct {
  s32 id;
  dn_asset_kind_t kind;

  union {
    dn_background_t* background;
    dn_atlas_t* atlas;
  };
} dn_asset_load_request_t;

typedef struct {
  dn_thread_t thread;
  dn_condition_variable_t condition;
  dn_mutex_t mutex;

  dn_ring_buffer(dn_asset_load_request_t) load_requests;
  dn_ring_buffer(dn_asset_load_request_t) completion_queue;
} dn_asset_loader_t;

DN_IMP void dn_asset_loader_process_requests();
DN_IMP void dn_asset_loader_process_completion_queue();
DN_IMP void dn_asset_loader_submit(dn_asset_load_request_t request);

//////////////
// IMPORTER //
//////////////
typedef struct {
  void* user_data;
} dn_asset_import_request_t;

dn_typedef_fn(void,                         dn_asset_import_fn,     dn_asset_import_request_t* request);
dn_typedef_fn(dn_asset_completion_status_t, dn_asset_completion_fn, dn_asset_import_request_t* request);

typedef struct {
  dn_asset_name_t id;
  dn_asset_import_fn on_import;
  dn_asset_completion_fn on_complete;
} dn_asset_importer_t;

//////////////
// REGISTRY //
//////////////
typedef struct {
  struct {
    dn_asset_importer_t* data;
    u32 count;
  } importers;
} dn_asset_config_t;

typedef struct {
  gs_hash_table(dn_hash_t, dn_asset_t) assets;
  gs_hash_table(dn_hash_t, dn_asset_importer_t) importers;
} dn_asset_registry_t;

DN_API void            dn_asset_registry_init(dn_asset_config_t config);
DN_API void            dn_asset_registry_add(const char* name, dn_asset_data_t data);
DN_API dn_asset_data_t dn_asset_registry_find(const char* name);
DN_IMP void            dn_asset_registry_update();

/////////////////////////
// BACKGROUND IMPORTER //
/////////////////////////
typedef enum {
  DN_BACKGROUND_COMPLETION_STATUS_UPDATE_CONFIG,
  DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES,
  DN_BACKGROUND_COMPLETION_STATUS_DONE,
} dn_background_completion_status_t;

typedef struct {
  dn_background_completion_status_t status;
  dn_background_t* background;
} dn_background_import_request_t;

DN_IMP void                         dn_background_import(dn_asset_import_request_t* request);
DN_IMP dn_asset_completion_status_t dn_background_complete(dn_asset_import_request_t* request);


typedef struct {
  // Input
  const char* file_path;
  struct {
    dn_path_t* data;
    u32 count;
  } include_dirs;

  // Output
  char error [256];
  char* result;

  // Work
  dn_os_file_t file;
  dn_string_t file_data;
} dn_preprocessor_context_t;

DN_API void dn_preprocess(dn_preprocessor_context_t* context);


//  ████████╗███████╗███████╗████████╗██╗███╗   ██╗ ██████╗
//  ╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██║████╗  ██║██╔════╝
//     ██║   █████╗  ███████╗   ██║   ██║██╔██╗ ██║██║  ███╗
//     ██║   ██╔══╝  ╚════██║   ██║   ██║██║╚██╗██║██║   ██║
//     ██║   ███████╗███████║   ██║   ██║██║ ╚████║╚██████╔╝
//     ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝
#define DN_TEST_COLOR_BOLD "\033[1m"
#define DN_TEST_COLOR_RESET "\033[0m"
#define DN_TEST_COLOR_SUCCESS "\033[32m"
#define DN_TEST_COLOR_DESC "\033[33m"
#define DN_TEST_COLOR_FAIL "\033[31m"
#define DN_TEST_COLOR_MIXED "\033[35m"

typedef enum {
  DN_TEST_CONTEXT_FLAG_DEFAULT = 0,
  DN_TEST_CONTEXT_FLAG_LOG_FAILURE = 1 << 0,
  DN_TEST_CONTEXT_FLAG_LOG_SUCCESS = 1 << 1,
} dn_test_context_flags_t;

typedef struct {
  dn_string_t description;
  bool success;
} dn_assertion_t;

typedef struct {
  dn_string_t name;
  bool success;
  u32 num_assertions;
  dn_dynamic_array(dn_assertion_t) assertions;
} dn_test_context_t;

typedef struct {
  dn_string_t name;
  dn_test_context_flags_t flags;
  dn_dynamic_array(dn_test_context_t) tests;
} dn_test_suite_t;

typedef struct {
  dn_test_suite_t* suite;
  dn_test_context_t* context;
} dn_tests_t;
dn_tests_t dn_tests;

#define dn_test_assert_ex(desc, condition) \
do { \
  dn_assertion_t* assertion = (dn_assertion_t*)dn_dynamic_array_push(&dn_tests.context->assertions, dn_ptr_lval(dn_assertion_t, dn_zero_initialize())); \
  assertion->description = desc; \
  assertion->success = (condition); \
  dn_tests.context->num_assertions++;  \
} while (false);

#define dn_test_assert(condition)            dn_test_assert_ex(dn_string_literal(dn_macro_str(condition)), (condition))
#define dn_test_assert_desc(desc, condition) dn_test_assert_ex(dn_string_literal(desc),                    (condition))

void dn_test_init();
void dn_test_context_init(dn_test_context_t* context);
void dn_test_suite_init(dn_test_suite_t* suite);
void dn_test_begin_suite(dn_test_suite_t* suite);
void dn_test_begin(dn_string_t name);
void dn_test_end();
void dn_test_end_suite();


//  ██╗    ██╗██╗███╗   ██╗██████╗  ██████╗ ██╗    ██╗
//  ██║    ██║██║████╗  ██║██╔══██╗██╔═══██╗██║    ██║
//  ██║ █╗ ██║██║██╔██╗ ██║██║  ██║██║   ██║██║ █╗ ██║
//  ██║███╗██║██║██║╚██╗██║██║  ██║██║   ██║██║███╗██║
//  ╚███╔███╔╝██║██║ ╚████║██████╔╝╚██████╔╝╚███╔███╔╝
//   ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═════╝  ╚═════╝  ╚══╝╚══╝
typedef enum {
  DN_WINDOW_FLAG_NONE,
  DN_WINDOW_FLAG_WINDOWED,
  DN_WINDOW_FLAG_BORDER,
  DN_WINDOW_FLAG_VSYNC,
} dn_window_flags_t;

typedef enum {
  DN_DISPLAY_MODE_AUTO,

  // 16:9
  DN_DISPLAY_MODE_180,
  DN_DISPLAY_MODE_480,
  DN_DISPLAY_MODE_720,
  DN_DISPLAY_MODE_1080,
  DN_DISPLAY_MODE_1440,
  DN_DISPLAY_MODE_2160,

  // 16:10
  DN_DISPLAY_MODE_1280_800,

  // Fullscreen
  DN_DISPLAY_MODE_FULLSCREEN
} dn_display_mode_t;

typedef struct {
  dn_string_t title;
  dn_string_t icon;
  dn_display_mode_t display_mode;
  dn_vector2_t native_resolution;
  dn_window_flags_t flags;
  u32 target_fps;
} dn_window_config_t;

#ifndef DN_BUILD_FFI
  typedef GLFWwindow* dn_window_handle_t;
#else
  typedef void* dn_window_handle_t;
#endif

typedef struct {
  dn_window_handle_t handle;
  dn_window_flags_t flags;
  dn_display_mode_t display_mode;
  dn_vector2i_t windowed_position;
  dn_vector2_t native_resolution;
  dn_vector2_t requested_area;
  dn_vector2_t content_area;
} dn_window_t;

DN_API dn_window_config_t dn_window_config_default();
DN_API void               dn_window_init(dn_window_config_t config);
DN_API void               dn_window_set_native_resolution(float width, float height);
DN_API dn_vector2_t       dn_window_get_content_area();
DN_API dn_vector2_t       dn_window_get_native_resolution();
DN_API void               dn_window_set_icon(dn_string_t path);
DN_API void               dn_window_set_display_mode(dn_display_mode_t mode);
DN_API dn_display_mode_t  dn_window_get_display_mode();
DN_API void               dn_window_set_cursor_visible(bool visible);
DN_API void               dn_window_set_size(int x, int y);
DN_IMP void               dn_window_shutdown();



typedef enum {
	// Exactly where you are on the monitor. In other words, a fraction of the output resolution
	// in screen units of [0, 1]
	DN_COORD_UNIT_SCREEN = 0,

	// Where you are on the window of the screen displaying the game; the game is rendered
	// to a framebuffer, which could be displayed as some fraction of the screen in any position.
	// Window dn_coord_data take into account the position and size of that framebuffer. In the
	// case where the game is running full screen, this is equivalent to Screen
	//
	// In other words, a fraction of the framebuffer resolution in screen units of [0, 1]
	DN_COORD_UNIT_WINDOW = 1,

	// Same as Window, except it's in the range of [0, native_resolution]
	DN_COORD_UNIT_GAME = 2,

	// Same as Game, except takes the camera into account
	DN_COORD_UNIT_WORLD = 3,
} dn_coord_t;

typedef struct {
	dn_vector2_t camera;
	dn_vector2_t framebuffer_position;
	dn_vector2_t framebuffer_size;
} dn_coord_data_t;

DN_API dn_coord_data_t dn_coord_get();
DN_API void            dn_coord_set_camera(float x, float y);
DN_API void            dn_coord_set_framebuffer_position(float x, float y);
DN_API void            dn_coord_set_framebuffer_size(float x, float y);
DN_API dn_vector2_t    dn_coord_screen_to_window(float x, float y);
DN_API dn_vector2_t    dn_coord_screen_to_game(float x, float y);
DN_API dn_vector2_t    dn_coord_screen_to_world(float x, float y);
DN_API dn_vector2_t    dn_coord_window_to_screen(float x, float y);
DN_API dn_vector2_t    dn_coord_window_to_game(float x, float y);
DN_API dn_vector2_t    dn_coord_window_to_world(float x, float y);
DN_API dn_vector2_t    dn_coord_game_to_screen(float x, float y);
DN_API dn_vector2_t    dn_coord_game_to_window(float x, float y);
DN_API dn_vector2_t    dn_coord_game_to_world(float x, float y);
DN_API dn_vector2_t    dn_coord_world_to_screen(float x, float y);
DN_API dn_vector2_t    dn_coord_world_to_window(float x, float y);
DN_API dn_vector2_t    dn_coord_world_to_game(float x, float y);
DN_API dn_vector2_t    dn_coord_screen_to_window_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_screen_to_game_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_screen_to_world_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_window_to_screen_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_window_to_game_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_window_to_world_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_game_to_screen_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_game_to_window_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_game_to_world_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_world_to_screen_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_world_to_window_mag(float x, float y);
DN_API dn_vector2_t    dn_coord_world_to_game_mag(float x, float y);


// ██╗███╗   ██╗██████╗ ██╗   ██╗████████╗
// ██║████╗  ██║██╔══██╗██║   ██║╚══██╔══╝
// ██║██╔██╗ ██║██████╔╝██║   ██║   ██║   
// ██║██║╚██╗██║██╔═══╝ ██║   ██║   ██║   
// ██║██║ ╚████║██║     ╚██████╔╝   ██║   
// ╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝    ╚═╝   
#define GLFW_KEY_CONTROL 349
#define GLFW_KEY_SUPER 350
#define GLFW_KEY_SHIFT 351
#define GLFW_KEY_ALT 352

#ifdef DN_BUILD_FFI
#define GLFW_KEY_LAST 348
#endif

typedef enum {
	DN_INPUT_DEVICE_MOUSE_AND_KEYBOARD = 0,
	DN_INPUT_DEVICE_CONTROLLER = 1,
} dn_input_device_t;

typedef struct {
	dn_vector2_t framebuffer_position;
	dn_vector2_t framebuffer_size;
	dn_vector2_t camera;
	dn_vector2_t mouse;
	dn_vector2_t mouse_delta;
	dn_vector2_t scroll;
	bool got_keyboard_input;
	bool got_mouse_input;

	bool is_down[GLFW_KEY_LAST];
	bool was_down[GLFW_KEY_LAST];
	char shift_map[128];
} dn_input_t;

DN_API bool              dn_input_pressed(s32 key);
DN_API bool              dn_input_released(s32 key);
DN_API bool              dn_input_down(s32 key);
DN_API bool              dn_input_mod_down(s32 mod);
DN_API bool              dn_input_chord_pressed(s32 mod, s32 key);
DN_API dn_vector2_t      dn_input_scroll();
DN_API dn_vector2_t      dn_input_mouse(dn_coord_t unit);
DN_API dn_vector2_t      dn_input_mouse_delta(dn_coord_t unit);
DN_API u32               dn_input_shift_key(u32 key);
DN_API dn_input_device_t dn_input_get_device();
DN_IMP void              dn_input_init();
DN_IMP void              dn_input_update();       
DN_IMP void              dn_input_callback_cursor(GLFWwindow* window, f64 xpos, f64 ypos);
DN_IMP void              dn_input_callback_click(GLFWwindow* window, s32 button, s32 action, s32 mods);
DN_IMP void              dn_input_callback_key(GLFWwindow* window, s32 key, s32 scancode, s32 action, s32 mods);
DN_IMP void              dn_input_callback_scroll(GLFWwindow* window, f64 xoffset, f64 yoffset);
DN_IMP void              dn_input_callback_error(s32 err, const char* msg);
DN_IMP void              dn_input_callback_window_size(GLFWwindow* window, s32 width, s32 height);


typedef struct {
  dn_vector4_t light;
  dn_vector4_t medium_light;
  dn_vector4_t low_light;
  dn_vector4_t neutral;
  dn_vector4_t medium_dark;
} dn_imgui_colors_t;

typedef struct {
  dn_string_t queued_layout;
  dn_imgui_colors_t colors;
} dn_imgui_t;
dn_imgui_t dn_imgui;

DN_API void         dn_imgui_push_font(const char* font_name, u32 size);
DN_API void         dn_imgui_image(const char* image, float sx, float sy);
DN_API void         dn_imgui_file_browser_open();
DN_API void         dn_imgui_file_browser_close();
DN_API void         dn_imgui_file_browser_set_work_dir(const char* directory);
DN_API bool         dn_imgui_file_browser_is_file_selected();
DN_API dn_tstring_t dn_imgui_file_browser_get_selected_file();
DN_API void         dn_imgui_load_layout(dn_string_t file_name);
DN_API void         dn_imgui_save_layout(dn_string_t file_name);
DN_API void         dn_imgui_load_colors(dn_imgui_colors_t colors);
DN_IMP void         dn_imgui_init();
DN_IMP void         dn_imgui_shutdown();
DN_IMP void         dn_imgui_update();


//  ██╗     ██╗   ██╗ █████╗
//  ██║     ██║   ██║██╔══██╗
//  ██║     ██║   ██║███████║
//  ██║     ██║   ██║██╔══██║
//  ███████╗╚██████╔╝██║  ██║
//  ╚══════╝ ╚═════╝ ╚═╝  ╚═╝
#ifndef DN_BUILD_FFI
  typedef lua_State* dn_lua_interpreter_t;
#else
  typedef void* dn_lua_interpreter_t;
#endif

#define DN_LUA_OK 0
#define DN_LUA_ERROR_PATH_DOES_NOT_EXIST 1
#define DN_LUA_ERROR_PATH_IS_NOT_DIRECTORY 2
#define DN_LUA_ERROR_FILE_LOAD_ERROR 3
#define DN_LUA_ERROR_FILE_RUN_ERROR 4

typedef struct {
  dn_string_t scripts;
} dn_lua_config_t;

#define DN_LUA_MAX_DIRECTORIES 32
typedef struct {
  dn_string_t scripts;
  dn_lua_interpreter_t state;
  dn_fixed_array(dn_string_t, DN_LUA_MAX_DIRECTORIES) directories;
} dn_lua_t;
dn_lua_t dn_lua;

DN_IMP void        dn_lua_init(dn_lua_config_t config);
DN_IMP dn_error_t  dn_lua_script_file(dn_string_t file_path);
DN_IMP dn_error_t  dn_lua_script_dir(dn_string_t path);
DN_IMP dn_error_t  dn_lua_script_named_dir(dn_string_t name);
DN_IMP s32         dn_lua_directory_sort_kernel(const void* va, const void* vb);
DN_IMP const char* dn_lua_format_file_load_error(const char* error);
DN_IMP s32         dn_lua_format_file_load_error_l(dn_lua_interpreter_t lua);


// ███████╗ ██████╗ ███╗   ██╗████████╗███████╗
// ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔════╝
// █████╗  ██║   ██║██╔██╗ ██║   ██║   ███████╗
// ██╔══╝  ██║   ██║██║╚██╗██║   ██║   ╚════██║
// ██║     ╚██████╔╝██║ ╚████║   ██║   ███████║
// ╚═╝      ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚══════╝
typedef enum {
  DN_FONT_FLAG_NONE = 0,
  DN_FONT_FLAG_IMGUI = 1,
  DN_FONT_FLAG_DEFAULT = 2,
  DN_FONT_FLAG_BOLD = 4,
  DN_FONT_FLAG_EXTRABOLD = 8,
  DN_FONT_FLAG_ITALIC = 16,
} dn_font_flags_t;

typedef struct {
  dn_vector2_t* verts;
  dn_vector2_t* uv;
  dn_vector2_t size;
  dn_vector2_t bearing;
  dn_vector2_t advance;
  f32 descender;
} dn_baked_glyph_t;

typedef struct {
  dn_asset_name_t name;
  dn_hash_t hash;
  dn_path_t path;
  dn_font_flags_t flags;
  
  u32 size;
  u32 texture;
  // ImFont* imfont;
  dn_vector2_t resolution;
   
  dn_baked_glyph_t* glyphs;
  dn_vector2_t max_advance;
  dn_vector2_t max_glyph;
  f32 line_spacing;
} dn_baked_font_t;

typedef struct {
  const char* id;
  const char* file_path;
  u32 sizes [16];
  dn_font_flags_t flags;
} dn_font_descriptor_t;

typedef struct {
  dn_font_descriptor_t* fonts;
  u32 num_fonts;
} dn_font_config_t;

typedef struct {
  dn_fixed_array(dn_baked_font_t, 64) baked_fonts;
  dn_fixed_array(dn_baked_glyph_t, 8192) baked_glyphs;
  dn_fixed_array(dn_vector2_t, 65536>) vertex_data;
  dn_fixed_array(dn_vector2_t, 65536>) uv_data;

  dn_baked_font_t* default_font;
} dn_fonts_t;
dn_fonts_t dn_fonts;

DN_API void             dn_font_init(dn_font_config_t config);
DN_API void             dn_font_bake_n(dn_font_descriptor_t* descriptors, u32 num_descriptors);
DN_API void             dn_font_bake(dn_font_descriptor_t descriptor);
DN_API dn_font_config_t dn_font_config_default();
DN_API dn_baked_font_t* dn_font_default();
DN_API dn_baked_font_t* dn_font_find(const char* id, u32 size);
DN_IMP dn_hash_t        dn_font_hash(const char* id, u32 size);


//  ██████╗ ██████╗ ██╗   ██╗
// ██╔════╝ ██╔══██╗██║   ██║
// ██║  ███╗██████╔╝██║   ██║
// ██║   ██║██╔═══╝ ██║   ██║
// ╚██████╔╝██║     ╚██████╔╝
//  ╚═════╝ ╚═╝      ╚═════╝ 
#define DN_GPU_NEAR_PLANE -100.0
#define DN_GPU_FAR_PLANE 100.0

typedef enum {
  DN_GPU_COMMAND_OP_BIND_BUFFERS = 10,
  DN_GPU_COMMAND_OP_BEGIN_RENDER_PASS = 20,
  DN_GPU_COMMAND_OP_END_RENDER_PASS = 21,
  DN_GPU_COMMAND_OP_BIND_PIPELINE = 30,
  DN_GPU_COMMAND_OP_SET_CAMERA = 40,
  DN_GPU_COMMAND_OP_SET_LAYER = 41,
  DN_GPU_COMMAND_OP_SET_WORLD_SPACE = 42,
  DN_GPU_COMMAND_OP_SET_SCISSOR = 43,
  DN_GPU_COMMAND_OP_DRAW = 70,
} dn_gpu_command_op_t;

typedef enum {
  DN_GPU_PRIMITIVE_TRIANGLES = 0
} dn_gpu_draw_primitive_t;

typedef enum {
  DN_GPU_DRAW_MODE_ARRAYS = 0,
  DN_GPU_DRAW_MODE_INSTANCE = 1,
} dn_gpu_draw_mode_t;

typedef enum {
  DN_GPU_VERTEX_ATTRIBUTE_FLOAT = 0,
  DN_GPU_VERTEX_ATTRIBUTE_U32 = 1,
} dn_gpu_vertex_attribute_kind_t;

typedef enum {
  DN_GPU_UNIFORM_NONE = 0,
  DN_GPU_UNIFORM_MATRIX4 = 1,
  DN_GPU_UNIFORM_MATRIX3 = 2,
  DN_GPU_UNIFORM_MATRIX2 = 3,
  DN_GPU_UNIFORM_VECTOR4 = 4,
  DN_GPU_UNIFORM_VECTOR3 = 5,
  DN_GPU_UNIFORM_VECTOR2 = 6,
  DN_GPU_UNIFORM_I32 = 7,
  DN_GPU_UNIFORM_F32 = 8,
  DN_GPU_UNIFORM_TEXTURE = 9,
  DN_GPU_UNIFORM_ENUM = 10,
} dn_gpu_uniform_kind_t;

typedef enum {
  DN_GPU_BUFFER_KIND_STORAGE = 0,
  DN_GPU_BUFFER_KIND_ARRAY = 1,
  DN_GPU_BUFFER_KIND_UNIFORM = 2,
} dn_gpu_buffer_kind_t;

typedef enum {
  DN_GPU_BUFFER_USAGE_STATIC = 0,
  DN_GPU_BUFFER_USAGE_DYNAMIC = 1,
  DN_GPU_BUFFER_USAGE_STREAM = 2,
} dn_gpu_buffer_usage_t;

typedef enum {
  DN_GPU_BLEND_FUNC_NONE,
  DN_GPU_BLEND_FUNC_ADD,
  DN_GPU_BLEND_FUNC_SUBTRACT,
  DN_GPU_BLEND_FUNC_REVERSE_SUBTRACT,
  DN_GPU_BLEND_FUNC_MIN,
  DN_GPU_BLEND_FUNC_MAX
} dn_gpu_blend_func_t;

typedef enum {
  DN_GPU_BLEND_MODE_ZERO,
  DN_GPU_BLEND_MODE_ONE,
  DN_GPU_BLEND_MODE_SRC_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC_COLOR,
  DN_GPU_BLEND_MODE_DST_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_DST_COLOR,
  DN_GPU_BLEND_MODE_SRC_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA,
  DN_GPU_BLEND_MODE_DST_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_DST_ALPHA,
  DN_GPU_BLEND_MODE_CONSTANT_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_CONSTANT_COLOR,
  DN_GPU_BLEND_MODE_CONSTANT_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA,
  DN_GPU_BLEND_MODE_SRC_ALPHA_SATURATE,
  DN_GPU_BLEND_MODE_SRC1_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC1_COLOR,
  DN_GPU_BLEND_MODE_SRC1_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC1_ALPHA
} dn_gpu_blend_mode_t;

typedef enum {
  DN_GPU_LOAD_OP_NONE = 0,
  DN_GPU_LOAD_OP_CLEAR = 1
} dn_gpu_load_op_t;

typedef enum {
  DN_GPU_RESOURCE_FRAMEBUFFER = 0,
  DN_GPU_RESOURCE_SHADER = 1,
  DN_GPU_RESOURCE_PROGRAM = 2,
} dn_gpu_resource_id_t;

typedef enum {
  DN_GPU_MEMORY_BARRIER_STORAGE = 0,
  DN_GPU_MEMORY_BARRIER_BUFFER_UPDATE = 1,
} dn_gpu_memory_barrier_t;

typedef enum {
  DN_GPU_SHADER_GRAPHICS = 0,
  DN_GPU_SHADER_COMPUTE = 1,
} dn_gpu_shader_kind_t;

/////////////
// SHADERS //
/////////////

typedef struct {
  const char* name;
  const char* vertex_shader;
  const char* fragment_shader;
  const char* compute_shader;

  dn_gpu_shader_kind_t kind;
} dn_gpu_shader_descriptor_t;

typedef struct {
  dn_gpu_shader_kind_t kind;
  dn_asset_name_t name;
  u32 program;
  
  union {
    struct {
      char* vertex_path;
      char* fragment_path;
      u32 vertex_shader;
      u32 fragment_shader;
    } graphics;

    struct {
      char* path;
      u32 shader;
    } compute;
  };
} dn_gpu_shader_t;

//////////////
// UNIFORMS //
//////////////
typedef union {
  dn_matrix4_t mat4;
  dn_matrix3_t mat3;
  dn_matrix2_t mat2;
  dn_vector4_t vec4;
  dn_vector3_t vec3;
  dn_vector2_t vec2;
  float f32;
  s32 texture;
  s32 i32;
} dn_gpu_uniform_data_t;

typedef struct {
  dn_asset_name_t name;
  dn_gpu_uniform_kind_t kind;
} dn_gpu_uniform_descriptor_t;

typedef struct {
  dn_asset_name_t name;
  dn_gpu_uniform_kind_t kind;
} dn_gpu_uniform_t;

/////////////////
// GPU BUFFERS //
/////////////////
typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_kind_t kind;
  dn_gpu_buffer_usage_t usage;
  u32 capacity;
  u32 element_size;
} dn_gpu_buffer_descriptor_t;

typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_kind_t kind;
  dn_gpu_buffer_usage_t usage;
  u32 size;
  u32 handle;
} dn_gpu_buffer_t;

typedef struct {
  dn_fixed_array_t buffer;
  dn_gpu_buffer_t* gpu_buffer;
} dn_gpu_backed_buffer_t;

///////////////////////
// GPU RENDER TARGET //
///////////////////////
typedef struct {
  dn_asset_name_t name;
  dn_vector2_t size;
} dn_gpu_render_target_descriptor_t;

typedef struct {
  dn_asset_name_t name;
  u32 handle;
  u32 color_buffer;
  dn_vector2_t size;
} dn_gpu_render_target_t;

/////////////////////
// GPU RENDER PASS //
/////////////////////
typedef struct {
  struct {
    dn_gpu_load_op_t load;
    dn_gpu_render_target_t* attachment;
  } color;
} dn_gpu_render_pass_t;

////////////////////////
// GPU BUFFER BINDING //
////////////////////////
typedef struct {
  dn_gpu_buffer_t* buffer;
} dn_gpu_vertex_buffer_binding_t;

typedef struct {
  dn_gpu_vertex_buffer_binding_t bindings [8];
  u32 count;
} dn_gpu_vertex_buffer_binding_array_t;

typedef struct {
  dn_gpu_buffer_t* buffer;
  u32 base;
} dn_gpu_storage_buffer_binding_t;

typedef struct {
  dn_gpu_storage_buffer_binding_t bindings [8];
  u32 count;
} dn_gpu_storage_buffer_binding_array_t;

typedef struct {
  dn_gpu_uniform_data_t data;
  dn_gpu_uniform_t* uniform;
  u32 binding_index;
} dn_gpu_uniform_binding_t;

typedef struct {
  dn_gpu_uniform_binding_t bindings [8];
  u32 count;
} dn_gpu_uniform_binding_array_t;

typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_t* buffer;
  u32 binding_index;
} dn_gpu_uniform_buffer_binding_t;

typedef struct {
  dn_gpu_uniform_buffer_binding_t bindings [8];
  u32 count;
} dn_gpu_uniform_buffer_binding_array_t;

typedef struct {
  dn_align(16) dn_gpu_vertex_buffer_binding_array_t vertex;
  dn_align(16) dn_gpu_uniform_binding_array_t uniforms;
  dn_align(16) dn_gpu_storage_buffer_binding_array_t storage;
  dn_align(16) dn_gpu_uniform_buffer_binding_array_t uniform_buffers;
} dn_gpu_buffer_binding_t;

//////////////////
// GPU PIPELINE //
//////////////////
typedef struct {
  dn_gpu_blend_func_t fn;
  dn_gpu_blend_mode_t source;
  dn_gpu_blend_mode_t destination;
} dn_gpu_blend_state_t;

typedef struct {
  dn_gpu_shader_t* shader;
  dn_gpu_draw_primitive_t primitive;
} dn_gpu_raster_state_t;

typedef struct {
  dn_vector2_t position;
  dn_vector2_t size;
  bool enabled;
} dn_gpu_scissor_state_t;

typedef struct {
  u32 layer;
  bool world_space;
  dn_vector2_t camera;
  dn_matrix4_t projection;
} dn_gpu_renderer_state_t;

typedef struct {
  dn_gpu_vertex_attribute_kind_t kind;
  u32 count;
  u32 divisor;
} dn_gpu_vertex_attribute_t;

typedef struct {
  dn_gpu_vertex_attribute_t vertex_attributes [8];
  u32 num_vertex_attributes;
} dn_gpu_buffer_layout_t;

typedef struct {
  u32 size;
  u32 value;
} dn_gpu_vertex_attr_info_t;

typedef struct {
  dn_gpu_blend_state_t blend;
  dn_gpu_raster_state_t raster;
  dn_gpu_buffer_layout_t buffer_layouts [8];
  u32 num_buffer_layouts;
} dn_gpu_pipeline_descriptor_t;

typedef struct {
  dn_gpu_blend_state_t blend;
  dn_gpu_raster_state_t raster;
  dn_gpu_buffer_layout_t buffer_layouts [8];
  u32 num_buffer_layouts;
} dn_gpu_pipeline_t;

////////////////////////
// GPU COMMAND BUFFER //
////////////////////////
typedef struct {
  dn_gpu_draw_mode_t mode;
  u32 vertex_offset;
  u32 num_vertices;
  u32 num_instances;
} dn_gpu_draw_call_t;

typedef struct {
  dn_gpu_command_op_t op;
  union {
    dn_gpu_pipeline_t*       pipeline;
    dn_gpu_buffer_binding_t  bindings;
    dn_gpu_render_pass_t     render_pass;
    dn_gpu_renderer_state_t  render;
    dn_gpu_scissor_state_t   scissor;
    dn_gpu_draw_call_t       draw;
  };
} dn_gpu_command_t;

typedef struct {
  u32 max_commands;
} dn_gpu_command_buffer_descriptor_t;

typedef struct {
  dn_gpu_pipeline_t* pipeline;
  dn_gpu_buffer_binding_t bindings;
  dn_gpu_render_pass_t render_pass;
  dn_gpu_renderer_state_t render;
  dn_gpu_scissor_state_t scissor;

  dn_fixed_array(dn_gpu_command_t, DN_FIXED_ARRAY_RUNTIME_SIZE) commands;
  u32 vao;
} dn_gpu_command_buffer_t;

////////////
// DN GPU //
////////////
typedef struct {
  dn_matrix4_t view;
  dn_matrix4_t projection;
  dn_vector2_t camera;
  dn_vector2_t native_resolution;
  dn_vector2_t output_resolution;
  f32 master_time;
} dn_gpu_uniforms_t;

typedef struct {
  dn_gpu_buffer_t* buffer;
  dn_gpu_uniform_buffer_binding_t binding;
  dn_gpu_uniforms_t data;
} dn_gpu_builtin_uniforms_t;

typedef struct {
  const char* shader_path;
  dn_gpu_shader_descriptor_t* shaders;
  u32 num_shaders;
  const char** search_paths;
  u32 num_search_paths;
  dn_gpu_render_target_descriptor_t* render_targets;
  u32 num_render_targets;
} dn_gpu_config_t;

#define DN_GPU_MAX_COMMAND_BUFFERS 32
#define DN_GPU_MAX_UNIFORMS 1024
#define DN_GPU_MAX_PIPELINES 64
#define DN_GPU_MAX_BUFFERS 128
#define DN_GPU_MAX_RENDER_TARGETS 32
#define DN_GPU_MAX_SHADERS 128
#define DN_GPU_MAX_SHADER_SEARCH_PATHS 16
typedef struct {
  dn_fixed_array(dn_gpu_command_buffer_t, DN_GPU_MAX_COMMAND_BUFFERS)     command_buffers;
  dn_fixed_array(dn_gpu_uniform_t,        DN_GPU_MAX_UNIFORMS)            uniforms;
  dn_fixed_array(dn_gpu_pipeline_t,       DN_GPU_MAX_PIPELINES)           pipelines;
  dn_fixed_array(dn_gpu_buffer_t,         DN_GPU_MAX_BUFFERS)             gpu_buffers;
  dn_fixed_array(dn_gpu_render_target_t,  DN_GPU_MAX_RENDER_TARGETS)      targets;
  dn_fixed_array(dn_gpu_shader_t,         DN_GPU_MAX_SHADERS)             shaders;
  dn_fixed_array(dn_path_t,               DN_GPU_MAX_SHADER_SEARCH_PATHS) search_paths;

  // FileMonitor* shader_monitor;
  dn_gpu_builtin_uniforms_t builtin_uniforms;
} dn_gpu_t;
dn_gpu_t dn_gpu;

DN_API void                      dn_gpu_init(dn_gpu_config_t config);
DN_API dn_gpu_command_buffer_t*  dn_gpu_command_buffer_create(dn_gpu_command_buffer_descriptor_t descriptor);
DN_API void                      dn_gpu_command_buffer_draw(dn_gpu_command_buffer_t* command_buffer, dn_gpu_draw_call_t draw_call);
DN_API void                      dn_gpu_command_buffer_submit(dn_gpu_command_buffer_t* command_buffer);
DN_API void                      dn_gpu_bind_pipeline(dn_gpu_command_buffer_t* command_buffer, dn_gpu_pipeline_t* pipeline);
DN_API void                      dn_gpu_begin_render_pass(dn_gpu_command_buffer_t* command_buffer, dn_gpu_render_pass_t render_pass);
DN_API void                      dn_gpu_end_render_pass(dn_gpu_command_buffer_t* command_buffer);
DN_API void                      dn_gpu_apply_bindings(dn_gpu_command_buffer_t* command_buffer, dn_gpu_buffer_binding_t bindings);
DN_API void                      dn_gpu_bind_render_state(dn_gpu_command_buffer_t* command_buffer, dn_gpu_renderer_state_t render);
DN_API void                      dn_gpu_set_layer(dn_gpu_command_buffer_t* command_buffer, u32 layer);
DN_API void                      dn_gpu_set_world_space(dn_gpu_command_buffer_t* command_buffer, bool world_space);
DN_API void                      dn_gpu_set_camera(dn_gpu_command_buffer_t* command_buffer, dn_vector2_t camera);
DN_API dn_gpu_pipeline_t*        dn_gpu_pipeline_create(dn_gpu_pipeline_descriptor_t descriptor);
DN_API dn_gpu_uniform_t*         dn_gpu_uniform_create(dn_gpu_uniform_descriptor_t descriptor);
DN_API dn_gpu_buffer_t*          dn_gpu_buffer_create(dn_gpu_buffer_descriptor_t descriptor);
DN_API void                      dn_gpu_buffer_bind(dn_gpu_buffer_t* buffer);
DN_API void                      dn_gpu_buffer_bind_base(dn_gpu_buffer_t* buffer, u32 base);
DN_API void                      dn_gpu_buffer_sync(dn_gpu_buffer_t* buffer, void* data, u32 size);
DN_API void                      dn_gpu_buffer_sync_subdata(dn_gpu_buffer_t* buffer, void* data, u32 byte_size, u32 byte_offset);
DN_API void                      dn_gpu_buffer_zero(dn_gpu_buffer_t* buffer, u32 size);
DN_API dn_gpu_backed_buffer_t    dn_gpu_backed_buffer_create(dn_gpu_buffer_descriptor_t descriptor);
DN_API u32                       dn_gpu_backed_buffer_size(dn_gpu_backed_buffer_t* buffer);
DN_API void                      dn_gpu_backed_buffer_clear(dn_gpu_backed_buffer_t* buffer);
DN_API u8*                       dn_gpu_backed_buffer_push(dn_gpu_backed_buffer_t* buffer, void* data, u32 num_elements);
DN_API void                      dn_gpu_backed_buffer_sync(dn_gpu_backed_buffer_t* buffer);
DN_API dn_gpu_shader_t*          dn_gpu_shader_create(dn_gpu_shader_descriptor_t descriptor);
DN_API dn_gpu_shader_t*          dn_gpu_shader_find(const char* name);
DN_API dn_gpu_render_target_t*   dn_gpu_render_target_create(dn_gpu_render_target_descriptor_t descriptor);
DN_API dn_gpu_render_target_t*   dn_gpu_acquire_swapchain();
DN_API dn_gpu_render_target_t*   dn_gpu_render_target_find(const char* name);
DN_API void                      dn_gpu_render_target_bind(dn_gpu_render_target_t* target);
DN_API void                      dn_gpu_render_target_clear(dn_gpu_render_target_t* target);
DN_API void                      dn_gpu_render_target_blit(dn_gpu_render_target_t* source, dn_gpu_render_target_t* destination);
DN_API void                      dn_gpu_memory_barrier(dn_gpu_memory_barrier_t barrier);
DN_API void                      dn_gpu_dispatch_compute(dn_gpu_buffer_t* buffer, u32 size);
DN_API void                      dn_gpu_swap_buffers();
DN_API void                      dn_gpu_error_clear();
DN_API char*                     dn_gpu_error_read();
DN_API void                      dn_gpu_error_log_one();
DN_API void                      dn_gpu_error_log_all();
DN_API void                      dn_gpu_set_resource_name(dn_gpu_resource_id_t id, u32 handle, u32 name_len, const char* name);
DN_IMP char*                     dn_gpu_shader_preprocess(const char* file_path);
DN_IMP void                      dn_gpu_shader_check_compilation(u32 shader, const char* file_path);
DN_IMP void                      dn_gpu_shader_check_linkage(u32 shader, const char* file_path);
DN_IMP void                      dn_gpu_shader_build_source(const char* file_path);
DN_IMP void                      dn_gpu_shader_init(dn_gpu_shader_t* shader, dn_gpu_shader_descriptor_t descriptor);
DN_IMP void                      dn_gpu_shader_init_graphics(dn_gpu_shader_t* shader, const char* name);
DN_IMP void                      dn_gpu_shader_init_graphics_ex(dn_gpu_shader_t* shader, const char* name, const char* vertex_path, const char* fragment_path);
DN_IMP void                      dn_gpu_shader_init_compute(dn_gpu_shader_t* shader, const char* name);
DN_IMP void                      dn_gpu_shader_init_compute_ex(dn_gpu_shader_t* shader, const char* name, const char* compute_path);
DN_IMP void                      dn_gpu_shader_reload(dn_gpu_shader_t* shader); 
DN_IMP void                      dn_gpu_command_buffer_clear_cached_state(dn_gpu_command_buffer_t* command_buffer);
DN_IMP u32                       dn_gpu_vertex_layout_calculate_stride(dn_gpu_buffer_layout_t* layout);
DN_IMP u32                       dn_gpu_draw_primitive_to_gl_draw_primitive(dn_gpu_draw_primitive_t primitive);
DN_IMP dn_gpu_vertex_attr_info_t dn_gpu_vertex_attribute_info(dn_gpu_vertex_attribute_kind_t kind);
DN_IMP void*                     dn_gpu_u32_to_gl_void_pointer(u32 value);
DN_IMP u32                       dn_gpu_buffer_kind_to_gl_buffer_kind(dn_gpu_buffer_kind_t kind);
DN_IMP u32                       dn_gpu_buffer_usage_to_gl_buffer_usage(dn_gpu_buffer_usage_t usage);
DN_IMP u32                       dn_gpu_buffer_kind_to_gl_barrier(dn_gpu_buffer_kind_t kind);
DN_IMP u32                       dn_gpu_blend_func_to_gl_blend_func(dn_gpu_blend_func_t func);
DN_IMP u32                       dn_gpu_blend_mode_to_gl_blend_mode(dn_gpu_blend_mode_t mode);
DN_IMP u32                       dn_gpu_resource_id_to_gl_id(dn_gpu_resource_id_t id);
DN_IMP u32                       dn_gpu_memory_barrier_to_gl_barrier(dn_gpu_memory_barrier_t barrier);
DN_IMP void                      dn_gpu_apply_uniform_buffer_binding(dn_gpu_command_buffer_t* command_buffer, dn_gpu_uniform_buffer_binding_t* binding);


// ███████╗██████╗ ███████╗
// ██╔════╝██╔══██╗██╔════╝
// ███████╗██║  ██║█████╗  
// ╚════██║██║  ██║██╔══╝  
// ███████║██████╔╝██║     
// ╚══════╝╚═════╝ ╚═╝     
///////////
// ENUMS //
///////////
typedef enum {
  DN_SDF_SHAPE_CIRCLE = 0,
  DN_SDF_SHAPE_RING = 1,
  DN_SDF_SHAPE_BOX = 2,
  DN_SDF_SHAPE_ORIENTED_BOX = 3,
  DN_SDF_SHAPE_COMBINE = 100,
} dn_sdf_shape_t;

typedef enum {
  DN_SDF_COMBINE_OP_UNION = 0,
  DN_SDF_COMBINE_OP_INTERSECTION = 1,
  DN_SDF_COMBINE_OP_SUBTRACTION = 2,
} dn_sdf_combine_op_t;

typedef enum {
  DN_SDF_SMOOTH_KERNEL_NONE = 0,
  DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC = 1,
} dn_sdf_smoothing_kernel_t;

typedef enum {
  DN_SDF_RENDERER_STATE_NONE,
  DN_SDF_RENDERER_STATE_COMBINATION,
} dn_sdf_renderer_state_t;


/////////////////////
// SDF BUFFER DATA //
/////////////////////
typedef struct {
  dn_vector2_t position;
  dn_vector2_t uv;
} dn_sdf_vertex_t;

typedef struct {
  dn_sdf_shape_t shape;
  u32 buffer_index;
} dn_sdf_instance_t;

typedef struct {
  u32 num_sdfs;
} dn_sdf_combine_header_t;

typedef struct {
  u32 buffer_index;
  dn_sdf_shape_t shape;
  dn_sdf_combine_op_t op;
  dn_sdf_smoothing_kernel_t kernel;
} dn_sdf_combine_entry_t;


////////////////////
// SDF SHAPE DATA //
////////////////////
typedef struct {
  dn_vector3_t color;
  dn_vector2_t position;
  float rotation;
  float edge_thickness;
  dn_sdf_shape_t shape;
} dn_sdf_header_t;

typedef struct {
  dn_sdf_header_t header;
  float radius;
} dn_sdf_circle_t;

typedef struct {
  dn_sdf_header_t header;
  float inner_radius;
  float outer_radius;
} dn_sdf_ring_t;

typedef struct {
  dn_sdf_header_t header;
  dn_vector2_t size;
} dn_sdf_oriented_box_t;


typedef struct {
  dn_sdf_renderer_state_t state;
  dn_gpu_backed_buffer_t vertices;
  dn_gpu_backed_buffer_t instances;
  dn_gpu_backed_buffer_t combinations;
  dn_gpu_backed_buffer_t shape_data;
  dn_gpu_buffer_binding_t bindings;
  dn_gpu_pipeline_t* pipeline;
} dn_sdf_renderer_t;

DN_API dn_sdf_renderer_t        dn_sdf_renderer_create(u32 buffer_size);
DN_API void                     dn_sdf_renderer_draw(dn_sdf_renderer_t* renderer, dn_gpu_command_buffer_t* command_buffer);
DN_API void                     dn_sdf_renderer_push_instance(dn_sdf_renderer_t* renderer, dn_sdf_shape_t shape);
DN_API void                     dn_sdf_renderer_push_header(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness);
DN_API dn_sdf_combine_header_t* dn_sdf_combination_begin(dn_sdf_renderer_t* renderer);
DN_API void                     dn_sdf_combination_append(dn_sdf_renderer_t* renderer, dn_sdf_combine_header_t* header, dn_sdf_shape_t shape, dn_sdf_combine_op_t op, dn_sdf_smoothing_kernel_t kernel);
DN_API void                     dn_sdf_combination_commit(dn_sdf_renderer_t* renderer);
DN_API void                     dn_sdf_circle_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius);
DN_API void                     dn_sdf_ring_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius);
DN_API void                     dn_sdf_oriented_box_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float length, float thickness);
DN_API void                     dn_sdf_grid(dn_sdf_renderer_t* renderer, u32 grid_width, u32 grid_size);


// ███████╗████████╗███████╗ █████╗ ███╗   ███╗
// ██╔════╝╚══██╔══╝██╔════╝██╔══██╗████╗ ████║
// ███████╗   ██║   █████╗  ███████║██╔████╔██║
// ╚════██║   ██║   ██╔══╝  ██╔══██║██║╚██╔╝██║
// ███████║   ██║   ███████╗██║  ██║██║ ╚═╝ ██║
// ╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝
typedef enum {
  DN_STEAM_INIT_NONE,
  DN_STEAM_INIT_FAILED,
  DN_STEAM_INIT_OK,
} dn_steam_init_state_t;

typedef enum {
  DN_STEAM_INPUT_TEXT_UNINITIALIZED,
  DN_STEAM_INPUT_TEXT_OPEN,
  DN_STEAM_INPUT_TEXT_UNREAD,
  DN_STEAM_INPUT_TEXT_CLOSED,
} dn_steam_text_input_state_t;

typedef struct {
  // STEAM_CALLBACK(dn_steam_callbacks_t, on_dismiss_text_input, GamepadTextInputDismissed_t);
  u32 dummy;
} dn_steam_callbacks_t ;
dn_steam_callbacks_t dn_steam_callbacks;

typedef struct {
  u32 app_id;
} dn_steam_config_t;

typedef struct {
  u32 app_id;
  dn_steam_init_state_t state;

  struct {
    dn_steam_text_input_state_t state;
    dn_string_t contents;
  } text_input;
} dn_steam_t;
dn_steam_t dn_steam;

DN_API void                        dn_steam_init(dn_steam_config_t config);
DN_API bool                        dn_steam_initialized();
DN_API void                        dn_steam_open_store_page();
DN_API void                        dn_steam_open_store_page_utm(dn_string_t utm);
DN_API void                        dn_steam_open_store_page_ex(dn_string_t url);
DN_API void                        dn_steam_open_text_input();
DN_API dn_steam_text_input_state_t dn_steam_text_input_state();
DN_API dn_string_t                 dn_string_read_text_input();
DN_API bool                        dn_steam_is_deck();
DN_IMP void                        dn_steam_shutdown();

//   █████╗ ██████╗ ██████╗
//  ██╔══██╗██╔══██╗██╔══██╗
//  ███████║██████╔╝██████╔╝
//  ██╔══██║██╔═══╝ ██╔═══╝
//  ██║  ██║██║     ██║
//  ╚═╝  ╚═╝╚═╝     ╚═╝
// DN can either be used as a standalone standard library (i.e. no Lua, no windowing, no input, no app stuff at all), 
// a standalone library with a Lua environment (but still no application stuff), or an application framework.
typedef enum {
  DN_MODE_CORE,
  DN_MODE_CORE_LUA,
  DN_MODE_APP,
} dn_mode_t;


// Since Doublenickel is intended to be consumed via source, and since it contains iterpreted code which needs
// to be located at runtime (as opposed to just C++ source), we need a way to locate those sources. Also, a core
// idea in my programs is to never use relative paths. They're too error prone. Instead, we build absolute paths
// at initialization time.
// 
// The paths which we need are:
// 1. install_path: Where is the game installed, relative to the executable? In a packaged build, this will probably
//    be the same directory, but in debug builds this isn't the case. This is needed to build any path relative to
//    the top-level directory where the game is installed
// 
// 2. engine_path: Where are the Doublenickel Lua sources? Specify this from the install directory.
// 
// 3. write_path: Where should runtime data be written to? In packaged builds, this is relative to the OS-approved
//    write directory (i.e. AppData). In editor builds, this is relative to the install directory.
typedef struct {
  dn_mode_t mode;
  dn_path_config_t path;
  dn_lua_config_t lua;
} dn_config_t;

typedef struct {
  dn_window_config_t window;
  dn_audio_config_t audio;
  dn_font_config_t font;
  dn_gpu_config_t gpu;
  dn_asset_config_t asset;
  dn_steam_config_t steam;
  dn_image_config_t image;
} dn_app_config_t;


typedef struct {
  const char* name;
  u32 size;
} dn_type_info_t;
#define dn_type_info(t) { .name = dn_macro_str(t), .size = sizeof(t) }

typedef struct {
  dn_type_info_t* data;
  u32 count;
} dn_type_info_list_t;

typedef struct {
  dn_window_t window;
  dn_asset_loader_t loader;
  dn_asset_registry_t asset_registry;
  dn_input_t input;
  dn_coord_data_t coords;

  f32 target_fps;
  f32 dt;
  f32 elapsed_time;
  s32 frame;
  DN_ATOMIC u32 monotonic_id;
  bool exit_game;
  bool steam;
} dn_app_t;
dn_app_t dn_app;

DN_API void dn_app_configure(dn_app_config_t config);
DN_IMP void dn_app_default();
DN_IMP void dn_init(dn_config_t config);
DN_IMP int  dn_main();

DN_API void                dn_app_set_exit_game();
DN_API const char*         dn_app_get_game_hash();
DN_API void                dn_app_set_target_fps(f32 fps);
DN_API f32                 dn_app_get_target_fps();
DN_API bool                dn_app_exceeded_frame_time();
DN_API bool                dn_app_should_exit();
DN_API dn_type_info_list_t dn_app_query_types();
DN_API u32                 dn_app_monotonic_id();
DN_IMP void                dn_app_update();


//  ██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
//  ██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
//  ██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
//  ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
//  ██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
//  ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
#ifdef DN_IMPLEMENTATION
///////////
// STEAM //
///////////
void dn_steam_init(dn_steam_config_t config) {
  DN_BROKEN();
}

bool dn_steam_initialized() {
  DN_BROKEN();
  return false;
}

void dn_steam_open_store_page() {
  DN_BROKEN();
}

void dn_steam_open_store_page_utm(dn_string_t utm) {
  DN_BROKEN();
}

void dn_steam_open_store_page_ex(dn_string_t url) {
  DN_BROKEN();
}

void dn_steam_open_text_input() {
  DN_BROKEN();
}

dn_steam_text_input_state_t dn_steam_text_input_state() {
  DN_BROKEN();
  return DN_STEAM_INPUT_TEXT_UNINITIALIZED;
}

dn_string_t dn_string_read_text_input() {
  DN_BROKEN();
  return dn_zero_struct(dn_string_t);
}

bool dn_steam_is_deck() {
  DN_BROKEN();
  return false;
}

void dn_steam_shutdown() {
  DN_BROKEN();
}


///////////
// NOISE //
///////////
void dn_noise_init() {
  DN_BROKEN();
}

f64 dn_noise_perlin(f64 x, f64 y) {
  DN_BROKEN();
  return 0;
}

f64 dn_noise_perlin_scaled(f64 x, f64 y, f64 vmin, f64 vmax) {
  DN_BROKEN();
  return 0;
}

f64 dn_noise_chaotic(f64 x, f64 y) {
  DN_BROKEN();
  return 0;
}

f64 dn_noise_chaotic_scaled(f64 x, f64 y, f64 vmin, f64 vmax) {
  DN_BROKEN();
  return 0;
}


///////////
// FONTS //
///////////
void dn_font_init(dn_font_config_t config) {
  DN_BROKEN();
}

void dn_font_bake_n(dn_font_descriptor_t* descriptors, u32 num_descriptors) {
  DN_BROKEN();
}

void dn_font_bake(dn_font_descriptor_t descriptor) {
  DN_BROKEN();
}

dn_font_config_t dn_font_config_default() {
  DN_BROKEN();
  return dn_zero_struct(dn_font_config_t);
}

dn_baked_font_t* dn_font_default() {
  DN_BROKEN();
  return NULL;
}

dn_baked_font_t* dn_font_find(const char* id, u32 size) {
  DN_BROKEN();
  return NULL;
}

dn_hash_t dn_font_hash(const char* id, u32 size) {
  DN_BROKEN();
  return 0;
}


/////////
// GPU //
/////////
void dn_gpu_init(dn_gpu_config_t config) {
  DN_LOG_FN();
  dn_fixed_array_init(&dn_gpu.command_buffers, DN_GPU_MAX_COMMAND_BUFFERS,     sizeof(dn_gpu_command_buffer_t), &dn_allocators.standard.allocator);
  dn_fixed_array_init(&dn_gpu.uniforms,        DN_GPU_MAX_UNIFORMS,            sizeof(dn_gpu_uniform_t),        &dn_allocators.standard.allocator);
  dn_fixed_array_init(&dn_gpu.pipelines,       DN_GPU_MAX_PIPELINES,           sizeof(dn_gpu_pipeline_t),       &dn_allocators.standard.allocator);
  dn_fixed_array_init(&dn_gpu.gpu_buffers,     DN_GPU_MAX_BUFFERS,             sizeof(dn_gpu_buffer_t),         &dn_allocators.standard.allocator);
  dn_fixed_array_init(&dn_gpu.targets,         DN_GPU_MAX_RENDER_TARGETS,      sizeof(dn_gpu_render_target_t),  &dn_allocators.standard.allocator);
  dn_fixed_array_init(&dn_gpu.shaders,         DN_GPU_MAX_SHADERS,             sizeof(dn_gpu_shader_t),         &dn_allocators.standard.allocator);
  dn_fixed_array_init(&dn_gpu.search_paths,    DN_GPU_MAX_SHADER_SEARCH_PATHS, sizeof(dn_path_t),               &dn_allocators.standard.allocator);

  dn_gpu_render_target_t* swapchain = (dn_gpu_render_target_t*)dn_fixed_array_reserve(&dn_gpu.targets, 1);
  swapchain->handle = 0;
  swapchain->color_buffer = 0;
  swapchain->size = dn_app.window.content_area;

  // DN_LOG("Initializing shader file monitor");
  // auto reload_all_shaders = [](FileMonitor* file_monitor, FileChange* event, void* userdata) {
  //   dn_log("SHADER_RELOAD");
  //   dn_array_for(dn_gpu.shaders, shader) {
  //     dn_gpu_shader_reload(shader);
  //   }
  // };
  // dn_gpu.shader_monitor = dn_file_monitors_add();
  // dn_gpu.shader_monitor->init(reload_all_shaders, DN_FILE_CHANGE_EVENT_MODIFIED, nullptr);
  // dn_gpu.shader_monitor->add_directory(config.shader_path);
  // dn_gpu.shader_monitor->add_directory(dn_paths_resolve("dn_shaders"));


  DN_LOG_2("Initializing default uniform block");
  dn_gpu.builtin_uniforms.buffer = dn_gpu_buffer_create((dn_gpu_buffer_descriptor_t) {
    .name = "dn_uniform_buffer_default",
    .kind = DN_GPU_BUFFER_KIND_UNIFORM,
    .usage = DN_GPU_BUFFER_USAGE_DYNAMIC,
    .capacity = 1,
    .element_size = sizeof(dn_gpu_uniforms_t)
  });

  dn_gpu.builtin_uniforms.binding = (dn_gpu_uniform_buffer_binding_t) {
    .name = "DnUniformBlock",
    .buffer = dn_gpu.builtin_uniforms.buffer,
    .binding_index = 0
  };

  DN_LOG_2("Initializing default shader include directories");
  dn_string_t default_include_paths [] = {
    dn_paths_resolve(dn_string_literal("dn_shaders")),
    dn_paths_resolve(dn_string_literal("dn_shader_includes")),
  };
  dn_for_arr(default_include_paths, i) {
    dn_path_t* path = (dn_path_t*)dn_fixed_array_reserve(&dn_gpu.search_paths, 1);
    dn_string_copy_to(default_include_paths[i], *path, DN_MAX_PATH_LEN);
  }

  DN_LOG_2("Initializing default shaders");
  dn_gpu_shader_descriptor_t default_shaders [] = {
    {
      .name = "shape",
      .vertex_shader   = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("sdf/shape.vertex"))),
      .fragment_shader = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("sdf/shape.fragment"))),
      .kind = DN_GPU_SHADER_GRAPHICS
    },
    {
        .name = "solid",
        .vertex_shader   = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("solid/solid.vertex"))),
        .fragment_shader = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("solid/solid.fragment"))),
        .kind = DN_GPU_SHADER_GRAPHICS,
    },
    {
        .name = "sprite",
        .vertex_shader   = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("sprite/sprite.vertex"))),
        .fragment_shader = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("sprite/sprite.fragment"))),
        .kind = DN_GPU_SHADER_GRAPHICS,
    },
    {
        .name = "text",
        .vertex_shader   = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("text/text.vertex"))),
        .fragment_shader = dn_string_to_cstr(dn_paths_resolve_format(dn_string_literal("dn_shader"), dn_string_literal("text/text.fragment"))),
        .kind = DN_GPU_SHADER_GRAPHICS,
    },
  };
  dn_for_arr(default_shaders, i) {
    dn_gpu_shader_descriptor_t descriptor = default_shaders[i];
    dn_gpu_shader_create(descriptor);
  }

  // Add user search paths and shaders
  DN_LOG_2("Initializing default shader search paths");
  dn_for(i, config.num_search_paths) {
    dn_path_t* path = (dn_path_t*)dn_fixed_array_reserve(&dn_gpu.search_paths, 1);
    dn_cstr_copy_to(config.search_paths[i], *path, DN_MAX_PATH_LEN);
    DN_LOG("Added %s", *path);
  }

  DN_LOG_2("Initializing user shaders");
  dn_for(i, config.num_shaders) {
    DN_LOG("Added %s", config.shaders[i].name);
    dn_gpu_shader_create(config.shaders[i]);
  }

  dn_for(i, config.num_render_targets) {
    dn_gpu_render_target_create(config.render_targets[i]);
  }
}

dn_gpu_command_buffer_t* dn_gpu_command_buffer_create(dn_gpu_command_buffer_descriptor_t descriptor) {
  dn_gpu_command_buffer_t* command_buffer = (dn_gpu_command_buffer_t*)dn_fixed_array_reserve(&dn_gpu.command_buffers, 1);
  dn_fixed_array_init(&command_buffer->commands, descriptor.max_commands, sizeof(dn_gpu_command_t), &dn_allocators.standard.allocator);
  glGenVertexArrays(1, &command_buffer->vao);
  glBindVertexArray(command_buffer->vao);

  return command_buffer;
}

void dn_gpu_command_buffer_draw(dn_gpu_command_buffer_t* command_buffer, dn_gpu_draw_call_t draw_call) {
  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_DRAW,
    .draw = draw_call
  }, 1);
}


void dn_gpu_command_buffer_submit(dn_gpu_command_buffer_t* command_buffer) {
  dn_gpu_command_buffer_clear_cached_state(command_buffer);
  glBindVertexArray(command_buffer->vao);

  dn_fixed_array_for(command_buffer->commands, command, dn_gpu_command_t) {
    dn_gpu_pipeline_t* pipeline = command_buffer->pipeline;

    switch (command->op) {
      case DN_GPU_COMMAND_OP_BEGIN_RENDER_PASS: {
        switch (command->render_pass.color.load) {
          case DN_GPU_LOAD_OP_CLEAR: {
            dn_gpu_render_target_clear(command->render_pass.color.attachment);
          }
        }

        dn_gpu_render_target_bind(command->render_pass.color.attachment);
        command_buffer->render_pass = command->render_pass;
      } break;

      case DN_GPU_COMMAND_OP_END_RENDER_PASS: {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_SCISSOR_TEST);
      } break;

      case DN_GPU_COMMAND_OP_BIND_PIPELINE: {
        glUseProgram(command->pipeline->raster.shader->program);

        if (command->pipeline->blend.fn == DN_GPU_BLEND_FUNC_NONE) {
          glDisable(GL_BLEND);
        }
        else {
          glEnable(GL_BLEND);
          glBlendEquation(dn_gpu_blend_func_to_gl_blend_func(command->pipeline->blend.fn));
          glBlendFunc(
            dn_gpu_blend_mode_to_gl_blend_mode(command->pipeline->blend.source), 
            dn_gpu_blend_mode_to_gl_blend_mode(command->pipeline->blend.destination)
          );

        }

        command_buffer->pipeline = command->pipeline;
      } break;

      case DN_GPU_COMMAND_OP_BIND_BUFFERS: {
        // VERTEX BUFFERS
        dn_gpu_vertex_buffer_binding_array_t* vertex_buffers = &command->bindings.vertex;

        DN_ASSERT(vertex_buffers->count <= command_buffer->pipeline->num_buffer_layouts);

        u32 attribute_index = 0;
        for (u32 buffer_index = 0; buffer_index < vertex_buffers->count; buffer_index++) {
          dn_gpu_buffer_layout_t* buffer_layout = &command_buffer->pipeline->buffer_layouts[buffer_index];
          dn_gpu_buffer_t* buffer = vertex_buffers->bindings[buffer_index].buffer;

          dn_gpu_buffer_bind(buffer);

          u32 stride = dn_gpu_vertex_layout_calculate_stride(buffer_layout);

          u64 offset = 0;
          for (u32 i = 0; i < buffer_layout->num_vertex_attributes; i++) {
            glEnableVertexAttribArray(attribute_index);

            dn_gpu_vertex_attribute_t attribute = buffer_layout->vertex_attributes[i];
            
            switch(attribute.kind) {
              case DN_GPU_VERTEX_ATTRIBUTE_FLOAT: glVertexAttribPointer(attribute_index, attribute.count, GL_FLOAT,        GL_FALSE, stride, dn_gpu_u32_to_gl_void_pointer(offset)); break;
              case DN_GPU_VERTEX_ATTRIBUTE_U32:   glVertexAttribIPointer(attribute_index, attribute.count, GL_UNSIGNED_INT,           stride, dn_gpu_u32_to_gl_void_pointer(offset)); break;
              default: {
                assert(false);
              } break;
            }

            glVertexAttribDivisor(attribute_index, attribute.divisor);

            dn_gpu_vertex_attr_info_t type_info = dn_gpu_vertex_attribute_info(attribute.kind);
            offset += attribute.count * type_info.size;
            attribute_index++;
          }
        }

        // UNIFORMS
        dn_gpu_uniform_binding_array_t* uniforms = &command->bindings.uniforms;
        for (u32 i = 0; i < uniforms->count; i++) {
          dn_gpu_uniform_binding_t* binding = &uniforms->bindings[i];
          dn_gpu_uniform_t* uniform = binding->uniform;

          s32 index = glGetUniformLocation(command_buffer->pipeline->raster.shader->program, uniform->name);

          switch(uniform->kind) {
            case DN_GPU_UNIFORM_MATRIX4: glUniformMatrix4fv(index, 1, GL_FALSE, (const float*)&binding->data.mat4); break;
            case DN_GPU_UNIFORM_MATRIX3: glUniformMatrix3fv(index, 1, GL_FALSE, (const float*)&binding->data.mat3); break;
            case DN_GPU_UNIFORM_MATRIX2: glUniformMatrix2fv(index, 1, GL_FALSE, (const float*)&binding->data.mat2); break;
            case DN_GPU_UNIFORM_VECTOR4: glUniform4fv(index, 1, (const float*)&binding->data.vec4); break;
            case DN_GPU_UNIFORM_VECTOR3: glUniform3fv(index, 1, (const float*)&binding->data.vec3); break;
            case DN_GPU_UNIFORM_VECTOR2: glUniform2fv(index, 1, (const float*)&binding->data.vec2); break;
            case DN_GPU_UNIFORM_F32:     glUniform1fv(index, 1, (const float*)&binding->data.f32); break;
            case DN_GPU_UNIFORM_ENUM:    glUniform1iv(index, 1, (const s32*)&binding->data.i32); break;
            case DN_GPU_UNIFORM_TEXTURE: glActiveTexture(GL_TEXTURE0 + binding->binding_index); glBindTexture(GL_TEXTURE_2D, binding->data.texture); break;
          }
        }

        // UNIFORM BUFFERS
        dn_gpu_uniform_buffer_binding_array_t* uniform_buffers = &command->bindings.uniform_buffers;
        dn_for(i, uniform_buffers->count) {
          dn_gpu_uniform_buffer_binding_t* binding = &uniform_buffers->bindings[i];
          dn_gpu_apply_uniform_buffer_binding(command_buffer, binding);
        }

        // STORAGE BUFFERS
        dn_gpu_storage_buffer_binding_array_t* storage = &command->bindings.storage;
        for (u32 i = 0; i < storage->count; i++) {
          dn_gpu_storage_buffer_binding_t* binding = &storage->bindings[i];

          DN_ASSERT(binding->buffer->kind == DN_GPU_BUFFER_KIND_STORAGE);
          glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding->base, binding->buffer->handle);
        }

        command_buffer->bindings = command->bindings;
      } break;

      case DN_GPU_COMMAND_OP_SET_SCISSOR: {
        if (command->scissor.enabled != command_buffer->scissor.enabled) {
          if (command->scissor.enabled) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(
              command->scissor.position.x, command->scissor.position.y, 
              command->scissor.size.x, command->scissor.size.y);
          }
          else {
            glDisable(GL_SCISSOR_TEST);
          }
        }

        command_buffer->scissor = command->scissor;
      } break;

      case DN_GPU_COMMAND_OP_SET_WORLD_SPACE: {
        command_buffer->render.world_space = command->render.world_space;
      } break;
      case DN_GPU_COMMAND_OP_SET_CAMERA: {
        command_buffer->render.camera = command->render.camera;
      } break;
      case DN_GPU_COMMAND_OP_SET_LAYER: {
        command_buffer->render.layer = command->render.layer;
      } break;

      case DN_GPU_COMMAND_OP_DRAW: {      
        dn_gpu_render_target_t* target = command_buffer->render_pass.color.attachment;
        dn_gpu.builtin_uniforms.data = (dn_gpu_uniforms_t) {
          .view = command_buffer->render.world_space ? 
            HMM_Translate(HMM_V3(-command_buffer->render.camera.x, -command_buffer->render.camera.y, 0.f)) :
            HMM_M4D(1.0),
          .projection = HMM_Orthographic_RH_NO(0, target->size.x, 0, target->size.y, DN_GPU_NEAR_PLANE, DN_GPU_FAR_PLANE),
          //.projection = HMM_Orthographic_RH_NO(0, window.native_resolution.x, 0, window.native_resolution.y, DN_GPU_NEAR_PLANE, DN_GPU_FAR_PLANE),
          .camera = command_buffer->render.camera,
          .native_resolution = dn_app.window.native_resolution,
          .output_resolution = target->size,
          .master_time = dn_app.elapsed_time
        };
        dn_gpu_buffer_sync(dn_gpu.builtin_uniforms.buffer, &dn_gpu.builtin_uniforms.data, sizeof(dn_gpu_uniforms_t));
        dn_gpu_apply_uniform_buffer_binding(command_buffer, &dn_gpu.builtin_uniforms.binding);
        
        u32 primitive = dn_gpu_draw_primitive_to_gl_draw_primitive(command_buffer->pipeline->raster.primitive);
        switch (command->draw.mode) {
          case DN_GPU_DRAW_MODE_ARRAYS: glDrawArrays(primitive, command->draw.vertex_offset, command->draw.num_vertices); break;
          case DN_GPU_DRAW_MODE_INSTANCE: glDrawArraysInstanced(primitive, command->draw.vertex_offset, command->draw.num_vertices, command->draw.num_instances); break;
        }
      } break;
    }
  }

  glBindVertexArray(0);
  dn_gpu_command_buffer_clear_cached_state(command_buffer);
  dn_fixed_array_clear(&command_buffer->commands);
}

void dn_gpu_command_buffer_clear_cached_state(dn_gpu_command_buffer_t* command_buffer) {
  command_buffer->pipeline = NULL;
  dn_os_zero_memory(&command_buffer->bindings, sizeof(dn_gpu_buffer_binding_t));
  dn_os_zero_memory(&command_buffer->render_pass, sizeof(dn_gpu_render_pass_t));
  dn_os_zero_memory(&command_buffer->render, sizeof(dn_gpu_renderer_state_t));
  dn_os_zero_memory(&command_buffer->scissor, sizeof(dn_gpu_scissor_state_t));
}

//////////////
// PIPELINE //
//////////////
dn_gpu_pipeline_t* dn_gpu_pipeline_create(dn_gpu_pipeline_descriptor_t descriptor) {
  dn_gpu_pipeline_t* pipeline = (dn_gpu_pipeline_t*)dn_fixed_array_reserve(&dn_gpu.pipelines, 1);
  pipeline->raster = descriptor.raster;
  pipeline->blend = descriptor.blend;
  dn_os_memory_copy(descriptor.buffer_layouts, pipeline->buffer_layouts, descriptor.num_buffer_layouts * sizeof(dn_gpu_buffer_layout_t));
  pipeline->num_buffer_layouts = descriptor.num_buffer_layouts;

  return pipeline;
}

void dn_gpu_bind_pipeline(dn_gpu_command_buffer_t* command_buffer, dn_gpu_pipeline_t* pipeline) {
  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_BIND_PIPELINE,
    .pipeline = pipeline
  }, 1);
}


/////////////
// BINDING //
/////////////
void dn_gpu_begin_render_pass(dn_gpu_command_buffer_t* command_buffer, dn_gpu_render_pass_t render_pass) {
  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_BEGIN_RENDER_PASS,
    .render_pass = render_pass
  }, 1);
}

void dn_gpu_end_render_pass(dn_gpu_command_buffer_t* command_buffer) {
  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_END_RENDER_PASS,
  }, 1);
}

void dn_gpu_apply_bindings(dn_gpu_command_buffer_t* command_buffer, dn_gpu_buffer_binding_t bindings) {
  if (dn_os_is_memory_equal(&command_buffer->bindings, &bindings, sizeof(dn_gpu_buffer_binding_t))) return;

  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_BIND_BUFFERS,
    .bindings = bindings
  }, 1);
}

//////////////////
// RENDER STATE //
//////////////////
void dn_gpu_set_layer(dn_gpu_command_buffer_t* command_buffer, u32 layer) {
  if (command_buffer->render.layer == layer) return;

  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_SET_LAYER,
    .render = {
      .layer = layer
    }
  }, 1);
}

void dn_gpu_set_world_space(dn_gpu_command_buffer_t* command_buffer, bool world_space) {
  if (command_buffer->render.world_space == world_space) return;

  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_SET_WORLD_SPACE,
    .render = {
      .world_space = world_space
    }
  }, 1);
}

void dn_gpu_set_camera(dn_gpu_command_buffer_t* command_buffer, dn_vector2_t camera) {
  if (dn_os_is_memory_equal(&command_buffer->render.camera,  &camera, sizeof(dn_vector2_t))) return;

  dn_fixed_array_push(&command_buffer->commands, &(dn_gpu_command_t) {
    .op = DN_GPU_COMMAND_OP_SET_CAMERA,
    .render = {
      .camera = camera
    }
  }, 1);
}

void dn_gpu_bind_render_state(dn_gpu_command_buffer_t* command_buffer, dn_gpu_renderer_state_t render) {
  dn_gpu_set_layer(command_buffer, render.layer);
  dn_gpu_set_camera(command_buffer, render.camera);
  dn_gpu_set_world_space(command_buffer, render.world_space);
}

//////////////
// UNIFORMS //
//////////////
dn_gpu_uniform_t* dn_gpu_uniform_create(dn_gpu_uniform_descriptor_t descriptor) {
  dn_gpu_uniform_t* uniform = (dn_gpu_uniform_t*)dn_fixed_array_reserve(&dn_gpu.uniforms, 1);
  dn_cstr_copy_to(descriptor.name, uniform->name, DN_ASSET_NAME_LEN);
  uniform->kind = descriptor.kind;
  
  return uniform;
}

/////////////////
// GPU BUFFERS //
/////////////////
dn_gpu_buffer_t* dn_gpu_buffer_create(dn_gpu_buffer_descriptor_t descriptor) {
  dn_gpu_buffer_t* buffer = (dn_gpu_buffer_t*)dn_fixed_array_reserve(&dn_gpu.gpu_buffers, 1);
  dn_cstr_copy_to(descriptor.name, buffer->name, DN_ASSET_NAME_LEN);
  buffer->kind = descriptor.kind;
  buffer->usage = descriptor.usage;
  buffer->size = descriptor.capacity * descriptor.element_size;
  glGenBuffers(1, &buffer->handle);
  
  dn_gpu_buffer_sync(buffer, NULL, buffer->size);

  return buffer;
}

void dn_gpu_memory_barrier(dn_gpu_memory_barrier_t barrier) {
  glMemoryBarrier(dn_gpu_memory_barrier_to_gl_barrier(barrier));
}

void dn_gpu_buffer_bind(dn_gpu_buffer_t* buffer) {
  glBindBuffer(dn_gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), buffer->handle);
}

void dn_gpu_buffer_bind_base(dn_gpu_buffer_t* buffer, u32 base) {
  glBindBufferBase(dn_gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), base, buffer->handle);
}

void dn_gpu_buffer_sync(dn_gpu_buffer_t* buffer, void* data, u32 size) {
  dn_gpu_buffer_bind(buffer);
  glBufferData(dn_gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), size, data, dn_gpu_buffer_usage_to_gl_buffer_usage(buffer->usage));
  glMemoryBarrier(dn_gpu_buffer_kind_to_gl_barrier(buffer->kind));
}

void dn_gpu_buffer_sync_subdata(dn_gpu_buffer_t* buffer, void* data, u32 byte_size, u32 byte_offset) {
  dn_gpu_buffer_bind(buffer);
  glBufferSubData(dn_gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), byte_offset, byte_size, data);
  glMemoryBarrier(dn_gpu_buffer_kind_to_gl_barrier(buffer->kind));
}

void dn_gpu_buffer_zero(dn_gpu_buffer_t* buffer, u32 size) {
  u8* data = (u8*)dn_allocator_alloc(&dn_allocators.bump.allocator, size);
  dn_gpu_buffer_sync(buffer, data, size);
}

dn_gpu_backed_buffer_t dn_gpu_backed_buffer_create(dn_gpu_buffer_descriptor_t descriptor) {
  dn_gpu_backed_buffer_t backed_buffer;
  backed_buffer.gpu_buffer = dn_gpu_buffer_create(descriptor);
  dn_fixed_array_init(&backed_buffer.buffer, descriptor.capacity, descriptor.element_size, &dn_allocators.standard.allocator);
  return backed_buffer;
}

void dn_gpu_backed_buffer_clear(dn_gpu_backed_buffer_t* buffer) {
  buffer->buffer.size = 0;
}

u32 dn_gpu_backed_buffer_size(dn_gpu_backed_buffer_t* buffer) {
  return buffer->buffer.size;
}

u8* dn_gpu_backed_buffer_push(dn_gpu_backed_buffer_t* buffer, void* data, u32 num_elements) {
  return dn_fixed_array_push(&buffer->buffer, data, num_elements);
}

void dn_gpu_backed_buffer_sync(dn_gpu_backed_buffer_t* buffer) {
  dn_gpu_buffer_sync(buffer->gpu_buffer, buffer->buffer.data, buffer->buffer.size * buffer->buffer.element_size);
}

////////////////
// GPU SHADER //
////////////////
dn_gpu_shader_t* dn_gpu_shader_create(dn_gpu_shader_descriptor_t descriptor) {
  switch (descriptor.kind) {
    case DN_GPU_SHADER_GRAPHICS: dn_log("%s: DN_GPU_SHADER_GRAPHICS (%s) (%s)", __func__, descriptor.vertex_shader, descriptor.fragment_shader); break;
    case DN_GPU_SHADER_COMPUTE: dn_log("%s: DN_GPU_SHADER_COMPUTE (%s)", __func__, descriptor.compute_shader); break;
  }
  
  dn_gpu_shader_t* shader = (dn_gpu_shader_t*)dn_fixed_array_reserve(&dn_gpu.shaders, 1);
  dn_gpu_shader_init(shader, descriptor);
  dn_asset_registry_add(shader->name, shader);
  
  return shader;
}

dn_gpu_shader_t* dn_gpu_shader_find(const char* name) {
  dn_fixed_array_for(dn_gpu.shaders, shader, dn_gpu_shader_t) {
    if (dn_cstr_equal(shader->name, name)) return shader;
  }

  return NULL;
}


void dn_gpu_shader_init(dn_gpu_shader_t* shader, dn_gpu_shader_descriptor_t descriptor) {
  switch (descriptor.kind) {
    case DN_GPU_SHADER_COMPUTE: {
      shader->kind = DN_GPU_SHADER_COMPUTE;
      dn_cstr_copy_to(descriptor.name, shader->name, DN_ASSET_NAME_LEN);
      shader->compute.path = dn_cstr_copy(descriptor.compute_shader, &dn_allocators.standard.allocator);
      char* source = dn_gpu_shader_preprocess(shader->compute.path);

      u32 num_shaders = 1;
      shader->compute.shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader->compute.shader, num_shaders, &source, NULL);
      glCompileShader(shader->compute.shader);
      dn_gpu_shader_check_compilation(shader->compute.shader, shader->compute.path);

      shader->program = glCreateProgram();
      glAttachShader(shader->program, shader->compute.shader);
      glLinkProgram(shader->program);
      dn_gpu_shader_check_linkage(shader->program, shader->compute.path);

    } break;
    case DN_GPU_SHADER_GRAPHICS: {
      shader->kind = DN_GPU_SHADER_GRAPHICS;
      dn_cstr_copy_to(descriptor.name, shader->name, DN_ASSET_NAME_LEN);
      shader->graphics.vertex_path = dn_cstr_copy(descriptor.vertex_shader, &dn_allocators.standard.allocator);
      shader->graphics.fragment_path = dn_cstr_copy(descriptor.fragment_shader, &dn_allocators.standard.allocator);

      const char* paths[] = {
        descriptor.vertex_shader,
        descriptor.fragment_shader
      };

      shader->program = glCreateProgram();
      
      for (u32 index = 0; index < 2; index++) {
        const char* file_path = paths[index];
        char* source = dn_gpu_shader_preprocess(file_path);
        if (!source) return;
        
        // Compile the shader
        u32 shader_kind = (index == 0) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        u32 shader_id = glCreateShader(shader_kind);
        if (shader_kind == GL_VERTEX_SHADER) {
          shader->graphics.vertex_shader = shader_id;
        }
        else if (shader_kind == GL_FRAGMENT_SHADER) {
          shader->graphics.fragment_shader = shader_id;
        }

        u32 num_shaders = 1;
        glShaderSource(shader_id, num_shaders, &source, NULL);
        glCompileShader(shader_id);
        dn_gpu_shader_check_compilation(shader_id, file_path);

        glAttachShader(shader->program, shader_id);
      }
        
      // Link into a shader program
      glLinkProgram(shader->program);
      dn_gpu_shader_check_linkage(shader->program, shader->graphics.vertex_path);
      dn_gpu_set_resource_name(DN_GPU_RESOURCE_PROGRAM, shader->program, dn_cstr_len(shader->name), shader->name);
    } break;
  }
}

void dn_gpu_shader_reload(dn_gpu_shader_t* shader) {
  glDeleteProgram(shader->program);

  switch (shader->kind) {
    case DN_GPU_SHADER_GRAPHICS: {
      glDeleteShader(shader->graphics.vertex_shader);
      glDeleteShader(shader->graphics.fragment_shader);
      dn_gpu_shader_init(shader, (dn_gpu_shader_descriptor_t) {
        .name = shader->name,
        .vertex_shader = shader->graphics.vertex_path,
        .fragment_shader = shader->graphics.fragment_path,
        .compute_shader = NULL,
        .kind = DN_GPU_SHADER_GRAPHICS,
      });
    } break;
    case DN_GPU_SHADER_COMPUTE: {
      glDeleteShader(shader->compute.shader);
      dn_gpu_shader_init(shader, (dn_gpu_shader_descriptor_t) {
        .name = shader->name,
        .vertex_shader = NULL,
        .fragment_shader = NULL,
        .compute_shader = shader->compute.path,
        .kind = DN_GPU_SHADER_COMPUTE,
      });
    } break;
    default: {
      DN_ASSERT(false);
    }
  }
}

dn_tstring_t dn_gpu_shader_preprocess(const char* file_path) {
  const char* shader_file = dn_cstr_copy(file_path, &dn_allocators.bump.allocator);
  u8* error = dn_allocator_alloc(&dn_allocators.bump.allocator, 256);
  
  dn_preprocessor_context_t context = {
    .file_path = file_path,
    .include_dirs = {
      .data = (dn_path_t*)dn_fixed_array_at(&dn_gpu.search_paths, 0),
      .count = dn_gpu.search_paths.size
    }
  };
  dn_preprocess(&context);

  if (!context.result) {
    dn_log("shader preprocessor error; shader = %s, err = %s", shader_file, context.error);
    return dn_cstr_copy("YOUR_SHADER_FAILED_TO_COMPILE", &dn_allocators.bump.allocator);
  }
  
  return dn_cstr_copy(context.result, &dn_allocators.bump.allocator);
}

void dn_gpu_shader_check_compilation(u32 shader, const char* file_path) {
  s32 success = 0;
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char* compilation_status = (char*)dn_allocator_alloc(&dn_allocators.bump.allocator, 512);
    glGetShaderInfoLog(shader, 512, NULL, compilation_status);
    DN_LOG("%s: Compilation error; shader = %s, err = %s", file_path, compilation_status);
  }
}

void dn_gpu_shader_check_linkage(u32 program, const char* file_path) {
  s32 success = 0;
  
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char* compilation_status = (char*)dn_allocator_alloc(&dn_allocators.bump.allocator, 512);
    glGetProgramInfoLog(program, 512, NULL, compilation_status);
    DN_LOG("%s: Linker error; shader = %s, err = %s", file_path, compilation_status);
  }
}

dn_gpu_render_target_t* dn_gpu_render_target_create(dn_gpu_render_target_descriptor_t descriptor) {
  dn_gpu_render_target_t* target = (dn_gpu_render_target_t*)dn_fixed_array_reserve(&dn_gpu.targets, 1);
  dn_cstr_copy_to(descriptor.name, target->name, DN_MAX_PATH_LEN);
  target->size = descriptor.size;
  
  glGenFramebuffers(1, &target->handle);
  glBindFramebuffer(GL_FRAMEBUFFER, target->handle);

  // Generate the color buffer, allocate GPU memory for it, and attach it to the framebuffer
  glGenTextures(1, &target->color_buffer);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, target->color_buffer);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, target->size.x, target->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target->color_buffer, 0);

  // Clean up
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  dn_asset_registry_add(target->name, target);

  return target;
}

void dn_gpu_destroy_target(dn_gpu_render_target_t* target) {
  glDeleteTextures(1, &target->color_buffer);
  glDeleteFramebuffers(1, &target->handle);
}

dn_gpu_render_target_t* dn_gpu_render_target_find(const char* name) {
  for (u32 i = 0; i < dn_gpu.targets.size; i++) {
    dn_gpu_render_target_t* target = (dn_gpu_render_target_t*)dn_fixed_array_at(&dn_gpu.targets, i);
    if (!strncmp(target->name, name, DN_ASSET_NAME_LEN)) {
        return target;
    }
  }

  return NULL;
}

void dn_gpu_render_target_bind(dn_gpu_render_target_t* target) {
  if (!target) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, target->handle);
  glViewport(0, 0, target->size.x, target->size.y);
}

dn_gpu_render_target_t* dn_gpu_acquire_swapchain() {
  return (dn_gpu_render_target_t*)dn_fixed_array_at(&dn_gpu.targets, 0);
}

void dn_gpu_render_target_clear(dn_gpu_render_target_t* target) {
  DN_ASSERT(target);
  
  dn_gpu_render_target_bind(target);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void dn_gpu_render_target_blit(dn_gpu_render_target_t* source, dn_gpu_render_target_t* destination) {
  DN_ASSERT(source);
  DN_ASSERT(destination);
  
  glBindFramebuffer(GL_READ_FRAMEBUFFER, source->handle);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination->handle);
  glBlitFramebuffer(0, 0, source->size.x, source->size.y, 0, 0, destination->size.x, destination->size.y,  GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
}

void dn_gpu_swap_buffers() {
  glfwSwapBuffers(dn_app.window.handle);
}

/////////////////////
// ENUM CONVERSION //
/////////////////////
u32 dn_gpu_draw_primitive_to_gl_draw_primitive(dn_gpu_draw_primitive_t primitive) {
  switch (primitive) {
    case DN_GPU_PRIMITIVE_TRIANGLES: return GL_TRIANGLES;
  }

  assert(false);
  return 0;
}

u32 dn_gpu_blend_func_to_gl_blend_func(dn_gpu_blend_func_t func) {
  switch (func) {
    case DN_GPU_BLEND_FUNC_NONE:             return 0; break;
    case DN_GPU_BLEND_FUNC_ADD:              return GL_FUNC_ADD; break;
    case DN_GPU_BLEND_FUNC_SUBTRACT:         return GL_FUNC_SUBTRACT; break;
    case DN_GPU_BLEND_FUNC_REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT; break;
    case DN_GPU_BLEND_FUNC_MIN:              return GL_MIN; break;
    case DN_GPU_BLEND_FUNC_MAX:              return GL_MAX; break;
  };

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_blend_mode_to_gl_blend_mode(dn_gpu_blend_mode_t mode) {
  switch (mode) {
    case DN_GPU_BLEND_MODE_ZERO: return GL_ZERO; break;
    case DN_GPU_BLEND_MODE_ONE: return GL_ONE; break;
    case DN_GPU_BLEND_MODE_SRC_COLOR: return GL_SRC_COLOR; break;
    case DN_GPU_BLEND_MODE_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR; break;
    case DN_GPU_BLEND_MODE_DST_COLOR: return GL_DST_COLOR; break;
    case DN_GPU_BLEND_MODE_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR; break;
    case DN_GPU_BLEND_MODE_SRC_ALPHA: return GL_SRC_ALPHA; break;
    case DN_GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA; break;
    case DN_GPU_BLEND_MODE_DST_ALPHA: return GL_DST_ALPHA; break;
    case DN_GPU_BLEND_MODE_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA; break;
    case DN_GPU_BLEND_MODE_CONSTANT_COLOR: return GL_CONSTANT_COLOR; break;
    case DN_GPU_BLEND_MODE_ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR; break;
    case DN_GPU_BLEND_MODE_CONSTANT_ALPHA: return GL_CONSTANT_ALPHA; break;
    case DN_GPU_BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA; break;
  }

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_resource_id_to_gl_id(dn_gpu_resource_id_t id) {
  switch (id) {
    case DN_GPU_RESOURCE_FRAMEBUFFER: return GL_FRAMEBUFFER; break;
    case DN_GPU_RESOURCE_SHADER: return GL_SHADER; break;
    case DN_GPU_RESOURCE_PROGRAM: return GL_PROGRAM; break;
  }
  
  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_memory_barrier_to_gl_barrier(dn_gpu_memory_barrier_t barrier) {
  switch (barrier) {
    case DN_GPU_MEMORY_BARRIER_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
    case DN_GPU_MEMORY_BARRIER_BUFFER_UPDATE: return GL_BUFFER_UPDATE_BARRIER_BIT; break;
  }
  
  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_buffer_kind_to_gl_buffer_kind(dn_gpu_buffer_kind_t kind) {
  switch (kind) {
    case DN_GPU_BUFFER_KIND_STORAGE: return GL_SHADER_STORAGE_BUFFER; break;
    case DN_GPU_BUFFER_KIND_ARRAY: return GL_ARRAY_BUFFER; break;
    case DN_GPU_BUFFER_KIND_UNIFORM: return GL_UNIFORM_BUFFER; break;
  }

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_buffer_usage_to_gl_buffer_usage(dn_gpu_buffer_usage_t usage) {
  switch (usage) {
    case DN_GPU_BUFFER_USAGE_STATIC: return GL_STATIC_DRAW; break;
    case DN_GPU_BUFFER_USAGE_DYNAMIC: return GL_DYNAMIC_DRAW; break;
    case DN_GPU_BUFFER_USAGE_STREAM: return GL_STREAM_DRAW; break;
  }

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_buffer_kind_to_gl_barrier(dn_gpu_buffer_kind_t kind) {
  switch (kind) {
    case DN_GPU_BUFFER_KIND_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
    case DN_GPU_BUFFER_KIND_ARRAY: return GL_BUFFER_UPDATE_BARRIER_BIT; break;
    case DN_GPU_BUFFER_KIND_UNIFORM: return GL_UNIFORM_BARRIER_BIT; break;
  }

  DN_ASSERT(false);
  return 0;
}

dn_gpu_vertex_attr_info_t dn_gpu_vertex_attribute_info(dn_gpu_vertex_attribute_kind_t kind) {
  switch (kind) {
    case DN_GPU_VERTEX_ATTRIBUTE_FLOAT: {
      return (dn_gpu_vertex_attr_info_t) {
        .size = sizeof(GLfloat),
        .value = GL_FLOAT,
      };
    } break;
    case DN_GPU_VERTEX_ATTRIBUTE_U32: {
      return (dn_gpu_vertex_attr_info_t) {
        .size = sizeof(GLuint),
        .value = GL_UNSIGNED_INT,
      };
    } break;
  }

  DN_ASSERT(false);
  return dn_zero_struct(dn_gpu_vertex_attr_info_t);
}

void* dn_gpu_u32_to_gl_void_pointer(u32 value) {
  return (void*)(uintptr_t)value;
}

u32 dn_gpu_vertex_layout_calculate_stride(dn_gpu_buffer_layout_t* layout) {
  assert(layout);

  u32 stride = 0;

  for (u32 i = 0; i < layout->num_vertex_attributes; i++) {
    dn_gpu_vertex_attribute_t attribute = layout->vertex_attributes[i];
    dn_gpu_vertex_attr_info_t type_info = dn_gpu_vertex_attribute_info(attribute.kind);
    stride += attribute.count * type_info.size;
  }

  return stride;
}

void dn_gpu_apply_uniform_buffer_binding(dn_gpu_command_buffer_t* command_buffer, dn_gpu_uniform_buffer_binding_t* binding) {
  u32 program = command_buffer->pipeline->raster.shader->program;
  u32 location = glGetUniformBlockIndex(program, binding->name);
  glUniformBlockBinding(program, location, binding->binding_index); 
  dn_gpu_buffer_bind_base(binding->buffer, binding->binding_index);
}

///////////////////
// GPU DEBUGGING //
///////////////////
void dn_gpu_error_clear() {
  while (glGetError() != GL_NO_ERROR) {}
}

dn_tstring_t dn_gpu_error_read() {
  u32 error = glGetError();
  if (error == GL_INVALID_ENUM) {
    return dn_cstr_copy("GL_INVALID_ENUM", &dn_allocators.bump.allocator);
  }
  else if (error == GL_INVALID_OPERATION) {
    return dn_cstr_copy("GL_INVALID_OPERATION", &dn_allocators.bump.allocator);
  }
  else if (error == GL_OUT_OF_MEMORY) {
    return dn_cstr_copy("GL_OUT_OF_MEMORY", &dn_allocators.bump.allocator);
  }
  else if (error == GL_NO_ERROR) {
    return dn_cstr_copy("GL_NO_ERROR", &dn_allocators.bump.allocator);
  }

  return NULL;
}

void dn_gpu_error_log_one() {
  dn_log(dn_gpu_error_read());
}

void dn_gpu_error_log_all() {
  while (true) {
    dn_tstring_t error = dn_gpu_error_read();
    if (!error) break;
    if (!strcmp(error, "GL_NO_ERROR")) break;

    dn_log(error);
  }
}

void dn_gpu_set_resource_name(dn_gpu_resource_id_t id, u32 handle, u32 name_len, const char* name) {
  glObjectLabel(dn_gpu_resource_id_to_gl_id(id), handle, name_len, name);
}

/////////
// SDF //
/////////
dn_sdf_renderer_t dn_sdf_renderer_create(u32 buffer_size) {
  dn_sdf_renderer_t renderer = (dn_sdf_renderer_t) {
    .state = DN_SDF_RENDERER_STATE_NONE,
    .vertices = dn_gpu_backed_buffer_create((dn_gpu_buffer_descriptor_t) {
      .name = "SdfVertices",
      .kind = DN_GPU_BUFFER_KIND_ARRAY,
      .usage = DN_GPU_BUFFER_USAGE_STATIC,
      .capacity = buffer_size,
      .element_size = sizeof(dn_sdf_vertex_t)
    }),
    .instances = dn_gpu_backed_buffer_create((dn_gpu_buffer_descriptor_t) {
      .name = "SdfInstances",
      .kind = DN_GPU_BUFFER_KIND_ARRAY,
      .usage = DN_GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(dn_sdf_instance_t)
    }),
    .combinations = dn_gpu_backed_buffer_create((dn_gpu_buffer_descriptor_t) {
      .name = "SdfCombinations",
      .kind = DN_GPU_BUFFER_KIND_STORAGE,
      .usage = DN_GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(u32)
    }),
    .shape_data = dn_gpu_backed_buffer_create((dn_gpu_buffer_descriptor_t) {
      .name = "SdfShapeData",
      .kind = DN_GPU_BUFFER_KIND_STORAGE,
      .usage = DN_GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(float)
    }),
    .pipeline = dn_gpu_pipeline_create((dn_gpu_pipeline_descriptor_t) {
      .blend = (dn_gpu_blend_state_t) {
        .fn = DN_GPU_BLEND_FUNC_ADD,
        .source = DN_GPU_BLEND_MODE_SRC_ALPHA,
        .destination = DN_GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA,
      },
      .raster = (dn_gpu_raster_state_t) {
        .shader = dn_gpu_shader_find("shape"),
        .primitive = DN_GPU_PRIMITIVE_TRIANGLES
      },
      .buffer_layouts = {
        { 
          .vertex_attributes = {
            { .kind = DN_GPU_VERTEX_ATTRIBUTE_FLOAT, .count = 2 },
            { .kind = DN_GPU_VERTEX_ATTRIBUTE_FLOAT, .count = 2 },
          },   
          .num_vertex_attributes = 2 
        },
        { 
          .vertex_attributes = {
            { .kind = DN_GPU_VERTEX_ATTRIBUTE_U32, .count = 2, .divisor = 1 },
          }, 
          .num_vertex_attributes = 1 
        },
      },
      .num_buffer_layouts = 2
    })
  };

  renderer.bindings = (dn_gpu_buffer_binding_t) {
    .vertex = (dn_gpu_vertex_buffer_binding_array_t) {
      .bindings = {
        (dn_gpu_vertex_buffer_binding_t) { 
          .buffer = renderer.vertices.gpu_buffer 
        },
        (dn_gpu_vertex_buffer_binding_t) { 
          .buffer = renderer.instances.gpu_buffer 
        },
      },
      .count = 2
    },
    .storage = (dn_gpu_storage_buffer_binding_array_t) {
      .bindings = {
        (dn_gpu_storage_buffer_binding_t) { 
          .buffer = renderer.shape_data.gpu_buffer,   
          .base = 0 
        },
        (dn_gpu_storage_buffer_binding_t) { 
          .buffer = renderer.combinations.gpu_buffer, 
          .base = 1 
        },
      },
      .count = 2
    }
  };

  dn_vector2_t vertices [6] = dn_quad_literal(0.5, -0.5, -0.5, 0.5);
  for (u32 i = 0; i < 6; i++) {
    dn_sdf_vertex_t vertex = {
      .position = vertices[i],
      .uv = vertices[i],
    };
    dn_gpu_backed_buffer_push(&renderer.vertices, &vertex, 1);
  }
  dn_gpu_backed_buffer_sync(&renderer.vertices);

  return renderer;
}

void dn_sdf_renderer_draw(dn_sdf_renderer_t* renderer, dn_gpu_command_buffer_t* command_buffer) {
  DN_ASSERT(renderer);

  dn_gpu_backed_buffer_sync(&renderer->instances);
  dn_gpu_backed_buffer_sync(&renderer->shape_data);
  dn_gpu_backed_buffer_sync(&renderer->combinations);

  dn_gpu_bind_pipeline(command_buffer, renderer->pipeline);
  dn_gpu_apply_bindings(command_buffer, renderer->bindings);
  dn_gpu_command_buffer_draw(command_buffer, (dn_gpu_draw_call_t) {
    .mode = DN_GPU_DRAW_MODE_INSTANCE,
    .vertex_offset = 0,
    .num_vertices = 6,
    .num_instances = dn_gpu_backed_buffer_size(&renderer->instances)
  });
  
  dn_gpu_backed_buffer_clear(&renderer->instances);
  dn_gpu_backed_buffer_clear(&renderer->shape_data);
  dn_gpu_backed_buffer_clear(&renderer->combinations);
}

void dn_sdf_renderer_push_instance(dn_sdf_renderer_t* renderer, dn_sdf_shape_t shape) {
  DN_ASSERT(renderer);

  if (renderer->state == DN_SDF_RENDERER_STATE_NONE) {
    dn_sdf_instance_t instance = (dn_sdf_instance_t) {
      .shape = shape,
      .buffer_index = dn_gpu_backed_buffer_size(&renderer->shape_data),
    };
    dn_gpu_backed_buffer_push(&renderer->instances, &instance, 1);
  }
}

void dn_sdf_renderer_push_header(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness) {
  DN_ASSERT(renderer);

  dn_gpu_backed_buffer_push(&renderer->shape_data, &r, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &g, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &b, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &px, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &py, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &rotation, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &edge_thickness, 1);
}

dn_sdf_combine_header_t* dn_sdf_combination_begin(dn_sdf_renderer_t* renderer) {
  DN_ASSERT(renderer);

  // First, push an instance that points into the combination buffer.
  dn_sdf_instance_t instance = (dn_sdf_instance_t) {
    .shape = DN_SDF_SHAPE_COMBINE,
    .buffer_index = dn_gpu_backed_buffer_size(&renderer->combinations),
  };
  dn_gpu_backed_buffer_push(&renderer->instances, &instance, 1);

  renderer->state = DN_SDF_RENDERER_STATE_COMBINATION;

  // Then, put a combination header in the data buffer; this'll tell the GPU how many combination entries 
  // there are. Since we don't know that yet, return a pointer that gets filled in as you push shapes.
  dn_sdf_combine_header_t* header = (dn_sdf_combine_header_t*)dn_gpu_backed_buffer_push(&renderer->combinations, NULL, 1);
  header->num_sdfs = 0;
  return header;
}

void dn_sdf_combination_append(dn_sdf_renderer_t* renderer, dn_sdf_combine_header_t* header, dn_sdf_shape_t shape, dn_sdf_combine_op_t op, dn_sdf_smoothing_kernel_t kernel) {
  DN_ASSERT(renderer);

  header->num_sdfs++;
  
  dn_sdf_combine_entry_t entry = (dn_sdf_combine_entry_t) {
    .buffer_index = dn_gpu_backed_buffer_size(&renderer->shape_data),
    .shape = shape,
    .op = op,
    .kernel = kernel
  };
  dn_gpu_backed_buffer_push(&renderer->combinations, &entry, 4);
}

void dn_sdf_combination_commit(dn_sdf_renderer_t* renderer) {
  DN_ASSERT(renderer);
  renderer->state = DN_SDF_RENDERER_STATE_NONE;
}

void dn_sdf_circle_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius) {
  DN_ASSERT(renderer);
  dn_sdf_renderer_push_instance(renderer, DN_SDF_SHAPE_CIRCLE);
  dn_sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &radius, 1);
}

void dn_sdf_ring_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius) {
  DN_ASSERT(renderer);
  dn_sdf_renderer_push_instance(renderer, DN_SDF_SHAPE_RING);
  dn_sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &inner_radius, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &outer_radius, 1);
}

void dn_sdf_oriented_box_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy) {
  DN_ASSERT(renderer);
  dn_sdf_renderer_push_instance(renderer, DN_SDF_SHAPE_ORIENTED_BOX);
  dn_sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &dx, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &dy, 1);
}

void dn_sdf_grid(dn_sdf_renderer_t* renderer, u32 grid_width, u32 grid_size) {
  DN_ASSERT(renderer);
  dn_time_metric_begin(dn_string_literal("sdf_c"));
  for (u32 x = 0; x < grid_size; x += grid_width) {
    for (u32 y = 0; y < grid_size; y += grid_width) {
      dn_sdf_circle_ex(renderer,
        x, y,
        0.40f, 0.65f, 0.55f, 
        0.0f,
        1.5f,
        3.0f
      );
    }
    
  }
  dn_time_metric_end(dn_string_literal("sdf_c"));
}


/////////////////////////
// BACKGROUND IMPORTER //
/////////////////////////
void dn_asset_registry_init(dn_asset_config_t user_config) {
  DN_LOG_FN();

  dn_asset_importer_t default_importers [] = {
    {
      .id = dn_type_name(dn_background_t),
      .on_import = &dn_background_import,
      .on_complete = &dn_background_complete
    }
  };

  dn_asset_config_t configs [] = {
    {
      .importers = {
        .data = default_importers,
        .count = dn_arr_len(default_importers)
      }
    },
    user_config
  };
   
  dn_for_arr(configs, config_index) {
    dn_asset_config_t* config = &configs[config_index];
    dn_for(importer_index, config->importers.count) {
      dn_asset_importer_t* importer = &config->importers.data[importer_index];

      DN_LOG("Registered importer; id = %s, on_import = %d, on_complete = %d", 
        importer->id, 
        importer->on_import, importer->on_complete
      );

      gs_hash_table_insert(dn_app.asset_registry.importers, dn_hash_string(dn_string_cstr(importer->id)), *importer);
    }
  }  

  dn_ring_buffer_init(&dn_app.loader.load_requests, 2048, sizeof(dn_asset_load_request_t));
  dn_ring_buffer_init(&dn_app.loader.completion_queue, 2048, sizeof(dn_asset_load_request_t));
  
  // asset_loader.thread = std::thread(&AssetLoader::process_requests, &asset_loader);
  // asset_loader.thread.detach();
}

void dn_asset_registry_add(const char* name, dn_asset_data_t data) {
  DN_LOG("Registered asset; name = %s, data = %p", name, data);

  dn_asset_t asset;
  dn_cstr_copy_to(name, asset.name, DN_ASSET_NAME_LEN);
  asset.data = data;

  gs_hash_table_insert(dn_app.asset_registry.assets, dn_hash_string(dn_string_cstr(name)), asset);
}

dn_asset_data_t dn_asset_registry_find(const char* name) {
    DN_ASSERT(name);

    dn_hash_t hash = dn_hash_string(dn_string_cstr(name));
    if (!gs_hash_table_exists(dn_app.asset_registry.assets, hash)) {
      DN_LOG("Tried to find asset, but it was not registered; name = %.*s", name);
      return NULL;
    }
  
    return gs_hash_table_getp(dn_app.asset_registry.assets, hash);
}

void dn_asset_registry_update() {
  DN_BROKEN();
}

void dn_images_init(dn_image_config_t config) {
  DN_BROKEN();
}

void dn_atlases_init() {
  DN_BROKEN();
}

void dn_screenshots_init() {
  DN_BROKEN();
}

void dn_backgrounds_init() {
  DN_BROKEN();
}

void dn_background_import(dn_asset_import_request_t* request) {
  DN_BROKEN();
}

dn_asset_completion_status_t dn_background_complete(dn_asset_import_request_t* request) {
  DN_BROKEN();
  return DN_ASSET_COMPLETION_STATUS_DONE;
}

void dn_asset_loader_process_requests() {
  DN_BROKEN();
}

void dn_asset_loader_process_completion_queue() {
  DN_BROKEN();
}

void dn_asset_loader_submit(dn_asset_load_request_t request) {
  DN_BROKEN();
}

///////////
// MUTEX //
///////////
void dn_mutex_init(dn_mutex_t* mutex) {
  DN_ASSERT(mutex);
  DN_ASSERT(mutex->initialized);
  InitializeCriticalSection(&mutex->handle);
}

void dn_mutex_lock(dn_mutex_t* mutex) {
  DN_ASSERT(mutex);
  DN_ASSERT(mutex->initialized);
  EnterCriticalSection(&mutex->handle);
}

void dn_mutex_unlock(dn_mutex_t* mutex) {
  DN_ASSERT(mutex);
  DN_ASSERT(mutex->initialized);
  LeaveCriticalSection(&mutex->handle);
}
/////////////
// UTILITY //
/////////////
dn_color_t dn_color_rgb_to_hsv(dn_color_t color) {
  DN_BROKEN();
  dn_vector4_t k = (dn_color_t) { (0.0f), (-1.0f / 3.0f), (2.0f / 3.0f), (-1.0f) };

  dn_vector4_t pa = (dn_vector4_t) { color.b, color.g, k.w, k.z };
  dn_vector4_t pb = (dn_vector4_t) { color.g, color.b, k.x, k.y };
  dn_vector4_t p = dn_math_lerp4(pa, pb, dn_math_step(color.b, color.g));

  dn_vector4_t qa = (dn_vector4_t) { p.x, p.y, p.w, color.r };
  dn_vector4_t qb = (dn_vector4_t) { color.r, p.y, p.z, p.x };
  dn_vector4_t q = dn_math_lerp4(qa, qb, dn_math_step(p.x, color.r));

  f32 d = q.x - dn_min(q.w, q.y);
  f32 e = 1.0e-10f;
  return (dn_color_t) {
    fabs(q.z + (q.w - q.y) / (6.0 * d + e)),
    d / (q.x + e),
    q.x,
    color.a
  };
}

dn_color_t dn_color_hsv_to_rgb(dn_color_t color) {
  DN_BROKEN();
  // dn_vector4_t k = (dn_color_t) { (1.0f), (2.0f / 3.0f), (1.0f / 3.0f), (3.0f) };
  // dn_vector4_t p = (dn_color_t) { color.x + k.x, color.x + k.y, color.x + k.z };
  return color;
}

f32 dn_math_step(f32 edge, f32 x) {
    return (x < edge) ? 0.0f : 1.0f;
}

f32 dn_math_lerp(f32 a, f32 b, f32 t) {
    return a * (1.0f - t) + b * t;
}

dn_vector4_t dn_math_lerp4(dn_vector4_t a, dn_vector4_t b, f32 t) {
  return (dn_vector4_t){
    .x = dn_math_lerp(a.x, b.x, t),
    .y = dn_math_lerp(a.y, b.y, t),
    .z = dn_math_lerp(a.z, b.z, t),
    .w = dn_math_lerp(a.w, b.w, t),
  };
}

dn_vector4_t dn_math_abs4(dn_vector4_t v) {
    dn_vector4_t result;
    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    result.z = fabsf(v.z);
    result.w = fabsf(v.w);
    return result;
}

dn_vector4_t dn_math_fract4(dn_vector4_t v) {
    dn_vector4_t result;
    result.x = v.x - floorf(v.x);
    result.y = v.y - floorf(v.y);
    result.z = v.z - floorf(v.z);
    result.w = v.w - floorf(v.w);
    return result;
}

f32 dn_math_clamp(f32 value, f32 lower, f32 upper) {
  return dn_min(dn_max(value, lower), upper);
}

dn_vector4_t dn_math_clamp4(dn_vector4_t v, float minVal, float maxVal) {
    dn_vector4_t result;
    result.x = fmaxf(minVal, fminf(v.x, maxVal));
    result.y = fmaxf(minVal, fminf(v.y, maxVal));
    result.z = fmaxf(minVal, fminf(v.z, maxVal));
    result.w = fmaxf(minVal, fminf(v.w, maxVal));
    return result;
}

u32 dn_math_ceilf(f32 f) {
    u32 i = (u32)f;
    return f > i ? i + 1 : i;
}

u32 dn_math_ceil_divide(u32 a, u32 b) {
  return dn_math_ceilf((f32)a / (f32)b);
}

f32 dn_random_f32_slow(f32 vmin, f32 vmax) {
  DN_BROKEN();
  return 0.0;
}

s32 dn_random_i32(s32 vmin, s32 vmax) {
  DN_BROKEN();
  return 0;
}

void dn_random_init() {
  srand(time(NULL));
}


//////////////////
// TIME METRICS //
//////////////////
void dn_time_metrics_init() {
  dn_time_metrics_add(dn_string_literal("frame"));
}

void dn_time_metrics_update() {
  dn_time_metric_t* frame = dn_time_metrics_find(dn_string_literal("frame"));
  dn_time_metric_end_ex(frame);
  dn_time_metric_busy_wait(frame, dn_app.dt);
}

void dn_time_metric_init(dn_time_metric_t* metric) {
  dn_ring_buffer_init(&metric->queue, 64, sizeof(dn_time_metric_t));
}

void dn_time_metric_begin_ex(dn_time_metric_t* metric) {
  metric->time_begin = glfwGetTime();
}

void dn_time_metric_end_ex(dn_time_metric_t* metric) {
  f64 time_end = glfwGetTime();
  f64 delta = time_end - metric->time_begin;
  dn_ring_buffer_push_overwrite(&metric->queue, &delta);
}

f64 dn_time_metric_get_average(dn_time_metric_t* metric) {
  if (dn_ring_buffer_is_empty(&metric->queue)) return 0;
  
  f64 total = 0;
  dn_ring_buffer_for(metric->queue, entry) {
    total += *dn_rb_it(entry, f64);
  }

  return total / metric->queue.size;
}

f64 dn_time_metric_get_last(dn_time_metric_t* metric) {
  if (dn_ring_buffer_is_empty(&metric->queue)) return 0;
  return *(f64*)dn_ring_buffer_back(&metric->queue);
}

f64 dn_time_metric_get_largest(dn_time_metric_t* metric) {
  f64 max_entry = 0;
  dn_ring_buffer_for(metric->queue, entry) {
    max_entry = dn_max(max_entry, *dn_rb_it(entry, f64));
  }

  return max_entry;
}

f64 dn_time_metric_get_smallest(dn_time_metric_t* metric) {
  f64 min_entry = DN_F64_MAX;
  dn_ring_buffer_for(metric->queue, entry) {
    min_entry = dn_min(min_entry, *dn_rb_it(entry, f64));
  }

  return min_entry;
}

void dn_time_metric_sleep_wait(dn_time_metric_t* metric, f64 target) {
  while (true) {
    f64 delta = glfwGetTime() - metric->time_begin;
    if (delta >= target) break;

    f64 remaining_time = target - delta;
    if (remaining_time > 0) {
      dn_os_sleep_ms(remaining_time * 1e3);
    }
  }
}

void dn_time_metric_busy_wait(dn_time_metric_t* metric, f64 target) {
  while (true) {
    auto delta = glfwGetTime() - metric->time_begin;
    if (delta >= target) break;
  }
}

dn_time_metric_t* dn_time_metrics_find(dn_string_t name) {
  dn_hash_t hash = dn_hash_string(name);
  if (!gs_hash_table_exists(dn_time_metrics.metrics, hash)) {
    dn_log("Tried to find time metric, but it was not registered; name = %.*s", __func__, name.len, name.data);
    return NULL;
  }

  return gs_hash_table_getp(dn_time_metrics.metrics, hash);
}

void dn_time_metrics_add(dn_string_t name) {
  dn_time_metric_t time_metric;
  dn_time_metric_init(&time_metric);
  gs_hash_table_insert(dn_time_metrics.metrics, dn_hash_string(name), time_metric);
}

void dn_time_metric_begin(dn_string_t name) {
  dn_time_metric_t* metric = dn_time_metrics_find(name);
  if (!metric) return;
  
  dn_time_metric_begin_ex(metric);
}

void dn_time_metric_end(dn_string_t name) {
  dn_time_metric_t* metric = dn_time_metrics_find(name);
  if (!metric) return;

  dn_time_metric_end_ex(metric);
}

/////////
// LOG //
/////////
void dn_log_init() {
  dn_os_zero_memory(&dn_logger, sizeof(dn_log_t));
}

void dn_log(const char* fmt, ...) {
  va_list fmt_args;
  va_start(fmt_args, fmt);
  dn_log_v(DN_LOG_FLAG_DEFAULT, fmt, fmt_args);
  va_end(fmt_args);
}

void dn_log_format_str(dn_string_t fmt, ...) {
  va_list fmt_args;
  va_start(fmt_args, fmt);
  dn_log_v(DN_LOG_FLAG_DEFAULT, dn_string_to_cstr(fmt), fmt_args);
  va_end(fmt_args);
}

void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...) {
  va_list fmt_args;
  va_start(fmt_args, fmt);
  dn_log_v(flags, fmt, fmt_args);
  va_end(fmt_args);
}

void dn_log_v(dn_log_flags_t flags, const char* fmt, va_list fmt_args) {
  dn_log_build_preamble();
  vsnprintf(&dn_logger.message_buffer[0], DN_LOGGER_MESSAGE_BUFFER_SIZE, fmt, fmt_args);
  dn_log_flush(flags);
}

void dn_log_str(dn_string_t message) {
  dn_log_build_preamble();
  snprintf(dn_logger.message_buffer, DN_LOGGER_MESSAGE_BUFFER_SIZE, "%.*s", message.len, message.data);
  dn_log_flush(DN_LOG_FLAG_CONSOLE);
}

void dn_log_builder(dn_string_builder_t builder) {
  dn_log_str(dn_string_builder_write(&builder));
}

void dn_log_build_preamble() {
  uint64_t ms_since_epoch = dn_os_time_since_epoch();
  time_t sec_since_epoch = (time_t)(ms_since_epoch / 1000);
  struct tm* time_info = localtime(&sec_since_epoch);

  snprintf(dn_logger.preamble_buffer, DN_LOGGER_PREAMBLE_BUFFER_SIZE, "[%04d-%02d-%02d %02d:%02d:%02d.%03" DN_FMT_U64 "]",
       1900 + time_info->tm_year, 1 + time_info->tm_mon, time_info->tm_mday,
       time_info->tm_hour, time_info->tm_min, time_info->tm_sec, ms_since_epoch % 1000);
}

void dn_log_flush(dn_log_flags_t flags) {
  if (flags & DN_LOG_FLAG_CONSOLE) {
    printf("%s %s\n", dn_logger.preamble_buffer, dn_logger.message_buffer);
  }

  dn_log_zero();
}

void dn_log_zero() {
  memset(&dn_logger.preamble_buffer[0], 0, DN_LOGGER_PREAMBLE_BUFFER_SIZE);
  memset(&dn_logger.message_buffer[0], 0, DN_LOGGER_MESSAGE_BUFFER_SIZE);
}

/////////////////
// RING BUFFER //
/////////////////
void* dn_ring_buffer_at(dn_ring_buffer_t* buffer, u32 index) {
    return buffer->data + ((buffer->head + buffer->element_size * index) % buffer->capacity);
}

void dn_ring_buffer_init(dn_ring_buffer_t* buffer, u32 capacity, u32 element_size) {
  buffer->size = 0;
  buffer->head = 0;
  buffer->capacity = capacity;
  buffer->element_size = element_size;
  buffer->data = dn_allocator_alloc(&dn_allocators.standard.allocator, capacity * element_size);
}

void* dn_ring_buffer_back(dn_ring_buffer_t* buffer) {
  DN_ASSERT(buffer->size);
  return dn_ring_buffer_at(buffer, buffer->size - 1);
}

void* dn_ring_buffer_push(dn_ring_buffer_t* buffer, void* data) {
  DN_ASSERT(buffer->size < buffer->capacity);

  u32 index = (buffer->head + buffer->size) % buffer->capacity;
  dn_os_memory_copy(data, buffer->data + index * buffer->element_size, buffer->element_size);
  buffer->size += 1;
  return dn_ring_buffer_back(buffer);
}

void* dn_ring_buffer_push_zero(dn_ring_buffer_t* buffer) {
  DN_ASSERT(buffer->size < buffer->capacity);

  u32 index = (buffer->head + buffer->size) % buffer->capacity;
  dn_os_zero_memory(buffer->data + index, buffer->element_size);
  buffer->size += 1;
  return dn_ring_buffer_back(buffer);
}

void* dn_ring_buffer_push_overwrite(dn_ring_buffer_t* buffer, void* data) {
  if (buffer->size == buffer->capacity) dn_ring_buffer_pop(buffer);
  return dn_ring_buffer_push(buffer, data);
}

void* dn_ring_buffer_push_overwrite_zero(dn_ring_buffer_t* buffer) {
  if (buffer->size == buffer->capacity) dn_ring_buffer_pop(buffer);
  return dn_ring_buffer_push_zero(buffer);
}

void* dn_ring_buffer_pop(dn_ring_buffer_t* buffer) {
  DN_ASSERT(buffer->size);

  void* element = buffer->data + (buffer->head * buffer->element_size);
  buffer->head = (buffer->head + 1) % buffer->capacity;
  buffer->size--;
  return element;
}

u32 dn_ring_buffer_bytes(dn_ring_buffer_t* buffer) {
  return buffer->capacity * buffer->element_size;
}

void dn_ring_buffer_clear(dn_ring_buffer_t* buffer) {
  memset(buffer->data, 0, dn_ring_buffer_bytes(buffer));
  buffer->size = 0;
  buffer->head = 0;
}

bool dn_ring_buffer_is_full(dn_ring_buffer_t* buffer) {
  return buffer->capacity == buffer->size;
}

bool dn_ring_buffer_is_empty(dn_ring_buffer_t* buffer) {
  return buffer->size == 0;
}

void* dn_ring_buffer_iter_deref(dn_ring_buffer_iterator_t* it) {
  return dn_ring_buffer_at(it->buffer, it->index);
}

void dn_ring_buffer_iter_next(dn_ring_buffer_iterator_t* it) {
  DN_ASSERT(it->index < it->buffer->size);
  it->index++;
}

void dn_ring_buffer_iter_prev(dn_ring_buffer_iterator_t* it) {
  DN_ASSERT(it->index < it->buffer->size);
  it->index--;
}

bool dn_ring_buffer_iter_done(dn_ring_buffer_iterator_t* it) {
  if (it->reverse) return it->index == -1;
  return it->index == it->buffer->size;
}

dn_ring_buffer_iterator_t dn_ring_buffer_iter(dn_ring_buffer_t* buffer) {
  dn_ring_buffer_iterator_t iterator;
  iterator.index = 0;
  iterator.reverse = false;
  iterator.buffer = buffer;
  return iterator;
}

dn_ring_buffer_iterator_t dn_ring_buffer_riter(dn_ring_buffer_t* buffer) {
  dn_ring_buffer_iterator_t iterator;
  iterator.index = buffer->size - 1;
  iterator.reverse = true;
  iterator.buffer = buffer;
  return iterator;
}

//////////
// POOL //
//////////
void dn_pool_init(dn_pool_t* pool, u32 capacity, u32 element_size) {
  pool->capacity = capacity;
  pool->free_list = 0;
  pool->element_size = element_size;
  pool->data = (u8*)dn_allocator_alloc(&dn_allocators.standard.allocator, capacity * element_size);
  pool->slots = (dn_pool_slot_t*)dn_allocator_alloc(&dn_allocators.standard.allocator, capacity * sizeof(dn_pool_slot_t));

  dn_for(i, capacity) {
    pool->slots[i].next_free = i + 1;
  }
  pool->slots[capacity - 1].next_free = DN_POOL_NO_FREE_SLOT;
}

dn_pool_handle_t dn_pool_insert(dn_pool_t* pool, void* value) {
  assert(!pool->slots[pool->free_list].occupied);

  dn_pool_slot_t* slot = pool->slots + pool->free_list;
  slot->occupied = true;
  slot->generation++;

  dn_os_memory_copy(value, pool->data + pool->element_size * pool->free_list, pool->element_size);

  dn_pool_handle_t handle;
  handle.index = pool->free_list;
  handle.generation = slot->generation;

  pool->free_list = slot->next_free;

  return handle;
}

dn_pool_handle_t dn_pool_reserve(dn_pool_t* pool) {
  assert(!pool->slots[pool->free_list].occupied);

  dn_pool_slot_t* slot = pool->slots + pool->free_list;
  slot->occupied = true;
  slot->generation++;

  dn_os_zero_memory(pool->data + pool->element_size * pool->free_list, pool->element_size);

  dn_pool_handle_t handle;
  handle.index = pool->free_list;
  handle.generation = slot->generation;

  pool->free_list = slot->next_free;

  return handle;
}

void dn_pool_remove(dn_pool_t* pool, dn_pool_handle_t handle) {
  if (handle.index >= pool->capacity) return;

  dn_pool_slot_t* slot = pool->slots + handle.index;
  if (!dn_pool_slot_is_match(slot, handle)) return;
  if (!dn_pool_slot_has_next_free(slot)) return;

  slot->occupied = false;
  slot->generation++;
  slot->next_free = pool->free_list;
  pool->free_list = handle.index;
}

bool dn_pool_contains(dn_pool_t* pool, dn_pool_handle_t handle) {
  if (handle.index >= pool->capacity) return false;

  dn_pool_slot_t* slot = pool->slots + handle.index;
  return dn_pool_slot_is_match(slot, handle);
}

void dn_pool_clear(dn_pool_t* pool) {
  dn_for(i, pool->capacity) {
    pool->slots[i].next_free = i + 1;
    pool->slots[i].occupied = false;
    pool->slots[i].generation++;
  }

  pool->slots[pool->capacity - 1].next_free = -1;

  pool->free_list = 0;
}

dn_pool_handle_t dn_pool_invalid_handle() {
  return (dn_pool_handle_t){
    .index = 0,
    .generation = 0
  };
}

bool dn_pool_is_handle_valid(dn_pool_handle_t handle) {
  return handle.generation > 0;
}

bool dn_pool_slot_has_next_free(dn_pool_slot_t* slot) {
  return slot->next_free >= 0;
}

bool dn_pool_slot_is_match(dn_pool_slot_t* slot, dn_pool_handle_t handle) {
  return slot->generation == handle.generation;
}

dn_pool_iterator_t dn_pool_iterator_init(dn_pool_t* pool) {
  return (dn_pool_iterator_t) {
    .index = 0,
    .pool = pool
  };
}

void dn_pool_iterator_next(dn_pool_iterator_t* it) {
  while (true) {
    it->index++;
    if (it->index >= it->pool->capacity) break;

    dn_pool_slot_t* slot = &it->pool->slots[it->index];
    if (slot->occupied) break;
  } 
}

bool dn_pool_iterator_done(dn_pool_iterator_t* it) {
  if (it->index >= it->pool->capacity) return true;
  return false;
}

///////////////////
// DYNAMIC ARRAY //
///////////////////
void dn_dynamic_array_init(dn_dynamic_array_t* buffer, u32 element_size, dn_allocator_t* allocator) {
  DN_ASSERT(buffer);

  buffer->size = 0;
  buffer->capacity = 2;
  buffer->element_size = element_size;
  buffer->data = (u8*)dn_allocator_alloc(allocator, buffer->capacity * element_size);
  buffer->allocator = allocator;
}

u8* dn_dynamic_array_at(dn_dynamic_array_t* buffer, u32 index) {
  DN_ASSERT(buffer);
  return buffer->data + (index * buffer->element_size);
}

u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data) {
  return dn_dynamic_array_push_n(dynamic_array, data, 1);
}

u8* dn_dynamic_array_push_n(dn_dynamic_array_t* buffer, void* data, u32 count) {
  DN_ASSERT(buffer);

  void* reserved = dn_dynamic_array_reserve(buffer, count);
  if (data) dn_os_memory_copy(data, reserved, buffer->element_size * count);
  return reserved;
}

u8* dn_dynamic_array_reserve(dn_dynamic_array_t* buffer, u32 count) {
  DN_ASSERT(buffer);

  dn_dynamic_array_grow(buffer, buffer->size + count);

  void* element = dn_dynamic_array_at(buffer, buffer->size);
  buffer->size += count;
  return element;
}

void dn_dynamic_array_clear(dn_dynamic_array_t* buffer) {
  DN_ASSERT(buffer);

  buffer->size = 0;
}

u32 dn_dynamic_array_byte_size(dn_dynamic_array_t* buffer) {
  DN_ASSERT(buffer);

  return buffer->size * buffer->element_size;
}

void dn_dynamic_array_grow(dn_dynamic_array_t* buffer, u32 capacity) {
  DN_ASSERT(buffer);

  if (buffer->capacity >= capacity) return;
  buffer->capacity = dn_max(buffer->capacity * 2, capacity);
  buffer->data = (u8*)dn_allocator_realloc(buffer->allocator, buffer->data, buffer->capacity * buffer->element_size);
}

/////////////////
// FIXED ARRAY //
/////////////////
void dn_fixed_array_init(dn_fixed_array_t* buffer, u32 max_vertices, u32 element_size, dn_allocator_t* allocator) {
  DN_ASSERT(buffer);

  buffer->size = 0;
  buffer->capacity = max_vertices;
  buffer->element_size = element_size;
  buffer->data = (u8*)dn_allocator_alloc(allocator, max_vertices * element_size);
  buffer->allocator = allocator;
}

u8* dn_fixed_array_at(dn_fixed_array_t* buffer, u32 index) {
  DN_ASSERT(buffer);
  return buffer->data + (index * buffer->element_size);
}

u8* dn_fixed_array_push(dn_fixed_array_t* buffer, void* data, u32 count) {
  DN_ASSERT(buffer);
  DN_ASSERT(buffer->size < buffer->capacity);

  void* reserved = dn_fixed_array_reserve(buffer, count);
  if (data) dn_os_memory_copy(data, reserved, buffer->element_size * count);
  return reserved;
}

u8* dn_fixed_array_reserve(dn_fixed_array_t* buffer, u32 count) {
  DN_ASSERT(buffer);

  void* element = dn_fixed_array_at(buffer, buffer->size);
  buffer->size += count;
  return element;
}

void dn_fixed_array_clear(dn_fixed_array_t* buffer) {
  DN_ASSERT(buffer);

  buffer->size = 0;
}

u32 dn_fixed_array_byte_size(dn_fixed_array_t* buffer) {
  DN_ASSERT(buffer);

  return buffer->size * buffer->element_size;
}

//////////
// HASH //
//////////
dn_hash_t hash_siphash_bytes(const void *p, size_t len, size_t seed) {
  unsigned char *d = (unsigned char *) p;
  size_t i,j;
  size_t v0,v1,v2,v3, data;

  // hash that works on 32- or 64-bit registers without knowing which we have
  // (computes different results on 32-bit and 64-bit platform)
  // derived from siphash, but on 32-bit platforms very different as it uses 4 32-bit state not 4 64-bit
  v0 = ((((size_t) 0x736f6d65 << 16) << 16) + 0x70736575) ^  seed;
  v1 = ((((size_t) 0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
  v2 = ((((size_t) 0x6c796765 << 16) << 16) + 0x6e657261) ^  seed;
  v3 = ((((size_t) 0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

  #define gs_sipround() \
    do {                   \
      v0 += v1; v1 = gs_rotate_left(v1, 13);  v1 ^= v0; v0 = gs_rotate_left(v0,GS_SIZE_T_BITS/2); \
      v2 += v3; v3 = gs_rotate_left(v3, 16);  v3 ^= v2;                                                 \
      v2 += v1; v1 = gs_rotate_left(v1, 17);  v1 ^= v2; v2 = gs_rotate_left(v2,GS_SIZE_T_BITS/2); \
      v0 += v3; v3 = gs_rotate_left(v3, 21);  v3 ^= v0;                                                 \
    } while (0)

  for (i=0; i+sizeof(size_t) <= len; i += sizeof(size_t), d += sizeof(size_t)) {
    data = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    data |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24)) << 16 << 16; // discarded if size_t == 4

    v3 ^= data;
    for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
      gs_sipround();
    v0 ^= data;
  }
  data = len << (GS_SIZE_T_BITS-8);
  switch (len - i) {
    case 7: data |= ((size_t) d[6] << 24) << 24; // fall through
    case 6: data |= ((size_t) d[5] << 20) << 20; // fall through
    case 5: data |= ((size_t) d[4] << 16) << 16; // fall through
    case 4: data |= (d[3] << 24); // fall through
    case 3: data |= (d[2] << 16); // fall through
    case 2: data |= (d[1] << 8); // fall through
    case 1: data |= d[0]; // fall through
    case 0: break;
  }
  v3 ^= data;
  for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
    gs_sipround();
  v0 ^= data;
  v2 ^= 0xff;
  for (j=0; j < GS_SIPHASH_D_ROUNDS; ++j)
    gs_sipround();

#if 0
  return v0^v1^v2^v3;
#else
  return v1^v2^v3; // slightly stronger since v0^v3 in above cancels out final round operation? I tweeted at the authors of SipHash about this but they didn't reply
#endif
}

dn_hash_t dn_hash_bytes(const void *p, size_t len) {
  return hash_siphash_bytes(p, len, DN_HASH_SEED);
}

dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed) {
  return hash_siphash_bytes(p,len,seed);
}

dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b) {
  return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

dn_hash_t dn_hash_cstr_dumb(const char* str) {
  return dn_hash_str_dumb(dn_string_cstr(str));
}

dn_hash_t dn_hash_str_dumb(dn_string_t str) {
  const size_t prime = 31;

  size_t result = 0;
  for (int i = 0; i < str.len; i++) {
    result = str.data[i] + (result * prime);
  }
    return result;
}

dn_hash_t dn_hash_string(dn_string_t str) {
  return dn_hash_bytes(str.data, str.len);
}

void dn_hash_encode_hex(char* destination, const char* data, size_t len) {
  static char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  for (int i = 0; i < len; i++) {
    char const byte = data[i];

    int base_index = i * 2;
    destination[base_index] = hex_chars[(byte & 0xF0) >> 4];
    destination[base_index + 1]     = hex_chars[(byte & 0x0F) >> 0];
  }
}

void dn_hash_encode_base64(char* destination, const char* source, size_t len) {
  static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  u32 val = 0;
  int valb = -6;

  int i = 0;
  for (; i < len; ++i) {
    val = (val << 8) + source[i];
    valb += 8;
    while (valb >= 0) {
      destination[i] = base64_chars[(val >> valb) & 0x3F];
      valb -= 6;
    }
  }
  if (valb > -6) {
    destination[i] = base64_chars[((val << 8) >> (valb + 8)) & 0x3F];
  }

  while (i % 4) destination[i++] = '=';
}

////////////
// STRING //
////////////
char* dn_cstr_copy_n(const char* str, u32 length, dn_allocator_t* allocator) {
  if (!allocator) allocator = &dn_allocators.standard.allocator;

  u32 buffer_length = length + 1;
  char* copy = (char*)dn_allocator_alloc(allocator, buffer_length);
  dn_cstr_copy_to_n(str, length, copy, buffer_length);
  return copy;
}

char* dn_cstr_copy(const char* str, dn_allocator_t* allocator) {
  return dn_cstr_copy_n(str, dn_cstr_len(str), allocator);
}

char* dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator) {
  return dn_cstr_copy_n((const char*)str, length, allocator);
}

void dn_cstr_copy_to(const char* str, char* buffer, u32 buffer_length) {
  dn_cstr_copy_to_n(str, dn_cstr_len(str), buffer, buffer_length);
}

void dn_cstr_copy_to_n(const char* str, u32 length, char* buffer, u32 buffer_length) {
  if (!str) return;
  if (!buffer) return;
  if (!buffer_length) return;

  u32 copy_length = dn_min(length, buffer_length - 1);
  for (u32 i = 0; i < copy_length; i++) {
    buffer[i] = str[i];
  }
  buffer[copy_length] = '\0';
}

bool dn_cstr_equal(const char* a, const char* b) {
  u32 len_a = dn_cstr_len(a);
  u32 len_b = dn_cstr_len(b);
  if (len_a != len_b) return false;

  return dn_os_is_memory_equal(a, b, len_a);
}

u32 dn_cstr_len(const char* str) {
  u32 len = 0;
  if (str) {
    while (str[len]) len++;
  }

  return len;
}

char* dn_string_to_cstr(dn_string_t str) {
  return dn_string_to_cstr_ex(str, &dn_allocators.bump.allocator);
}

char* dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator) {
  return dn_cstr_copy_n((char*)str.data, str.len, allocator);
}

char* dn_string_to_double_null_terminated(dn_string_t str) {
  u8* buffer = (u8*)dn_allocator_alloc(&dn_allocators.bump.allocator, str.len + 2);
  dn_string_copy_to(str, buffer, str.len);
  return buffer;
}

dn_string_t dn_string_alloc(u32 capacity, dn_allocator_t* allocator) {
  return (dn_string_t){
    .len = 0,
    .data = (u8*)dn_allocator_alloc(allocator, capacity),
  };
}

bool dn_string_equal(dn_string_t a, dn_string_t b) {
  if (a.len != b.len) return false;

  return dn_os_is_memory_equal(a.data, b.data, a.len);
}

bool dn_string_equal_cstr(dn_string_t a, const char* b) {
  u32 len = dn_cstr_len(b);
  if (a.len != len) return false;

  return dn_os_is_memory_equal(a.data, b, len);
}

s32 dn_string_sort_kernel_alphabetical(const void* a, const void* b) {
  return dn_string_compare_alphabetical(*(dn_string_t*)a, *(dn_string_t*)b);
}

s32 dn_string_compare_alphabetical(dn_string_t a, dn_string_t b) {
  u32 i = 0;
  while (true) {
    if (i >= a.len && i >= b.len) return DN_QSORT_EQUAL;
    if (i >= a.len)               return DN_QSORT_A_FIRST;
    if (i >= b.len)               return DN_QSORT_B_FIRST;

    i++;
    
    if (a.data[i] == b.data[i]) continue;
    if (a.data[i] > b.data[i]) return DN_QSORT_B_FIRST;
    if (b.data[i] > a.data[i]) return DN_QSORT_A_FIRST;
  }

}

bool dn_string_valid(dn_string_t str) {
  return str.data != NULL;
}

dn_string_t dn_string_copy_cstr_n(const char* str, u32 length, dn_allocator_t* allocator) {
  dn_string_t copy;
  copy.len = dn_cstr_len(str);
  copy.data = (u8*)dn_allocator_alloc(allocator, copy.len);

  dn_os_memory_copy(str, copy.data, copy.len);
  return copy;
}

dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator) {
  dn_string_t copy;
  copy.len = dn_cstr_len(str);
  copy.data = (u8*)dn_allocator_alloc(allocator, copy.len);

  dn_os_memory_copy(str, copy.data, copy.len);
  return copy;
}

dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator) {
  dn_string_t copy = {
    .len = str.len,
    .data = (u8*)dn_allocator_alloc(allocator, str.len),
  };

  dn_os_memory_copy(str.data, copy.data, str.len);
  return copy;
}

void dn_string_copy_to_str(dn_string_t source, dn_string_t* dest, u32 capacity) {
  dest->len = dn_min(source.len, capacity);
  dn_os_memory_copy(source.data, dest->data, dest->len);
}

void dn_string_copy_to(dn_string_t str, u8* buffer, u32 capacity) {
  dn_os_memory_copy(str.data, buffer, dn_min(str.len, capacity));
}

void dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity) {
  if (builder->buffer.capacity > requested_capacity) return;

  builder->buffer.data = (u8*)dn_allocator_realloc(builder->allocator, builder->buffer.data, requested_capacity);
  builder->buffer.capacity = requested_capacity;
}

void dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str) {
  dn_string_builder_grow(builder, builder->buffer.count + str.len);
  dn_os_memory_copy(str.data, builder->buffer.data + builder->buffer.count, str.len);
  builder->buffer.count += str.len;
}

void dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str) {
  u32 len = dn_cstr_len(str);
  dn_string_builder_grow(builder, builder->buffer.count + len);
  dn_os_memory_copy(str, builder->buffer.data + builder->buffer.count, len);
  builder->buffer.count += len;
}

void dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...) {
  va_list args, len_args;
  va_start(args, fmt);
  va_copy(len_args, args);

  const char* format = dn_string_to_cstr(fmt);
  u32 fmt_len = vsnprintf(NULL, 0, format, len_args);
  u32 vsnprintf_len = fmt_len + 1;
  dn_string_builder_grow(builder, builder->buffer.count + vsnprintf_len); // vsnprintf() null terminates, so ensure there's enough room (even though we don't care)
  vsnprintf((char*)builder->buffer.data + builder->buffer.count, vsnprintf_len, format, args);
  builder->buffer.count += fmt_len;

  va_end(args);
  va_end(args);
}

dn_string_t dn_string_builder_write(dn_string_builder_t* builder) {
  dn_string_t string = {
    .data = (u8*)dn_allocator_alloc(builder->allocator, builder->buffer.count),
    .len = builder->buffer.count
  };

  dn_os_memory_copy(builder->buffer.data, string.data, builder->buffer.count);
  return string;
}

char* dn_string_builder_write_cstr(dn_string_builder_t* builder) {
  return dn_cstr_copy_n((char*)builder->buffer.data, builder->buffer.count, builder->allocator);
}

////////
// OS //
////////
bool dn_os_does_path_exist(dn_string_t path) {
  dn_win32_dword_t attributes = GetFileAttributesA(dn_string_to_cstr(path));
  return (attributes != INVALID_FILE_ATTRIBUTES);
}

bool dn_os_is_regular_file(dn_string_t path) {
  dn_win32_dword_t attribute = GetFileAttributesA(dn_string_to_cstr(path));
  if (attribute == INVALID_FILE_ATTRIBUTES) return false;
  return !(attribute & FILE_ATTRIBUTE_DIRECTORY);
}

bool dn_os_is_directory(dn_string_t path) {
  dn_win32_dword_t attribute = GetFileAttributesA(dn_string_to_cstr(path));
  if (attribute == INVALID_FILE_ATTRIBUTES) return false;
  return attribute & FILE_ATTRIBUTE_DIRECTORY;
}

void dn_os_remove_directory(dn_string_t path) {
  SHFILEOPSTRUCTA file_op = dn_zero_initialize();
  file_op.wFunc = FO_DELETE;
  file_op.pFrom = dn_string_to_double_null_terminated(path);
  file_op.fFlags = FOF_NO_UI;
  SHFileOperationA(&file_op);
}

void dn_os_create_directory(dn_string_t path) {
  CreateDirectoryA(dn_string_to_cstr(path), NULL);
}

void dn_os_create_file(dn_string_t path) {
  dn_win32_handle_t handle = CreateFileA(dn_string_to_cstr(path), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  CloseHandle(handle);
}

void dn_os_remove_file(dn_string_t path) {
  DeleteFileA(dn_string_to_cstr(path));
}

dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path) {
  if (!dn_os_is_directory(path) || !dn_os_does_path_exist(path)) {
    return dn_zero_struct(dn_os_directory_entry_list_t);
  }

  dn_dynamic_array(dn_os_directory_entry_t) entries;
  dn_dynamic_array_init(&entries, sizeof(dn_os_directory_entry_t), &dn_allocators.bump.allocator);

  dn_string_builder_t builder = dn_tstring_builder();
  dn_string_builder_append_fmt(
    &builder,
    dn_string_literal("%.*s/*"),
    path.len, path.data
  );
  dn_string_t glob = dn_string_builder_write(&builder);

  dn_win32_find_data_t find_data;
  dn_win32_handle_t handle = FindFirstFile(dn_string_to_cstr(glob), &find_data);
  if (handle == INVALID_HANDLE_VALUE) {
    return dn_zero_struct(dn_os_directory_entry_list_t);
  }

  do {
    if (dn_cstr_equal(find_data.cFileName, ".")) continue;
    if (dn_cstr_equal(find_data.cFileName, "..")) continue;
    
    dn_string_builder_t builder = dn_tstring_builder();
    dn_string_builder_append(&builder, path);
    dn_string_builder_append(&builder, dn_string_literal("/"));
    dn_string_builder_append_cstr(&builder, find_data.cFileName);
    dn_string_t file_path = dn_string_builder_write(&builder);
    dn_os_normalize_path(file_path);

    dn_dynamic_array_push(&entries, &(dn_os_directory_entry_t) {
      .file_path = file_path,
      .file_name = dn_string_copy_cstr(find_data.cFileName, &dn_allocators.bump.allocator),
      .attributes = dn_os_winapi_attr_to_dn_attr(GetFileAttributesA(dn_string_to_cstr(file_path))),
    });
  } while (FindNextFile(handle, &find_data));

  FindClose(handle);

  return (dn_os_directory_entry_list_t) {
    .data = (dn_os_directory_entry_t*)entries.data,
    .count = entries.size
  };
}

dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path) {
  DN_BROKEN();
  dn_zero_return(dn_os_directory_entry_list_t);
}

dn_os_date_time_t dn_os_get_date_time() {
  DN_BROKEN();
  dn_zero_return(dn_os_date_time_t);
}

f64 dn_os_file_mod_time(dn_string_t file_path) {
  DN_BROKEN();
  return 0;
}

void dn_os_normalize_path(dn_string_t path) {
  for (u32 i = 0; i < path.len; i++) {
    if (path.data[i] == '\\') {
      path.data[i] = '/';
    }
  }

}

dn_string_t dn_os_parent_path(dn_string_t path, dn_allocator_t* allocator) {
  u8* c = path.data + path.len;
  while (true) {
    if (c == path.data) break;
    if (*c == '/') break;
    c--;
  }

  path.len = (c != path.data) ? (c - path.data) : 0;
  return dn_string_copy(path, allocator);
}

dn_string_t dn_os_path_extension(dn_string_t path) {
  if (!dn_os_is_regular_file(path)) {
    return dn_string_copy(dn_string_literal(""), &dn_allocators.bump.allocator);
  }

  u8* c = path.data + path.len;
  while (true) {
    if ((*c == '/') || (c == path.data)) {
      return dn_string_copy(dn_string_literal(""), &dn_allocators.bump.allocator);
    }

    if (*c == '.') break;
    c--;
  }

  dn_string_t extension = {
    .data = c,
    .len = (path.data + path.len) - (c)
  };
  return dn_string_copy(extension, &dn_allocators.bump.allocator);
}

dn_os_file_attr_t dn_os_winapi_attr_to_dn_attr(u32 attr) {
  u32 result = DN_OS_FILE_ATTR_NONE;
  if ( (attr & FILE_ATTRIBUTE_DIRECTORY)) result |= DN_OS_FILE_ATTR_DIRECTORY;
  if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) result |= DN_OS_FILE_ATTR_REGULAR_FILE;
  return (dn_os_file_attr_t)result;
}

bool dn_os_is_memory_equal(const void* a, const void* b, size_t len) {
    return !memcmp(a, b, len);
}

void dn_os_memory_copy(const void* source, void* dest, u32 num_bytes) {
    memcpy(dest, source, num_bytes);
}

void dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size) {
  u8* current_byte = (u8*)buffer;

  int i = 0;
  while (true) {
    if (i + fill_size > buffer_size) return;
    memcpy(current_byte + i, (u8*)fill, fill_size);
    i += fill_size;
  }
}

void dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill) {
  dn_os_fill_memory(buffer, buffer_size, &fill, sizeof(u8));
}

void dn_os_zero_memory(void* buffer, u32 buffer_size) {
  dn_os_fill_memory_u8(buffer, buffer_size, 0);
}

void dn_os_sleep_ms(f64 ms) {
  Sleep(ms);
}

u64 dn_os_time_since_epoch() {
  FILETIME file_time;
  ULARGE_INTEGER time;

  GetSystemTimeAsFileTime(&file_time);

  time.LowPart  = file_time.dwLowDateTime;
  time.HighPart = file_time.dwHighDateTime;

  u64 epoch_delta = 11644473600ULL * 10000000ULL;
  u64 time_100ns = time.QuadPart - epoch_delta;

  return time_100ns / 10000ULL;
}

////////////////////
// BUMP ALLOCATOR //
////////////////////
void dn_bump_allocator_init(dn_bump_allocator_t* allocator, u32 capacity) {
  allocator->buffer = (u8*)dn_allocator_alloc(&dn_allocators.standard.allocator, capacity);
  allocator->capacity = capacity;
  allocator->allocator.on_alloc = &dn_bump_allocator_on_alloc;
}

void dn_bump_allocator_clear(dn_bump_allocator_t* allocator) {
  memset(allocator->buffer, 0, allocator->bytes_used);
  allocator->bytes_used = 0;
}

void* dn_bump_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* old_memory) {
  dn_bump_allocator_t* bump = (dn_bump_allocator_t*)allocator;
  switch (mode) {
    case DN_ALLOCATOR_MODE_ALLOC: {
      if (bump->bytes_used + size > bump->capacity) {
        DN_LOG("overflow: bytes used = %d, requested size = %d", bump->bytes_used, size);
        DN_ASSERT(bump->bytes_used + size <= bump->capacity);
      }

      u8* memory_block = bump->buffer + bump->bytes_used;
      gs_hash_table_insert(bump->allocations, bump->bytes_used, size);
      bump->bytes_used += size;

      return memory_block;
    }
    case DN_ALLOCATOR_MODE_FREE: {
      return NULL;
    }
    case DN_ALLOCATOR_MODE_RESIZE: {
      if (!old_memory) {
        return allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_ALLOC, size, NULL);
      }

      u32 offset = (u32)((u8*)old_memory - (u8*)bump->buffer);
      u32 old_size = gs_hash_table_get(bump->allocations, offset);
      if (old_size >= size) {
        return old_memory;
      }

      void* memory_block = allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_ALLOC, size, NULL);
      dn_os_memory_copy(old_memory, memory_block, old_size);
      return memory_block;
    }
    default: {
      DN_UNREACHABLE();
      return NULL;
    }
  }
}

////////////////////////
// STANDARD ALLOCATOR //
////////////////////////
void dn_standard_allocator_init(dn_standard_allocator_t* allocator) {
  allocator->allocator.on_alloc = &dn_standard_allocator_on_alloc;
}

void* dn_standard_allocator_on_alloc(dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* old_memory) {
  if (mode == DN_ALLOCATOR_MODE_ALLOC) {
    return calloc(size, 1);
  }
  else if (mode == DN_ALLOCATOR_MODE_FREE) {
    free(old_memory);
    return NULL;
  }
  else if (mode == DN_ALLOCATOR_MODE_RESIZE) {
    return realloc(old_memory, size);
  }

  DN_UNREACHABLE();
  return NULL;
}

///////////////
// ALLOCATOR //
///////////////
void dn_allocator_add(const char* name, dn_allocator_t* allocator) {
  gs_hash_table_insert(dn_allocators.allocators, dn_hash_cstr_dumb(name), allocator);
}

dn_allocator_t* dn_allocator_find(const char* name) {
  return gs_hash_table_get(dn_allocators.allocators, dn_hash_cstr_dumb(name));
}

void* dn_allocator_alloc(dn_allocator_t* allocator, u32 size) {
  DN_ASSERT(allocator);
  return allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_ALLOC, size, NULL);
}

void* dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size) {
  DN_ASSERT(allocator);
  return allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_RESIZE, size, memory);
}

void dn_allocator_free(dn_allocator_t* allocator, void* buffer) {
  DN_ASSERT(allocator);
  DN_ASSERT(buffer);
  allocator->on_alloc(allocator, DN_ALLOCATOR_MODE_FREE, 0, buffer);
}

void dn_allocators_init() {
  dn_standard_allocator_init(&dn_allocators.standard);
  dn_bump_allocator_init(&dn_allocators.bump, 50 * 1024 * 1024);

  dn_allocator_add("bump", &dn_allocators.bump.allocator);
  dn_allocator_add("standard", &dn_allocators.standard.allocator);
}

void dn_allocators_update() {
  dn_bump_allocator_clear(&dn_allocators.bump);
}

///////////
// PATHS //
///////////
void dn_paths_add_ex(dn_string_t name, dn_string_t absolute_path) {
  dn_hash_t hash = dn_hash_string(name);
  
  if (gs_hash_table_exists(dn_paths.entries, hash)) {
    dn_string_t entry = gs_hash_table_get(dn_paths.entries, hash);

    dn_string_builder_t builder = dn_tstring_builder();
    dn_string_builder_append_fmt(
      &builder, 
      dn_string_literal("Tried to add named path, but name was already registered; name = %.*s, existing_path = %.*s, new_path = %.*s"), 
      name.len, name.data, 
      entry.len, entry.data,
      absolute_path.len, absolute_path.data
    );
    dn_log_builder(builder);
  }

  gs_hash_table_insert(dn_paths.entries, hash, dn_string_copy(absolute_path, &dn_allocators.standard.allocator));
}

void dn_paths_add_subpath(dn_string_t name, dn_string_t parent_name, dn_string_t relative_path) {
  dn_string_t parent_path = dn_paths_resolve(parent_name);
  dn_string_builder_t builder = dn_tstring_builder();
  dn_string_builder_append_fmt(
    &builder,
    dn_string_literal("%.*s/%.*s"),
    parent_path.len, parent_path.data,
    relative_path.len, relative_path.data
  );

  dn_paths_add_ex(name, dn_string_builder_write(&builder));
}

void dn_paths_add_install_subpath(dn_string_t name, dn_string_t relative_path) {
  dn_paths_add_subpath(name, dn_string_literal("dn_install"), relative_path);
}

void dn_paths_add_engine_subpath(dn_string_t name, dn_string_t relative_path) {
  dn_paths_add_subpath(name, dn_string_literal("dn_engine"), relative_path);
}

void dn_paths_add_app_subpath(dn_string_t name, dn_string_t relative_path) {
  dn_paths_add_subpath(name, dn_string_literal("dn_app"), relative_path);
}

void dn_paths_add_write_subpath(dn_string_t name, dn_string_t relative_path) {
  dn_paths_add_subpath(name, dn_string_literal("dn_write"), relative_path);
}

dn_string_t dn_paths_strip(dn_string_t name, dn_string_t absolute_path) {
  dn_string_t named_path = dn_paths_resolve_ex(name, &dn_allocators.bump.allocator);
  dn_string_t stripped_path = dn_string_copy(absolute_path, &dn_allocators.bump.allocator);
  
  u32 num_strip_chars = dn_min(named_path.len, absolute_path.len);
  u32 num_stripped = 0;
  for (u32 i = 0; i < num_strip_chars; i++) {
    if (stripped_path.data[i] != named_path.data[i]) break;
    num_stripped++;
  }

  return (dn_string_t) {
    .data = stripped_path.data + num_stripped,
    .len = absolute_path.len - num_stripped
  };
}

dn_string_t dn_paths_resolve(dn_string_t name) {
  return dn_paths_resolve_ex(name, &dn_allocators.bump.allocator);
}

dn_string_t dn_paths_resolve_cstr(const char* name) {
  return dn_paths_resolve_ex(dn_string_cstr(name), &dn_allocators.bump.allocator);
}

const char* dn_paths_resolve_bootstrap(const char* name) {
  dn_string_t result = dn_paths_resolve_ex(dn_string_cstr(name), &dn_allocators.bump.allocator);
  return result.data;
}

dn_string_t dn_paths_resolve_ex(dn_string_t name, dn_allocator_t* allocator) {
  dn_hash_t hash = dn_hash_string(name);
  
  if (!gs_hash_table_exists(dn_paths.entries, hash)) {
    dn_string_builder_t builder = dn_tstring_builder();
    dn_string_builder_append_fmt(
      &builder, 
      dn_string_literal("%s: named path was not found; name = %.*s"), __func__,
      name.len, name.data
    );
    dn_log_builder(builder);

    return dn_zero_struct(dn_string_t);
  }

  dn_string_t path = gs_hash_table_get(dn_paths.entries, hash);
  return dn_string_copy(path, allocator);   
}

dn_string_t dn_paths_resolve_format(dn_string_t name, dn_string_t file_name) {
  return dn_paths_resolve_format_ex(name, file_name, &dn_allocators.bump.allocator);
}

dn_string_t dn_paths_resolve_format_cstr(const char* name, const char* file_name) {
  return dn_paths_resolve_format_ex(dn_string_cstr(name), dn_string_cstr(file_name), &dn_allocators.bump.allocator);
}

dn_string_t dn_paths_resolve_format_ex(dn_string_t name, dn_string_t file_name, dn_allocator_t* allocator) {
  dn_hash_t hash = dn_hash_string(name);

  if (!gs_hash_table_exists(dn_paths.entries, hash)) {
    dn_string_builder_t builder = dn_tstring_builder();
    dn_string_builder_append_fmt(
      &builder, 
      dn_string_literal("%s: named format path was not found; name = %.*s"), __func__,
      name.len, name.data
    );
    dn_log_builder(builder);

    return dn_zero_struct(dn_string_t);
  }

  dn_string_t format_path = gs_hash_table_get(dn_paths.entries, hash);

  dn_string_builder_t builder = (dn_string_builder_t) { .allocator = allocator, .buffer = NULL };
  dn_string_builder_append_fmt(&builder, format_path, dn_string_to_cstr(file_name));
  return dn_string_builder_write(&builder);
}

dn_named_path_result_t dn_paths_find_all() {
  DN_BROKEN();
  return dn_zero_struct(dn_named_path_result_t);
}

dn_string_t _dn_paths_build_root_path(dn_string_t relative_path) {
  // Find the directory of the executable to use as a base
  dn_string_t executable_file = {
    .data = (u8*)dn_allocator_alloc(&dn_allocators.bump.allocator, DN_MAX_PATH_LEN),
    .len = 0
  };
  executable_file.len = GetModuleFileNameA(NULL, (LPSTR)executable_file.data, DN_MAX_PATH_LEN);
  dn_os_normalize_path(executable_file);

  // Find the parent directory (i.e. strip the executable filename from the path)
  dn_string_t executable_dir = dn_os_parent_path(executable_file, &dn_allocators.bump.allocator);

  dn_string_builder_t builder = dn_tstring_builder();
  dn_string_builder_append_fmt(
    &builder,
    dn_string_literal("%.*s/%.*s"),
    executable_dir.len, executable_dir.data,
    relative_path.len, relative_path.data
  );
  dn_string_t noncanonical_path = dn_string_builder_write(&builder);

  // Canonicalize it
  dn_string_t canonical_path = {
    .data = (u8*)dn_allocator_alloc(&dn_allocators.bump.allocator, DN_MAX_PATH_LEN),
    .len = 0
  };
  canonical_path.len = GetFullPathNameA((LPSTR)noncanonical_path.data, DN_MAX_PATH_LEN, (LPSTR)canonical_path.data, NULL);

  // Normalize it
  dn_os_normalize_path(canonical_path);
  
  return canonical_path;
}

void _dn_paths_set_install_roots(dn_path_config_t config) {
  dn_paths_add_ex(dn_string_literal("dn_executable"), _dn_paths_build_root_path(dn_string_literal("")));
  dn_paths_add_ex(dn_string_literal("dn_install"),    _dn_paths_build_root_path(config.install));
  dn_paths_add_ex(dn_string_literal("dn_engine"),     _dn_paths_build_root_path(config.engine));
}

void _dn_paths_set_write_path(dn_path_config_t config) { 
#if defined(DN_EDITOR)
  dn_paths_add_ex(dn_string_literal("dn_write"), _dn_paths_build_root_path(config.write));
#else
  dn_string_t appdata_dir = {
    .data = (u8*)dn_allocator_alloc(&dn_allocators.bump, DN_MAX_PATH_LEN),
    .len = 0
  };

  // In release mode, we have to write to an OS-approved directory. 
  SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, (LPSTR)appdata_dir.data);
  
  // Normalize
  dn_os_normalize_path(appdata_dir);
  dn_paths_add_ex(dn_string_literal("dn_write"), appdata_dir);
#endif
}

void dn_paths_init(dn_path_config_t config) {
  // Then, register these absolute paths and add some subpaths using hardcoded, well-known keys
  _dn_paths_set_install_roots(config);
  _dn_paths_set_write_path(config);

  dn_paths_add_install_subpath(dn_string_literal("dn_log"), dn_string_literal("doublenickel.log"));
  dn_paths_add_engine_subpath(dn_string_literal("dn_bootstrap"), dn_string_literal("source/scripts/core/bootstrap.lua"));
  dn_paths_add_engine_subpath(dn_string_literal("dn_paths"), dn_string_literal("source/scripts/data/paths.lua"));
  dn_paths_add_engine_subpath(dn_string_literal("dn_ffi_h"), dn_string_literal("source/scripts/data/dn_ffi.h"));
}

//////////
// TEST //
//////////
void dn_test_context_init(dn_test_context_t* context) {
  dn_dynamic_array_init(&context->assertions, sizeof(dn_assertion_t), &dn_allocators.bump.allocator);
}

void dn_test_suite_init(dn_test_suite_t* suite) {
  dn_dynamic_array_init(&suite->tests, sizeof(dn_test_suite_t), &dn_allocators.bump.allocator);
}

void dn_test_begin_suite(dn_test_suite_t* suite) {
  dn_test_suite_init(suite);
  dn_tests.suite = suite;
}

void dn_test_end_suite() {
  dn_test_suite_t* suite = dn_tests.suite;
  dn_log("%s", dn_string_to_cstr(suite->name));

  dn_for(i, dn_tests.suite->tests.size) {
    dn_test_context_t* context = (dn_test_context_t*)dn_dynamic_array_at(&dn_tests.suite->tests, i);

    bool found_success = false;
    bool found_failure = false;
    dn_for(i, context->assertions.size) {
      dn_assertion_t* assertion = (dn_assertion_t*)dn_dynamic_array_at(&context->assertions, i);
      found_success |= assertion->success;
      found_failure |= !assertion->success;
    }

    const char* test_color = DN_TEST_COLOR_SUCCESS;
    if (found_failure && !found_success) test_color = DN_TEST_COLOR_FAIL;
    if (!found_failure && found_success) test_color = DN_TEST_COLOR_SUCCESS;
    if (found_failure && found_success) test_color = DN_TEST_COLOR_MIXED;

    dn_log("  %s%s%s%s", DN_TEST_COLOR_BOLD, test_color, dn_string_to_cstr(context->name), DN_TEST_COLOR_RESET);

    dn_for(i, context->assertions.size) {
      dn_assertion_t* assertion = (dn_assertion_t*)dn_dynamic_array_at(&context->assertions, i);

      bool log_assertion =
        ((suite->flags & DN_TEST_CONTEXT_FLAG_LOG_SUCCESS) && assertion->success) ||
        ((suite->flags & DN_TEST_CONTEXT_FLAG_LOG_FAILURE) && !assertion->success) ||
        (suite->flags == DN_TEST_CONTEXT_FLAG_DEFAULT);
      const char* assertion_color = assertion->success ? DN_TEST_COLOR_SUCCESS : DN_TEST_COLOR_FAIL;
      if (log_assertion) {
        dn_log("  %s-> %s%s", assertion_color, dn_string_to_cstr(assertion->description),  DN_TEST_COLOR_RESET);
      }
    }
  }
}

void dn_test_begin(dn_string_t name) {
  dn_test_context_t* context = (dn_test_context_t*)dn_dynamic_array_reserve(&dn_tests.suite->tests, 1);
  context->name = name;
  dn_test_context_init(context);
  dn_tests.context = context;
}

void dn_test_end() {
}

void dn_pool_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_pool_t"),
    .flags = DN_TEST_CONTEXT_FLAG_DEFAULT
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_pool"));
    dn_pool(u32) arena = dn_zero_initialize();
    dn_pool_init(&arena, 32, sizeof(u32));
    dn_pool_handle_t rza = dn_pool_insert(&arena, dn_ptr_lval(u32, 69));
    dn_pool_handle_t gza = dn_pool_insert(&arena, dn_ptr_lval(u32, 420));
    dn_pool_handle_t bill = dn_pool_insert(&arena, dn_ptr_lval(u32, 7));

    dn_test_assert(*dn_pool_at(&arena, rza, u32) == 69);
    dn_test_assert(*dn_pool_at(&arena, gza, u32) == 420);
    dn_test_assert(*dn_pool_at(&arena, bill, u32) == 7);

    // Check that dn_pool_for() works
    {
      u32 values [] = { 69, 420, 7 };
      dn_pool_for(&arena, it) {
        u32* value = dn_pool_it(&arena, it, u32);
        dn_test_assert(*value = values[it.index]);
      }
    }

    // Remove an element at the end
    dn_pool_remove(&arena, bill);

    dn_pool_handle_t murray = dn_pool_insert(&arena, dn_ptr_lval(u32, 9001));
    dn_test_assert(!dn_pool_contains(&arena, bill));
    dn_test_assert(dn_pool_contains(&arena, murray));
    dn_test_assert(*dn_pool_at(&arena, murray, u32) == 9001);

    {
      u32 values [] = { 69, 420, 9001 };
      dn_pool_for(&arena, it) {
        u32* value = dn_pool_it(&arena, it, u32);
        dn_test_assert(*value = values[it.index]);
      }
    }

    // Remove an element in the middle
    dn_pool_remove(&arena, gza);

    {
      u32 values [] = { 69, 9001 };
      dn_pool_for(&arena, it) {
        u32* value = dn_pool_it(&arena, it, u32);
        dn_test_assert(*value = values[it.index]);
      }
    }

  dn_test_end();

  dn_test_end_suite();
}

void dn_string_test() {
  typedef struct {
    dn_string_t jerry;
    dn_string_t garcia;
    const char* jerry_cstr;
  } dn_strings_t;

  dn_strings_t data = {
    .jerry  = dn_string_literal("jerry garcia"),
    .garcia = dn_string_literal("jerry garcia"),
    .jerry_cstr = "jerry garcia",
  };

  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_string_t"),
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_string_literal"));
  dn_test_assert(data.jerry.len == strlen(data.jerry_cstr));
  dn_test_end();

  // u32                dn_cstr_len(const char* str);
  dn_test_begin(dn_string_literal("dn_cstr_len"));
  dn_test_assert(dn_cstr_len(data.jerry_cstr) == 12);
  dn_test_assert(dn_cstr_len("") == 0);
  dn_test_end();

  // bool               dn_cstr_equal(const char* a, const char* b);
  dn_test_begin(dn_string_literal("dn_cstr_equal"));
  dn_test_assert(dn_cstr_equal(data.jerry_cstr, data.jerry_cstr));
  dn_test_end();

  // DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
  dn_test_begin(dn_string_literal("dn_string_equal"));
  dn_test_assert(dn_string_equal(data.jerry, data.garcia));
  dn_test_end();

  // DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);
  dn_test_begin(dn_string_literal("dn_string_equal_cstr"));
  dn_test_assert(dn_string_equal_cstr(data.jerry, data.jerry_cstr));
  dn_test_end();

  // DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
  dn_test_begin(dn_string_literal("dn_string_copy"));
  dn_string_t jerry_from_dn = dn_string_copy(data.jerry,  &dn_allocators.bump.allocator);
  dn_test_assert(dn_string_equal(data.jerry, jerry_from_dn));
  dn_test_end();

  // DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
  dn_test_begin(dn_string_literal("dn_string_copy_cstr"));
  dn_string_t jerry_from_cstr = dn_string_copy_cstr(data.jerry_cstr,  &dn_allocators.bump.allocator);
  dn_test_assert(dn_string_equal(data.jerry, jerry_from_cstr));
  dn_test_end();

  // DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
  dn_test_begin(dn_string_literal("dn_string_to_cstr_ex"));
  char* jerry_cstr = dn_string_to_cstr_ex(data.jerry, &dn_allocators.bump.allocator);
  dn_test_assert(dn_cstr_len(jerry_cstr) == dn_cstr_len(data.jerry_cstr));
  dn_test_assert(dn_cstr_equal(jerry_cstr, data.jerry_cstr));
  dn_test_end();

  dn_test_end_suite();

  // DN_IMP char*       dn_cstr_copy(const char* str, u32 length, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy(const std::string& str, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator = nullptr);
  // DN_API void        dn_cstr_copy(const char* str, char* buffer, u32 buffer_length);
  // DN_API void        dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);
}

void dn_string_builder_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_string_t"),
  };
  dn_test_begin_suite(&suite);

  // DN_API void        dn_string_builder_grow(dn_string_builder_t* builder);
  dn_test_begin(dn_string_literal("dn_string_builder_grow"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_grow(&builder, 32);
    dn_test_assert(builder.buffer.capacity >= 32);
    dn_string_builder_grow(&builder, 8);
    dn_test_assert(builder.buffer.capacity >= 32);
    dn_string_builder_grow(&builder, 64);
    dn_test_assert(builder.buffer.capacity >= 64);
  }
  dn_test_end();

  // DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
  // DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
  // DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
  dn_test_begin(dn_string_literal("dn_string_builder_append"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_append(&builder, dn_string_literal("jerry"));
    dn_string_builder_append_cstr(&builder, "/");
    dn_string_builder_append(&builder, dn_string_literal("garcia"));
    dn_test_assert(dn_string_equal(dn_string_builder_write(&builder), dn_string_literal("jerry/garcia")));
  }
  dn_test_end();

  // DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
  dn_test_begin(dn_string_literal("dn_string_builder_write_cstr"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_append(&builder, dn_string_literal("jerry"));
    dn_test_assert(dn_cstr_equal(dn_string_builder_write_cstr(&builder), "jerry"));
  }
  dn_test_end();

  // DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
  dn_test_begin(dn_string_literal("dn_string_builder_write_fmt"));
  {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump.allocator };
    dn_string_builder_append_fmt(&builder, dn_string_literal("%d:%.2f:%s"), 69, 420.69, "blazeit");
    dn_test_assert(dn_string_equal(dn_string_builder_write(&builder), dn_string_literal("69:420.69:blazeit")));
  }
  dn_test_end();

  dn_test_end_suite();
}

void dn_color_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_color_t"),
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_color_rgb_to_hsv")); {
    dn_color_t rgb = dn_rgb_255(244, 211, 94);
    dn_color_t hsv = dn_color_rgb_to_hsv(rgb);
    dn_color_t hsv_expected = dn_rgb_255(47, 61, 96);
    dn_test_assert(dn_os_is_memory_equal(&hsv, &hsv_expected, sizeof(dn_color_t)));
  }
  dn_test_end();

  dn_test_end_suite();
}

void dn_test_init() {
  #ifdef DN_TEST_RUN_INTERNAL_TESTS
  dn_string_test();
  dn_string_builder_test();
  dn_pool_test();
  // dn_color_test();
  #endif
}


/////////
// LUA //
/////////
void dn_lua_init(dn_lua_config_t config) {
  // dn_string_t dn_install = dn_paths_resolve(dn_string_literal("dn_install"));
  // dn_string_builder_t builder = {
  //   .allocator = &dn_allocators.standard.allocator
  // };
  // dn_string_builder_append_fmt(
  //   &builder, 
  //   dn_string_literal("%.*s/%.*s"), 
  //   dn_install.len, dn_install.data, 
  //   config.scripts.len, config.scripts.data
  // );
  // dn_lua.scripts = dn_string_builder_write(&builder);
  dn_paths_add_ex(dn_string_literal("dn_app"), _dn_paths_build_root_path(config.scripts));
  dn_lua.scripts = dn_paths_resolve_ex(dn_string_literal("dn_app"), &dn_allocators.standard.allocator);

  s32 defer_pop = 0;

  dn_lua.state = luaL_newstate();
  luaL_openlibs(dn_lua.state);

  dn_fixed_array_init(&dn_lua.directories, DN_LUA_MAX_DIRECTORIES, sizeof(dn_string_t), &dn_allocators.standard.allocator);

  luaL_openlibs(dn_lua.state);

  // PHASE BOOTSTRAP:
  // Define the main table where all game data and functions go
  lua_newtable(dn_lua.state);
  lua_setglobal(dn_lua.state, "doublenickel");

  // Define a variable so we can ~ifdef in Lua
  lua_getglobal(dn_lua.state, "doublenickel");
  defer_pop++;
  lua_pushstring(dn_lua.state, "is_packaged_build");
#if defined(DN_EDITOR)
  lua_pushboolean(dn_lua.state, false);
#else
  lua_pushboolean(dn_lua.state, true);
#endif
  lua_settable(dn_lua.state, -3);

  // PHASE 0:
  // Load the bootstrap file, which creates tables for each module and loads the
  // most basic packages needed to load the rest of the code (e.g. classes and enums)
  dn_string_t bootstrap = dn_paths_resolve(dn_string_literal("dn_bootstrap"));
  dn_lua_script_file(bootstrap);

  lua_pushstring(dn_lua.state, "init_phase_0");
  lua_gettable(dn_lua.state, -2);
  s32 lresult = lua_pcall(dn_lua.state, 0, 0, 0);
  if (lresult) {
    const char* error = lua_tostring(dn_lua.state, -1);
    DN_LOG("error = %s", error);
    DN_BREAK();
  }
  
  // PHASE 1:
  // With the base tables created, we can now do things like define classes
  // and entity types. In this phase, load the core engine packages and then
  // any static engine data
  dn_error_t result;
  result = dn_lua_script_named_dir(dn_string_literal("dn_libs"));
  if (result)  DN_BREAK();
  result = dn_lua_script_named_dir(dn_string_literal("dn_core"));
  if (result)  DN_BREAK();
  result = dn_lua_script_named_dir(dn_string_literal("dn_editor"));
  if (result)  DN_BREAK();

  lua_pushstring(dn_lua.state, "init_phase_1");
  lua_gettable(dn_lua.state, -2);
  lresult = lua_pcall(dn_lua.state, 0, 0, 0);
  if (lresult) {
    const char* error = lua_tostring(dn_lua.state, -1);
    DN_LOG("error = %s", error);
    DN_BREAK();
  }

  // PHASE 2:
  // Lua itself has been initialized, and we've loaded in other assets our scripts
  // may use (shaders, fonts, etc). The last step is to load the game scripts and
  // configure the game itself through Lua
  result = dn_lua_script_named_dir(dn_string_literal("dn_components"));
  if (result)  DN_BREAK();
  result = dn_lua_script_named_dir(dn_string_literal("dn_editor"));
  if (result)  DN_BREAK();
  result = dn_lua_script_named_dir(dn_string_literal("dn_entities"));
  if (result)  DN_BREAK();
  result = dn_lua_script_named_dir(dn_string_literal("dn_app"));
  if (result)  DN_BREAK();
  
  lua_pushstring(dn_lua.state, "init_phase_2");
  lua_gettable(dn_lua.state, -2);
  lresult = lua_pcall(dn_lua.state, 0, 0, 0);
  if (lresult) {
    const char* error = lua_tostring(dn_lua.state, -1);
    DN_LOG("error = %s", error);
    DN_BREAK();
  }

  dn_for(it, defer_pop) {
    lua_pop(dn_lua.state, 1);
  }
}

void dn_lua_update() {
  dn_lua_interpreter_t l = dn_lua.state;
  u32 defer_pop = 0;
  
  lua_pushcfunction(l, &dn_lua_format_file_load_error_l);
  defer_pop++;
  
  lua_getglobal(l, "doublenickel");
  defer_pop++;
  lua_pushstring(l, "update_game");
  lua_gettable(l, -2);

  lua_pushnumber(l, dn_app.dt);

  u32 result = lua_pcall(dn_lua.state, 1, 0, -4);
  if (result) {
    const char* error = lua_tostring(dn_lua.state, -1);
    DN_LOG("error = %s", error);
    DN_BREAK();
  }

  dn_for(it, defer_pop) {
    lua_pop(dn_lua.state, 1);
  }
}

void dn_lua_pcall(dn_string_t fn) {
  lua_pushstring(dn_lua.state, dn_string_to_cstr(fn));
  lua_gettable(dn_lua.state, -2);
  
  bool result = lua_pcall(dn_lua.state, 0, 0, 0);
  if (result) {
    dn_string_builder_t builder = dn_tstring_builder();
    dn_string_builder_append_fmt(&builder, dn_string_literal("%.*s() failed; error = %s"),
      fn.len, fn.data,
      lua_tostring(dn_lua.state, -1)
    );

    dn_log_builder(builder);
    exit(0);
  }
}

dn_error_t dn_lua_script_file(dn_string_t file_path) {
  dn_string_t extension = dn_os_path_extension(file_path);
  if (!dn_string_equal(extension, dn_string_literal(".lua"))) {
    return DN_LUA_OK;
  }

  dn_lua_interpreter_t l = dn_lua.state;
  s32 initial_stack_size = lua_gettop(l);

  DN_LOG("%.*s", file_path.len, file_path.data);

  lua_pushcfunction(l, &dn_lua_format_file_load_error_l);

  const char* file_path_cstr = dn_string_to_cstr(file_path);
  bool result = luaL_loadfile(l, file_path_cstr);

  // In all error cases, do not return early.
  if (result) {
    // There's a syntax error in the file. Since loadfile doesn't call the
    // function we put on the stack, format the message manually.
    const char* unformatted_error = lua_tostring(l, -1);
    dn_log_format_str(dn_string_literal("%s: error loading %.*s\n%s"),
      __func__,
      file_path.len, file_path.data,
      dn_lua_format_file_load_error(unformatted_error)
    );

    lua_pop(l, 2);

    return DN_LUA_ERROR_FILE_LOAD_ERROR;
  }
  else {
    // The chunk compiled OK. Run it.
    result = lua_pcall(l, 0, 0, -2);

    if (result) {
      // There was a runtime error running the chunk.
      const char* unformatted_error = lua_tostring(l, -1);
      dn_log_format_str(dn_string_literal("%s: error running %.*s\n%s"),
        __func__,
        file_path.len, file_path.data,
        dn_lua_format_file_load_error(unformatted_error)
      );

      lua_pop(l, 2);

      return DN_LUA_ERROR_FILE_RUN_ERROR;
    }

    // The chunk loaded successfully!
    lua_pop(l, 1);
    return DN_LUA_OK;
  }

}

dn_error_t dn_lua_script_named_dir(dn_string_t name) {
  dn_string_t path = dn_paths_resolve(name);
  return dn_lua_script_dir(path);
}

dn_error_t dn_lua_script_dir(dn_string_t path) {
  if (!dn_os_does_path_exist(path)) return DN_LUA_ERROR_PATH_DOES_NOT_EXIST;
  if (!dn_os_is_directory(path)) return DN_LUA_ERROR_PATH_IS_NOT_DIRECTORY;

  dn_log_flags(DN_LOG_FLAG_FILE, "Loading scripts from directory; path = %.*s", path.len, path.data);

  // dn_lua.file_monitor->add_directory(path); @fix

  dn_os_directory_entry_list_t directory_entries = dn_os_scan_directory(path);
  qsort(directory_entries.data, directory_entries.count, sizeof(dn_os_directory_entry_t), dn_lua_directory_sort_kernel);

  dn_for(it, directory_entries.count) {
    dn_os_directory_entry_t* entry = directory_entries.data + it;
    dn_error_t result = DN_LUA_OK;
    
    if (entry->attributes & DN_OS_FILE_ATTR_REGULAR_FILE) {
      result = dn_lua_script_file(entry->file_path);
    }
    else if (entry->attributes & DN_OS_FILE_ATTR_DIRECTORY) {
      result = dn_lua_script_dir(entry->file_path);
    }

    if (result != DN_LUA_OK) {
      return result;
    }
  } 

  return DN_LUA_OK;
}

s32 dn_lua_directory_sort_kernel(const void* va, const void* vb) {
  dn_os_directory_entry_t* a = (dn_os_directory_entry_t*)va;
  dn_os_directory_entry_t* b = (dn_os_directory_entry_t*)vb;

  bool da = a->attributes & DN_OS_FILE_ATTR_DIRECTORY;
  bool db = b->attributes & DN_OS_FILE_ATTR_DIRECTORY;
  if (da && !db) return DN_QSORT_B_FIRST;
  if (db && !da) return DN_QSORT_A_FIRST;

  return dn_string_compare_alphabetical(a->file_path, b->file_path);
};

const char* dn_lua_format_file_load_error(const char* error) {
  static char buffer [2048];
  const char* fmt = "  %s";
  snprintf(&buffer[0], 2048, fmt, error);

  return &buffer[0];
}

s32 dn_lua_format_file_load_error_l(dn_lua_interpreter_t l) {
  const char* error = lua_tostring(dn_lua.state, 1);
  error = dn_lua_format_file_load_error(error);

  lua_pop(dn_lua.state, 1);
  lua_pushstring(dn_lua.state, error);
  return 1;
}


void dn_imgui_init() {
  DN_LOG_FN();
  dn_imgui = dn_zero_struct(dn_imgui_t);

  igCreateContext(NULL);

  ImGuiIO* io = igGetIO_Nil();
  io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io->ConfigWindowsMoveFromTitleBarOnly = true;
  igStyleColorsDark(NULL);

  io->IniFilename = NULL;

  ImGuiStyle* style = igGetStyle();
  style->FrameRounding = 6;
  style->WindowRounding = 6;
  style->WindowBorderSize = 0;

  dn_imgui_load_colors((dn_imgui_colors_t) {
    .light        = { .r = 0.47f, .g = 0.47f, .b = 0.47f, .a = 255.0f },
    .medium_light = { .r = 0.35f, .g = 0.35f, .b = 0.35f, .a = 255.0f },
    .low_light    = { .r = 0.31f, .g = 0.31f, .b = 0.31f, .a = 255.0f },
    .neutral      = { .r = 0.22f, .g = 0.22f, .b = 0.22f, .a = 255.0f },
    .medium_dark  = { .r = 0.16f, .g = 0.16f, .b = 0.16f, .a = 255.0f },
  });
  
  // Engine will pick this up on the first tick (before ImGui renders, so no flickering)
  dn_imgui_load_layout(dn_string_literal("default"));

  dn_os_create_directory(dn_paths_resolve(DN_USER_PATH_LAYOUTS));

  DN_LOG_2("Initializing ImGui backend");
  ImGui_ImplGlfw_InitForOpenGL(dn_app.window.handle, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void dn_imgui_update() {
  if (dn_string_valid(dn_imgui.queued_layout)) {
    igLoadIniSettingsFromDisk(dn_string_to_cstr(dn_imgui.queued_layout));
    dn_allocator_free(&dn_allocators.standard.allocator, dn_imgui.queued_layout.data);
    dn_imgui.queued_layout = dn_zero_struct(dn_string_t);
  }
  
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  igNewFrame();
}

void dn_imgui_shutdown() {
  DN_BROKEN();
}

void dn_imgui_load_colors(dn_imgui_colors_t colors) {
  DN_BROKEN();
}

void dn_imgui_load_layout(dn_string_t file_name) {
  DN_LOG("%.*s", file_name.len, file_name.data);

  if (dn_string_valid(dn_imgui.queued_layout)) {
    dn_allocator_free(&dn_allocators.standard.allocator, dn_imgui.queued_layout.data);
  }

  dn_imgui.queued_layout = dn_paths_resolve_format_ex(DN_PATH_LAYOUT, file_name, &dn_allocators.standard.allocator);
  if (!dn_os_does_path_exist(dn_imgui.queued_layout)) {
    dn_imgui.queued_layout = dn_paths_resolve_format_ex(DN_USER_PATH_LAYOUT, file_name, &dn_allocators.standard.allocator);
  }
}

void dn_imgui_save_layout(dn_string_t file_name) {
  dn_string_t file_path = dn_paths_resolve_format(DN_USER_PATH_LAYOUT, file_name);  
  igSaveIniSettingsToDisk(dn_string_to_cstr(file_path));
}


void dn_imgui_push_font(const char* font_name, u32 size) {
  DN_BROKEN();
}

void dn_imgui_image(const char* image, float sx, float sy) {
  DN_BROKEN();
}

void dn_imgui_file_browser_open() {
  DN_BROKEN();
}

void dn_imgui_file_browser_close() {
  DN_BROKEN();
}

void dn_imgui_file_browser_set_work_dir(const char* directory) {
  DN_BROKEN();
}

bool dn_imgui_file_browser_is_file_selected() {
  DN_BROKEN();
  return false;
}

dn_tstring_t dn_imgui_file_browser_get_selected_file() {
  DN_BROKEN();
  return "DN_BROKEN";
}


/////////
// GPU //
/////////
void dn_gl_error_callback(GLenum source, GLenum type, GLuint id,GLenum severity, GLsizei length,const GLchar *msg, const void *data) {
  u32 GL_DEBUG_SEVERITY_NOTHING_EVER = GL_DEBUG_SEVERITY_HIGH - 1;
  u32 minimum_severity = GL_DEBUG_SEVERITY_MEDIUM;
  
  if (severity > minimum_severity) return;

  const char* _source;
  const char* _type;
  const char* _severity;

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: {
      _severity = "HIGH";
      break;
    }

    case GL_DEBUG_SEVERITY_MEDIUM: {
      _severity = "MEDIUM";
      break;
    }

    case GL_DEBUG_SEVERITY_LOW: {
      _severity = "LOW";
      break;
    }

    default: {
      // It's a NOTIFICATION, which I will never ever care about
      return;
    }
  }

  switch (source) {
    case GL_DEBUG_SOURCE_API:             { _source = "API";             break; }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   { _source = "WINDOW SYSTEM";   break; }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: { _source = "SHADER COMPILER"; break; }
    case GL_DEBUG_SOURCE_THIRD_PARTY:     { _source = "THIRD PARTY";     break; }
    case GL_DEBUG_SOURCE_APPLICATION:     { _source = "APPLICATION";     break; }
    case GL_DEBUG_SOURCE_OTHER:           { _source = "UNKNOWN";         break; }
    default:                              { _source = "UNKNOWN";         break; }
  }

  switch (type) {
      case GL_DEBUG_TYPE_ERROR:
      _type = "ERROR";
      break;

      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      _type = "DEPRECATED BEHAVIOR";
      break;

      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      _type = "UDEFINED BEHAVIOR";
      break;

      case GL_DEBUG_TYPE_PORTABILITY:
      _type = "PORTABILITY";
      break;

      case GL_DEBUG_TYPE_PERFORMANCE:
      _type = "PERFORMANCE";
      break;

      case GL_DEBUG_TYPE_OTHER:
      _type = "OTHER";
      break;

      case GL_DEBUG_TYPE_MARKER:
      _type = "MARKER";
      break;

      default:
      _type = "UNKNOWN";
      break;
  }


  dn_log(
    "%d: %s of %s severity, raised from %s: %s\n",
    id, 
    _type, 
    _severity, 
    _source, 
    msg
  );
}

dn_coord_data_t dn_coord_get() {
	return dn_app.coords;
}

void dn_coord_set_camera(float x, float y) {
	dn_app.coords.camera = (dn_vector2_t) { .x = x, .y = y };
}

void dn_coord_set_framebuffer_position(float x, float y) {
	dn_app.coords.framebuffer_position = (dn_vector2_t) { .x = x, .y = y };
}

void dn_coord_set_framebuffer_size(float x, float y) {
	dn_app.coords.framebuffer_size = (dn_vector2_t) { .x = x, .y = y };
}

dn_vector2_t dn_coord_screen_to_window(float x, float y) {
	f32 framebuffer_bottom = dn_app.window.content_area.y - (dn_app.coords.framebuffer_position.y + dn_app.coords.framebuffer_size.y);
	return (dn_vector2_t) { 
		.x = ((x * dn_app.window.content_area.x) - dn_app.coords.framebuffer_position.x) / dn_app.coords.framebuffer_size.x,
		.y = ((y * dn_app.window.content_area.y) - framebuffer_bottom) / dn_app.coords.framebuffer_size.y
	};
}

dn_vector2_t dn_coord_screen_to_game(float x, float y) {
	dn_vector2_t win = dn_coord_screen_to_window(x, y);
	return dn_coord_window_to_game(win.x, win.y);
}

dn_vector2_t dn_coord_screen_to_world(float x, float y) {
	dn_vector2_t win = dn_coord_screen_to_window(x, y);
	dn_vector2_t game = dn_coord_window_to_game(win.x, win.y);
	return dn_coord_game_to_world(game.x, game.y);
}

dn_vector2_t dn_coord_window_to_screen(float x, float y) {
	return (dn_vector2_t) {
		.x = ((x * dn_app.coords.framebuffer_size.x) + dn_app.coords.framebuffer_position.x) / dn_app.window.content_area.x,
		.y = ((y * dn_app.coords.framebuffer_size.y) + dn_app.coords.framebuffer_position.y) / dn_app.window.content_area.y,
	};
}

dn_vector2_t dn_coord_window_to_game(float x, float y) {
	return (dn_vector2_t) {
		.x = x * dn_app.window.native_resolution.x,
		.y = y * dn_app.window.native_resolution.y
	};
}

dn_vector2_t dn_coord_window_to_world(float x, float y) {
	dn_vector2_t game = dn_coord_window_to_game(x, y);
	return dn_coord_game_to_world(game.x, game.y);
}

dn_vector2_t dn_coord_game_to_screen(float x, float y) {
	dn_vector2_t win = dn_coord_game_to_window(x, y);
	return dn_coord_window_to_screen(win.x, win.y);
}

dn_vector2_t dn_coord_game_to_window(float x, float y) {
	return (dn_vector2_t) {
		.x = x / dn_app.window.native_resolution.x,
		.y = y / dn_app.window.native_resolution.y,
	};
}

dn_vector2_t dn_coord_game_to_world(float x, float y) {
	return (dn_vector2_t) {
		.x = x + dn_app.coords.camera.x,
		.y = y + dn_app.coords.camera.y,
	};
}

dn_vector2_t dn_coord_world_to_screen(float x, float y) {
	dn_vector2_t game = dn_coord_world_to_game(x, y);
	dn_vector2_t win = dn_coord_game_to_window(game.x, game.y);
	return dn_coord_window_to_screen(win.x, win.y);
}

dn_vector2_t dn_coord_world_to_window(float x, float y) {
	dn_vector2_t game = dn_coord_world_to_game(x, y);
	return dn_coord_game_to_window(game.x, game.y);
}

dn_vector2_t dn_coord_world_to_game(float x, float y) {
	return (dn_vector2_t) {
		.x = x - dn_app.coords.camera.x,
		.y = y - dn_app.coords.camera.y,
	};
}
     
dn_vector2_t dn_coord_screen_to_window_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x * dn_app.window.content_area.x / dn_app.coords.framebuffer_size.x,
		.y = y * dn_app.window.content_area.y / dn_app.coords.framebuffer_size.y,
	};
}

dn_vector2_t dn_coord_screen_to_game_mag(float x, float y) {
	dn_vector2_t win = dn_coord_screen_to_window_mag(x, y);
	return (dn_vector2_t) {
		.x = win.x * dn_app.window.native_resolution.x,
		.y = win.y * dn_app.window.native_resolution.y,
	};
}

dn_vector2_t dn_coord_screen_to_world_mag(float x, float y) {
	dn_vector2_t win = dn_coord_screen_to_window_mag(x, y);
	dn_vector2_t game = dn_coord_window_to_game_mag(win.x, win.y);
	return dn_coord_game_to_world_mag(game.x, game.y);
}

dn_vector2_t dn_coord_window_to_screen_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x * dn_app.coords.framebuffer_size.x / dn_app.window.content_area.x,
		.y = y * dn_app.coords.framebuffer_size.y / dn_app.window.content_area.y,
	};
}

dn_vector2_t dn_coord_window_to_game_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x * dn_app.coords.framebuffer_size.x,
		.y = y * dn_app.coords.framebuffer_size.y,
	};
}

dn_vector2_t dn_coord_window_to_world_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x * dn_app.coords.framebuffer_size.x,
		.y = y * dn_app.coords.framebuffer_size.y,
	};
}

dn_vector2_t dn_coord_game_to_screen_mag(float x, float y) {
	dn_vector2_t w = dn_coord_game_to_window_mag(x, y);
	return dn_coord_window_to_screen_mag(w.x, w.y);
}

dn_vector2_t dn_coord_game_to_window_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x / dn_app.coords.framebuffer_size.x,
		.y = y / dn_app.coords.framebuffer_size.y,
	};
}

dn_vector2_t dn_coord_game_to_world_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x,
		.y = y
	};
}

dn_vector2_t dn_coord_world_to_screen_mag(float x, float y) {
	return dn_coord_game_to_screen_mag(x, y);
}

dn_vector2_t dn_coord_world_to_window_mag(float x, float y) {
	return dn_coord_game_to_window_mag(x, y);
}

dn_vector2_t dn_coord_world_to_game_mag(float x, float y) {
	return (dn_vector2_t) {
		.x = x, 
		.y = y
	};
}

///////////
// INPUT //
///////////
void dn_input_init() {
	dn_os_zero_memory(dn_app.input.shift_map, sizeof(dn_app.input.shift_map));
		
	dn_app.input.shift_map[' ']  =  ' ';
	dn_app.input.shift_map['\''] =  '"';
	dn_app.input.shift_map[',']  =  '<';
	dn_app.input.shift_map['-']  =  '_';
	dn_app.input.shift_map['.']  =  '>';
	dn_app.input.shift_map['/']  =  '?';

	dn_app.input.shift_map['0']  =  ')';
	dn_app.input.shift_map['1']  =  '!';
	dn_app.input.shift_map['2']  =  '@';
	dn_app.input.shift_map['3']  =  '#';
	dn_app.input.shift_map['4']  =  '$';
	dn_app.input.shift_map['5']  =  '%';
	dn_app.input.shift_map['6']  =  '^';
	dn_app.input.shift_map['7']  =  '&';
	dn_app.input.shift_map['8']  =  '*';
	dn_app.input.shift_map['9']  =  '(';

	dn_app.input.shift_map[';']  =  ':';
	dn_app.input.shift_map['=']  =  '+';
	dn_app.input.shift_map['[']  =  '{';
	dn_app.input.shift_map['\\'] =  '|';
	dn_app.input.shift_map[']']  =  '}';
	dn_app.input.shift_map['`']  =  '~';
		
	dn_app.input.shift_map['a']  =  'A';
	dn_app.input.shift_map['b']  =  'B';
	dn_app.input.shift_map['c']  =  'C';
	dn_app.input.shift_map['d']  =  'D';
	dn_app.input.shift_map['e']  =  'E';
	dn_app.input.shift_map['f']  =  'F';
	dn_app.input.shift_map['g']  =  'G';
	dn_app.input.shift_map['h']  =  'H';
	dn_app.input.shift_map['i']  =  'I';
	dn_app.input.shift_map['j']  =  'J';
	dn_app.input.shift_map['k']  =  'K';
	dn_app.input.shift_map['l']  =  'L';
	dn_app.input.shift_map['m']  =  'M';
	dn_app.input.shift_map['n']  =  'N';
	dn_app.input.shift_map['o']  =  'O';
	dn_app.input.shift_map['p']  =  'P';
	dn_app.input.shift_map['q']  =  'Q';
	dn_app.input.shift_map['r']  =  'R';
	dn_app.input.shift_map['s']  =  'S';
	dn_app.input.shift_map['t']  =  'T';
	dn_app.input.shift_map['u']  =  'U';
	dn_app.input.shift_map['v']  =  'V';
	dn_app.input.shift_map['w']  =  'W';
	dn_app.input.shift_map['x']  =  'X';
	dn_app.input.shift_map['y']  =  'Y';
	dn_app.input.shift_map['z']  =  'Z';
}

void dn_input_update() {
	// Reset between frames
	dn_for(key, GLFW_KEY_LAST) {
		dn_app.input.was_down[key] = dn_app.input.is_down[key];
	}

	dn_app.input.scroll.x = 0;
	dn_app.input.scroll.y = 0;
	dn_app.input.mouse_delta.x = 0;
	dn_app.input.mouse_delta.y = 0;
	dn_app.input.got_keyboard_input = false;
	dn_app.input.got_mouse_input = false;

	glfwPollEvents();
}

void dn_input_callback_cursor(GLFWwindow* glfw, double x, double y) {
	dn_app.input.got_mouse_input = true;
	
	if (x < 0) x = 0;
	if (y < 0) y = 0;

	dn_vector2_t last_mouse = dn_app.input.mouse;

	dn_app.input.mouse.x = x / dn_app.window.content_area.x;
	dn_app.input.mouse.y = 1 - (y / dn_app.window.content_area.y);

	// I'm not totally sure why this is the case, but these events need to be kind of debounced. That is,
	// one call to glfwPollEvents() might call this callback more than one times (in practice, two, but
	// who knows if that's a hard limit.
	//
	// In other words, we need to *accumulate* delta, and then at the beginning of the next frame the
	// input manager will reset the delta before it polls events
	dn_app.input.mouse_delta.x += dn_app.input.mouse.x - last_mouse.x;
	dn_app.input.mouse_delta.y += dn_app.input.mouse.y - last_mouse.y;
}

void dn_input_callback_click(GLFWwindow* window, int button, int action, int mods) {
	dn_app.input.got_mouse_input = true;

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			dn_app.input.is_down[GLFW_MOUSE_BUTTON_LEFT] = true;
		}
		if (action == GLFW_RELEASE) {
			dn_app.input.is_down[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			dn_app.input.is_down[GLFW_MOUSE_BUTTON_RIGHT] = true;
		}
		if (action == GLFW_RELEASE) {
			dn_app.input.is_down[GLFW_MOUSE_BUTTON_RIGHT] = false;
		}
	}
}

void dn_input_callback_scroll(GLFWwindow* window, double dx, double dy) {
	dn_app.input.got_mouse_input = true;

	dn_app.input.scroll.x = dx;
	dn_app.input.scroll.y = dy;
}

void dn_input_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	dn_app.input.got_keyboard_input = true;
	
	if (action == GLFW_PRESS) {
			dn_app.input.is_down[key] = true;
	}
	if (action == GLFW_RELEASE) {
			dn_app.input.is_down[key] = false;
	}
}

void dn_input_callback_error(int err, const char* msg) {
	DN_LOG("GLFW error: code = %d, message = %s", err, msg);
}

void dn_input_callback_window_size(GLFWwindow* window_handle, int width, int height) {
	DN_LOG("width = %d, height = %d", width, height);
	
	dn_app.window.content_area.x = width;
	dn_app.window.content_area.y = height;
	glViewport(0, 0, width, height);

	if (!dn_gpu.targets.size) {
		return;
	}

	dn_gpu_render_target_t* swapchain = dn_gpu_acquire_swapchain();
	swapchain->size = dn_app.window.content_area;
}


/////////////
// LUA API //
/////////////
bool dn_input_pressed(int key) {
	return dn_app.input.is_down[key] && !dn_app.input.was_down[key];
}

bool dn_input_released(int key) {
	return !dn_app.input.is_down[key] && dn_app.input.was_down[key];
}

bool dn_input_down(int key) {
	return dn_app.input.is_down[key];
}

bool dn_input_mod_down(int mod) {
	bool down = false;
	if (mod == GLFW_KEY_CONTROL) {
		down |= dn_app.input.is_down[GLFW_KEY_RIGHT_CONTROL];
		down |= dn_app.input.is_down[GLFW_KEY_LEFT_CONTROL];
	}
	if (mod == GLFW_KEY_SUPER) {
		down |= dn_app.input.is_down[GLFW_KEY_LEFT_SUPER];
		down |= dn_app.input.is_down[GLFW_KEY_RIGHT_SUPER];
	}
	if (mod == GLFW_KEY_SHIFT) {
		down |= dn_app.input.is_down[GLFW_KEY_LEFT_SHIFT];
		down |= dn_app.input.is_down[GLFW_KEY_RIGHT_SHIFT];
	}
	if (mod == GLFW_KEY_ALT) {
		down |= dn_app.input.is_down[GLFW_KEY_LEFT_ALT];
		down |= dn_app.input.is_down[GLFW_KEY_RIGHT_ALT];
	}

	return down;
}

bool dn_input_chord_pressed(int mod, int key) {
	return dn_input_mod_down(mod) && dn_input_pressed(key);
}

dn_vector2_t dn_input_mouse(dn_coord_t unit) {
	switch (unit) {
		case DN_COORD_UNIT_SCREEN: return dn_app.input.mouse; break;
		case DN_COORD_UNIT_WINDOW: return dn_coord_screen_to_window(dn_app.input.mouse.x, dn_app.input.mouse.y); break;
		case DN_COORD_UNIT_GAME:   return dn_coord_screen_to_game(dn_app.input.mouse.x, dn_app.input.mouse.y); break;
		case DN_COORD_UNIT_WORLD:  return dn_coord_screen_to_world(dn_app.input.mouse.x, dn_app.input.mouse.y); break;
	}

	DN_ASSERT(false);
	return dn_zero_struct(dn_vector2_t);
}

dn_vector2_t dn_input_mouse_delta(dn_coord_t unit) {
	switch (unit) {
		case DN_COORD_UNIT_SCREEN: return dn_app.input.mouse_delta; break;
		case DN_COORD_UNIT_WINDOW: return dn_coord_screen_to_window_mag(dn_app.input.mouse_delta.x, dn_app.input.mouse_delta.y); break;
		case DN_COORD_UNIT_GAME:   return dn_coord_screen_to_game_mag(dn_app.input.mouse_delta.x, dn_app.input.mouse_delta.y); break;
		case DN_COORD_UNIT_WORLD:  return dn_coord_screen_to_world_mag(dn_app.input.mouse_delta.x, dn_app.input.mouse_delta.y); break;
	}

	DN_ASSERT(false);
	return dn_zero_struct(dn_vector2_t);
}

dn_vector2_t dn_input_scroll() {
	return dn_app.input.scroll;
}

u32 dn_input_shift_key(u32 key) {
	bool upper = key >= GLFW_KEY_A && key <= GLFW_KEY_Z;
	bool shift = dn_input_mod_down(GLFW_KEY_SHIFT);

	if (shift && upper) {
    return key;
	}
	else if (shift && !upper) {
		return dn_app.input.shift_map[key];
	}
	else if (!shift && upper) {
		return key + 32;
	}
	else if (!shift && !upper) {
		return key;
	}

	return key;
}

dn_input_device_t dn_input_get_device() {
  DN_BROKEN();
  return 0;
}


////////////
// WINDOW //
////////////
void dn_window_shutdown() {
  glfwDestroyWindow(dn_app.window.handle);
  glfwTerminate();
}

dn_window_config_t dn_window_config_default() {
  return (dn_window_config_t) {
    .title = dn_string_literal("doublenickel"),
    .icon = dn_paths_resolve_format(dn_string_literal("dn_image"), dn_string_literal("logo/icon.png")),
    .display_mode = DN_DISPLAY_MODE_AUTO,
    .native_resolution = {
      .x = 1920,
      .y = 1080
    },
    .flags = (dn_window_flags_t)(DN_WINDOW_FLAG_BORDER | DN_WINDOW_FLAG_WINDOWED | DN_WINDOW_FLAG_VSYNC),
    .target_fps = 60
  };
}

void dn_window_init(dn_window_config_t config) {
  dn_window_set_native_resolution(config.native_resolution.x, config.native_resolution.y);

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

  GLFWmonitor*  monitor = glfwGetPrimaryMonitor();

  int wax, way, wsx, wsy;
  glfwGetMonitorWorkarea(monitor, &wax, &way, &wsx, &wsy);


  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Translate the window flags into GLFW stuff that'll set up the window correctly
  if (config.flags & DN_WINDOW_FLAG_WINDOWED) {
    monitor = NULL;
  }

  if (!(config.flags & DN_WINDOW_FLAG_BORDER)) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  }

  DN_LOG("Creating window; native_resolution = [%.0f, %.0f], content_area = [%.0f, %.0f], windowed = %d, border = %d, vsync = %d, refresh_rate = %d, monitor = %d",
           dn_app.window.native_resolution.x, dn_app.window.native_resolution.y,
           dn_app.window.content_area.x, dn_app.window.content_area.y,
           config.flags & DN_WINDOW_FLAG_WINDOWED,
           config.flags & DN_WINDOW_FLAG_BORDER,
           config.flags & DN_WINDOW_FLAG_VSYNC,
           mode->refreshRate,
           monitor != NULL);

  // Init the window, give it a GL context, and load OpenGL. Don't bother passing in the real size here, because we're going to set it later.
  dn_app.window.handle = glfwCreateWindow(1, 1, dn_string_to_cstr(config.title), monitor, NULL);
  glfwMakeContextCurrent(dn_app.window.handle);

  // Initialize OpenGL
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  const char* version = glGetString(GL_VERSION);
  if (version) {
    dn_log("OpenGL version: %s", version);
  } 
  else {
    dn_log("Failed to get OpenGL version");
  }

  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(dn_gl_error_callback, 0);

  // This has to be done after context creation
  if (config.flags & DN_WINDOW_FLAG_VSYNC) {
    glfwSwapInterval(1);
  }
  else {
    glfwSwapInterval(0);
  }

  glfwSetCursorPosCallback(dn_app.window.handle, dn_input_callback_cursor);
  glfwSetMouseButtonCallback(dn_app.window.handle, dn_input_callback_click);
  glfwSetKeyCallback(dn_app.window.handle, dn_input_callback_key);
  glfwSetScrollCallback(dn_app.window.handle, dn_input_callback_scroll);
  glfwSetWindowSizeCallback(dn_app.window.handle, dn_input_callback_window_size);

  if (config.display_mode == DN_DISPLAY_MODE_AUTO) {
    // Set a best guess for our default output resolution based on the monitor
    if (mode->width == 3840) {
      dn_window_set_display_mode(DN_DISPLAY_MODE_2160);
    }
    else if (mode->width == 2560) {
      dn_window_set_display_mode(DN_DISPLAY_MODE_1440);
    }
    else if (mode->width == 1920) {
      dn_window_set_display_mode(DN_DISPLAY_MODE_1080);
    }
    else {
      dn_window_set_display_mode(DN_DISPLAY_MODE_1080);
    }
  }
  else {
    dn_window_set_display_mode(config.display_mode);
  }

  dn_window_set_icon(config.icon);
}

void dn_window_set_icon(dn_string_t path) {
  dn_log("%s: %s", __func__, dn_string_to_cstr(path));

  int width, height, channels;
  unsigned char* pixels = stbi_load(dn_string_to_cstr(path), &width, &height, &channels, STBI_rgb_alpha);
  if (!pixels) {
    dn_log("%s: Failed to load icon image", __func__);
    return;
  }

  GLFWimage icon;
  icon.width = width;
  icon.height = height;
  icon.pixels = pixels;

  glfwSetWindowIcon(dn_app.window.handle, 1, &icon);

  stbi_image_free(pixels);
}

void dn_window_set_native_resolution(float width, float height) {
  dn_app.window.native_resolution.x = width;
  dn_app.window.native_resolution.y = height;
}

void dn_window_set_display_mode(dn_display_mode_t mode) {
  dn_log("%s: mode = %d", __func__, mode);

  if (dn_app.window.display_mode == DN_DISPLAY_MODE_FULLSCREEN && mode != DN_DISPLAY_MODE_FULLSCREEN) {
    // Toggle back to windowed
    glfwSetWindowMonitor(dn_app.window.handle, NULL, dn_app.window.windowed_position.x, dn_app.window.windowed_position.y, dn_app.window.requested_area.x, dn_app.window.requested_area.y, GLFW_DONT_CARE);
  }

  dn_app.window.display_mode = mode;

  switch (dn_app.window.display_mode) {
    case DN_DISPLAY_MODE_FULLSCREEN: {
      GLFWmonitor* monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
      glfwGetWindowPos(dn_app.window.handle, &dn_app.window.windowed_position.x, &dn_app.window.windowed_position.y);
      glfwSetWindowMonitor(dn_app.window.handle, monitor, 0, 0, video_mode->width, video_mode->height, video_mode->refreshRate);

      return;
    } break;
    case DN_DISPLAY_MODE_180:      dn_app.window.requested_area = (dn_vector2_t) { .x = 320,  .y = 180  }; break;
    case DN_DISPLAY_MODE_480:      dn_app.window.requested_area = (dn_vector2_t) { .x = 854,  .y = 480  }; break;
    case DN_DISPLAY_MODE_720:      dn_app.window.requested_area = (dn_vector2_t) { .x = 1280, .y = 720  }; break;
    case DN_DISPLAY_MODE_1080:     dn_app.window.requested_area = (dn_vector2_t) { .x = 1920, .y = 1080 }; break;
    case DN_DISPLAY_MODE_1440:     dn_app.window.requested_area = (dn_vector2_t) { .x = 2560, .y = 1440 }; break;
    case DN_DISPLAY_MODE_2160:     dn_app.window.requested_area = (dn_vector2_t) { .x = 3840, .y = 2160 }; break;
    case DN_DISPLAY_MODE_1280_800: dn_app.window.requested_area = (dn_vector2_t) { .x = 1280, .y = 800  }; break;
  }

  glfwSetWindowSize(dn_app.window.handle, dn_app.window.requested_area.x, dn_app.window.requested_area.y);
}

void dn_window_set_size(int x, int y) {
  glfwSetWindowSize(dn_app.window.handle, x, y);
}

dn_display_mode_t dn_window_get_display_mode() {
  return dn_app.window.display_mode;
}

void dn_window_set_cursor_visible(bool visible) {
  glfwSetInputMode(dn_app.window.handle, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

dn_vector2_t dn_window_get_native_resolution() {
  return dn_app.window.native_resolution;
}

dn_vector2_t dn_window_get_content_area() {
  return dn_app.window.content_area;
}

///////////
// AUDIO //
///////////
dn_audio_config_t dn_audio_config_default() {
  return (dn_audio_config_t) {
    .dirs = NULL,
    .num_dirs = 0,
    .compressor = {
      .threshold = 0.5f,
      .ratio = 2.0f,
      .attack_time = 0.95f,
      .release_time = 1.0f,
      .enabled = true
    },
    .filter = {
      .mode = DN_AUDIO_FILTER_MODE_BUTTERWORTH,
      .enabled = false,
      .cutoff_frequency = 10000,
      .cutoff_alpha = 0,
      .a0 = 0.f, .a1 = 0.f, .a2 = 0.f, .b1 = 0.f, .b2 = 0.f,
      .input_history = { 0 },
      .output_history = { 0 },
    },
    .sample_frequency = 44100,
    .master_volume = 1.0f,
    .master_volume_mod = 1.0f,
  };
}

void dn_audio_init(dn_audio_config_t config) {
  dn_log("%s", __func__);

  dn_audio = (dn_audio_t) {
    .compressor = config.compressor,
    .filter = config.filter,
    .sample_frequency = config.sample_frequency,
    .master_volume = config.master_volume,
    .master_volume_mod = config.master_volume_mod,
    // .file_monitor = dn_file_monitors_add(),
    .sample_buffer = {0}
  };

  dn_pool_init(&dn_audio.sounds, DN_AUDIO_MAX_SOUNDS, sizeof(dn_audio_info_t));
  dn_pool_init(&dn_audio.instances, DN_AUDIO_MAX_INSTANCES, sizeof(dn_audio_instance_t));

  dn_low_pass_filter_set_cutoff(&dn_audio.filter, 10000);

}

void dn_audio_add_samples(dn_audio_instance_t* instance, int samples_requested, int offset) {
  DN_BROKEN();  
}
  
void dn_audio_update(f32* buffer, int frames_requested, int num_channels) {
  DN_BROKEN();
}

void dn_audio_shutdown() {
  saudio_shutdown();
}

bool dn_audio_is_any_playing() {
  DN_BROKEN();
  return false;
}

dn_audio_info_t* dn_audio_find_no_default(dn_string_t name) {
  DN_BROKEN();
  return NULL;
}

dn_audio_info_t* dn_audio_find(dn_string_t name) {
  DN_BROKEN();
  return NULL;
}

dn_audio_instance_handle_t dn_audio_reserve() {
  DN_BROKEN();
  return dn_zero_struct(dn_audio_instance_handle_t);
}

dn_audio_info_t* dn_audio_resolve(dn_audio_info_handle_t handle) {
  dn_mutex_lock(&dn_audio.mutex);

  if (!dn_pool_contains(&dn_audio.sounds, handle)) return NULL;
  return dn_pool_at(&dn_audio.sounds, handle, dn_audio_info_t);
}

dn_audio_instance_t* dn_audio_resolve_instance(dn_audio_instance_handle_t handle) {
  dn_mutex_lock(&dn_audio.mutex);

  if (!dn_pool_contains(&dn_audio.instances, handle)) return NULL;
  return dn_pool_at(&dn_audio.instances, handle, dn_audio_instance_t);
}

dn_audio_instance_handle_t dn_audio_play_sound_ex(dn_audio_info_t* sound, bool loop) { 
  DN_BROKEN();
  return dn_zero_struct(dn_audio_instance_handle_t);
}

void dn_audio_stop_ex(dn_audio_instance_t* active_sound) {
  dn_mutex_lock(&dn_audio.mutex);  

  if (!active_sound) return;
  
  active_sound->occupied = false;
  active_sound->generation++;

  dn_audio_stop_ex(dn_audio_resolve_instance(active_sound->next));
}

dn_audio_info_t* alloc_sound(const char* file_name) {
  DN_BROKEN();
  return NULL;
}

void dn_audio_load_dir(const char* path) {
  DN_BROKEN();
}

void dn_audio_load(const char* file_path, const char* file_name) {
  DN_BROKEN();
};


//////////////////////
// LOW PASS FILTER  //
//////////////////////
void dn_low_pass_filter_set_mode(dn_low_pass_filter_t* filter, dn_audio_filter_mode_t mode) {
  filter->mode = mode;
  dn_low_pass_filter_set_cutoff(filter, filter->cutoff_frequency);
}

void dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, float cutoff) {
  // The low pass filters are unstable at frequencies higher than the Nyquist frequency, so clamp. Add a little wiggle room
  // to make sure we're not close to it, because it sounds a little bad that high anyway.
  float nyquist = dn_audio.sample_frequency / 2.1;
  filter->cutoff_frequency = dn_min(cutoff, nyquist);

  // Butterworth filter
  float omega = 2.0f * 3.14159 * filter->cutoff_frequency / dn_audio.sample_frequency;
  float cos_omega = cos(omega);
  float sin_omega = sin(omega);
  float alpha = sin_omega / sqrt(2.0f); // Butterworth filter (sqrt(2) damping factor)

  filter->a0 = (1.0f - cos_omega) / 2.0f;
  filter->a1 = 1.0f - cos_omega;
  filter->a2 = (1.0f - cos_omega) / 2.0f;
  filter->b1 = -2.0f * cos_omega;
  filter->b2 = 1.0f - alpha;

  float a0_inv = 1.0f / (1.0f + alpha);
  filter->a0 *= a0_inv;
  filter->a1 *= a0_inv;
  filter->a2 *= a0_inv;
  filter->b1 *= a0_inv;
  filter->b2 *= a0_inv;

  // Simple first order low pass filter
  filter->cutoff_alpha = 2.0f * 3.14159 * filter->cutoff_frequency / (dn_audio.sample_frequency + 2.0f * 3.14159 * filter->cutoff_frequency);
}

float dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, float input) {
  if (!filter->enabled) return input;
  
  switch (filter->mode) {
    case DN_AUDIO_FILTER_MODE_BUTTERWORTH: {
      float output = 0;
      output += (filter->a0 * input) + (filter->a1 * filter->input_history[0]) + (filter->a2 * filter->input_history[1]);
      output -= (filter->b1 * filter->output_history[0]) + (filter->b2 * filter->output_history[1]);

      filter->input_history[1] = filter->input_history[0];
      filter->input_history[0] = input;

      filter->output_history[1] = filter->output_history[0];
      filter->output_history[0] = output;

      return output;
    } break;

    case DN_AUDIO_FILTER_MODE_FIRST_ORDER: {
      float output = 0;
      output += filter->cutoff_alpha * input;
      output += (1.0f - filter->cutoff_alpha) * filter->output_history[0];
    
      filter->output_history[0] = output;
    
      return output;
    } break;

    default: {
      DN_ASSERT(false);
      return input;
    }
  }
}


/////////////
// LUA API //
/////////////
dn_audio_instance_handle_t dn_audio_play_sound(const char* name) {
  DN_BROKEN();
  return dn_zero_struct(dn_audio_instance_handle_t);
}

dn_audio_instance_handle_t dn_audio_play_looped(const char* name) {
  DN_BROKEN();
  return dn_zero_struct(dn_audio_instance_handle_t);
}

void dn_audio_stop(dn_audio_instance_handle_t handle) {
  DN_BROKEN();
}

void dn_audio_stop_all() {
  DN_BROKEN();
}

void dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume) {
  DN_BROKEN();
}

void dn_audio_set_filter_mode(dn_audio_instance_handle_t handle, dn_audio_filter_mode_t mode) {
  DN_BROKEN();
}

void dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, float cutoff) {
  DN_BROKEN();
}

void dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled) {
  DN_BROKEN();
}

void dn_audio_set_compressor_threshold(float threshold) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.compressor.threshold = threshold;
}

void dn_audio_set_compressor_ratio(float ratio) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.compressor.ratio = ratio;
}

void dn_audio_set_compressor_attack(float attack) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.compressor.attack_time = attack;
}

void dn_audio_set_compressor_release(float release) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.compressor.release_time = release;
}

void dn_audio_set_sample_rate(float rate) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.sample_frequency = rate;
}

void dn_audio_set_master_volume(float volume) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.master_volume = volume;
}

void dn_audio_set_master_volume_mod(float volume_mod) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.master_volume_mod = volume_mod;
}

void dn_audio_set_master_filter_cutoff(float frequency) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_low_pass_filter_set_cutoff(&dn_audio.filter, frequency);
}

void dn_audio_set_master_filter_cutoff_enabled(bool enabled) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.filter.enabled = enabled;
}

void dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode) {
  dn_mutex_lock(&dn_audio.mutex);
  dn_audio.filter.mode = mode;
}

float dn_audio_get_master_filter_cutoff() {
  return dn_audio.filter.cutoff_frequency;
}

float dn_audio_get_master_volume() {
  return dn_audio.master_volume;
}

float dn_audio_get_master_volume_mod() {
  return dn_audio.master_volume_mod;
}

bool dn_audio_is_playing(dn_audio_instance_handle_t handle) {
  DN_BROKEN();
  return false;
}

void dn_audio_pause(dn_audio_instance_handle_t handle) {
  DN_BROKEN();
}

void dn_audio_resume(dn_audio_instance_handle_t handle) {
  DN_BROKEN();
}

void dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next) {
  DN_BROKEN();
}


//////////////////
// PREPROCESSOR //
//////////////////
void dn_preprocess(dn_preprocessor_context_t* context) {
  context->result = NULL;

  context->file = fopen(context->file_path, "rb");
  if (!context->file) {
    snprintf(context->error, 256, "Failed to open file for preprocessing; file = %s", context->file_path);
    return;
  }

  fseek(context->file, 0, SEEK_END);
  context->file_data.len = ftell(context->file);
  context->file_data.data = (u8*)dn_allocator_alloc(&dn_allocators.bump.allocator, context->file_data.len + 1);
  char* file_data = (char*)context->file_data.data;

  fseek(context->file, 0, SEEK_SET);
  fread(file_data, 1, context->file_data.len, context->file);
  fclose(context->file);

  char* temp = (char*)dn_allocator_alloc(&dn_allocators.bump.allocator, 4096);

  char* str = (char*)context->file_data.data;
  include_info *inc_list;
  u32 num_includes = stb_include_find_includes(str, &inc_list);
  size_t source_len = context->file_data.len;
  size_t textlen=0, last=0;
  for (u32 i = 0; i < num_includes; ++i) {
    context->result = stb_include_append(context->result, &textlen, str+last, inc_list[i].offset - last);

    if (inc_list[i].filename) {
      char* included_file_data = NULL;
      for (u32 dir_index = 0; dir_index < context->include_dirs.count; dir_index++) {
        dn_string_builder_t builder = dn_tstring_builder();
        dn_string_builder_append_cstr(&builder, context->include_dirs.data[dir_index]);
        dn_string_builder_append_cstr(&builder, "/");
        dn_string_builder_append_cstr(&builder, inc_list[i].filename);

        const char* file_path = dn_string_builder_write_cstr(&builder);
        FILE* file = fopen(file_path, "rb");
        if (file) {
          fclose(file);
          dn_preprocessor_context_t subcontext = {
            .file_path = file_path,
            .include_dirs = context->include_dirs
          };
          dn_preprocess(&subcontext);
          included_file_data = subcontext.result;
          break;
        }
      }

      if (!included_file_data) {
        snprintf(context->error, 256, "Failed to find include; file = %s", inc_list[i].filename);
        stb_include_free_includes(inc_list, num_includes);
        return;
      }

      context->result = stb_include_append(context->result, &textlen, included_file_data, strlen(included_file_data));
      free(included_file_data);
    }
    
    last = inc_list[i].end;
  }
  context->result = stb_include_append(context->result, &textlen, str+last, source_len - last + 1); // append '\0'
  stb_include_free_includes(inc_list, num_includes);
}


/////////
// APP //
/////////
void dn_update() {
  dn_allocators_update();
}

void dn_init(dn_config_t config) {
  dn_allocators_init();
  dn_log_init();
  dn_random_init();
  dn_paths_init(config.path);
  dn_time_metrics_init();
  // dn_file_monitors_init();
  dn_test_init();
  dn_lua_init(config.lua);
}

void dn_app_configure(dn_app_config_t config) {
  dn_log("%s", __func__);

  dn_window_init(config.window);
  dn_asset_registry_init(config.asset);
  dn_images_init(config.image);
  dn_audio_init(config.audio);
  dn_backgrounds_init();
  dn_screenshots_init();
  dn_atlases_init();
  dn_backgrounds_init();
  dn_gpu_init(config.gpu);
  dn_font_init(config.font);
  dn_noise_init();
  dn_steam_init(config.steam);

  dn_imgui_init();

  // init_particles();
  // init_fluid();
}

void dn_app_update() {
  // dn_time_metric_begin("frame"); @fix

  // Clearly this is not representative of elapsed real time
  dn_app.elapsed_time += dn_app.dt;
  dn_app.frame++;
}

void dn_app_set_target_fps(f32 fps) {
  dn_app.target_fps = fps;
  dn_app.dt = 1.f / fps;
}

f32 dn_app_get_target_fps() {
  return dn_app.target_fps;
}

void dn_app_set_exit_game() {
  dn_app.exit_game = true;
}

const char* dn_app_get_game_hash() {
  return "DN_NO_HASH";
}

bool dn_app_should_exit() {
  bool done = false;
  done |= glfwWindowShouldClose(dn_app.window.handle);
  done |= dn_app.exit_game;
  return done;
}

dn_type_info_list_t dn_app_query_types() {
  dn_type_info_list_t list = dn_zero_initialize();

  static dn_type_info_t types [] = {
    // dn_type_info(dn_app_config_t),
    // dn_type_info(dn_app_descriptor_t),
    // dn_type_info(dn_asset_t),
    // dn_type_info(dn_asset_completion_status_t),
    // dn_type_info(dn_asset_config_t),
    // dn_type_info(dn_asset_data_t),
    // dn_type_info(dn_asset_importer_t),
    // dn_type_info(dn_asset_import_request_t),
    dn_type_info(dn_asset_name_t),
    // dn_type_info(dn_audio_config_t),
    // dn_type_info(dn_compressor_t),
    // dn_type_info(dn_coord_data_t),
    dn_type_info(dn_fixed_array_t),
    // dn_type_info(dn_font_config_t),
    // dn_type_info(dn_font_descriptor_t),
    // dn_type_info(dn_gen_arena_handle_t),
    dn_type_info(dn_gpu_backed_buffer_t),
    dn_type_info(dn_gpu_buffer_t),
    dn_type_info(dn_gpu_buffer_binding_t),
    dn_type_info(dn_gpu_buffer_descriptor_t),
    dn_type_info(dn_gpu_config_t),
    dn_type_info(dn_gpu_command_buffer_descriptor_t),
    dn_type_info(dn_gpu_draw_call_t),
    dn_type_info(dn_gpu_pipeline_descriptor_t),
    dn_type_info(dn_gpu_raster_state_t),
    dn_type_info(dn_gpu_render_pass_t),
    dn_type_info(dn_gpu_render_target_t),
    dn_type_info(dn_gpu_render_target_descriptor_t),
    dn_type_info(dn_gpu_renderer_state_t),
    dn_type_info(dn_gpu_scissor_state_t),
    dn_type_info(dn_gpu_shader_descriptor_t),
    dn_type_info(dn_gpu_storage_buffer_binding_t),
    dn_type_info(dn_gpu_uniform_t),
    dn_type_info(dn_gpu_uniform_binding_t),
    dn_type_info(dn_gpu_uniform_buffer_binding_t),
    dn_type_info(dn_gpu_uniform_descriptor_t),
    dn_type_info(dn_gpu_uniform_data_t),
    dn_type_info(dn_gpu_vertex_attr_info_t),
    dn_type_info(dn_gpu_vertex_buffer_binding_t),
    // dn_type_info(dn_low_pass_filter_t),    
    dn_type_info(dn_named_path_t),
    dn_type_info(dn_named_path_result_t),
    dn_type_info(dn_os_directory_entry_t),
    dn_type_info(dn_os_directory_entry_list_t),
    // dn_type_info(dn_prepared_text_t),
    dn_type_info(dn_sdf_circle_t),
    dn_type_info(dn_sdf_combine_entry_t),
    dn_type_info(dn_sdf_combine_header_t),
    dn_type_info(dn_sdf_combine_op_t),
    dn_type_info(dn_sdf_header_t),
    dn_type_info(dn_sdf_instance_t),
    dn_type_info(dn_sdf_oriented_box_t),
    dn_type_info(dn_sdf_renderer_t),
    dn_type_info(dn_sdf_renderer_state_t),
    dn_type_info(dn_sdf_ring_t),
    dn_type_info(dn_sdf_shape_t),
    dn_type_info(dn_sdf_smoothing_kernel_t),
    dn_type_info(dn_type_info_t),
    dn_type_info(dn_type_info_list_t),
    // dn_type_info(dn_window_config_t),
  };

  return (dn_type_info_list_t) {
    .data = types,
    .count = dn_arr_len(types)
  };
}

u32 dn_app_monotonic_id() {
  // return atomic_fetch_add(&dn_app.monotonic_id, 1);
  return 0;
}

bool dn_app_exceeded_frame_time() {
  return false; // @fix
  // auto& frame_timer = time_metrics["frame"];
  // auto now = glfwGetTime();
  // double elapsed = (now - frame_timer.time_begin);
  // return elapsed >= engine.dt;
}

int dn_main() {
  while(!dn_app_should_exit()) {
    dn_app_update();
    dn_allocators_update();
  //   dn_file_monitors_update();
  //   dn_assets_update();
    dn_imgui_update();
    dn_input_update();
  //   update_actions();
    dn_lua_update();
  //   dn_time_metrics_update();
  }

  // dn_audio_shutdown();
  // dn_steam_shutdown();
  // dn_imgui_shutdown();
  dn_window_shutdown();

  return 0;
}
#endif
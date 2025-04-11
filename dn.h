#include "stdint.h"
#include "assert.h"
#include <time.h>
#include <stdint.h>
// #include <sys/time.h> @fix
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>
#include <float.h>

#ifdef DN_APP
#define DN_LUA

// Platform includes
#define NOMINMAX
#include "windows.h"
#include "shlobj.h"

#include "KHR/khrplatform.h"

// Library includes
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "handmade/HandmadeMath.h"

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

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"

// #include "steam/steam_api.h"
#endif

//  ████████╗██╗   ██╗██████╗ ███████╗███████╗
//  ╚══██╔══╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔════╝
//     ██║    ╚████╔╝ ██████╔╝█████╗  ███████╗
//     ██║     ╚██╔╝  ██╔═══╝ ██╔══╝  ╚════██║
//     ██║      ██║   ██║     ███████╗███████║
//     ╚═╝      ╚═╝   ╚═╝     ╚══════╝╚══════╝
#ifndef DN_NO_HIJACK_NUMERIC_TYPES
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
#endif

#define DN_F64_MAX DBL_MAX

typedef u64 dn_hash_t;

#define DN_ASSET_NAME_LEN 64
typedef char dn_asset_name_t [DN_ASSET_NAME_LEN];

typedef HANDLE dn_win32_handle_t;
typedef DWORD dn_win32_dword_t;
typedef WIN32_FIND_DATA dn_win32_find_data_t;


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

#define DN_API __declspec(dllexport)
#define DN_IMP

#define dn_align(n) __declspec(align(n))
#define DN_ASSERT(condition) assert(condition)
#define DN_SOFT_ASSERT(condition) DN_ASSERT(condition) // For things you want to crash in development, but fix before release (as opposed to assers which are left in release builds and, say, logged)
#define DN_UNTESTED() DN_ASSERT(false)
#define DN_UNREACHABLE() DN_ASSERT(false)
#define DN_UNREACHABLE_MESSAGE(message) DN_ASSERT(false && (message))
// #define DN_BROKEN() DN_ASSERT(false)
#define DN_BROKEN() dn_log("DN_BROKEN(%s)", __func__)

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

#include "gunslinger/gs.h"

#define gs_hash_table_for(_HT, it) for (gs_hash_table_iter it = 0; gs_hash_table_iter_valid((_HT), it); gs_hash_table_iter_advance((_HT), it))


//  ███╗   ███╗ █████╗ ████████╗██╗  ██╗
//  ████╗ ████║██╔══██╗╚══██╔══╝██║  ██║
//  ██╔████╔██║███████║   ██║   ███████║
//  ██║╚██╔╝██║██╔══██║   ██║   ██╔══██║
//  ██║ ╚═╝ ██║██║  ██║   ██║   ██║  ██║
//  ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝
#ifdef DN_MATH_BACKEND_HANDMADE
#include "HandmadeMath.h"
typedef HMM_Vec2 dn_vector2_t;
typedef HMM_Vec3 dn_vector3_t;
typedef HMM_Vec4 dn_vector4_t;
typedef HMM_Mat4 dn_matrix4_t;
typedef HMM_Mat3 dn_matrix3_t;
typedef HMM_Mat2 dn_matrix2_t;
#else
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
#endif

typedef struct {
	s32 x;
	s32 y;
} dn_vector2i_t;

f32          dn_math_step(f32 edge, f32 x);
f32          dn_math_lerp(f32 a, f32 b, f32 t);
dn_vector4_t dn_math_lerp4(dn_vector4_t a, dn_vector4_t b, f32 t);
dn_vector4_t dn_math_abs4(dn_vector4_t v);
dn_vector4_t dn_math_fract4(dn_vector4_t v);
dn_vector4_t dn_math_clamp4(dn_vector4_t v, float minVal, float maxVal);


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

dn_color_t dn_color_rgb_to_hsv(dn_color_t color);
dn_color_t dn_color_hsv_to_rgb(dn_color_t color);


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
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int millisecond;
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
DN_API void                         dn_os_memory_copy(const void* source, void* dest, u32 num_bytes);
DN_API bool                         dn_os_is_memory_equal(const void* a, const void* b, size_t len);
DN_API void                         dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
DN_API void                         dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
DN_API void                         dn_os_zero_memory(void* buffer, u32 buffer_size);
DN_API void                         dn_os_sleep_ms(f64 ms);
DN_API u64                          dn_os_time_since_epoch();
DN_IMP dn_os_file_attr_t            dn_os_winapi_attr_to_dn_attr(u32 attr);

#define dn_os_arr_copy(source, dest) { static_assert(sizeof(source) == sizeof(dest), "dn_os_arr_copy expects two C arrays of the same size"); dn_os_memory_copy(source, dest, sizeof(dest)); }


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

DN_API void dn_fixed_array_init(dn_fixed_array_t* fixed_array, u32 max_vertices, u32 element_size, dn_allocator_t* allocator);
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

#define dn_pool(t) dn_pool_t

void             dn_pool_init(dn_pool_t* pool, u32 capacity, u32 element_size);
dn_pool_handle_t dn_pool_insert(dn_pool_t* pool, void* value);
dn_pool_handle_t dn_pool_reserve(dn_pool_t* pool);
void             dn_pool_remove(dn_pool_t* pool, dn_pool_handle_t handle);
bool             dn_pool_contains(dn_pool_t* pool, dn_pool_handle_t handle);
void             dn_pool_clear(dn_pool_t* pool);
dn_pool_handle_t dn_pool_invalid_handle();
bool             dn_pool_is_handle_valid(dn_pool_handle_t handle);
bool             dn_pool_slot_has_next_free(dn_pool_slot_t* slot);
bool             dn_pool_slot_is_match(dn_pool_slot_t* slot, dn_pool_handle_t handle);
#define dn_pool_at(T, POOL, HANDLE) ((T*)((POOL)->data + ((HANDLE).index * (POOL)->element_size)))

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

#define DN_LOGGER_MESSAGE_BUFFER_SIZE 4096
#define DN_LOGGER_PREAMBLE_BUFFER_SIZE 512
typedef struct {
	char message_buffer [DN_LOGGER_MESSAGE_BUFFER_SIZE];
	char preamble_buffer [DN_LOGGER_PREAMBLE_BUFFER_SIZE];
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
  dn_string_t install;
  dn_string_t engine;
  dn_string_t write;
} dn_path_config_t;

typedef struct {
  gs_hash_table(dn_hash_t, dn_string_t) entries;
} dn_paths_t;
dn_paths_t dn_paths;

DN_API dn_named_path_result_t dn_paths_find_all();
DN_API void                   dn_paths_add_install_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_write_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_engine_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_app_subpath(dn_string_t name, dn_string_t relative_path);
DN_API void                   dn_paths_add_subpath(dn_string_t name, dn_string_t parent_name, dn_string_t relative_path);
DN_API dn_string_t            dn_paths_resolve(dn_string_t name);
DN_API dn_string_t            dn_paths_resolve_format(dn_string_t name, dn_string_t file_name);
DN_API dn_string_t            dn_paths_resolve_cstr(const char* name);
DN_API dn_string_t            dn_paths_resolve_ex(dn_string_t name, dn_allocator_t* allocator);
DN_API dn_string_t            dn_paths_resolve_format_cstr(const char* name, const char* file_name);
DN_API dn_string_t            dn_paths_resolve_format_ex(dn_string_t name, dn_string_t file_name, dn_allocator_t* allocator);
DN_API dn_string_t            dn_paths_strip(dn_string_t name, dn_string_t absolute_path);
DN_API void                   dn_paths_add_ex(dn_string_t name, dn_string_t absolute_path);
DN_IMP void                   dn_paths_init();
DN_IMP dn_string_t            _dn_paths_build_root_path();
DN_IMP void                   _dn_paths_set_install_roots(dn_path_config_t config);
DN_IMP void                   _dn_paths_set_write_path(dn_path_config_t config);


// BACKGROUND
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
  // dn_array_t<Background> backgrounds;
} dn_backgrounds_t;

void dn_background_init(dn_background_t* background);
void dn_background_deinit(dn_background_t* background);
void dn_background_load_paths(dn_background_t* background);
void dn_background_set_source_image_size(dn_background_t* background, s32 width, i32 height);
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

#ifdef DN_APP
//  ██╗    ██╗██╗███╗   ██╗██████╗  ██████╗ ██╗    ██╗
//  ██║    ██║██║████╗  ██║██╔══██╗██╔═══██╗██║    ██║
//  ██║ █╗ ██║██║██╔██╗ ██║██║  ██║██║   ██║██║ █╗ ██║
//  ██║███╗██║██║██║╚██╗██║██║  ██║██║   ██║██║███╗██║
//  ╚███╔███╔╝██║██║ ╚████║██████╔╝╚██████╔╝╚███╔███╔╝
//   ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═════╝  ╚═════╝  ╚══╝╚══╝
typedef enum {
  DN_WINDOW_FLAG_NONE = 0,
  DN_WINDOW_FLAG_WINDOWED = 1,
  DN_WINDOW_FLAG_BORDER = 2,
  DN_WINDOW_FLAG_VSYNC = 4
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
} dn_window_config_t;

typedef struct {
  GLFWwindow* handle;
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


//  ███╗   ██╗██╗   ██╗██╗  ██╗██╗     ███████╗ █████╗ ██████╗
//  ████╗  ██║██║   ██║██║ ██╔╝██║     ██╔════╝██╔══██╗██╔══██╗
//  ██╔██╗ ██║██║   ██║█████╔╝ ██║     █████╗  ███████║██████╔╝
//  ██║╚██╗██║██║   ██║██╔═██╗ ██║     ██╔══╝  ██╔══██║██╔══██╗
//  ██║ ╚████║╚██████╔╝██║  ██╗███████╗███████╗██║  ██║██║  ██║
//  ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝
#ifdef DN_NUKLEAR
typedef struct nk_context nk_context;
typedef struct nk_color nk_color;
typedef struct nk_style_item nk_style_item;
typedef struct nk_style_button nk_style_button;

#define NK_RATIO(...) ((float []){ __VA_ARGS__ })

void          nk_dn_string(nk_context* nk, dn_string_t str, nk_flags flags);
void          nk_dn_string_wrap(nk_context* nk, dn_string_t str);
void          nk_dn_string_colored(nk_context* nk, dn_string_t str, nk_flags alignment, dn_color_t color);
void          nk_dn_string_wrap_colored(nk_context* nk, dn_string_t str, dn_color_t color);
void          nk_edit_dn_string(nk_context* nk, nk_flags flags, dn_string_t* buffer, u32 max_len, nk_plugin_filter filter);
bool          nk_selectable_dn_string(nk_context* nk, dn_string_t str, nk_flags flags, nk_bool* value);
bool          nk_button_dn_string(nk_context* nk, dn_string_t str);
bool          nk_button_dn_string_styled(nk_context* nk, nk_style_button* style, dn_string_t str);
nk_color      dn_color_to_nk_color(dn_color_t color);
nk_style_item dn_color_to_nk_style_item(dn_color_t color);
#endif

// IMGUI
#ifdef DN_IMGUI
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

DN_API void    dn_imgui_push_font(const char* font_name, u32 size);
DN_API void    dn_imgui_image(const char* image, float sx, float sy);
DN_API void    dn_imgui_file_browser_open();
DN_API void    dn_imgui_file_browser_close();
DN_API void    dn_imgui_file_browser_set_work_dir(const char* directory);
DN_API bool    dn_imgui_file_browser_is_file_selected();
DN_API dn_tstring_t dn_imgui_file_browser_get_selected_file();
DN_API void    dn_imgui_load_layout(const char* file_name);
DN_API void    dn_imgui_save_layout(const char* file_name);
DN_API void    dn_imgui_load_colors(dn_imgui_colors_t colors);
DN_IMP void    dn_imgui_init();
DN_IMP void    dn_imgui_shutdown();
DN_IMP void    dn_imgui_update();
#endif


//  ██╗     ██╗   ██╗ █████╗
//  ██║     ██║   ██║██╔══██╗
//  ██║     ██║   ██║███████║
//  ██║     ██║   ██║██╔══██║
//  ███████╗╚██████╔╝██║  ██║
//  ╚══════╝ ╚═════╝ ╚═╝  ╚═╝
#ifdef DN_LUA
#ifndef LUA_VERSION
	#error "Please include Lua before including dn.h. If you aren't using Lua, find where DN_LUA is being defined and fix it."
#endif

typedef lua_State* dn_lua_interpreter_t;

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
DN_IMP bool        dn_lua_script_file(dn_string_t file_path);
DN_IMP void        dn_lua_script_dir(dn_string_t path);
DN_IMP void        dn_lua_script_named_dir(dn_string_t name);
DN_IMP s32         dn_lua_directory_sort_kernel(const void* va, const void* vb);
DN_IMP const char* dn_lua_format_file_load_error(const char* error);
DN_IMP s32         dn_lua_format_file_load_error_l(dn_lua_interpreter_t lua);
#endif


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
  // dn_audio_config_t audio;
  // dn_font_config_t font;
  // dn_gpu_config_t gpu;
  // dn_asset_config_t asset;
  // dn_steam_config_t steam;
  // dn_image_config_t image;
  u32 target_fps;
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
  dn_backgrounds_t backgrounds;
  f32 target_fps;
  f32 dt;
  f32 elapsed_time;
  s32 frame;
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
DN_IMP void                dn_app_update();

#endif


//  ██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
//  ██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
//  ██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
//  ██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
//  ██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
//  ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

#ifdef DN_IMPLEMENTATION
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

dn_vector4_t dn_math_clamp4(dn_vector4_t v, float minVal, float maxVal) {
    dn_vector4_t result;
    result.x = fmaxf(minVal, fminf(v.x, maxVal));
    result.y = fmaxf(minVal, fminf(v.y, maxVal));
    result.z = fmaxf(minVal, fminf(v.z, maxVal));
    result.w = fmaxf(minVal, fminf(v.w, maxVal));
    return result;
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
	pool->slots[capacity - 1].next_free = -1;
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
  DN_BROKEN();
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

  dn_string_builder_t builder = dn_tstring_builder();
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

void dn_gen_arena_test() {
  dn_test_suite_t suite = {
    .name = dn_string_literal("dn_gen_arena_t"),
    .flags = DN_TEST_CONTEXT_FLAG_DEFAULT
  };
  dn_test_begin_suite(&suite);

  dn_test_begin(dn_string_literal("dn_gen_arena"));
    dn_pool(u32) arena = dn_zero_initialize();
    dn_pool_init(&arena, 32, sizeof(u32));
    dn_pool_handle_t rza = dn_pool_insert(&arena, dn_ptr_lval(u32, 69));
    dn_pool_handle_t gza = dn_pool_insert(&arena, dn_ptr_lval(u32, 420));
    dn_pool_handle_t bill = dn_pool_insert(&arena, dn_ptr_lval(u32, 7));

    dn_test_assert(*dn_pool_at(u32, &arena, rza) == 69);
    dn_test_assert(*dn_pool_at(u32, &arena, gza) == 420);
    dn_test_assert(*dn_pool_at(u32, &arena, bill) == 7);

    dn_pool_remove(&arena, bill);

    dn_pool_handle_t murray = dn_pool_insert(&arena, dn_ptr_lval(u32, 9001));
    dn_test_assert(!dn_pool_contains(&arena, bill));
    dn_test_assert(dn_pool_contains(&arena, murray));
    dn_test_assert(*dn_pool_at(u32, &arena, murray) == 9001);
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

void dn_test_run_internal() {
  dn_string_test();
  dn_string_builder_test();
  dn_gen_arena_test();
  // dn_color_test();
}

///////////////
// LIFECYCLE //
///////////////
void dn_init(dn_config_t config) {
  dn_allocators_init();
  dn_log_init();
  // dn_random_init();
  // dn_test_init();

  if (config.mode == DN_MODE_APP || config.mode == DN_MODE_CORE_LUA) {
    dn_paths_init(config.path);

    #ifdef DN_LUA
    dn_lua_init(config.lua);
    #endif
  }

}

void dn_update() {
  dn_allocators_update();
}

/////////
// LUA //
/////////
#ifdef DN_LUA
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
  s32 result = lua_pcall(dn_lua.state, 0, 0, 0);
  if (result) {
    const char* error = lua_tostring(dn_lua.state, -1);
    dn_log("init_phase_0(): error = %s", error);
    exit(0);
  }
  
  // PHASE 1:
  // With the base tables created, we can now do things like define classes
  // and entity types. In this phase, load the core engine packages and then
  // any static engine data
  dn_lua_script_named_dir(dn_string_literal("dn_libs"));
  dn_lua_script_named_dir(dn_string_literal("dn_core"));
  dn_lua_script_named_dir(dn_string_literal("dn_editor"));

  lua_pushstring(dn_lua.state, "init_phase_1");
  lua_gettable(dn_lua.state, -2);
  result = lua_pcall(dn_lua.state, 0, 0, 0);
  if (result) {
    const char* error = lua_tostring(dn_lua.state, -1);
    dn_log("init_phase_1(): error = %s", error);
    exit(0);
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

bool dn_lua_script_file(dn_string_t file_path) {
  dn_lua_interpreter_t l = dn_lua.state;
  s32 initial_stack_size = lua_gettop(l);

	dn_log_format_str(dn_string_literal("%s: scripting %.*s"), __func__, file_path.len, file_path.data);

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
    goto check_stack;
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
      goto check_stack;
    }

    // The chunk loaded successfully!
    lua_pop(l, 1);
    goto check_stack;
  }

 check_stack:
  s32 final_stack_size = lua_gettop(l);
  assert(initial_stack_size == final_stack_size);
  return !result;
}

void dn_lua_script_named_dir(dn_string_t name) {
  dn_string_t path = dn_paths_resolve(name);
  dn_lua_script_dir(path);
}

void dn_lua_script_dir(dn_string_t path) {
  if (!dn_os_does_path_exist(path)) return;
  if (!dn_os_is_directory(path)) return;

  dn_log_flags(DN_LOG_FLAG_FILE, "Loading scripts from directory; path = %.*s", path.len, path.data);

  // dn_lua.file_monitor->add_directory(path); @fix

  dn_os_directory_entry_list_t directory_entries = dn_os_scan_directory(path);
  qsort(directory_entries.data, directory_entries.count, sizeof(dn_os_directory_entry_t), dn_lua_directory_sort_kernel);

  dn_for(it, directory_entries.count) {
    dn_os_directory_entry_t* entry = directory_entries.data + it;
    if (entry->attributes & DN_OS_FILE_ATTR_REGULAR_FILE) {
      dn_lua_script_file(entry->file_path);
    }
    else if (entry->attributes & DN_OS_FILE_ATTR_DIRECTORY) {
      dn_lua_script_dir(entry->file_path);
    }
  } 
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
#endif

/////////////
// NUKLEAR //
/////////////
#ifdef DN_NUKLEAR
void nk_dn_string(struct nk_context* nk, dn_string_t str, nk_flags flags) {
  nk_text(nk, (char*)str.data, (s32)str.len, flags);
}

void nk_dn_string_wrap(nk_context* nk, dn_string_t str) {
  nk_text_wrap(nk, (char*)str.data, (s32)str.len);
}

void nk_dn_string_colored(struct nk_context* nk, dn_string_t str, nk_flags alignment, dn_color_t color) {
  nk_text_colored(nk, (char*)str.data, (s32)str.len, alignment, dn_color_to_nk_color(color));
}

void nk_dn_string_wrap_colored(nk_context* nk, dn_string_t str, dn_color_t color) {
  nk_text_wrap_colored(nk, (char*)str.data, (s32)str.len, dn_color_to_nk_color(color));
}

void nk_edit_dn_string(struct nk_context* nk, nk_flags flags, dn_string_t* buffer, u32 max_len, nk_plugin_filter filter) {
  nk_edit_string(nk, flags, (char*)buffer->data, (s32*)&buffer->len, max_len, filter);
}

bool nk_selectable_dn_string(nk_context* nk, dn_string_t str, nk_flags flags, nk_bool* value) {
  return nk_selectable_text(nk, (char*)str.data, (s32)str.len, flags, value);
}

bool nk_button_dn_string(nk_context* nk, dn_string_t str) {
  return nk_button_text(nk, (char*)str.data, (s32)str.len);
}

bool nk_button_dn_string_styled(nk_context* nk, nk_style_button* style, dn_string_t str) {
  return nk_button_text_styled(nk, style, (char*)str.data, (s32)str.len);
}

nk_color dn_color_to_nk_color(dn_color_t color) {
  return nk_rgba(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

nk_style_item dn_color_to_nk_style_item(dn_color_t color) {
  return nk_style_item_color(dn_color_to_nk_color(color));
}
#endif

#ifdef DN_IMGUI
void dn_imgui_init() {
  DN_BROKEN();
}

void dn_imgui_update() {
  DN_BROKEN();
}

void dn_imgui_shutdown() {
  DN_BROKEN();
}

void dn_imgui_load_colors(dn_imgui_colors_t colors) {
  DN_BROKEN();
}

void dn_imgui_load_layout(const char* file_name) {
  DN_BROKEN();
}

void dn_imgui_save_layout(const char* file_name) {
  DN_BROKEN();
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
}

dn_tstring_t dn_imgui_file_browser_get_selected_file() {
  DN_BROKEN();
  return dn_string_copy_cstr("DN_BROKEN");
}
#endif


#ifdef DN_APP
void dn_backgrounds_init() {
  DN_BROKEN();
}


void dn_background_init(dn_background_t* background) {
  DN_BROKEN();
}

void Background::deinit(dn_background_t* background) {
  DN_BROKEN();
}

void Background::load_paths(dn_background_t* background) {
	auto& lua = dn_lua;
	auto l = dn_lua.state;
		
	// Find the path to the source image, i.e. the full size background
	//lua.parse_string("source", &source_image);
	this->source_image_full_path = dn_paths_resolve_format_ex("dn_image", source_image, &dn_allocators.standard);

	// Find the folder we're going to write tiles to
	//lua.parse_string(-2, &tile_output_folder);
	this->tile_output_full_path = dn_paths_resolve_format_ex("dn_atlas", tile_output_folder, &dn_allocators.standard);
		
	// The name of the background is the folder we use for tiling
	name = tile_output_folder;
}

void Background::set_source_image_size(dn_background_t* background, i32 width, i32 height) {
	this->width = width;
	this->height = height;
}

void Background::set_source_data(dn_background_t* background, u32* data) {
	this->data = data;
}

bool Background::add_tile(dn_background_t* background) {
	assert(width > 0);
	assert(height > 0);
		
	// Calculate where the next tile will be
	if (!tile_positions.size) {
		// If we just started, it's just (0, 0)
		dn_array_push(&tile_positions);
	} else {
		// Otherwise, advance the previous tile one column, and move to the next row if needed.
		dn_vector2i_t last_tile_position = *dn_array_back(&tile_positions);
		dn_vector2i_t tile_position = { 0, 0 };
		tile_position.x = last_tile_position.x + Background::TILE_SIZE;
		tile_position.y = last_tile_position.y;
		if (tile_position.x >= width) {
			tile_position.x = 0;
			tile_position.y += Background::TILE_SIZE;
		}

		// If it falls outside of the image, we're done
		if (tile_position.y >= height) {
			return false;
		} else {
			dn_array_push(&tile_positions, tile_position);
		}
	}
		
		
	char** tile = dn_array_push(&tiles);
	*tile = dn::allocator::alloc<char>(&dn_allocators.standard, DN_MAX_PATH_LEN);
	snprintf(*tile, 256, "%s_%03lld.png", name, tiles.size);
		
	char** tile_full_path = dn_array_push(&tile_full_paths);
	*tile_full_path = dn::allocator::alloc<char>(&dn_allocators.standard, DN_MAX_PATH_LEN);
	snprintf(*tile_full_path, DN_MAX_PATH_LEN, "%s/%s", tile_output_full_path, *tile);

	return true;
}

void Background::add_tiles(dn_background_t* background) {
	while(add_tile()) {}
}

void Background::build_from_source(dn_background_t* background) {
	// Clean out old tiles 
	std::filesystem::remove_all(tile_output_full_path);
	std::filesystem::create_directories(tile_output_full_path);

	// Load the source image
	stbi_set_flip_vertically_on_load(false);
	u32* source_data = (u32*)stbi_load(source_image_full_path, &width, &height, &channels, 0);
	dn_defer { free(source_data); };

	assert(channels == 4);

	set_source_image_size(width, height);
	add_tiles();

	constexpr u32 nthreads = 16;
	TileProcessor::current_tile = 0;
	dn_array_t<TileProcessor> tile_processors;
	dn_array_init(&tile_processors, nthreads, TileProcessor());

	// Use a thread per-tile, up to the maximum number of threads in the pool.
	auto use_threads = dn_min(tiles.size, nthreads);
	for (u32 i = 0; i < use_threads; i++) {
		auto tile_processor = tile_processors[i];
		tile_processor->init(source_data);
		tile_processor->thread = std::thread(&TileProcessor::process, tile_processor, this);
	}
	
	for (u32 i = 0; i < use_threads; i++) {
		auto tile_processor = tile_processors[i];
		tile_processor->thread.join();
		tile_processor->deinit();
	}
}

void Background::update_config(dn_background_t* background) {
	lua_State* l = dn_lua.state;

	lua_getglobal(l, "doublenickel");
	DEFER_POP(l);
	lua_pushstring(l, "background");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, name);
	lua_gettable(l, -2);
	DEFER_POP(l);

	// Write out the new tile data to disk
	lua_pushstring(l, "mod_time");
	lua_pushnumber(l, filesystem_mod_time);
	lua_settable(l, -3);

	// Clear the old tile names from the config
	lua_pushstring(l, "tiles");
	lua_newtable(l);
	lua_settable(l, -3);

	// Write each file's name to the config
	for (u32 i = 0; i < tiles.size; i++) {
		auto tile_file_name = *tiles[i];
		lua_pushstring(l, "tiles");
		lua_gettable(l, -2);
		DEFER_POP(l);
		lua_pushnumber(l, i + 1);
		lua_pushstring(l, tile_file_name);
		lua_settable(l, -3);
	}

	// Write size
	lua_newtable(l);
	lua_pushstring(l, "x");
	lua_pushnumber(l, width);
	lua_settable(l, -3);

	lua_pushstring(l, "y");
	lua_pushnumber(l, height);
	lua_settable(l, -3);

	lua_pushstring(l, "size");
	lua_insert(l, -2);
	lua_settable(l, -3);
	
	// Write the file to disk
	lua_getglobal(l, "doublenickel");
	lua_pushstring(l, "write_file_to_return_table");
	lua_gettable(l, -2);

	auto background_info = dn_paths_resolve("background_info");
	lua_pushstring(l, background_info);

	lua_getglobal(l, "doublenickel");
	DEFER_POP(l);
	lua_pushstring(l, "background");
	lua_gettable(l, -2);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	lua_insert(l, -3);
	lua_pop(l, 2);

	lua_pushboolean(l, true);

	lua_pcall(l, 3, 0, 0);
}

void Background::load_tiles(dn_background_t* background) {
	// At this point, the background is correctly tiled (i.e. the tile images exist on disk, and are more current
	// than the source image). Load the tiles into the GPU.
	//
	// This is definitely slower than it needs to be; when we regenerate tiles, we already have the source data.
	// But instead of loading it directly into the GPU, we write it to a PNG and then reload it here. Why? Well,
	// laziness. It's probably smarter to have LoadTileFromMemory(), and just call that in both the regenerate
	// case + after we load them from disk in the cache case. But to be honest, I really don't care right
	// now!

	// Make a completion queue that's the exact size we need. (If you had a vector, this would be a great time
	// to use a vector that allocates from temporary storage)

	dn_array_init(&this->loaded_tiles, this->tiles.size, &dn_allocators.standard);

	for (int tile_index = 0; tile_index < tiles.size; tile_index++) {
		// Pull tile data
		auto tile = *tiles[tile_index];
		auto tile_full_path = *tile_full_paths[tile_index];

		// Allocate any buffer resources
		auto texture = alloc_texture();
		texture->hash = dn_hash_cstr_dumb(tile);
		stbi_set_flip_vertically_on_load(false);
		u32* data = (u32*)stbi_load(tile_full_path, &texture->width, &texture->height, &texture->channels, 0);
			
		// Create a sprite so the tile can be drawn with the draw_image() API
		auto sprite = alloc_sprite();
		strncpy(sprite->file_path, tile, DN_MAX_PATH_LEN);
		sprite->texture = texture->hash;
		sprite->hash = texture->hash;
		sprite->size = { texture->width, texture->height };

		// Each tile spans the entire image, so use trivial UVs
		dn_vector2_t uv [6] = dn_quad_literal(0, 1, 0, 1);
		for (u32 i = 0; i < 6; i++) sprite->uv[i] = uv[i];

		// Mark the data to be loaded to the GPU
		auto item = dn_array_push(&loaded_tiles);
		item->texture = texture;
		item->data = data;
	}
}

bool Background::is_dirty(dn_background_t* background) {
	if (filesystem_mod_time > mod_time) return true;
	if (!std::filesystem::exists(tile_output_full_path)) return true;

	return false;
}


void Background::load_one_to_gpu(dn_background_t* background) {
	auto tile = loaded_tiles[gpu_load_index++];
	tile->texture->load_to_gpu(tile->data);
	free(tile->data);

	if (gpu_load_index == loaded_tiles.size) {
		gpu_done = true;
	}
}

void Background::load_to_gpu(dn_background_t* background) {
	gpu_ready = true;
	while (!gpu_done) {
		load_one_to_gpu();
	}
}


//
// TILE PROCESSOR
//
void TileProcessor::init(u32* source_data) {
	if (!tile_data) {
		tile_data = dn::allocator::alloc<u32>(&dn_allocators.standard, Background::TILE_SIZE * Background::TILE_SIZE);
	}
	this->source_data = source_data;
}

void TileProcessor::deinit() {
	dn::allocator::free(&dn_allocators.standard, tile_data);
	tile_data = nullptr;
	source_data = nullptr;
}

void TileProcessor::process(Background* background) {
	while (current_tile < background->tiles.size) {
		// Atomically get the next tile index, and pull that tile's data. I believe this could be an atomic
		// instead of needing a full mutex, but it doesn't matter.
		mutex.lock();
		u32 tile = current_tile++;
		mutex.unlock();

		dn_vector2i_t source_position = *background->tile_positions[tile];
		char*    tile_path       = *background->tile_full_paths[tile];
		memset(tile_data, 0, Background::TILE_SIZE * Background::TILE_SIZE * sizeof(u32));
			
		// Blit a single tile to its own texture
		u32 tile_offset = 0;
		u32 source_offset = (background->width * source_position.y) + source_position.x;
		u32 cols = dn_min(Background::TILE_SIZE, background->width - source_position.x);
		u32 rows = dn_min(Background::TILE_SIZE, background->height - source_position.y);
		for (u32 row = 0; row < rows; row++) {
			memcpy(tile_data + tile_offset, source_data + source_offset, cols * sizeof(u32));
			source_offset += background->width;
			tile_offset += Background::TILE_SIZE;
		}

		// Write the tile image out to a PNG file
		stbi_write_png(tile_path, Background::TILE_SIZE, Background::TILE_SIZE, 4, tile_data, 0);
	}
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
    case GL_DEBUG_SOURCE_API:
    _source = "API";
    break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    _source = "WINDOW SYSTEM";
    break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
    _source = "SHADER COMPILER";
    break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
    _source = "THIRD PARTY";
    break;

    case GL_DEBUG_SOURCE_APPLICATION:
    _source = "APPLICATION";
    break;

    case GL_DEBUG_SOURCE_OTHER:
    _source = "UNKNOWN";
    break;

    default:
    _source = "UNKNOWN";
    break;
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

///////////
// INPUT //
///////////
void dn_input_callback_cursor(GLFWwindow* glfw, double x, double y) {

}

void dn_input_callback_click(GLFWwindow* window, int button, int action, int mods) {

}
void dn_input_callback_scroll(GLFWwindow* window, double dx, double dy) {

}

void dn_input_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
  dn_log("%c", key);
}

void dn_input_callback_error(int err, const char* msg) {

}

void dn_input_callback_window_size(GLFWwindow* window_handle, int width, int height) {

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
    .flags = (dn_window_flags_t)(DN_WINDOW_FLAG_BORDER | DN_WINDOW_FLAG_WINDOWED | DN_WINDOW_FLAG_VSYNC)
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

  dn_log("creating window: native_resolution = [%.0f, %.0f], content_area = [%.0f, %.0f], windowed = %d, border = %d, vsync = %d, refresh_rate = %d, monitor = %d",
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


#ifndef DN_ASSET_H
#define DN_ASSET_H
typedef enum {
  DN_ASSET_BUILTIN_BACKGROUND,
  DN_ASSET_BUILTIN_ATLAS,
} dn_asset_builtin_t;

typedef struct {
  i32 id;
  static i32 next_id;

  AssetKind kind;
  union {
    Background* background;
    TextureAtlas* atlas;
  };
} dn_asset_load_request_t;

s32 AssetLoadRequest::next_id = 0;

struct AssetLoader {
  std::thread thread;
  std::condition_variable condition;
  std::mutex mutex;

  dn_ring_buffer_t<AssetLoadRequest> load_requests;
  dn_ring_buffer_t<AssetLoadRequest> completion_queue;

  void process_requests();
  void process_completion_queue();
  void submit(AssetLoadRequest request);
};
AssetLoader asset_loader;

typedef struct {
  void* user_data;
} dn_asset_import_request_t;

typedef enum {
  DN_ASSET_COMPLETION_STATUS_RUNNING,
  DN_ASSET_COMPLETION_STATUS_DONE,
} dn_asset_completion_status_t;

dn_typedef_fn(void,                         dn_asset_import_fn,     dn_asset_import_request_t* request);
dn_typedef_fn(dn_asset_completion_status_t, dn_asset_completion_fn, dn_asset_import_request_t* request);

typedef struct {
  dn_asset_name_t id;
  dn_asset_import_fn on_import;
  dn_asset_completion_fn on_complete;
} dn_asset_importer_t;

typedef void* dn_asset_data_t;

typedef struct {
    dn_asset_name_t name;
    dn_asset_data_t data;
} dn_asset_t;

typedef struct {
  struct {
    dn_asset_importer_t* data;
    u32 count;
  } importers;
} dn_asset_config_t;

//////////////
// INTERNAL //
//////////////
typedef enum {
  DN_BACKGROUND_COMPLETION_STATUS_UPDATE_CONFIG,
  DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES,
  DN_BACKGROUND_COMPLETION_STATUS_DONE,
} dn_background_completion_status_t;

typedef struct {
  dn_background_completion_status_t status;
  Background* background;
} dn_background_import_request_t;

typedef struct {
  std::thread thread;
  std::condition_variable condition;
  std::mutex mutex;

  std::unordered_map<std::string, dn_asset_t> assets;
  std::unordered_map<std::string, dn_asset_importer_t> importers;

  dn_ring_buffer_t<AssetLoadRequest> load_requests;
  dn_ring_buffer_t<AssetLoadRequest> completion_queue;
} dn_assets_t;
dn_assets_t dn_assets;


DN_API void dn_assets_init(dn_asset_config_t config);
DN_API dn_asset_data_t dn_assets_find(const char* name);
DN_API void dn_assets_add(const char* name, dn_asset_data_t data);
DN_API void dn_asset_copy_name(const char* source, dn_asset_name_t dest);

DN_IMP void dn_assets_update();
DN_IMP void dn_background_import(dn_asset_import_request_t* request);
DN_IMP dn_asset_completion_status_t dn_background_complete(dn_asset_import_request_t* request);
#endif

#ifdef DN_ASSET_IMPLEMENTATION
void dn_background_import(dn_asset_import_request_t* request) {
    auto background_request = (dn_background_import_request_t*)request->user_data;
    auto background = background_request->background;
    if (background->need_async_build) {
        background->build_from_source();
    }
    if (background->need_async_load) {
        background->load_tiles();
    }
}

dn_asset_completion_status_t dn_background_complete(dn_asset_import_request_t* request) {
    auto background_request = (dn_background_import_request_t*)request->user_data;
    auto background = background_request->background;

    switch (background_request->status) {
        case DN_BACKGROUND_COMPLETION_STATUS_UPDATE_CONFIG: {
            if (background->is_dirty()) {
               background->update_config();
            }

            background_request->status = DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES;
            return DN_ASSET_COMPLETION_STATUS_RUNNING;
        } break;

        case DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES: {
            background->load_one_to_gpu();
            if (background->gpu_done) {
                background_request->status = DN_BACKGROUND_COMPLETION_STATUS_DONE;
                return DN_ASSET_COMPLETION_STATUS_DONE;
            }

            return DN_ASSET_COMPLETION_STATUS_RUNNING;
        } break;

        default: {
            DN_ASSERT(false);
            return DN_ASSET_COMPLETION_STATUS_DONE;
        }
    }
}

void dn_assets_init(dn_asset_config_t user_config) {
  dn_log("%s", __func__);

  dn_asset_importer_t default_importers [] = {
    {
      .id = dn_type_name(Background),
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
   
  dn_for_each(configs, config) {
    dn_for_each_n(config->importers.data, importer, config->importers.count) {
      dn_log("%s: Registered importer; id = %s, on_import = %d, on_complete = %d", 
        __func__, 
        importer->id, 
        importer->on_import, importer->on_complete
      );

      dn_assets.importers[importer->id] = *importer;
    }
  }
  

  dn_ring_buffer_init(&asset_loader.load_requests, 2048);
  dn_ring_buffer_init(&asset_loader.completion_queue, 2048);
  
  // asset_loader.thread = std::thread(&AssetLoader::process_requests, &asset_loader);
  // asset_loader.thread.detach();

}

void dn_assets_add(const char* name, dn_asset_data_t data) {
  dn_log("%s: name = %s, data = %p", __func__, name, data);
  dn_asset_t asset;
  dn_asset_copy_name(name, asset.name);
  asset.data = data;
  dn_assets.assets[name] = asset;
}

dn_asset_data_t dn_assets_find(const char* name) {
    DN_ASSERT(name);

    if (!dn_assets.assets.contains(name)) {
    dn_log("Tried to find asset, but name was not registered; name = %s", name);
        return NULL;
    }

    return dn_assets.assets[name].data;
}

void dn_asset_copy_name(const char* source, dn_asset_name_t dest) {
  dn_cstr_copy(source, dest, DN_ASSET_NAME_LEN);
}


void dn_assets_update() {
  asset_loader.process_completion_queue();

  dn_array_for(backgrounds, background) {
    if (background->gpu_ready && !background->gpu_done) {
      background->load_one_to_gpu();

      if (background->gpu_done) {
        background->deinit();
      }

      if (dn_engine_exceeded_frame_time()) break;
    }
  }
}

void AssetLoader::process_requests() {
  while (true) {
    std::unique_lock lock(mutex);

    condition.wait(lock, [this] {
      return load_requests.size > 0;
      });

    auto request = dn_ring_buffer_pop(&load_requests);
    lock.unlock();

    if (request.kind == AssetKind::TextureAtlas) {
      auto atlas = request.atlas;
      if (atlas->need_async_build) {
        atlas->build_from_source();
      }
      if (atlas->need_async_load) {
        atlas->load_to_memory();
      }

      lock.lock();
      dn_ring_buffer_push(&completion_queue, request);
      lock.unlock();

    }
    else if (request.kind == AssetKind::Background) {
      auto background = request.background;
      if (background->need_async_build) {
        background->build_from_source();
      }
      if (background->need_async_load) {
        background->load_tiles();
      }

      lock.lock();
      dn_ring_buffer_push(&completion_queue, request);
      lock.unlock();
    }
  }
}

void AssetLoader::process_completion_queue() {
  int num_assets_loaded = 0;
  auto begin = glfwGetTime();
  while (true) {
    std::unique_lock lock(mutex);
    if (!completion_queue.size) {
      break;
    }

    auto completion = dn_ring_buffer_pop(&completion_queue);
    lock.unlock();

    num_assets_loaded++;

    if (completion.kind == AssetKind::Background) {
      auto background = completion.background;
      dn_log_flags(DN_LOG_FLAG_FILE, "%s: AssetKind = Background, AssetName = %s", __func__, background->name);
      
      if (background->is_dirty()) {
        background->update_config();
      }

      background->gpu_ready = true;
    }
    else if (completion.kind == AssetKind::TextureAtlas) {
      auto atlas = completion.atlas;
      dn_log_flags(DN_LOG_FLAG_FILE, "%s: atlas, %s", __func__, atlas->name);
      
      if (atlas->need_gl_init) {
        atlas->load_to_gpu();
        atlas->need_gl_init = false;
      }

      if (atlas->need_config_update) {
        atlas->write_to_config();
        atlas->need_config_update = false;
      }
      
    }

    if (dn_engine_exceeded_frame_time()) break;
  }

  if (num_assets_loaded) {
    std::unique_lock lock(mutex);
    auto now = glfwGetTime();
    dn_log_flags(DN_LOG_FLAG_FILE,
             "AssetLoader: frame = %d, assets_loaded =  %d, assets_remaining = %d, time_ms =  %f",
             engine.frame,
             num_assets_loaded, completion_queue.size,
             (now - begin) * 1000);
  }
}


void AssetLoader::submit(AssetLoadRequest request) {
  request.id = AssetLoadRequest::next_id++;

  mutex.lock();
  dn_ring_buffer_push(&load_requests, request);
  mutex.unlock();

  condition.notify_all();
}

#endif

/////////
// APP //
/////////
void dn_app_configure(dn_app_config_t config) {
  dn_log("%s", __func__);

  dn_window_init(config.window);
  // dn_imgui_init();
  // dn_assets_init(config.asset);
  // dn_audio_init(config.audio);
  // dn_font_init(config.font);
  // dn_gpu_init(config.gpu);
  // dn_images_init(config.image);
  // dn_noise_init();
  // dn_steam_init(config.steam);

  // init_texture_atlas(); // Invert control
  // init_backgrounds(); // Invert control
  // init_screenshots(); // Use the asset loader
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
    dn_type_info(dn_app_config_t),
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
    // dn_type_info(dn_gpu_backed_buffer_t),
    // dn_type_info(dn_gpu_buffer_t),
    // dn_type_info(dn_gpu_buffer_binding_t),
    // dn_type_info(dn_gpu_buffer_descriptor_t),
    // dn_type_info(dn_gpu_config_t),
    // dn_type_info(dn_gpu_command_buffer_descriptor_t),
    // dn_type_info(dn_gpu_draw_call_t),
    // dn_type_info(dn_gpu_pipeline_descriptor_t),
    // dn_type_info(dn_gpu_raster_state_t),
    // dn_type_info(dn_gpu_render_pass_t),
    // dn_type_info(dn_gpu_render_target_t),
    // dn_type_info(dn_gpu_render_target_descriptor_t),
    // dn_type_info(dn_gpu_renderer_state_t),
    // dn_type_info(dn_gpu_scissor_state_t),
    // dn_type_info(dn_gpu_shader_descriptor_t),
    // dn_type_info(dn_gpu_storage_buffer_binding_t),
    // dn_type_info(dn_gpu_uniform_t),
    // dn_type_info(dn_gpu_uniform_binding_t),
    // dn_type_info(dn_gpu_uniform_buffer_binding_t),
    // dn_type_info(dn_gpu_uniform_descriptor_t),
    // dn_type_info(dn_gpu_uniform_data_t),
    // dn_type_info(dn_gpu_vertex_attr_info_t),
    // dn_type_info(dn_gpu_vertex_buffer_binding_t),
    // dn_type_info(dn_low_pass_filter_t),    
    dn_type_info(dn_named_path_t),
    dn_type_info(dn_named_path_result_t),
    dn_type_info(dn_os_directory_entry_t),
    dn_type_info(dn_os_directory_entry_list_t),
    // dn_type_info(dn_prepared_text_t),
    // dn_type_info(dn_sdf_circle_t),
    // dn_type_info(dn_sdf_combine_entry_t),
    // dn_type_info(dn_sdf_combine_header_t),
    // dn_type_info(dn_sdf_combine_op_t),
    // dn_type_info(dn_sdf_header_t),
    // dn_type_info(dn_sdf_instance_t),
    // dn_type_info(dn_sdf_oriented_box_t),
    // dn_type_info(dn_sdf_renderer_t),
    // dn_type_info(dn_sdf_renderer_state_t),
    // dn_type_info(dn_sdf_ring_t),
    // dn_type_info(dn_sdf_shape_t),
    // dn_type_info(dn_sdf_smoothing_kernel_t),
    dn_type_info(dn_type_info_t),
    dn_type_info(dn_type_info_list_t),
    dn_type_info(dn_window_config_t),
  };

  return (dn_type_info_list_t) {
    .data = types,
    .count = dn_arr_len(types)
  };
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
  //   dn_allocators_update();
  //   dn_file_monitors_update();
  //   dn_assets_update();
  //   dn_imgui_update();
  //   dn_input_update();
  //   update_actions();
  //   dn_lua_update();
  //   dn_time_metrics_update();
  }

  // dn_audio_shutdown();
  // dn_steam_shutdown();
  // dn_imgui_shutdown();
  dn_window_shutdown();

  return 0;
}

#endif
#endif


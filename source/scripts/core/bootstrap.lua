local ffi_header = [[
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ██████╗  ██████╗ ██╗   ██╗██████╗ ██╗     ███████╗███╗   ██╗██╗ ██████╗██╗  ██╗███████╗██╗      //
// ██╔══██╗██╔═══██╗██║   ██║██╔══██╗██║     ██╔════╝████╗  ██║██║██╔════╝██║ ██╔╝██╔════╝██║      //
// ██║  ██║██║   ██║██║   ██║██████╔╝██║     █████╗  ██╔██╗ ██║██║██║     █████╔╝ █████╗  ██║      //
// ██║  ██║██║   ██║██║   ██║██╔══██╗██║     ██╔══╝  ██║╚██╗██║██║██║     ██╔═██╗ ██╔══╝  ██║      //
// ██████╔╝╚██████╔╝╚██████╔╝██████╔╝███████╗███████╗██║ ╚████║██║╚██████╗██║  ██╗███████╗███████╗ //
// ╚═════╝  ╚═════╝  ╚═════╝ ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═══╝╚═╝ ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝ //
/////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////
// ████████╗██╗   ██╗██████╗ ███████╗███████╗ //
// ╚══██╔══╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔════╝ //
//    ██║    ╚████╔╝ ██████╔╝█████╗  ███████╗ //
//    ██║     ╚██╔╝  ██╔═══╝ ██╔══╝  ╚════██║ //
//    ██║      ██║   ██║     ███████╗███████║ //
//    ╚═╝      ╚═╝   ╚═╝     ╚══════╝╚══════╝ //
////////////////////////////////////////////////
   

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t  i32;
typedef float f32;
typedef double f64;

typedef struct {
  float x;
  float y;
} dn_vector2_t;

typedef struct {
  float x;
  float y;
  float z;
} dn_vector3_t;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} dn_vector4_t;

typedef struct {
  float data [2] [2];
} dn_matrix2_t;

typedef struct {
  float data [3] [3];
} dn_matrix3_t;

typedef struct {
  float data [4] [4];
} dn_matrix4_t;

// These are just in Lua, so I can add a metatype to returned strings for a nice interning API.
// dn_tstring_t is a temporary string; i.e., one allocated with temporary storage. There's nothing different
// about the underlying data, it's just a hint to the programmer to not hold onto this pointer.
typedef struct {
  char* data;
} dn_tstring_t;

typedef struct {
  char* data;
} string;

typedef char dn_asset_name_t [64]; // [DN_ASSET_NAME_LEN];
typedef char dn_path_t [256]; // [DN_MAX_PATH_LEN];




///////////////////////
//  ██████╗ ███████╗ //
// ██╔═══██╗██╔════╝ //
// ██║   ██║███████╗ //
// ██║   ██║╚════██║ //
// ╚██████╔╝███████║ //
//  ╚═════╝ ╚══════╝ //
///////////////////////

///////////////
// ALLOCATOR //
///////////////
typedef struct dn_allocator_t dn_allocator_t;

void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
dn_allocator_t*  dn_allocator_find(const char* name);
void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);

typedef struct {
  u8* data;
  u32 len;
} dn_string_t;


////////////
// STRING //
////////////
typedef struct {
  struct {
    u8* data;
    u32 count;
    u32 capacity;
  } buffer;

  dn_allocator_t* allocator;
} dn_string_builder_t;

void        dn_string_builder_grow(dn_string_builder_t* builder);
void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
void        dn_cstr_copy(const char* str, char* buffer, u32 buffer_length);
void        dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);
char*       dn_string_to_cstr(dn_string_t str);
char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
bool        dn_string_equal(dn_string_t a, dn_string_t b);
bool        dn_string_equal_cstr(dn_string_t a, const char* b);
bool        dn_cstr_equal(const char* a, const char* b);
u32         dn_cstr_len(const char* str);
dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);


////////////////
// FILESYSTEM //
////////////////

typedef enum {
  DN_OS_FILE_ATTR_NONE = 0,
  DN_OS_FILE_ATTR_REGULAR_FILE = 1,
  DN_OS_FILE_ATTR_DIRECTORY = 2,
} dn_os_file_attr_t;

typedef struct dn_os_date_time_t {
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

bool                         dn_os_does_path_exist(dn_string_t path);
bool                         dn_os_is_regular_file(dn_string_t path);
bool                         dn_os_is_directory(dn_string_t path);
void                         dn_os_create_directory(dn_string_t path);
void                         dn_os_remove_directory(dn_string_t path);
void                         dn_os_create_file(dn_string_t path);
void                         dn_os_remove_file(dn_string_t path);
dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path);
dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path);
dn_os_date_time_t            dn_os_get_date_time();
f64                          dn_os_file_mod_time(dn_string_t path);
void                         dn_os_memory_copy(const void* source, void* dest, u32 num_bytes);
bool                         dn_os_is_memory_equal(const void* a, const void* b, size_t len);
void                         dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
void                         dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
void                         dn_os_zero_memory(void* buffer, u32 buffer_size);




//////////////////////////////////////////////////////
// ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗ //
// ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝ //
// █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗   //
// ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝   //
// ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗ //
// ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝ //
//////////////////////////////////////////////////////

typedef struct {
  const char* name;
  u32 size;
} dn_type_info_t;

typedef struct {
  dn_type_info_t* data;
  u32 count;
} dn_type_info_list_t;

void                dn_engine_set_exit_game();
const char*         dn_engine_get_game_hash();
void                dn_engine_set_target_fps(double fps);
double              dn_engine_get_target_fps();
bool                dn_engine_exceeded_frame_time();
bool                dn_engine_should_exit();
dn_type_info_list_t dn_engine_query_types();


void   dn_time_metric_add(const char* name);
void   dn_time_metric_begin(const char* name);
void   dn_time_metric_end(const char* name);
double dn_time_metric_average(const char* name);
double dn_time_metric_last(const char* name);
double dn_time_metric_largest(const char* name);
double dn_time_metric_smallest(const char* name);

typedef struct {
  const char* name;
  const char* path;
} dn_named_path_t;

typedef struct {
  dn_named_path_t* named_paths;
  u32 size;
} dn_named_path_result_t;

dn_named_path_result_t dn_paths_find_all();
void                   dn_paths_add_install_subpath(const char* name, const char* relative_path);
void                   dn_paths_add_engine_subpath(const char* name, const char* relative_path);
void                   dn_paths_add_write_subpath(const char* name, const char* relative_path);
void                   dn_paths_add_subpath(const char* name, const char* parent_name, const char* relative_path);
dn_tstring_t           dn_paths_resolve(const char* name);
dn_tstring_t           dn_paths_resolve_format(const char* name, const char* file_name);



///////////////////////////////////////////////////////////////////////////////////////
//  ██████╗ ██████╗ ███╗   ██╗████████╗ █████╗ ██╗███╗   ██╗███████╗██████╗ ███████╗ //
// ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██║████╗  ██║██╔════╝██╔══██╗██╔════╝ //
// ██║     ██║   ██║██╔██╗ ██║   ██║   ███████║██║██╔██╗ ██║█████╗  ██████╔╝███████╗ //
// ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██║██║██║╚██╗██║██╔══╝  ██╔══██╗╚════██║ //
// ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║██║██║ ╚████║███████╗██║  ██║███████║ //
//  ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚══════╝ //
///////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 vertex_size;
  dn_allocator_t* allocator;
} dn_fixed_array_t;

void dn_fixed_array_init(dn_fixed_array_t* vertex_buffer, u32 max_vertices, u32 vertex_size, dn_allocator_t* allocator);
u8*  dn_fixed_array_push(dn_fixed_array_t* vertex_buffer, void* data, u32 count);
u8*  dn_fixed_array_reserve(dn_fixed_array_t* vertex_buffer, u32 count);
void dn_fixed_array_clear(dn_fixed_array_t* vertex_buffer);
u32  dn_fixed_array_byte_size(dn_fixed_array_t* vertex_buffer);
u8*  dn_fixed_array_at(dn_fixed_array_t* vertex_buffer, u32 index);

typedef struct {
  i32 index;
  i32 generation;
} dn_gen_arena_handle_t;

void dn_cstr_copy(const char* str, char* buffer, u32 buffer_length);
void dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);


///////////////////////////////////////////
//  █████╗ ██╗   ██╗██████╗ ██╗ ██████╗  //
// ██╔══██╗██║   ██║██╔══██╗██║██╔═══██╗ //
// ███████║██║   ██║██║  ██║██║██║   ██║ //
// ██╔══██║██║   ██║██║  ██║██║██║   ██║ //
// ██║  ██║╚██████╔╝██████╔╝██║╚██████╔╝ //
// ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝  //
///////////////////////////////////////////

typedef enum {
  DN_AUDIO_FILTER_MODE_FIRST_ORDER = 0,
  DN_AUDIO_FILTER_MODE_BUTTERWORTH = 1,
} dn_audio_filter_mode_t;

typedef struct {
  dn_audio_filter_mode_t mode;
  bool enabled;
  float cutoff_frequency;
  float cutoff_alpha;
  float a0, a1, a2, b1, b2;
  float input_history [2];
  float output_history [2];
} dn_low_pass_filter_t;

typedef struct {
  float threshold;
  float ratio;
  float attack_time;
  float release_time;
  bool enabled;
} dn_compressor_t;

typedef struct {
  dn_path_t* dirs;
  u32 num_dirs;
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  float sample_frequency;
  float master_volume;
  float master_volume_mod;
} dn_audio_config_t;

typedef dn_gen_arena_handle_t dn_audio_instance_handle_t;

void                       dn_audio_set_compressor_threshold(float t);
void                       dn_audio_set_compressor_ratio(float v);
void                       dn_audio_set_compressor_attack(float v);
void                       dn_audio_set_compressor_release(float v);
void                       dn_audio_set_sample_rate(float v);
float                      dn_audio_get_master_volume();
void                       dn_audio_set_master_volume(float v);
float                      dn_audio_get_master_volume_mod();
void                       dn_audio_set_master_volume_mod(float v);
float                      dn_audio_get_master_filter_cutoff();
void                       dn_audio_set_master_filter_cutoff(float v);
void                       dn_audio_set_master_filter_cutoff_enabled(bool enabled);
void                       dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode);
void                       dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume);
void                       dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, float cutoff);
void                       dn_audio_set_filter_mode(dn_audio_instance_handle_t handle, float cutoff);
void                       dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled);
dn_audio_instance_handle_t dn_audio_play_sound(const char* name);
dn_audio_instance_handle_t dn_audio_play_looped(const char* name);
void                       dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next);
void                       dn_audio_stop(dn_audio_instance_handle_t handle);
void                       dn_audio_stop_all();
void                       dn_audio_pause(dn_audio_instance_handle_t handle);
void                       dn_audio_resume(dn_audio_instance_handle_t handle);
bool                       dn_audio_is_playing(dn_audio_instance_handle_t handle);
bool                       dn_audio_is_any_playing();
void                       dn_audio_load(const char* file_path, const char* file_name);
void                       dn_low_pass_filter_set_mode(dn_low_pass_filter_t* filter, dn_audio_filter_mode_t mode);
void                       dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, float cutoff);
float                      dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, float input);
dn_audio_config_t          dn_audio_config_default();
void                       dn_audio_init(dn_audio_config_t config);


/////////////////////////////////////////////
// ██╗███╗   ██╗██████╗ ██╗   ██╗████████╗ //
// ██║████╗  ██║██╔══██╗██║   ██║╚══██╔══╝ //
// ██║██╔██╗ ██║██████╔╝██║   ██║   ██║    //
// ██║██║╚██╗██║██╔═══╝ ██║   ██║   ██║    //
// ██║██║ ╚████║██║     ╚██████╔╝   ██║    //
// ╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝    ╚═╝    //
/////////////////////////////////////////////

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

dn_coord_data_t dn_coord_get();
void            dn_coord_set_camera(float x, float y);
void            dn_coord_set_framebuffer_position(float x, float y);
void            dn_coord_set_framebuffer_size(float x, float y);
dn_vector2_t         dn_coord_screen_to_window(float x, float y);
dn_vector2_t         dn_coord_screen_to_game(float x, float y);
dn_vector2_t         dn_coord_screen_to_world(float x, float y);
dn_vector2_t         dn_coord_window_to_screen(float x, float y);
dn_vector2_t         dn_coord_window_to_game(float x, float y);
dn_vector2_t         dn_coord_window_to_world(float x, float y);
dn_vector2_t         dn_coord_game_to_screen(float x, float y);
dn_vector2_t         dn_coord_game_to_window(float x, float y);
dn_vector2_t         dn_coord_game_to_world(float x, float y);
dn_vector2_t         dn_coord_world_to_screen(float x, float y);
dn_vector2_t         dn_coord_world_to_window(float x, float y);
dn_vector2_t         dn_coord_world_to_game(float x, float y);
dn_vector2_t         dn_coord_screen_to_window_mag(float x, float y);
dn_vector2_t         dn_coord_screen_to_game_mag(float x, float y);
dn_vector2_t         dn_coord_screen_to_world_mag(float x, float y);
dn_vector2_t         dn_coord_window_to_screen_mag(float x, float y);
dn_vector2_t         dn_coord_window_to_game_mag(float x, float y);
dn_vector2_t         dn_coord_window_to_world_mag(float x, float y);
dn_vector2_t         dn_coord_game_to_screen_mag(float x, float y);
dn_vector2_t         dn_coord_game_to_window_mag(float x, float y);
dn_vector2_t         dn_coord_game_to_world_mag(float x, float y);
dn_vector2_t         dn_coord_world_to_screen_mag(float x, float y);
dn_vector2_t         dn_coord_world_to_window_mag(float x, float y);
dn_vector2_t         dn_coord_world_to_game_mag(float x, float y);

typedef enum {
  DN_INPUT_DEVICE_MOUSE_AND_KEYBOARD = 0,
  DN_INPUT_DEVICE_CONTROLLER = 1,
} dn_input_device_t;

bool              dn_input_pressed(int key);
bool              dn_input_released(int key);
bool              dn_input_down(int key);
bool              dn_input_mod_down(int mod);
bool              dn_input_chord_pressed(int mod, int key);
dn_vector2_t           dn_input_scroll();
dn_vector2_t           dn_input_mouse(dn_coord_t unit);
dn_vector2_t           dn_input_mouse_delta(dn_coord_t unit);
u32               dn_input_shift_key(u32 key);
dn_input_device_t dn_input_get_device();


////////////////////////////////////////
// ████████╗███████╗██╗  ██╗████████╗ //
// ╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝ //
//    ██║   █████╗   ╚███╔╝    ██║    //
//    ██║   ██╔══╝   ██╔██╗    ██║    //
//    ██║   ███████╗██╔╝ ██╗   ██║    //
//    ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝    //
////////////////////////////////////////

typedef struct {
  char text [1024];
  dn_vector2_t position;
  dn_vector2_t padding;
  dn_vector4_t color;
  void* font;
  float wrap;
  float offset;
  bool world_space;

  int breaks [32];
  float width;
  float height;
  float baseline_offset;
  float baseline_offset_imprecise;
  float height_imprecise;
  bool precise;
} dn_prepared_text_t;

dn_prepared_text_t* dn_prepare_text(const char* text, f32 px, f32 py, const char* font);
dn_prepared_text_t* dn_prepare_text_wrap(const char* text, f32 px, f32 py, const char* font, f32 wrap);
dn_prepared_text_t* dn_prepare_text_ex(const char* text, f32 px, f32 py, const char* font, f32 wrap, dn_vector4_t color, bool precise);

typedef enum {
  DN_FONT_FLAG_NONE = 0,
  DN_FONT_FLAG_IMGUI = 1,
  DN_FONT_FLAG_DEFAULT = 2,
} dn_font_flags_t;

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

typedef struct dn_baked_font_t dn_baked_font_t;

void             dn_font_init(dn_font_config_t config);
void             dn_font_bake_n(dn_font_descriptor_t* descriptors, u32 num_descriptors);
void             dn_font_bake(dn_font_descriptor_t descriptor);
dn_baked_font_t* dn_font_default();
dn_baked_font_t* dn_font_find(const char* id, u32 size);
dn_font_config_t dn_font_config_default();


////////////////////////////////////////////////////////
// ██╗    ██╗██╗███╗   ██╗██████╗  ██████╗ ██╗    ██╗ //
// ██║    ██║██║████╗  ██║██╔══██╗██╔═══██╗██║    ██║ //
// ██║ █╗ ██║██║██╔██╗ ██║██║  ██║██║   ██║██║ █╗ ██║ //
// ██║███╗██║██║██║╚██╗██║██║  ██║██║   ██║██║███╗██║ //
// ╚███╔███╔╝██║██║ ╚████║██████╔╝╚██████╔╝╚███╔███╔╝ //
//  ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═════╝  ╚═════╝  ╚══╝╚══╝  //
////////////////////////////////////////////////////////

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
  const char* title;
  const char* icon;
  dn_display_mode_t display_mode;
  dn_vector2_t native_resolution;
  dn_window_flags_t flags;
} dn_window_config_t;

dn_window_config_t dn_window_config_default();
void               dn_window_init(dn_window_config_t descriptor);
void               dn_window_set_native_resolution(float width, float height);
dn_vector2_t            dn_window_get_content_area();
dn_vector2_t            dn_window_get_native_resolution();
void               dn_window_set_icon(const char* path);
void               dn_window_set_display_mode(dn_display_mode_t mode);
dn_display_mode_t  dn_window_get_display_mode();
void               dn_window_set_cursor_visible(bool visible);



////////////////////////////////
//  ██████╗ ██████╗ ██╗   ██╗ //
// ██╔════╝ ██╔══██╗██║   ██║ //
// ██║  ███╗██████╔╝██║   ██║ //
// ██║   ██║██╔═══╝ ██║   ██║ //
// ╚██████╔╝██║     ╚██████╔╝ //
//  ╚═════╝ ╚═╝      ╚═════╝  //
////////////////////////////////

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

typedef struct dn_gpu_shader_t dn_gpu_shader_t;

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
  i32 texture;
  i32 i32;
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
  dn_gpu_buffer_t* buffer;
  u32 base;
} dn_gpu_storage_buffer_binding_t;

typedef struct {
  dn_gpu_uniform_data_t data;
  dn_gpu_uniform_t* uniform;
  u32 binding_index;
} dn_gpu_uniform_binding_t;

typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_t* buffer;
  u32 binding_index;
} dn_gpu_uniform_buffer_binding_t;

typedef struct {
  __declspec(align(16)) struct {
    dn_gpu_vertex_buffer_binding_t bindings [8];
    u32 count;
  } vertex;

  __declspec(align(16)) struct {
    dn_gpu_uniform_binding_t bindings [8];
    u32 count;
  } uniforms;

  __declspec(align(16)) struct {
    dn_gpu_storage_buffer_binding_t bindings [8];
    u32 count;
  } storage;

  __declspec(align(16)) struct {
    dn_gpu_uniform_buffer_binding_t bindings [8];
    u32 count;
  } uniform_buffers;} dn_gpu_buffer_binding_t;


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

typedef struct dn_gpu_pipeline_t dn_gpu_pipeline_t;

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
  u32 max_commands;
} dn_gpu_command_buffer_descriptor_t;

typedef struct dn_gpu_command_buffer_t dn_gpu_command_buffer_t;

////////////
// DN GPU //
////////////
typedef struct {
  const char* shader_path;
  dn_gpu_shader_descriptor_t* shaders;
  u32 num_shaders;
  const char** search_paths;
  u32 num_search_paths;
  dn_gpu_render_target_descriptor_t* render_targets;
  u32 num_render_targets;
} dn_gpu_config_t;
///////////////
// FUNCTIONS //
///////////////
dn_gpu_command_buffer_t* dn_gpu_command_buffer_create(dn_gpu_command_buffer_descriptor_t descriptor);
void                     dn_gpu_command_buffer_draw(dn_gpu_command_buffer_t* command_buffer, dn_gpu_draw_call_t draw_call);
void                     dn_gpu_command_buffer_submit(dn_gpu_command_buffer_t* command_buffer);
void                     dn_gpu_bind_pipeline(dn_gpu_command_buffer_t* command_buffer, dn_gpu_pipeline_t* pipeline);
void                     dn_gpu_begin_render_pass(dn_gpu_command_buffer_t* command_buffer, dn_gpu_render_pass_t render_pass);
void                     dn_gpu_end_render_pass(dn_gpu_command_buffer_t* command_buffer);
void                     dn_gpu_apply_bindings(dn_gpu_command_buffer_t* command_buffer, dn_gpu_buffer_binding_t bindings);
void                     dn_gpu_bind_render_state(dn_gpu_command_buffer_t* command_buffer, dn_gpu_renderer_state_t render);
void                     dn_gpu_set_layer(dn_gpu_command_buffer_t* command_buffer, u32 layer);
void                     dn_gpu_set_world_space(dn_gpu_command_buffer_t* command_buffer, bool world_space);
void                     dn_gpu_set_camera(dn_gpu_command_buffer_t* command_buffer, dn_vector2_t camera);
dn_gpu_pipeline_t*       dn_gpu_pipeline_create(dn_gpu_pipeline_descriptor_t descriptor);
dn_gpu_uniform_t*        dn_gpu_uniform_create(dn_gpu_uniform_descriptor_t descriptor);
dn_gpu_buffer_t*         dn_gpu_buffer_create(dn_gpu_buffer_descriptor_t descriptor);
void                     dn_gpu_buffer_bind(dn_gpu_buffer_t* buffer);
void                     dn_gpu_buffer_bind_base(dn_gpu_buffer_t* buffer, u32 base);
void                     dn_gpu_buffer_sync(dn_gpu_buffer_t* buffer, void* data, u32 size);
void                     dn_gpu_buffer_sync_subdata(dn_gpu_buffer_t* buffer, void* data, u32 byte_size, u32 byte_offset);
void                     dn_gpu_buffer_zero(dn_gpu_buffer_t* buffer, u32 size);
dn_gpu_backed_buffer_t   dn_gpu_backed_buffer_create(dn_gpu_buffer_descriptor_t descriptor);
u32                      dn_gpu_backed_buffer_size(dn_gpu_backed_buffer_t* buffer);
void                     dn_gpu_backed_buffer_clear(dn_gpu_backed_buffer_t* buffer);
u8*                      dn_gpu_backed_buffer_push(dn_gpu_backed_buffer_t* buffer, void* data, u32 num_elements);
void                     dn_gpu_backed_buffer_sync(dn_gpu_backed_buffer_t* buffer);
dn_gpu_shader_t*         dn_gpu_shader_create(dn_gpu_shader_descriptor_t descriptor);
dn_gpu_shader_t*         dn_gpu_shader_find(const char* name);
dn_gpu_render_target_t*  dn_gpu_render_target_create(dn_gpu_render_target_descriptor_t descriptor);
dn_gpu_render_target_t*  dn_gpu_acquire_swapchain();
dn_gpu_render_target_t*  dn_gpu_render_target_find(const char* name);
void                     dn_gpu_render_target_bind(dn_gpu_render_target_t* target);
void                     dn_gpu_render_target_clear(dn_gpu_render_target_t* target);
void                     dn_gpu_render_target_blit(dn_gpu_render_target_t* source, dn_gpu_render_target_t* destination);
void                     dn_gpu_memory_barrier(dn_gpu_memory_barrier_t barrier);
void                     dn_gpu_dispatch_compute(dn_gpu_buffer_t* buffer, u32 size);
void                     dn_gpu_swap_buffers();
void                     dn_gpu_error_clear();
dn_tstring_t                  dn_gpu_error_read();
void                     dn_gpu_error_log_one();
void                     dn_gpu_error_log_all();
void                     dn_gpu_set_resource_name(dn_gpu_resource_id_t id, u32 handle, u32 name_len, const char* name);



//////////////////////////////
// ███████╗██████╗ ███████╗ //
// ██╔════╝██╔══██╗██╔════╝ //
// ███████╗██║  ██║█████╗   //
// ╚════██║██║  ██║██╔══╝   //
// ███████║██████╔╝██║      //
// ╚══════╝╚═════╝ ╚═╝      //
//////////////////////////////

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
  u32 kind;
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

dn_sdf_renderer_t        dn_sdf_renderer_create(u32 buffer_size);
void                     dn_sdf_renderer_draw(dn_sdf_renderer_t* renderer, dn_gpu_command_buffer_t* command_buffer);
void                     dn_sdf_renderer_push_instance(dn_sdf_renderer_t* renderer, dn_sdf_shape_t shape);
void                     dn_sdf_renderer_push_header(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness);
dn_sdf_combine_header_t* dn_sdf_combination_begin(dn_sdf_renderer_t* renderer);
void                     dn_sdf_combination_append(dn_sdf_renderer_t* renderer, dn_sdf_combine_header_t* header, dn_sdf_shape_t shape, dn_sdf_combine_op_t op, dn_sdf_smoothing_kernel_t kernel);
void                     dn_sdf_combination_commit(dn_sdf_renderer_t* renderer);
void                     dn_sdf_circle_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius);
void                     dn_sdf_ring_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius);
void                     dn_sdf_oriented_box_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy);
void                     dn_sdf_grid(dn_sdf_renderer_t* renderer, u32 grid_width, u32 grid_size);



/////////////////////////////////////////
// ██╗███╗   ███╗ ██████╗ ██╗   ██╗██╗ //
// ██║████╗ ████║██╔════╝ ██║   ██║██║ //
// ██║██╔████╔██║██║  ███╗██║   ██║██║ //
// ██║██║╚██╔╝██║██║   ██║██║   ██║██║ //
// ██║██║ ╚═╝ ██║╚██████╔╝╚██████╔╝██║ //
// ╚═╝╚═╝     ╚═╝ ╚═════╝  ╚═════╝ ╚═╝ //
/////////////////////////////////////////
                                   
typedef struct {
  dn_vector4_t light;
  dn_vector4_t medium_light;
  dn_vector4_t low_light;
  dn_vector4_t neutral;
  dn_vector4_t medium_dark;
} dn_imgui_colors_t;

void    dn_imgui_push_font(const char* font_name, u32 size);
void    dn_imgui_image(const char* image, float sx, float sy);
void    dn_imgui_file_browser_open();
void    dn_imgui_file_browser_close();
void    dn_imgui_file_browser_set_work_dir(const char* directory);
bool    dn_imgui_file_browser_is_file_selected();
dn_tstring_t dn_imgui_file_browser_get_selected_file();
void    dn_imgui_load_layout(const char* file_name);
void    dn_imgui_save_layout(const char* file_name);
void    dn_imgui_load_colors(dn_imgui_colors_t colors);



////////////////////////////////////////////////////////////////////////////////////////////////////////
// ██╗███╗   ██╗██████╗ ██╗   ██╗████████╗     █████╗  ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗ //
// ██║████╗  ██║██╔══██╗██║   ██║╚══██╔══╝    ██╔══██╗██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝ //
// ██║██╔██╗ ██║██████╔╝██║   ██║   ██║       ███████║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗ //
// ██║██║╚██╗██║██╔═══╝ ██║   ██║   ██║       ██╔══██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║ //
// ██║██║ ╚████║██║     ╚██████╔╝   ██║       ██║  ██║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║ //
// ╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝    ╚═╝       ╚═╝  ╚═╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝ //
////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
  KEY_ACTION_PRESS = 0,
  KEY_ACTION_DOWN = 1,
} KeyAction;

void register_action(const char* name, u32 key, u32 key_event, const char* action_set);
void register_action_set(const char* name);
void activate_action_set(const char* name);
bool is_digital_active(const char* name);
bool was_digital_active(const char* name);
bool was_digital_pressed(const char* name);
i32 get_action_set_cooldown();
const char* get_active_action_set();



////////////////////////////////////////////
// ███╗   ██╗ ██████╗ ██╗███████╗███████╗ //
// ████╗  ██║██╔═══██╗██║██╔════╝██╔════╝ //
// ██╔██╗ ██║██║   ██║██║███████╗█████╗   //
// ██║╚██╗██║██║   ██║██║╚════██║██╔══╝   //
// ██║ ╚████║╚██████╔╝██║███████║███████╗ //
// ╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚══════╝╚══════╝ //
////////////////////////////////////////////

double dn_noise_perlin(double x, double y);
double dn_noise_perlin_scaled(double x, double y, double vmin, double vmax);
double dn_noise_chaotic(double x, double y);
double dn_noise_chaotic_scaled(double x, double y, double vmin, double vmax);



///////////////////////////////////////////////
//  █████╗ ███████╗███████╗███████╗████████╗ //
// ██╔══██╗██╔════╝██╔════╝██╔════╝╚══██╔══╝ //
// ███████║███████╗███████╗█████╗     ██║    //
// ██╔══██║╚════██║╚════██║██╔══╝     ██║    //
// ██║  ██║███████║███████║███████╗   ██║    //
// ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝   ╚═╝    //
///////////////////////////////////////////////
                                         
typedef struct {
  void* user_data;
} dn_asset_import_request_t;

typedef enum {
  DN_ASSET_COMPLETION_STATUS_RUNNING,
  DN_ASSET_COMPLETION_STATUS_DONE,
} dn_asset_completion_status_t;

typedef void(*dn_asset_import_fn)(dn_asset_import_request_t* request);
typedef dn_asset_completion_status_t(*dn_asset_completion_fn)(dn_asset_import_request_t* request);

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

void dn_assets_init(dn_asset_config_t config);
dn_asset_data_t dn_assets_find(const char* name);
void dn_assets_add(const char* name, dn_asset_data_t data);
void dn_asset_copy_name(const char* source, dn_asset_name_t dest);





typedef struct {
  dn_string_t* dirs;
  u32 num_dirs;
} dn_image_config_t;



typedef enum {
  DN_STEAM_INIT_NONE = 0,
  DN_STEAM_INIT_FAILED = 1,
  DN_STEAM_INIT_OK = 2,
} dn_steam_init_state_t;

typedef enum {
  DN_STEAM_INPUT_TEXT_UNINITIALIZED = 0,
  DN_STEAM_INPUT_TEXT_OPEN = 1,
  DN_STEAM_INPUT_TEXT_UNREAD = 2,
  DN_STEAM_INPUT_TEXT_CLOSED = 3,
} dn_steam_text_input_state_t;

typedef struct {
  u32 app_id;
} dn_steam_config_t;

void                        dn_steam_init(dn_steam_config_t config);
bool                        dn_steam_initialized();
void                        dn_steam_open_store_page();
void                        dn_steam_open_store_page_utm(dn_string_t utm);
void                        dn_steam_open_store_page_ex(dn_string_t url);
void                        dn_steam_open_text_input();
dn_steam_text_input_state_t dn_steam_text_input_state();
dn_string_t                 dn_string_read_text_input();
bool                        dn_steam_is_deck();


//////////////////////////////
//  █████╗ ██████╗ ██████╗  //
// ██╔══██╗██╔══██╗██╔══██╗ //
// ███████║██████╔╝██████╔╝ //
// ██╔══██║██╔═══╝ ██╔═══╝  //
// ██║  ██║██║     ██║      //
// ╚═╝  ╚═╝╚═╝     ╚═╝      //
//////////////////////////////                        

typedef struct {
  const char* install_path;
  const char* engine_path;
  const char* write_path;
  const char* app_path;
} dn_app_descriptor_t;

typedef struct {
  dn_window_config_t window;
  dn_audio_config_t audio;
  dn_font_config_t font;
  dn_gpu_config_t gpu;
  dn_asset_config_t asset;
  dn_steam_config_t steam;
  dn_image_config_t image;
  u32 target_fps;
} dn_app_config_t;

typedef struct {
  dn_path_t install_path;
  dn_path_t engine_path;
  dn_path_t write_path;
  dn_path_t app_path;
} dn_app_t;
dn_app_t dn_app;

void dn_app_init(dn_app_descriptor_t descriptor);
void dn_app_configure(dn_app_config_t config);



///////////////////////////////////////////////////////////////////////////
// ██╗   ██╗███╗   ██╗██████╗  ██████╗ ██████╗ ████████╗███████╗██████╗  //
// ██║   ██║████╗  ██║██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗ //
// ██║   ██║██╔██╗ ██║██████╔╝██║   ██║██████╔╝   ██║   █████╗  ██║  ██║ //
// ██║   ██║██║╚██╗██║██╔═══╝ ██║   ██║██╔══██╗   ██║   ██╔══╝  ██║  ██║ //
// ╚██████╔╝██║ ╚████║██║     ╚██████╔╝██║  ██║   ██║   ███████╗██████╔╝ //
//  ╚═════╝ ╚═╝  ╚═══╝╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝  //
///////////////////////////////////////////////////////////////////////////

//
// FLUID
//
  typedef struct {
    u32 next_unprocessed_index;
    u32 grid_size;
  } EulerianFluidSystem;

dn_gen_arena_handle_t lf_create(u32 num_particles);
void lf_destroy(dn_gen_arena_handle_t handle);
void lf_destroy_all();
void lf_init(dn_gen_arena_handle_t handle);
void lf_inspect(dn_gen_arena_handle_t handle);
void lf_set_volume(dn_gen_arena_handle_t handle, float ax, float ay, float bx, float by, float radius);
void lf_set_velocity(dn_gen_arena_handle_t handle, float x, float y);
void lf_set_smoothing_radius(dn_gen_arena_handle_t handle, float r);
void lf_set_particle_mass(dn_gen_arena_handle_t handle, float mass);
void lf_set_viscosity(dn_gen_arena_handle_t handle, float viscosity);
void lf_set_pressure(dn_gen_arena_handle_t handle, float pressure);
void lf_set_gravity(dn_gen_arena_handle_t handle, float gravity);
void lf_set_timestep(dn_gen_arena_handle_t handle, float dt);
void lf_bind(dn_gen_arena_handle_t handle);
void lf_update(dn_gen_arena_handle_t handle);
void lf_draw(dn_gen_arena_handle_t handle);

dn_gen_arena_handle_t ef_create(u32 grid_size);
void ef_destroy(dn_gen_arena_handle_t handle);
void ef_destroy_all();
void ef_init(dn_gen_arena_handle_t handle);
void ef_inspect(dn_gen_arena_handle_t handle);
u32 ef_pair_to_index(u32 grid_size, u32 x, u32 y);
void ef_set_render_size(dn_gen_arena_handle_t handle, u32 size);
void ef_set_velocity(dn_gen_arena_handle_t handle, u32 x, u32 y, float vx, float vy);
void ef_clear_density_source(dn_gen_arena_handle_t handle);
void ef_set_density_source(dn_gen_arena_handle_t handle, u32 x, u32 y, float amount);
void ef_set_gauss_seidel(dn_gen_arena_handle_t handle, u32 iterations);
void ef_bind(dn_gen_arena_handle_t handle);
void ef_update(dn_gen_arena_handle_t handle);
void ef_draw(dn_gen_arena_handle_t handle);


// SCREENSHOTS
void take_screenshot();
void write_screenshot_to_png(const char* file_name);


//
// DRAW
//

typedef struct {
  dn_vector3_t position;
  dn_vector4_t color;
  dn_vector2_t uv;
} Vertex;


void draw_quad(dn_vector2_t position, dn_vector2_t size, dn_vector4_t color);
void draw_line(dn_vector2_t start, dn_vector2_t end, f32 thickness, dn_vector4_t color);
void draw_circle(f32 px, f32 py, f32 radius, dn_vector4_t color);
void draw_circle_sdf(f32 px, f32 py, f32 radius, dn_vector4_t color, f32 edge_thickness);
void draw_ring_sdf(f32 px, f32 py, f32 inner_radius, f32 radius, dn_vector4_t color, f32 edge_thickness);
void draw_image(const char* name, f32 px, f32 py);
void draw_image_size(const char* name, f32 px, f32 py, f32 dx, f32 dy);
void draw_image_ex(const char* name, float px, float py, float dx, float dy, float opacity);
void draw_image_pro(u32 texture, f32 px, f32 py, f32 dx, f32 dy, dn_vector2_t* uv, f32 opacity);
void draw_text(const char* text, f32 px, f32 py);
void draw_text_ex(const char* text, f32 px, f32 py, dn_vector4_t color, const char* font, f32 wrap);
void draw_prepared_text(dn_prepared_text_t* text);

u32 find_texture_handle(const char* name);

//
// INTERPOLATION
//
typedef enum {
  InterpolationFn_Linear,
  InterpolationFn_SmoothDamp
} InterpolationFn;


//
// PARTICLES
//
typedef enum {
  ParticleKind_Quad,
  ParticleKind_Circle,
  ParticleKind_Image,
  ParticleKind_Invalid,
} ParticleKind;

typedef enum {
  ParticlePositionMode_Bottom,
} ParticlePositionMode;

typedef struct {
  i32 index;
  i32 generation;
} ParticleSystemHandle;

typedef struct {
  int spawned;
  int despawned;
  int alive;
} ParticleSystemFrame;

ParticleSystemHandle make_particle_system();
void free_particle_system(ParticleSystemHandle system);
ParticleSystemFrame check_particle_system(ParticleSystemHandle handle);
void stop_particle_emission(ParticleSystemHandle handle);
void start_particle_emission(ParticleSystemHandle handle);
void clear_particles(ParticleSystemHandle handle);
void update_particles(ParticleSystemHandle handle);
void draw_particles(ParticleSystemHandle handle);
void stop_all_particles();

void set_particle_lifetime(ParticleSystemHandle system, float lifetime);
void set_particle_max_spawn(ParticleSystemHandle handle, int max_spawn);
void set_particle_spawn_rate(ParticleSystemHandle handle, float spawn_rate);
void set_particle_size(ParticleSystemHandle handle, float x, float y);
void set_particle_radius(ParticleSystemHandle handle, float r);
void set_particle_sprite(ParticleSystemHandle handle, const char* sprite);
void set_particle_position_mode(ParticleSystemHandle handle, ParticlePositionMode mode);
void set_particle_position(ParticleSystemHandle handle, float x, float y);
void set_particle_area(ParticleSystemHandle handle, float x, float y);
void set_particle_kind(ParticleSystemHandle handle, ParticleKind kind);
void set_particle_color(ParticleSystemHandle handle, float r, float g, float b, float a);
void set_particle_layer(ParticleSystemHandle system, int layer);
void set_particle_velocity_fn(ParticleSystemHandle handle, InterpolationFn function);
void set_particle_velocity_base(ParticleSystemHandle handle, float x, float y);
void set_particle_velocity_max(ParticleSystemHandle handle, float x, float y);
void set_particle_velocity_jitter(ParticleSystemHandle handle, float x, float y);
void set_particle_jitter_base_velocity(ParticleSystemHandle system, bool jitter);
void set_particle_jitter_max_velocity(ParticleSystemHandle system, bool jitter);
void set_particle_size_jitter(ParticleSystemHandle handle, float jitter);
void set_particle_jitter_size(ParticleSystemHandle handle, bool jitter);
void set_particle_opacity_jitter(ParticleSystemHandle handle, float jitter);
void set_particle_jitter_opacity(ParticleSystemHandle handle, bool jitter);
void set_particle_opacity_interpolation(ParticleSystemHandle handle, bool active, float start_time, float interpolate_to);
void set_particle_warm(ParticleSystemHandle system, bool warm);
void set_particle_warmup(ParticleSystemHandle system, i32 warmup);
void set_particle_gravity_source(ParticleSystemHandle handle, float x, float y);
void set_particle_gravity_intensity(ParticleSystemHandle handle, float intensity);
void set_particle_gravity_enabled(ParticleSystemHandle handle, bool enabled);
void set_particle_master_opacity(ParticleSystemHandle handle, float opacity);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ██╗   ██╗███╗   ██╗ ██████╗ █████╗ ████████╗███████╗ ██████╗  ██████╗ ██████╗ ██╗███████╗███████╗██████╗  //
// ██║   ██║████╗  ██║██╔════╝██╔══██╗╚══██╔══╝██╔════╝██╔════╝ ██╔═══██╗██╔══██╗██║╚══███╔╝██╔════╝██╔══██╗ //
// ██║   ██║██╔██╗ ██║██║     ███████║   ██║   █████╗  ██║  ███╗██║   ██║██████╔╝██║  ███╔╝ █████╗  ██║  ██║ //
// ██║   ██║██║╚██╗██║██║     ██╔══██║   ██║   ██╔══╝  ██║   ██║██║   ██║██╔══██╗██║ ███╔╝  ██╔══╝  ██║  ██║ //
// ╚██████╔╝██║ ╚████║╚██████╗██║  ██║   ██║   ███████╗╚██████╔╝╚██████╔╝██║  ██║██║███████╗███████╗██████╔╝ //
//  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝╚══════╝╚══════╝╚═════╝  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dn_lua_add_dir(const char* name);

typedef enum {
  DN_LOG_FLAG_CONSOLE = 1,
  DN_LOG_FLAG_FILE = 2,
  DN_LOG_FLAG_DEFAULT = 3,
} dn_log_flags_t;

//void dn_log(const char* fmt, ...);
//void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...);

]]

ffi = require('ffi')
bit = require('bit')

-- function doublenickel.handle_error(message)
--   -- Strip the message's filename the script filename to make it more readable
--   local parts = split(message, ' ')
--   local path = parts[1]
--   local path_elements = split(path, '/')
--   local filename = path_elements[#path_elements]

--   local message = filename
--   for index = 2, #parts do
--     message = message .. ' ' .. parts[index]
--   end

--   local stack_trace = debug.traceback()
--   stack_trace = stack_trace:gsub('stack traceback:\n', '')
--   stack_trace = stack_trace:gsub('\t', ' ')

--   -- The stack trace contains absolute paths, which are just hard to read. Also, if the path is long, it is
--   -- shortened with "...". Remove the absolute part of the path, including the "..."
--   local install_dir = dn.paths_resolve('install'):to_interned()
--   local escaped_install = install_dir:gsub('%.', '%%.')
--   local last_path_element = install_dir:match("([^/]+)$")
--   local pattern = '%.%.%.(.*)/' .. last_path_element

--   -- Replace the full path first
--   stack_trace = stack_trace:gsub(escaped_install, '')

--   -- Then replace any possible shortened versions with ...
--   local shortened_path_pattern = '[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+'
--   stack_trace = stack_trace:gsub(pattern, '')

--   -- Print
--   local error_message = string.format('lua runtime error:\n\t%s', message)
--   local trace_message = string.format('stack trace:\n%s', stack_trace)

--   doublenickel.debug.last_error = error_message
--   doublenickel.debug.last_trace = trace_message

--   doublenickel.dn_log(error_message)
--   doublenickel.dn_log(trace_message)

--   doublenickel.debug.open_debugger(1)
--   --doublenickel.analytics.submit_crash(error_message, trace_message)

--   return
-- end

function doublenickel.handle_error()
  local stack_trace = debug.traceback()
  stack_trace = stack_trace:gsub('stack traceback:\n', '')
  stack_trace = stack_trace:gsub('\t', '  ')

  -- The stack trace contains absolute paths, which are just hard to read. Also, if the path is long, it is
  -- shortened with "...". Remove the absolute part of the path, including the "..."
  local install_dir = dn.paths_resolve('install'):to_interned()
  local escaped_install = install_dir:gsub('%.', '%%.')
  local last_path_element = install_dir:match("([^/]+)$")
  local pattern = '%.%.%.(.*)/' .. last_path_element

  -- Replace the full path first
  stack_trace = stack_trace:gsub(escaped_install, '')

  -- Then replace any possible shortened versions with ...
  local shortened_path_pattern = '[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+'
  stack_trace = stack_trace:gsub(pattern, '')

  local trace_message = string.format('stack trace:\n%s', stack_trace)
  doublenickel.dn_log(trace_message)

  doublenickel.debug.open_debugger(1)
end

function doublenickel.init_phase_0()
  doublenickel.types = {}
  doublenickel.class = {}
  doublenickel.lifecycle = {}

  doublenickel.entity = {}
  doublenickel.entity.entities = {}
  doublenickel.entity.created_entities = {}
  doublenickel.entity.destroyed_entities = {}
  doublenickel.entity.persistent_entities = {}
  doublenickel.entity.types = {}
  doublenickel.entity.next_id = 1
  doublenickel.persistent = {}

  doublenickel.component = {}
  doublenickel.component.types = {}

  doublenickel.internal = {}
  doublenickel.internal.enum_metatable = {}

  doublenickel.debug = {}

  doublenickel.constants = {}
  doublenickel.enum = {}
  doublenickel.enums = {}
  doublenickel.enum_data = {}

  doublenickel.editor = {}
  doublenickel.editor.types = {}
  doublenickel.editor.sentinel = '__editor'

  doublenickel.save = {}

  doublenickel.state = {}
  doublenickel.state.data = {}

  doublenickel.path_constants = {}

  doublenickel.quests = {}

  doublenickel.scene = {}
  doublenickel.scene.save_data = {}
  doublenickel.current_scene = nil
  doublenickel.queued_scene = nil

  doublenickel.callback = {}
  doublenickel.callback.data = {}

  doublenickel.data_types = {}

  doublenickel.dialogue = {}
  doublenickel.dialogue.node_type = {}
  doublenickel.dialogue.node_kind = {}
  doublenickel.dialogue.sorted_node_kinds = {}
  doublenickel.dialogue.metrics = {
    words = 0,
    nodes = 0,
    dialogues = {}
  }
  doublenickel.dialogue.cache = {}
  doublenickel.dialogue.characters = {}

  doublenickel.audio = {}

  doublenickel.animation = {}
  doublenickel.animation.data = {}

  doublenickel.texture = {}
  doublenickel.texture.data = {}

  doublenickel.background = {}
  doublenickel.background.data = {}

  doublenickel.input = {}
  doublenickel.input.data = {}

  doublenickel.physics = {}
  doublenickel.physics.requests = {}
  doublenickel.physics.debug = false

  doublenickel.interaction = {}
  doublenickel.interaction.callbacks = {}
  doublenickel.interaction.check_flag = false

  doublenickel.interpolation = {}

  doublenickel.gui = {}
  doublenickel.gui.animation = {}
  doublenickel.gui.scroll = {}
  doublenickel.gui.drag = {}
  doublenickel.gui.menu = {}

  doublenickel.window = {}

  doublenickel.action = {}
  doublenickel.action.event_kind = {}

  doublenickel.analytics = {}

  doublenickel.fonts = {}

  doublenickel.module = {}

  doublenickel.paths = {}

  doublenickel.shaders = {}

  doublenickel.gpu = {}

  doublenickel.app = {}

  doublenickel.iterator = {}

  doublenickel.time_metric = {}

  doublenickel.subsystem = {}
  doublenickel.subsystem.types = {}

  doublenickel.math = {}

  doublenickel.filesystem = {}

  doublenickel.asset = {}

  doublenickel.draw = {}
  doublenickel.draw.internal = {}

  doublenickel.dt = 0
  doublenickel.elapsed_time = 0
  doublenickel.frame = 0

  doublenickel.tick = doublenickel.is_packaged_build
  doublenickel.next_tick = doublenickel.tick

  imgui = {}
  imgui.extensions = {}
  imgui.internal = {}

  -- Bootstrap the FFI, so that we can resolve paths
  doublenickel.ffi = {}
  dn = {}
  ffi.cdef(ffi_header)
  
  -- Run a sanity check on the compiled size of all types and what the parser inside LuaJIT generates.
  -- This is nothing more than a sanity check; i.e. we don't do anything besides bail violently if
  -- we see a mismatch.
  --
  -- The only reason this is here is because I *have* seen MSVC generate a different size than what 
  -- the FFI reports. See the types in the header which are force aligned to 16 bytes.
  local type_mismatch = false
  local type_infos = ffi.C.dn_engine_query_types()
  for i = 0, type_infos.count - 1 do
    local type_info = type_infos.data[i]
    local ffi_size = ffi.sizeof(ffi.string(type_info.name))
    
    if ffi_size ~= type_info.size then
      type_mismatch = true
      print(string.format(
        'Found type mismatch; type = %s, ffi_size = %d, compiled_size = %d',
        ffi.string(type_info.name),
        ffi_size, type_info.size)
      )
    end
  end
  if type_mismatch then 
    ffi.C.dn_engine_set_exit_game() -- Technically useless, since the game will explode without bootstrapping
    return 
  end

  -- Bootstrap the engine paths, so we can load the rest of the scripts
  local function collect_paths(paths, full_parent)
    local collected_paths = {}
    if not paths then return collected_paths end
    
    full_parent = full_parent or ''

    for name, data in pairs(paths) do
      local full_path = ''
      if type(data) == 'string' then
        full_path = full_parent .. data
        goto done
      else 
        full_path = full_parent .. data.path
      end
  
      if data.children then
        local child_paths = collect_paths(data.children, full_path .. '/')
        for index, path in pairs(child_paths) do
          table.insert(collected_paths, path)
        end
      end

      ::done::
      table.insert(collected_paths, {
        name = name,
        path = full_path
      })
    end
  
    return collected_paths
  end
  
  local file_path = ffi.string(ffi.C.dn_paths_resolve('dn_install').data)
  local path_info = dofile(file_path)

  local dn_install_paths = collect_paths(path_info.dn_install)
  for index, path in pairs(dn_install_paths) do
    ffi.C.dn_paths_add_engine_subpath(path.name, path.path)
  end

  local app_paths = collect_paths(path_info.app_paths)
  for index, path in pairs(app_paths) do
    ffi.C.dn_paths_add_subpath(path.name, 'app', path.path)
  end

  local write_paths = collect_paths(path_info.write_paths)
  for index, path in pairs(write_paths) do
    ffi.C.dn_paths_add_write_subpath(path.name, path.path)
  end

  -- We need a couple of files to even be able to load other files (since they
  -- define classes and enums and such). Order matters here. I consider this to be
  -- the Lua equivalent of the ordered includes in main.cpp
  --
  -- We technically load these files twice, but all files are idempotent...
  local loader = {
    'reflect.lua',
    'enum.lua',
    'class.lua',
    'math.lua'
  }

  for _, file_name in pairs(loader) do
    local file_path = ffi.string(ffi.C.dn_paths_resolve_format('engine_script', file_name).data)
    dofile(file_path)
  end


end

function doublenickel.init_phase_1()
  doublenickel.enum.init()
  doublenickel.ffi.init()
  doublenickel.paths.init()
  doublenickel.math.init()
  doublenickel.time_metric.init()
  doublenickel.input.init()
  doublenickel.gpu.init()
  doublenickel.state.init()
  doublenickel.animation.load()
  doublenickel.texture.load()
  doublenickel.background.load()
  doublenickel.dialogue.init()
  doublenickel.audio.init()
  doublenickel.gui.init()
  doublenickel.scene.init()
  doublenickel.asset.init()
end

function doublenickel.init_phase_2()
  doublenickel.subsystem.init()
  doublenickel.app = doublenickel.subsystem.find('App')

  doublenickel.dn_log('App:on_init_game')
  doublenickel.app:on_init_game()

  doublenickel.window.init()
  doublenickel.save.init()
  doublenickel.editor.init()
  doublenickel.persistent.init()

  doublenickel.dn_log('lifecycle.on_start_game')
  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_start_game)
end

function doublenickel.dn_log(...)
  print(string.format(...))
end
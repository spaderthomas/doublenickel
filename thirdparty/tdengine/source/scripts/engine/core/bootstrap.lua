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
} Vector2;

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct {
  float data [2] [2];
} Matrix2;

typedef struct {
  float data [3] [3];
} Matrix3;

typedef struct {
  float data [4] [4];
} Matrix4;

// These are just in Lua, so I can add a metatype to returned strings for a nice interning API.
// tstring is a temporary string; i.e., one allocated with temporary storage. There's nothing different
// about the underlying data, it's just a hint to the programmer to not hold onto this pointer.
typedef struct {
  char* data;
} tstring;

typedef struct {
  char* data;
} string;




//////////////////////////////////////////////////////
// ███████╗███╗   ██╗ ██████╗ ██╗███╗   ██╗███████╗ //
// ██╔════╝████╗  ██║██╔════╝ ██║████╗  ██║██╔════╝ //
// █████╗  ██╔██╗ ██║██║  ███╗██║██╔██╗ ██║█████╗   //
// ██╔══╝  ██║╚██╗██║██║   ██║██║██║╚██╗██║██╔══╝   //
// ███████╗██║ ╚████║╚██████╔╝██║██║ ╚████║███████╗ //
// ╚══════╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝╚═╝  ╚═══╝╚══════╝ //
//////////////////////////////////////////////////////

void        dn_engine_set_exit_game();
const char* dn_engine_get_game_hash();
void        dn_engine_set_target_fps(double fps);
double      dn_engine_get_target_fps();
bool        dn_engine_exceeded_frame_time();
bool        dn_engine_should_exit();

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
tstring                dn_paths_resolve(const char* name);
tstring                dn_paths_resolve_format(const char* name, const char* file_name);



///////////////////////
//  ██████╗ ███████╗ //
// ██╔═══██╗██╔════╝ //
// ██║   ██║███████╗ //
// ██║   ██║╚════██║ //
// ╚██████╔╝███████║ //
//  ╚═════╝ ╚══════╝ //
///////////////////////
 
typedef struct dn_os_date_time_t {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;
} dn_os_date_time_t;

bool              dn_os_does_path_exist(const char* path);
bool              dn_os_is_regular_file(const char* path);
bool              dn_os_is_directory(const char* path);
void              dn_os_create_directory(const char* path);
void              dn_os_remove_directory(const char* path);
dn_os_date_time_t dn_os_get_date_time();

typedef struct dn_allocator_t dn_allocator_t;

void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
dn_allocator_t*  dn_allocator_find(const char* name);
void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);


///////////////////////////////////////////////////////////////////////////////////////
//  ██████╗ ██████╗ ███╗   ██╗████████╗ █████╗ ██╗███╗   ██╗███████╗██████╗ ███████╗ //
// ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██║████╗  ██║██╔════╝██╔══██╗██╔════╝ //
// ██║     ██║   ██║██╔██╗ ██║   ██║   ███████║██║██╔██╗ ██║█████╗  ██████╔╝███████╗ //
// ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██║██║██║╚██╗██║██╔══╝  ██╔══██╗╚════██║ //
// ╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║██║██║ ╚████║███████╗██║  ██║███████║ //
//  ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚══════╝ //
///////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_dynamic_array_header_t;

void*                      dn_dynamic_array_alloc(u32 element_size, dn_allocator_t* allocator);
void                       dn_dynamic_array_push_n(void** array, void* data, u32 num_elements);
void*                      dn_dynamic_array_reserve(void** array, u32 num_elements);
dn_dynamic_array_header_t* dn_dynamic_array_head(void** array);
u32                        dn_dynamic_array_size(void** array);
u32                        dn_dynamic_array_capacity(void** array);
u32                        dn_dynamic_array_element_size(void** array);
dn_allocator_t*            dn_dynamic_array_allocator(void** array);
bool                       dn_dynamic_array_full(void** array);
bool                       dn_dynamic_array_need_grow(void** array, u32 num_elements);
void                       dn_dynamic_array_grow(void** array, u32 requested_size);
u32                        dn_dynamic_array_byte_size(void** array);

typedef struct {
	u8* data;
	u32 size;
	u32 capacity;

	u32 vertex_size;
} dn_fixed_array_t;

void dn_fixed_array_init(dn_fixed_array_t* vertex_buffer, u32 max_vertices, u32 vertex_size);
u8*  dn_fixed_array_push(dn_fixed_array_t* vertex_buffer, void* data, u32 count);
u8*  dn_fixed_array_reserve(dn_fixed_array_t* vertex_buffer, u32 count);
void dn_fixed_array_clear(dn_fixed_array_t* vertex_buffer);
u32  dn_fixed_array_byte_size(dn_fixed_array_t* vertex_buffer);
u8*  dn_fixed_array_at(dn_fixed_array_t* vertex_buffer, u32 index);

typedef struct {
	i32 index;
	i32 generation;
} dn_gen_arena_handle_t;

void copy_string(const char* str, char* buffer, u32 buffer_length);
void copy_string_n(const char* str, u32 length, char* buffer, u32 buffer_length);


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
} dn_compressor_t;

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
	Vector2 camera;
	Vector2 framebuffer_position;
	Vector2 framebuffer_size;
} dn_coord_data_t;
dn_coord_data_t dn_coord_data;

dn_coord_data_t dn_coord_get();
void            dn_coord_set_camera(float x, float y);
void            dn_coord_set_framebuffer_position(float x, float y);
void            dn_coord_set_framebuffer_size(float x, float y);
Vector2         dn_coord_screen_to_window(float x, float y);
Vector2         dn_coord_screen_to_game(float x, float y);
Vector2         dn_coord_screen_to_world(float x, float y);
Vector2         dn_coord_window_to_screen(float x, float y);
Vector2         dn_coord_window_to_game(float x, float y);
Vector2         dn_coord_window_to_world(float x, float y);
Vector2         dn_coord_game_to_screen(float x, float y);
Vector2         dn_coord_game_to_window(float x, float y);
Vector2         dn_coord_game_to_world(float x, float y);
Vector2         dn_coord_world_to_screen(float x, float y);
Vector2         dn_coord_world_to_window(float x, float y);
Vector2         dn_coord_world_to_game(float x, float y);
Vector2         dn_coord_screen_to_window_mag(float x, float y);
Vector2         dn_coord_screen_to_game_mag(float x, float y);
Vector2         dn_coord_screen_to_world_mag(float x, float y);
Vector2         dn_coord_window_to_screen_mag(float x, float y);
Vector2         dn_coord_window_to_game_mag(float x, float y);
Vector2         dn_coord_window_to_world_mag(float x, float y);
Vector2         dn_coord_game_to_screen_mag(float x, float y);
Vector2         dn_coord_game_to_window_mag(float x, float y);
Vector2         dn_coord_game_to_world_mag(float x, float y);
Vector2         dn_coord_world_to_screen_mag(float x, float y);
Vector2         dn_coord_world_to_window_mag(float x, float y);
Vector2         dn_coord_world_to_game_mag(float x, float y);

typedef enum {
	DN_INPUT_DEVICE_MOUSE_AND_KEYBOARD = 0,
	DN_INPUT_DEVICE_CONTROLLER = 1,
} dn_input_device_t;

bool              dn_input_pressed(int key);
bool              dn_input_released(int key);
bool              dn_input_down(int key);
bool              dn_input_mod_down(int mod);
bool              dn_input_chord_pressed(int mod, int key);
Vector2           dn_input_scroll();
Vector2           dn_input_mouse(dn_coord_t unit);
Vector2           dn_input_mouse_delta(dn_coord_t unit);
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
	Vector2 position;
	Vector2 padding;
  Vector4 color;
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
dn_prepared_text_t* dn_prepare_text_ex(const char* text, f32 px, f32 py, const char* font, f32 wrap, Vector4 color, bool precise);
void dn_font_create(const char* id, const char* family, u32 size);
void dn_font_add_to_imgui(const char* id);



////////////////////////////////////////////////////////
// ██╗    ██╗██╗███╗   ██╗██████╗  ██████╗ ██╗    ██╗ //
// ██║    ██║██║████╗  ██║██╔══██╗██╔═══██╗██║    ██║ //
// ██║ █╗ ██║██║██╔██╗ ██║██║  ██║██║   ██║██║ █╗ ██║ //
// ██║███╗██║██║██║╚██╗██║██║  ██║██║   ██║██║███╗██║ //
// ╚███╔███╔╝██║██║ ╚████║██████╔╝╚██████╔╝╚███╔███╔╝ //
//  ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝╚═════╝  ╚═════╝  ╚══╝╚══╝  //
////////////////////////////////////////////////////////

typedef enum {
	DisplayMode_p480,
	DisplayMode_p720,
	DisplayMode_p1080,
	DisplayMode_p1440,
	DisplayMode_p2160,
	DisplayMode_p1280_800,
	DisplayMode_FullScreen
} DisplayMode;

typedef struct {
	Vector2 camera;
	Vector2 framebuffer_position;
	Vector2 framebuffer_size;
} dn_coord_data_t;

dn_coord_data_t  coord_get();
void         coord_set_camera(float x, float y);
void         coord_set_framebuffer_position(float x, float y);
void         coord_set_framebuffer_size(float x, float y);

void create_window(const char* title, u32 x, u32 y, u32 flags);
void set_window_icon(const char* file_path);
void set_window_size(int x, int y);
Vector2 get_content_area();
Vector2 get_native_resolution();
void set_display_mode(u32 mode);
u32 get_display_mode();
void dn_window_set_cursor_visible(bool visible);
void use_editor_layout(const char* file_name);
void save_editor_layout(const char* file_name);





////////////////////////////////
//  ██████╗ ██████╗ ██╗   ██╗ //
// ██╔════╝ ██╔══██╗██║   ██║ //
// ██║  ███╗██████╔╝██║   ██║ //
// ██║   ██║██╔═══╝ ██║   ██║ //
// ╚██████╔╝██║     ╚██████╔╝ //
//  ╚═════╝ ╚═╝      ╚═════╝  //
////////////////////////////////
                          

typedef struct GpuShader GpuShader;
typedef struct GpuBuffer GpuBuffer;
typedef struct GpuVertexLayout GpuVertexLayout;

typedef enum {
  GPU_COMMAND_OP_BIND_BUFFERS = 10,
  GPU_COMMAND_OP_BEGIN_RENDER_PASS = 20,
  GPU_COMMAND_OP_END_RENDER_PASS = 21,
  GPU_COMMAND_OP_BIND_PIPELINE = 30,
  GPU_COMMAND_OP_SET_CAMERA = 40,
  GPU_COMMAND_OP_SET_LAYER = 41,
  GPU_COMMAND_OP_SET_WORLD_SPACE = 42,
  GPU_COMMAND_OP_SET_SCISSOR = 43,
  GPU_COMMAND_OP_DRAW = 70,
} GpuCommandOp;

typedef enum {
  GPU_PRIMITIVE_TRIANGLES = 0
} GpuDrawPrimitive;

typedef enum {
  GPU_DRAW_MODE_ARRAYS = 0,
  GPU_DRAW_MODE_INSTANCE = 1,
} GpuDrawMode;

typedef enum {
	GPU_VERTEX_ATTRIBUTE_FLOAT = 0,
	GPU_VERTEX_ATTRIBUTE_U32 = 1,
} GpuVertexAttributeKind;

typedef enum {
  GPU_UNIFORM_NONE = 0,
	GPU_UNIFORM_MATRIX4 = 1,
	GPU_UNIFORM_MATRIX3 = 2,
	GPU_UNIFORM_MATRIX2 = 3,
	GPU_UNIFORM_VECTOR4 = 4,
	GPU_UNIFORM_VECTOR3 = 5,
	GPU_UNIFORM_VECTOR2 = 6,
	GPU_UNIFORM_I32 = 7,
	GPU_UNIFORM_F32 = 8,
	GPU_UNIFORM_TEXTURE = 9,
	GPU_UNIFORM_ENUM = 10,
} GpuUniformKind;

typedef enum {
	GPU_BUFFER_KIND_STORAGE = 0,
	GPU_BUFFER_KIND_ARRAY = 1,
} GpuBufferKind;

typedef enum {
	GPU_BUFFER_USAGE_STATIC = 0,
	GPU_BUFFER_USAGE_DYNAMIC = 1,
	GPU_BUFFER_USAGE_STREAM = 2,
} GpuBufferUsage;

typedef enum {
  GPU_BLEND_FUNC_NONE,
  GPU_BLEND_FUNC_ADD,
  GPU_BLEND_FUNC_SUBTRACT,
  GPU_BLEND_FUNC_REVERSE_SUBTRACT,
  GPU_BLEND_FUNC_MIN,
  GPU_BLEND_FUNC_MAX
} GpuBlendFunction;

typedef enum {
	GPU_BLEND_MODE_ZERO,
	GPU_BLEND_MODE_ONE,
	GPU_BLEND_MODE_SRC_COLOR,
	GPU_BLEND_MODE_ONE_MINUS_SRC_COLOR,
	GPU_BLEND_MODE_DST_COLOR,
	GPU_BLEND_MODE_ONE_MINUS_DST_COLOR,
	GPU_BLEND_MODE_SRC_ALPHA,
	GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA,
	GPU_BLEND_MODE_DST_ALPHA,
	GPU_BLEND_MODE_ONE_MINUS_DST_ALPHA,
	GPU_BLEND_MODE_CONSTANT_COLOR,
	GPU_BLEND_MODE_ONE_MINUS_CONSTANT_COLOR,
	GPU_BLEND_MODE_CONSTANT_ALPHA,
	GPU_BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA,
	GPU_BLEND_MODE_SRC_ALPHA_SATURATE,
	GPU_BLEND_MODE_SRC1_COLOR,
	GPU_BLEND_MODE_ONE_MINUS_SRC1_COLOR,
	GPU_BLEND_MODE_SRC1_ALPHA,
	GPU_BLEND_MODE_ONE_MINUS_SRC1_ALPHA
} GpuBlendMode;

typedef enum {
	GPU_LOAD_OP_NONE = 0,
	GPU_LOAD_OP_CLEAR = 1
} GpuLoadOp;

typedef enum {
	GPU_RESOURCE_FRAMEBUFFER = 0,
	GPU_RESOURCE_SHADER = 1,
	GPU_RESOURCE_PROGRAM = 2,
} GpuResourceId;

typedef enum {
	GPU_MEMORY_BARRIER_STORAGE = 0,
	GPU_MEMORY_BARRIER_BUFFER_UPDATE = 1,
} GpuMemoryBarrier;

typedef enum {
	GPU_SHADER_GRAPHICS = 0,
	GPU_SHADER_COMPUTE = 0,
} GpuShaderKind;


////////////
// SHADER //
////////////
typedef struct {
	const char* name;
	const char* vertex_shader;
	const char* fragment_shader;
	const char* compute_shader;

	GpuShaderKind kind;
} GpuShaderDescriptor;


//////////////
// UNIFORMS //
//////////////
typedef union {
  Matrix4 mat4;
  Matrix3 mat3;
  Matrix2 mat2;
  Vector4 vec4;
  Vector3 vec3;
  Vector2 vec2;
  float f32;
  i32 texture;
  i32 i32;
} GpuUniformData;

typedef struct {
  char name [64];
  GpuUniformKind kind;
} GpuUniformDescriptor;

typedef struct {
  char name [64];
  GpuUniformKind kind;
} GpuUniform;


/////////////////
// GPU BUFFERS //
/////////////////
typedef struct {
  char name [64];
	GpuBufferKind kind;
  GpuBufferUsage usage;
	u32 capacity;
  u32 element_size;
} GpuBufferDescriptor;

struct GpuBuffer {
  char name [64];
	GpuBufferKind kind;
  GpuBufferUsage usage;
	u32 size;
	u32 handle;
};

typedef struct {
  dn_fixed_array_t buffer;
  GpuBuffer* gpu_buffer;
} GpuBackedBuffer;


///////////////////////
// GPU RENDER TARGET //
///////////////////////
typedef struct {
	Vector2 size;
} GpuRenderTargetDescriptor;

typedef struct {
	u32 handle;
	u32 color_buffer;
	Vector2 size;
} GpuRenderTarget;


/////////////////////
// GPU RENDER PASS //
/////////////////////
typedef struct {
  struct {
    GpuLoadOp load;
    GpuRenderTarget* attachment;
  } color;
} GpuRenderPass;


////////////////////////
// GPU BUFFER BINDING //
////////////////////////
typedef struct {
  GpuBuffer* buffer;
} GpuVertexBufferBinding;

typedef struct {
  GpuBuffer* buffer;
  u32 base;
} GpuStorageBufferBinding;

typedef struct {
  GpuUniformData data;
  GpuUniform* uniform;
  u32 binding_index;
} GpuUniformBinding;

typedef struct {
  __declspec(align(16)) struct {
    GpuVertexBufferBinding bindings [8];
    u32 count;
  } vertex;

  __declspec(align(16)) struct {
    GpuUniformBinding bindings [8];
    u32 count;
  } uniforms;

  __declspec(align(16)) struct {
    GpuStorageBufferBinding bindings [8];
    u32 count;
  } storage;
} GpuBufferBinding;


//////////////////
// GPU PIPELINE //
//////////////////
typedef struct {
  GpuBlendFunction fn;
  GpuBlendMode source;
  GpuBlendMode destination;
} GpuBlendState;

typedef struct {
  GpuShader* shader;
  GpuDrawPrimitive primitive;
} GpuRasterState;

typedef struct {
  Vector2 position;
  Vector2 size;
  bool enabled;
} GpuScissorState;

typedef struct {
  u32 layer;
  bool world_space;
  Vector2 camera;
} GpuRendererState;

typedef struct {
	GpuVertexAttributeKind kind;
	u32 count;
	u32 divisor;
} GpuVertexAttribute;

typedef struct {
	GpuVertexAttribute vertex_attributes [8];
	u32 num_vertex_attributes;
} GpuBufferLayout;

typedef struct {
  GpuBlendState blend;
  GpuRasterState raster;
	GpuBufferLayout buffer_layouts [8];
	u32 num_buffer_layouts;
} GpuPipelineDescriptor;

typedef struct {
  GpuBlendState blend;
  GpuRasterState raster;
	GpuBufferLayout buffer_layouts [8];
	u32 num_buffer_layouts;
} GpuPipeline;



////////////////////////
// GPU COMMAND BUFFER //
////////////////////////
typedef struct {
  GpuDrawMode mode;
  u32 vertex_offset;
  u32 num_vertices;
  u32 num_instances;
} GpuDrawCall;

typedef struct GpuCommandBuffer GpuCommandBuffer;

typedef struct {
  u32 max_commands;
} GpuCommandBufferDescriptor;


///////////////////
// GPU FUNCTIONS //
///////////////////
GpuCommandBuffer*  gpu_command_buffer_create(GpuCommandBufferDescriptor descriptor);
void               gpu_command_buffer_draw(GpuCommandBuffer* command_buffer, GpuDrawCall draw_call);
void               gpu_command_buffer_submit(GpuCommandBuffer* command_buffer);
void               gpu_bind_pipeline(GpuCommandBuffer* command_buffer, GpuPipeline* pipeline);
void               gpu_begin_render_pass(GpuCommandBuffer* command_buffer, GpuRenderPass render_pass);
void               gpu_end_render_pass(GpuCommandBuffer* command_buffer);
void               gpu_apply_bindings(GpuCommandBuffer* command_buffer, GpuBufferBinding bindings);
void               gpu_bind_render_state(GpuCommandBuffer* command_buffer, GpuRendererState render);
void               gpu_set_layer(GpuCommandBuffer* command_buffer, u32 layer);
void               gpu_set_world_space(GpuCommandBuffer* command_buffer, bool world_space);
void               gpu_set_camera(GpuCommandBuffer* command_buffer, Vector2 camera);
GpuPipeline*       gpu_pipeline_create(GpuPipelineDescriptor descriptor);
GpuUniform*        gpu_uniform_create(GpuUniformDescriptor descriptor);
GpuBuffer*         gpu_buffer_create(GpuBufferDescriptor descriptor);
void               gpu_buffer_bind(GpuBuffer* buffer);
void               gpu_buffer_bind_base(GpuBuffer* buffer, u32 base);
void               gpu_buffer_sync(GpuBuffer* buffer, void* data, u32 size);
void               gpu_buffer_sync_subdata(GpuBuffer* buffer, void* data, u32 byte_size, u32 byte_offset);
void               gpu_buffer_zero(GpuBuffer* buffer, u32 size);
GpuBackedBuffer    gpu_backed_buffer_create(GpuBufferDescriptor descriptor);
u32                gpu_backed_buffer_size(GpuBackedBuffer* buffer);
void               gpu_backed_buffer_clear(GpuBackedBuffer* buffer);
u8*                gpu_backed_buffer_push(GpuBackedBuffer* buffer, void* data, u32 num_elements);
void               gpu_backed_buffer_sync(GpuBackedBuffer* buffer);
GpuShader*         gpu_shader_create(GpuShaderDescriptor descriptor);
GpuShader*         gpu_shader_find(const char* name);
GpuRenderTarget*   gpu_render_target_create(GpuRenderTargetDescriptor descriptor);
GpuRenderTarget*   gpu_acquire_swapchain();
void               gpu_render_target_bind(GpuRenderTarget* target);
void               gpu_render_target_clear(GpuRenderTarget* target);
void               gpu_render_target_blit(GpuRenderTarget* source, GpuRenderTarget* destination);
void               gpu_memory_barrier(GpuMemoryBarrier barrier);
void               gpu_dispatch_compute(GpuBuffer* buffer, u32 size);
void               gpu_swap_buffers();
void               gpu_error_clear();
tstring            gpu_error_read();
void               gpu_error_log_one();
void               gpu_error_log_all();
void               gpu_set_resource_name(GpuResourceId id, u32 handle, u32 name_len, const char* name);



//////////////////////////////
// ███████╗██████╗ ███████╗ //
// ██╔════╝██╔══██╗██╔════╝ //
// ███████╗██║  ██║█████╗   //
// ╚════██║██║  ██║██╔══╝   //
// ███████║██████╔╝██║      //
// ╚══════╝╚═════╝ ╚═╝      //
//////////////////////////////

typedef enum {
  SDF_SHAPE_CIRCLE = 0,
  SDF_SHAPE_RING = 1,
  SDF_SHAPE_BOX = 2,
  SDF_SHAPE_ORIENTED_BOX = 3,
  SDF_SHAPE_COMBINE = 100,
} SdfShape;

typedef enum {
  SDF_COMBINE_OP_UNION = 0,
  SDF_COMBINE_OP_INTERSECTION = 1,
  SDF_COMBINE_OP_SUBTRACTION = 2,
} SdfCombineOp;

typedef enum {
  SDF_SMOOTH_KERNEL_NONE = 0,
  SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC = 1,
} SdfSmoothingKernel;
 
typedef enum {
  SDF_RENDERER_STATE_NONE,
  SDF_RENDERER_STATE_COMBINATION,
} SdfRendererState;


/////////////////////
// SDF BUFFER DATA //
/////////////////////
typedef struct {
  Vector2 position;
  Vector2 uv;
} SdfVertex;

typedef struct {
  SdfShape shape;
  u32 buffer_index;
} SdfInstance;

typedef struct {
  u32 num_sdfs;
} SdfCombineHeader;

typedef struct {
  u32 buffer_index;
  u32 kind;
  SdfCombineOp op;
  SdfSmoothingKernel kernel;
} SdfCombineEntry;


////////////////////
// SDF SHAPE DATA //
////////////////////
typedef struct {
  Vector3 color;
  Vector2 position;
  float rotation;
  float edge_thickness;
  SdfShape shape;
} SdfHeader;

typedef struct {
  SdfHeader header;
  float radius;
} SdfCircle;

typedef struct {
  SdfHeader header;
  float inner_radius;
  float outer_radius;
} SdfRing;

typedef struct {
  SdfHeader header;
  Vector2 size;
} SdfBox;

typedef struct {
  SdfHeader header;
  Vector2 size;
} SdfOrientedBox;

typedef struct {
  SdfRendererState state;
  GpuBackedBuffer vertices;
  GpuBackedBuffer instances;
  GpuBackedBuffer combinations;
  GpuBackedBuffer shape_data;
  GpuBufferBinding bindings;
  GpuPipeline* pipeline;
} SdfRenderer;

SdfRenderer       sdf_renderer_create(u32 buffer_size);
void              sdf_renderer_draw(SdfRenderer* renderer, GpuCommandBuffer* command_buffer);
void              sdf_renderer_push_instance(SdfRenderer* renderer, SdfShape shape);
void              sdf_renderer_push_header(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness);
SdfCombineHeader* sdf_combination_begin(SdfRenderer* renderer);
void              sdf_combination_append(SdfRenderer* renderer, SdfCombineHeader* header, SdfShape shape, SdfCombineOp op, SdfSmoothingKernel kernel);
void              sdf_combination_commit(SdfRenderer* renderer);
void              sdf_circle_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius);
void              sdf_ring_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius);
void              sdf_oriented_box_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy);
void              sdf_grid(SdfRenderer* renderer, u32 grid_width, u32 grid_size);


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

f64 perlin(f64 x, f64 y, f64 vmin, f64 vmax);




///////////////////////////////////////////////////////////////////////////
// ██╗   ██╗███╗   ██╗██████╗  ██████╗ ██████╗ ████████╗███████╗██████╗  //
// ██║   ██║████╗  ██║██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗ //
// ██║   ██║██╔██╗ ██║██████╔╝██║   ██║██████╔╝   ██║   █████╗  ██║  ██║ //
// ██║   ██║██║╚██╗██║██╔═══╝ ██║   ██║██╔══██╗   ██║   ██╔══╝  ██║  ██║ //
// ╚██████╔╝██║ ╚████║██║     ╚██████╔╝██║  ██║   ██║   ███████╗██████╔╝ //
//  ╚═════╝ ╚═╝  ╚═══╝╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═════╝	 //
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

// STEAM
void open_steam_page(const char* utm);
bool is_steam_deck();
void show_text_input(const char* description, const char* existing_text);
bool is_text_input_dirty();
const char* read_text_input();

//
// DRAW
//

typedef struct {
  Vector3 position;
  Vector4 color;
  Vector2 uv;
} Vertex;


void draw_quad(Vector2 position, Vector2 size, Vector4 color);
void draw_line(Vector2 start, Vector2 end, f32 thickness, Vector4 color);
void draw_circle(f32 px, f32 py, f32 radius, Vector4 color);
void draw_circle_sdf(f32 px, f32 py, f32 radius, Vector4 color, f32 edge_thickness);
void draw_ring_sdf(f32 px, f32 py, f32 inner_radius, f32 radius, Vector4 color, f32 edge_thickness);
void draw_image(const char* name, f32 px, f32 py);
void draw_image_size(const char* name, f32 px, f32 py, f32 dx, f32 dy);
void draw_image_ex(const char* name, float px, float py, float dx, float dy, float opacity);
void draw_image_pro(u32 texture, f32 px, f32 py, f32 dx, f32 dy, Vector2* uv, f32 opacity);
void draw_text(const char* text, f32 px, f32 py);
void draw_text_ex(const char* text, f32 px, f32 py, Vector4 color, const char* font, f32 wrap);
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
void IGE_PushGameFont(const char* font_name);
void IGE_GameImage(const char* image, float sx, float sy);
void IGE_OpenFileBrowser();
void IGE_CloseFileBrowser();
void IGE_SetFileBrowserWorkDir(const char* directory);
bool IGE_IsAnyFileSelected();
tstring IGE_GetSelectedFile();

void add_script_directory(const char* name);

]]

ffi = require('ffi')

function tdengine.handle_error(message)
  -- Strip the message's filename the script filename to make it more readable
  local parts = split(message, ' ')
  local path = parts[1]
  local path_elements = split(path, '/')
  local filename = path_elements[#path_elements]

  local message = filename
  for index = 2, #parts do
    message = message .. ' ' .. parts[index]
  end

  local stack_trace = debug.traceback()
  stack_trace = stack_trace:gsub('stack traceback:\n', '')
  stack_trace = stack_trace:gsub('\t', '	')

  -- The stack trace contains absolute paths, which are just hard to read. Also, if the path is long, it is
  -- shortened with "...". Remove the absolute part of the path, including the "..."
  local install_dir = tdengine.ffi.dn_paths_resolve('install'):to_interned()
  local escaped_install = install_dir:gsub('%.', '%%.')
  local last_path_element = install_dir:match("([^/]+)$")
  local pattern = '%.%.%.(.*)/' .. last_path_element

  -- Replace the full path first
  stack_trace = stack_trace:gsub(escaped_install, '')

  -- Then replace any possible shortened versions with ...
  local shortened_path_pattern = '[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+%.[^%.]+'
  stack_trace = stack_trace:gsub(pattern, '')

  -- Print
  local error_message = string.format('lua runtime error:\n\t%s', message)
  local trace_message = string.format('stack trace:\n%s', stack_trace)

  tdengine.debug.last_error = error_message
  tdengine.debug.last_trace = trace_message

  tdengine.log(error_message)
  tdengine.log(trace_message)

  tdengine.debug.open_debugger(1)
  --tdengine.analytics.submit_crash(error_message, trace_message)

  return
end

function tdengine.init_phase_0()
  tdengine.types = {}
  tdengine.class = {}
  tdengine.lifecycle = {}

  tdengine.entity = {}
  tdengine.entity.entities = {}
  tdengine.entity.created_entities = {}
  tdengine.entity.destroyed_entities = {}
  tdengine.entity.persistent_entities = {}
  tdengine.entity.types = {}
  tdengine.entity.next_id = 1
  tdengine.persistent = {}

  tdengine.component = {}
  tdengine.component.types = {}

  tdengine.internal = {}
  tdengine.internal.enum_metatable = {}

  tdengine.debug = {}

  tdengine.constants = {}
  tdengine.enum = {}
  tdengine.enums = {}
  tdengine.enum_data = {}

  tdengine.editor = {}
  tdengine.editor.types = {}
  tdengine.editor.sentinel = '__editor'

  tdengine.save = {}

  tdengine.state = {}
  tdengine.state.data = {}

  tdengine.path_constants = {}

  tdengine.quests = {}

  tdengine.scene = {}
  tdengine.scene.save_data = {}
  tdengine.current_scene = nil
  tdengine.queued_scene = nil

  tdengine.callback = {}
  tdengine.callback.data = {}

  tdengine.data_types = {}

  tdengine.dialogue = {}
  tdengine.dialogue.node_type = {}
  tdengine.dialogue.node_kind = {}
  tdengine.dialogue.sorted_node_kinds = {}
  tdengine.dialogue.metrics = {
    words = 0,
    nodes = 0,
    dialogues = {}
  }
  tdengine.dialogue.cache = {}
  tdengine.dialogue.characters = {}

  tdengine.audio = {}

  tdengine.animation = {}
  tdengine.animation.data = {}

  tdengine.texture = {}
  tdengine.texture.data = {}

  tdengine.background = {}
  tdengine.background.data = {}

  tdengine.input = {}
  tdengine.input.data = {}

  tdengine.physics = {}
  tdengine.physics.requests = {}
  tdengine.physics.debug = false

  tdengine.interaction = {}
  tdengine.interaction.callbacks = {}
  tdengine.interaction.check_flag = false

  tdengine.interpolation = {}

  tdengine.gui = {}
  tdengine.gui.animation = {}
  tdengine.gui.scroll = {}
  tdengine.gui.drag = {}
  tdengine.gui.menu = {}

  tdengine.steam = {}

  tdengine.window = {}

  tdengine.action = {}
  tdengine.action.event_kind = {}

  tdengine.analytics = {}

  tdengine.fonts = {}

  tdengine.module = {}

  tdengine.paths = {}

  tdengine.shaders = {}

  tdengine.gpu = {}

  tdengine.app = {}

  tdengine.iterator = {}

  tdengine.time_metric = {}

  tdengine.subsystem = {}
  tdengine.subsystem.types = {}

  tdengine.math = {}

  tdengine.draw = {}
  tdengine.draw.internal = {}

  tdengine.dt = 0
  tdengine.elapsed_time = 0
  tdengine.frame = 0

  tdengine.tick = tdengine.is_packaged_build
  tdengine.next_tick = tdengine.tick

  imgui = {}
  imgui.extensions = {}
  imgui.internal = {}

  -- Bootstrap the FFI, so that we can resolve paths
  tdengine.ffi = {}
  ffi.cdef(ffi_header)
  
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
  
  local file_path = ffi.string(ffi.C.dn_paths_resolve('engine_paths').data)
	local path_info = dofile(file_path)

	local engine_paths = collect_paths(path_info.engine_paths)
	for index, path in pairs(engine_paths) do
		ffi.C.dn_paths_add_engine_subpath(path.name, path.path)
	end

  local install_paths = collect_paths(path_info.install_paths)
	for index, path in pairs(install_paths) do
		ffi.C.dn_paths_add_install_subpath(path.name, path.path)
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

function tdengine.init_phase_1()
  tdengine.enum.init()
  tdengine.ffi.init()
  tdengine.paths.init()
  tdengine.time_metric.init()
  tdengine.input.init()
  tdengine.gpu.init()
  tdengine.state.init()
  tdengine.animation.load()
  tdengine.texture.load()
  tdengine.background.load()
  tdengine.fonts.load()
  tdengine.dialogue.init()
  tdengine.audio.init()
  tdengine.gui.init()
  tdengine.scene.init()
  tdengine.math.init()

end

function tdengine.init_phase_2()
  tdengine.subsystem.init()
  tdengine.app = tdengine.subsystem.find('App')

  tdengine.app:on_init_game()

  tdengine.window.init()
  tdengine.fonts.init()
  tdengine.shaders.init()
  tdengine.save.init()
  tdengine.editor.init()
  tdengine.persistent.init()

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_start_game)

end

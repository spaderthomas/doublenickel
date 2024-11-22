local ffi_header = [[
//
// TYPES
//
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t  i32;
typedef float f32;
typedef double f64;

typedef struct {
    char* data;
} tstring;

typedef struct {
    char* data;
} string;

/*
typedef union HMM_Vec2
{
    struct
    {
        float X, Y;
    };

    struct
    {
        float x, y;
    };


    struct
    {
        float U, V;
    };

    struct
    {
        float Left, Right;
    };

    struct
    {
        float Width, Height;
    };

    float Elements[2];
} HMM_Vec2;

typedef union HMM_Vec3
{
    struct
    {
        float X, Y, Z;
    };

    struct
    {
        float x, y, z;
    };

    struct
    {
        float U, V, W;
    };

    struct
    {
        float R, G, B;
    };

    struct
    {
        float r, g, b;
    };

    struct
    {
        HMM_Vec2 XY;
        float _Ignored0;
    };

    struct
    {
        float _Ignored1;
        HMM_Vec2 YZ;
    };

    struct
    {
        HMM_Vec2 UV;
        float _Ignored2;
    };

    struct
    {
        float _Ignored3;
        HMM_Vec2 VW;
    };

    float Elements[3];
} HMM_Vec3;

typedef union HMM_Vec4
{
    struct
    {
        union
        {
            HMM_Vec3 XYZ;
            struct
            {
                float X, Y, Z;
            };
        };

        float W;
    };
    struct
    {
        union
        {
            HMM_Vec3 RGB;
            struct
            {
                float R, G, B;
            };
        };

        float A;
    };

    struct
    {
        float x, y, z, w;
    };

    struct
    {
        float r, g, b, a;
    };

    struct
    {
        HMM_Vec2 XY;
        float _Ignored0;
        float _Ignored1;
    };

    struct
    {
        float _Ignored2;
        HMM_Vec2 YZ;
        float _Ignored3;
    };

    struct
    {
        float _Ignored4;
        float _Ignored5;
        HMM_Vec2 ZW;
    };

    float Elements[4];
} HMM_Vec4;

typedef union HMM_Mat2 {
    float Elements[2][2];
    HMM_Vec2 Columns[2];
} HMM_Mat2;

typedef union HMM_Mat3 {
    float Elements[3][3];
    HMM_Vec3 Columns[3];
} HMM_Mat3;

typedef union HMM_Mat4 {
    float Elements[4][4];
    HMM_Vec4 Columns[4];
} HMM_Mat4;

typedef HMM_Vec2 Vector2;
typedef HMM_Vec3 Vector3;
typedef HMM_Vec4 Vector4;
typedef HMM_Mat2 Matrix2;
typedef HMM_Mat3 Matrix3;
typedef HMM_Mat4 Matrix4;
*/
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

typedef struct MemoryAllocator MemoryAllocator;

typedef struct {
	u32 index;
	u32 generation;
} ArenaHandle;

typedef struct {
	u8* data;
	u32 size;
	u32 capacity;

	u32 vertex_size;
} FixedArray;

void set_gl_name(u32 kind, u32 handle, u32 name_len, const char* name);

void IGE_PushGameFont(const char* font_name);
void IGE_GameImage(const char* image, float sx, float sy);
void IGE_OpenFileBrowser();
void IGE_CloseFileBrowser();
void IGE_SetFileBrowserWorkDir(const char* directory);
bool IGE_IsAnyFileSelected();
tstring IGE_GetSelectedFile();

//
// AUDIO
//
typedef struct {
  i32 index;
  i32 generation;
} ActiveSoundHandle;

ActiveSoundHandle play_sound(const char* name);
ActiveSoundHandle play_sound_loop(const char* name);
void stop_all_sounds();
void stop_sound(ActiveSoundHandle handle);
void pause_sound(ActiveSoundHandle handle);
void unpause_sound(ActiveSoundHandle handle);
bool is_sound_playing(ActiveSoundHandle handle);
void play_sound_after(ActiveSoundHandle current, ActiveSoundHandle next);

void set_volume(ActiveSoundHandle handle, float volume);
void set_cutoff(ActiveSoundHandle handle, float cutoff);

float get_master_cutoff();
float get_master_volume();
float get_master_volume_mod();
void set_threshold(float t);
void set_ratio(float v);
void set_attack_time(float v);
void set_release_time(float v);
void set_sample_rate(float v);
void set_master_volume(float v);
void set_master_cutoff(float v);
void set_butterworth(bool v);
void set_master_volume_mod(float v);

void set_window_size(int x, int y);


//
// INPUT
//
bool is_editor_requesting_input();
bool was_key_pressed(int key);
bool was_key_released(int key);
bool is_key_down(int key);
bool is_mod_down(int mod);
bool was_chord_pressed(int mod, int key);
u32 shift_key(u32 key);
Vector2 get_scroll();
Vector2 get_mouse_delta();
Vector2 get_mouse_delta_converted(u32 coordinate);
void set_game_focus(bool focus);

void show_text_input(const char* description, const char* existing_text);
bool is_text_input_dirty();
const char* read_text_input();


//
// TEXT
//
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
} PreparedText;

PreparedText* prepare_text(const char* text, f32 px, f32 py, const char* font);
PreparedText* prepare_text_wrap(const char* text, f32 px, f32 py, const char* font, f32 wrap);
PreparedText* prepare_text_ex(const char* text, f32 px, f32 py, const char* font, f32 wrap, Vector4 color, bool precise);
void draw_prepared_text(PreparedText* text);

void create_font(const char* id, const char* family, u32 size);
void add_imgui_font(const char* id);


//
// ENGINE
//
void set_exit_game();
const char* get_game_hash();
void set_target_fps(f64 fps);
f64 get_target_fps();

void tm_add(const char* name);
void tm_begin(const char* name);
void tm_end(const char* name);
double tm_average(const char* name);
double tm_last(const char* name);
double tm_largest(const char* name);
double tm_smallest(const char* name);

void submit_feedback(const char* feedback);
void submit_analytics(const char* analytics);
void submit_crash(const char* crash);
void submit_image(const char* file_path);
void open_steam_page(const char* utm);
bool is_steam_deck();
void take_screenshot();
void write_screenshot_to_png(const char* file_name);

typedef struct {
    const char* name;
    const char* path;
} NamedPath;

typedef struct {
	NamedPath* named_paths;
	u32 size;
} NamedPathResult;

NamedPathResult find_all_named_paths();

void add_install_path(const char* name, const char* relative_path);
void add_write_path(const char* name, const char* relative_path);
tstring resolve_named_path(const char* name);
tstring resolve_format_path(const char* name, const char* file_name);

void add_script_directory(const char* name);

//
// WINDOW
//
typedef enum {
	DisplayMode_p480,
	DisplayMode_p720,
	DisplayMode_p1080,
	DisplayMode_p1440,
	DisplayMode_p2160,
	DisplayMode_p1280_800,
	DisplayMode_FullScreen
} DisplayMode;

typedef enum {
	CoordinateSystem_Screen,
	CoordinateSystem_Window,
	CoordinateSystem_Game,
	CoordinateSystem_World,
} CoordinateSystem;

void create_window(const char* title, u32 x, u32 y, u32 flags);
void set_window_icon(const char* file_path);
Vector2 get_content_area();
Vector2 get_game_area_size();
Vector2 get_native_resolution();
void set_game_area_size(f32 x, f32 y);
void set_game_area_position(f32 x, f32 y);
void set_display_mode(u32 mode);
u32 get_display_mode();
void hide_cursor();
void show_cursor();
void use_editor_layout(const char* file_name);
void save_editor_layout(const char* file_name);
void render_imgui();


/////////
// GPU //
/////////
typedef enum {
    VertexAttributeKind_Float,
    VertexAttributeKind_U32,
} VertexAttributeKind;

typedef enum {
	GpuShaderKind_Graphics,
	GpuShaderKind_Compute,
} GpuShaderKind;

typedef enum {
	UniformKind_None = 0,
	UniformKind_Matrix4 = 1,
	UniformKind_Matrix3 = 2,
	UniformKind_Vector4 = 3,
	UniformKind_Vector3 = 4,
	UniformKind_Vector2 = 5,
	UniformKind_I32 = 6,
	UniformKind_F32 = 7,
	UniformKind_Texture = 100,
	UniformKind_PipelineOutput = 101,
	UniformKind_RenderTarget = 102,
} UniformKind;

typedef enum {
    DrawMode_Triangles,
} DrawMode;

typedef enum {
  GlId_Framebuffer,
  GlId_Shader,
  GlId_Program,
} GlId;

typedef enum {
	GPU_LOAD_OP_NONE = 0,
	GPU_LOAD_OP_CLEAR = 1
} GpuLoadOp;

typedef enum {
	GpuMemoryBarrier_ShaderStorage,
} GpuMemoryBarrier;

typedef enum {
	GPU_BUFFER_KIND_STORAGE = 0,
	GPU_BUFFER_KIND_ARRAY = 1,
} GpuBufferKind;

typedef enum {
	GPU_BUFFER_USAGE_STATIC = 0,
	GPU_BUFFER_USAGE_DYNAMIC = 1,
	GPU_BUFFER_USAGE_STREAM = 2,
} GpuBufferUsage;


typedef struct GpuShader GpuShader;
typedef struct GpuBuffer GpuBuffer;
typedef struct GpuVertexLayout GpuVertexLayout;
typedef struct GpuCommandBufferBatched GpuCommandBufferBatched;
typedef struct DrawCall DrawCall;

typedef struct {
  const char* name;
  const char* vertex_shader;
  const char* fragment_shader;
  const char* compute_shader;

  GpuShaderKind kind;
} GpuShaderDescriptor;

typedef struct {
	Vector2 size;
} GpuRenderTargetDescriptor;

typedef struct {
	u32 handle;
	u32 color_buffer;
	Vector2 size;
} GpuRenderTarget;

typedef struct {
	GpuRenderTarget* read;
	GpuRenderTarget* write;
	GpuLoadOp load_op;
} GpuColorAttachment;

typedef struct {
	u32 count;
	u32 kind;
  u32 divisor;
} VertexAttribute;


typedef struct {
	GpuColorAttachment color_attachment;
	GpuCommandBufferBatched* command_buffer;
} GpuGraphicsPipelineDescriptor;
typedef struct {
	GpuColorAttachment color_attachment;
	GpuCommandBufferBatched* command_buffer;
} GpuGraphicsPipeline;


typedef struct {
	VertexAttribute* vertex_attributes;
	u32 num_vertex_attributes;
  u32 max_vertices;
  u32 max_draw_calls;
} GpuCommandBufferBatchedDescriptor;



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

typedef struct {
  char name [64];
	GpuBufferKind kind;
  GpuBufferUsage usage;
	u32 size;
	u32 handle;
} GpuBuffer;

typedef struct {
  FixedArray buffer;
  GpuBuffer* gpu_buffer;
} GpuBackedBuffer;



///////////////////
// GPU FUNCTIONS //
///////////////////
GpuShader*               gpu_shader_create(GpuShaderDescriptor descriptor);
GpuRenderTarget*         gpu_render_target_create(GpuRenderTargetDescriptor descriptor);
GpuRenderTarget*         gpu_acquire_swapchain();
void                     gpu_render_target_bind(GpuRenderTarget* target);
void                     gpu_render_target_clear(GpuRenderTarget* target);
void                     gpu_render_target_blit(GpuRenderTarget* source, GpuRenderTarget* destination);
void                     gpu_swap_buffers();
GpuCommandBufferBatched* gpu_create_command_buffer(GpuCommandBufferBatchedDescriptor descriptor);
DrawCall*                gpu_command_buffer_alloc_draw_call(GpuCommandBufferBatched* command_buffer);
DrawCall*                gpu_command_buffer_find_draw_call(GpuCommandBufferBatched* command_buffer);
DrawCall*                gpu_command_buffer_flush_draw_call(GpuCommandBufferBatched* command_buffer);
u8*                      gpu_command_buffer_alloc_vertex_data(GpuCommandBufferBatched* command_buffer, u32 count);
u8*                      gpu_command_buffer_push_vertex_data(GpuCommandBufferBatched* command_buffer, void* data, u32 count);
void                     gpu_command_buffer_bind(GpuCommandBufferBatched* command_buffer);
void                     gpu_command_buffer_preprocess(GpuCommandBufferBatched* command_buffer);
void                     gpu_command_buffer_render(GpuCommandBufferBatched* command_buffer);
void                     gpu_command_buffer_submit(GpuCommandBufferBatched* command_buffer);
GpuGraphicsPipeline*     gpu_graphics_pipeline_create(GpuGraphicsPipelineDescriptor descriptor);
void                     gpu_graphics_pipeline_begin_frame(GpuGraphicsPipeline* pipeline);
void                     gpu_graphics_pipeline_bind(GpuGraphicsPipeline* pipeline);
void                     gpu_graphics_pipeline_submit(GpuGraphicsPipeline* pipeline);
DrawCall*                gpu_graphics_pipeline_alloc_draw_call(GpuGraphicsPipeline* pipeline);
GpuBuffer*               gpu_buffer_create(GpuBufferDescriptor descriptor);
void                     gpu_memory_barrier(u32 barrier);
void                     gpu_dispatch_compute(GpuBuffer* buffer, u32 size);



//////////////////////////
// OPENGL CONFIGURATION //
//////////////////////////
void                     set_active_shader(const char* name);
void                     set_active_shader_ex(GpuShader* shader);
void                     set_uniform_texture(const char* name, i32 value);
void                     set_uniform_i32(const char* name, i32 value);
void                     set_uniform_f32(const char* name, float value);
void                     set_uniform_vec2(const char* name, Vector2 value);
void                     set_uniform_vec3(const char* name, Vector3 value);
void                     set_uniform_vec4(const char* name, Vector4 value);
void                     set_uniform_mat3(const char* name, Matrix3 value);
void                     set_uniform_mat4(const char* name, Matrix4 value);
void                     set_world_space(bool world_space);
void                     set_layer(i32 layer);
void                     set_camera(f32 px, f32 py);
void                     set_blend_enabled(bool enabled);
void                     set_blend_mode(i32 source, i32 destination);
void                     set_draw_mode(u32 mode);
void                     set_orthographic_projection(float l, float r, float b, float t, float n, float f);
void                     begin_scissor(f32 px, f32 py, f32 dx, f32 dy);
void                     end_scissor();
    
void                     set_shader_immediate(const char* name);
void                     set_shader_immediate_ex(GpuShader* shader);
void                     set_uniform_immediate_vec2(const char* name, Vector2 value);
void                     set_uniform_immediate_i32(const char* name, i32 value);
void                     set_uniform_immediate_f32(const char* name, float value);
void                     set_uniform_immediate_texture(const char* name, i32 value);
void                     set_uniform_immediate_mat4(const char* name, Matrix4 value);
void                     set_uniform_immediate_mat3(const char* name, Matrix3 value);
void                     set_uniform_immediate_vec4(const char* name, Vector4 value);
void                     set_uniform_immediate_vec3(const char* name, Vector3 value);
void                     set_uniform_immediate_vec2(const char* name, Vector2 value);
void                     set_uniform_immediate_i32(const char* name, i32 value);
void                     set_uniform_immediate_f32(const char* name, float value);
void                     set_uniform_immediate_texture(const char* name, i32 value);
void                     push_quad(float px, float py, float dx, float dy, Vector2* uv, float opacity);
i32                      find_uniform_index(const char* name);








///////////
// ENUMS //
///////////
typedef enum {
  GPU_COMMAND_OP_INITIALIZE = 0,
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


GpuCommandBuffer* _gpu_command_buffer_create(GpuCommandBufferDescriptor descriptor);
void              _gpu_command_buffer_draw(GpuCommandBuffer* command_buffer, GpuDrawCall draw_call);
void              _gpu_command_buffer_submit(GpuCommandBuffer* command_buffer);
void              _gpu_bind_pipeline(GpuCommandBuffer* command_buffer, GpuPipeline pipeline);
void              _gpu_begin_render_pass(GpuCommandBuffer* command_buffer, GpuRenderPass render_pass);
void              _gpu_end_render_pass(GpuCommandBuffer* command_buffer);
void              _gpu_apply_bindings(GpuCommandBuffer* command_buffer, GpuBufferBinding bindings);
void              _gpu_bind_render_state(GpuCommandBuffer* command_buffer, GpuRendererState render);
void              _gpu_set_layer(GpuCommandBuffer* command_buffer, u32 layer);
void              _gpu_set_world_space(GpuCommandBuffer* command_buffer, bool world_space);
void              _gpu_set_camera(GpuCommandBuffer* command_buffer, Vector2 camera);
GpuUniform*       _gpu_uniform_create(GpuUniformDescriptor descriptor);
void               gpu_buffer_bind(GpuBuffer* buffer);
void               gpu_buffer_bind_base(GpuBuffer* buffer, u32 base);
void               gpu_buffer_sync(GpuBuffer* buffer, void* data, u32 size);
void               gpu_buffer_sync_subdata(GpuBuffer* buffer, void* data, u32 byte_size, u32 byte_offset);
void               gpu_buffer_zero(GpuBuffer* buffer, u32 size);
GpuBackedBuffer    gpu_backed_buffer_create(GpuBufferDescriptor descriptor);
u32                gpu_backed_buffer_size(GpuBackedBuffer* buffer);
u8*                gpu_backed_buffer_push(GpuBackedBuffer* buffer, void* data, u32 num_elements);
void               gpu_backed_buffer_sync(GpuBackedBuffer* buffer);



//
// DRAW
//
typedef enum {
	ZERO,
	ONE,
	SRC_COLOR,
	ONE_MINUS_SRC_COLOR,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA,
	SRC_ALPHA_SATURATE,
	SRC1_COLOR,
	ONE_MINUS_SRC1_COLOR,
	SRC1_ALPHA,
	ONE_MINUS_SRC1_ALPHA
} BlendMode;

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

u32 find_texture_handle(const char* name);

//
// OS
//
bool does_path_exist(const char* path);
bool is_regular_file(const char* path);
bool is_directory(const char* path);
void create_directory(const char* path);
void remove_directory(const char* path);
void create_named_directory(const char* name);

typedef struct DateTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;
} DateTime;

DateTime get_date_time();

void ma_add(const char* name, MemoryAllocator* allocator);
MemoryAllocator* ma_find(const char* name);
void* ma_alloc(MemoryAllocator* allocator, u32 size);
void ma_free(MemoryAllocator* allocator, void* buffer);

typedef struct dyn_array
{
    u32 size;
    u32 capacity;
    u32 element_size;
    MemoryAllocator* allocator;
} dyn_array_header;

void*             _dyn_array_alloc(u32 element_size, MemoryAllocator* allocator);
void              _dyn_array_push_n(void** array, void* data, u32 num_elements);
void*             _dyn_array_reserve(void** array, u32 num_elements);
dyn_array_header* _dyn_array_head(void** array);
u32               _dyn_array_size(void** array);
u32               _dyn_array_capacity(void** array);
u32               _dyn_array_element_size(void** array);
MemoryAllocator*  _dyn_array_allocator(void** array);
bool              _dyn_array_full(void** array);
bool              _dyn_array_need_grow(void** array, u32 num_elements);
void              _dyn_array_grow(void** array, u32 requested_size);
u32               _dyn_array_byte_size(void** array);

void copy_string(const char* str, char* buffer, u32 buffer_length);
void copy_string_n(const char* str, u32 length, char* buffer, u32 buffer_length);

//
// ACTIONS
//
void register_action(const char* name, u32 key, u32 key_event, const char* action_set);
void register_action_set(const char* name);
void activate_action_set(const char* name);
bool is_digital_active(const char* name);
bool was_digital_active(const char* name);
bool was_digital_pressed(const char* name);
i32 get_input_device();
i32 get_action_set_cooldown();
const char* get_active_action_set();

enum {
	KeyAction_Press = 0,
	KeyAction_Down = 1
};


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

//
// MATH
//
f64 perlin(f64 x, f64 y, f64 vmin, f64 vmax);



//
// FLUID
//
	typedef struct {
		u32 next_unprocessed_index;
		u32 grid_size;
	} EulerianFluidSystem;

ArenaHandle lf_create(u32 num_particles);
void lf_destroy(ArenaHandle handle);
void lf_destroy_all();
void lf_init(ArenaHandle handle);
void lf_inspect(ArenaHandle handle);
void lf_set_volume(ArenaHandle handle, float ax, float ay, float bx, float by, float radius);
void lf_set_velocity(ArenaHandle handle, float x, float y);
void lf_set_smoothing_radius(ArenaHandle handle, float r);
void lf_set_particle_mass(ArenaHandle handle, float mass);
void lf_set_viscosity(ArenaHandle handle, float viscosity);
void lf_set_pressure(ArenaHandle handle, float pressure);
void lf_set_gravity(ArenaHandle handle, float gravity);
void lf_set_timestep(ArenaHandle handle, float dt);
void lf_bind(ArenaHandle handle);
void lf_update(ArenaHandle handle);
void lf_draw(ArenaHandle handle);

ArenaHandle ef_create(u32 grid_size);
void ef_destroy(ArenaHandle handle);
void ef_destroy_all();
void ef_init(ArenaHandle handle);
void ef_inspect(ArenaHandle handle);
u32 ef_pair_to_index(u32 grid_size, u32 x, u32 y);
void ef_set_render_size(ArenaHandle handle, u32 size);
void ef_set_velocity(ArenaHandle handle, u32 x, u32 y, float vx, float vy);
void ef_clear_density_source(ArenaHandle handle);
void ef_set_density_source(ArenaHandle handle, u32 x, u32 y, float amount);
void ef_set_gauss_seidel(ArenaHandle handle, u32 iterations);
void ef_bind(ArenaHandle handle);
void ef_update(ArenaHandle handle);
void ef_draw(ArenaHandle handle);
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
  local install_dir = tdengine.ffi.resolve_named_path('install'):to_interned()
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
  tdengine.ffi.render_imgui()

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
  tdengine.gpus = {}

  tdengine.app = {}

  tdengine.iterator = {}

  tdengine.time_metric = {}

  tdengine.subsystem = {}
  tdengine.subsystem.types = {}

  tdengine.math = {}


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
    full_parent = full_parent or ''

    local collected_paths = {}
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
  
  local file_path = ffi.string(ffi.C.resolve_named_path('engine_paths').data)
	local path_info = dofile(file_path)

	local install_paths = collect_paths(path_info.install_paths)
	for index, path in pairs(install_paths) do
		ffi.C.add_install_path(path.name, path.path)
	end

	local write_paths = collect_paths(path_info.write_paths)
	for index, path in pairs(write_paths) do
		ffi.C.add_write_path(path.name, path.path)
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
    local file_path = ffi.string(ffi.C.resolve_format_path('engine_script', file_name).data)
    dofile(file_path)
  end


end

function tdengine.init_phase_1()
  tdengine.enum.init()
  tdengine.ffi.init()
  tdengine.paths.init()
  tdengine.time_metric.init()
  tdengine.input.init()
  tdengine.gpus.init()
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

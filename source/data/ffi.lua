return [[
typedef struct {
  Vector4 color;
  Vector2 position;
  f32 radial_falloff;
  f32 angular_falloff;
  f32 intensity;
  f32 padding [3];
} Light;

/////////
// SDF //
/////////

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

]]
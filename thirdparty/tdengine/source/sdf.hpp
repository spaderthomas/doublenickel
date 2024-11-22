#ifndef SDF_H
#define SDF_H

///////////
// ENUMS //
///////////
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
  SdfShape shape;
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

FM_LUA_EXPORT SdfRenderer       sdf_renderer_create(u32 buffer_size);
FM_LUA_EXPORT void              sdf_renderer_draw(SdfRenderer* renderer, GpuCommandBuffer* command_buffer);
FM_LUA_EXPORT void              sdf_renderer_push_instance(SdfRenderer* renderer, SdfShape shape);
FM_LUA_EXPORT void              sdf_renderer_push_header(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness);
FM_LUA_EXPORT SdfCombineHeader* sdf_combination_begin(SdfRenderer* renderer);
FM_LUA_EXPORT void              sdf_combination_append(SdfRenderer* renderer, SdfCombineHeader* header, SdfShape shape, SdfCombineOp op, SdfSmoothingKernel kernel);
FM_LUA_EXPORT void              sdf_combination_commit(SdfRenderer* renderer);
FM_LUA_EXPORT void              sdf_circle_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius);
FM_LUA_EXPORT void              sdf_ring_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius);
FM_LUA_EXPORT void              sdf_oriented_box_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy);
FM_LUA_EXPORT void              sdf_grid(SdfRenderer* renderer, u32 grid_width, u32 grid_size);
#endif

#ifdef SDF_IMPLEMENTATION
SdfRenderer sdf_renderer_create(u32 buffer_size) {
  SdfRenderer renderer = {
    .state = SDF_RENDERER_STATE_NONE,

    .vertices = gpu_backed_buffer_create({
      .name = "SdfRendererVertices",
      .kind = GPU_BUFFER_KIND_ARRAY,
      .usage = GPU_BUFFER_USAGE_STATIC,
      .capacity = buffer_size,
      .element_size = sizeof(SdfVertex)
    }),
    .instances = gpu_backed_buffer_create({
      .name = "SdfRendererInstances",
      .kind = GPU_BUFFER_KIND_ARRAY,
      .usage = GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(SdfInstance)
    }),
    .combinations = gpu_backed_buffer_create({
      .name = "SdfRendererCombinations",
      .kind = GPU_BUFFER_KIND_STORAGE,
      .usage = GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(u32)
    }),
    .shape_data = gpu_backed_buffer_create({
      .name = "SdfRendererShapeData",
      .kind = GPU_BUFFER_KIND_STORAGE,
      .usage = GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(float)
    }),
    .bindings = {
      .vertex = {
        .bindings = {
          { .buffer = renderer.vertices.gpu_buffer },
          { .buffer = renderer.instances.gpu_buffer },
        },
        .count = 2
      },
      .storage = {
        .bindings = {
          { .buffer = renderer.shape_data.gpu_buffer,   .base = 0 },
          { .buffer = renderer.combinations.gpu_buffer, .base = 1 },
        },
        .count = 2
      }
    },
    .pipeline = _gpu_pipeline_create({
      .blend = {
        .fn = GPU_BLEND_FUNC_ADD,
        .source = GPU_BLEND_MODE_SRC_ALPHA,
        .destination = GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA,
      },
      .raster = {
        .shader = gpu_shader_find("shape"),
        .primitive = GPU_PRIMITIVE_TRIANGLES
      },
      .buffer_layouts = {
        { 
          .vertex_attributes = {
            { .kind = GPU_VERTEX_ATTRIBUTE_FLOAT, .count = 2 },
            { .kind = GPU_VERTEX_ATTRIBUTE_FLOAT, .count = 2 },
          },   
          .num_vertex_attributes = 2 
        },
        { 
          .vertex_attributes = {
            { .kind = GPU_VERTEX_ATTRIBUTE_U32, .count = 2, .divisor = 1 },
          }, 
          .num_vertex_attributes = 1 
        },
      },
      .num_buffer_layouts = 2
    })
  };

  Vector2 vertices [6] = TD_MAKE_QUAD(0.5, -0.5, -0.5, 0.5);
  for (u32 i = 0; i < 6; i++) {
    SdfVertex vertex = {
      .position = vertices[i],
      .uv = vertices[i],
    };
    gpu_backed_buffer_push(&renderer.vertices, &vertex, 1);
  }
  gpu_backed_buffer_sync(&renderer.vertices);

  return renderer;
}

void sdf_renderer_draw(SdfRenderer* renderer, GpuCommandBuffer* command_buffer) {
  gpu_backed_buffer_sync(&renderer->instances);
  gpu_backed_buffer_sync(&renderer->shape_data);
  gpu_backed_buffer_sync(&renderer->combinations);

  _gpu_bind_pipeline(command_buffer, renderer->pipeline);
  _gpu_apply_bindings(command_buffer, renderer->bindings);
  _gpu_command_buffer_draw(command_buffer, {
    .mode = GPU_DRAW_MODE_INSTANCE,
    .vertex_offset = 0,
    .num_vertices = 6,
    .num_instances = gpu_backed_buffer_size(&renderer->instances)
  });
  
  gpu_backed_buffer_clear(&renderer->instances);
  gpu_backed_buffer_clear(&renderer->shape_data);
  gpu_backed_buffer_clear(&renderer->combinations);
}

void sdf_renderer_push_instance(SdfRenderer* renderer, SdfShape shape) {
  if (renderer->state == SDF_RENDERER_STATE_NONE) {
    SdfInstance instance = {
      .shape = shape,
      .buffer_index = gpu_backed_buffer_size(&renderer->shape_data),
    };
    gpu_backed_buffer_push(&renderer->instances, &instance, 1);
  }
}

void sdf_renderer_push_header(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness) {
  gpu_backed_buffer_push(&renderer->shape_data, &r, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &g, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &b, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &px, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &py, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &rotation, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &edge_thickness, 1);
}

SdfCombineHeader* sdf_combination_begin(SdfRenderer* renderer) {
  // First, push an instance that points into the combination buffer.
  SdfInstance instance = {
    .shape = SDF_SHAPE_COMBINE,
    .buffer_index = gpu_backed_buffer_size(&renderer->combinations),
  };
  gpu_backed_buffer_push(&renderer->instances, &instance, 1);

  renderer->state = SDF_RENDERER_STATE_COMBINATION;

  // Then, put a combination header in the data buffer; this'll tell the GPU how many combination entries 
  // there are. Since we don't know that yet, return a pointer that gets filled in as you push shapes.
  SdfCombineHeader* header = (SdfCombineHeader*)gpu_backed_buffer_push(&renderer->combinations, NULL, 1);
  header->num_sdfs = 0;
  return header;
}

void sdf_combination_append(SdfRenderer* renderer, SdfCombineHeader* header, SdfShape shape, SdfCombineOp op, SdfSmoothingKernel kernel) {
  header->num_sdfs++;
  
  SdfCombineEntry entry = {
    .buffer_index = gpu_backed_buffer_size(&renderer->shape_data),
    .shape = shape,
    .op = op,
    .kernel = kernel
  };
  gpu_backed_buffer_push(&renderer->combinations, &entry, 4);
}

void sdf_combination_commit(SdfRenderer* renderer) {
  renderer->state = SDF_RENDERER_STATE_NONE;
}

void sdf_circle_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius) {
  sdf_renderer_push_instance(renderer, SDF_SHAPE_CIRCLE);
  sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  gpu_backed_buffer_push(&renderer->shape_data, &radius, 1);
}

void sdf_ring_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius) {
  sdf_renderer_push_instance(renderer, SDF_SHAPE_RING);
  sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  gpu_backed_buffer_push(&renderer->shape_data, &inner_radius, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &outer_radius, 1);
}

void sdf_oriented_box_ex(SdfRenderer* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy) {
  sdf_renderer_push_instance(renderer, SDF_SHAPE_ORIENTED_BOX);
  sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  gpu_backed_buffer_push(&renderer->shape_data, &dx, 1);
  gpu_backed_buffer_push(&renderer->shape_data, &dy, 1);
}



void sdf_grid(SdfRenderer* renderer, u32 grid_width, u32 grid_size) {
  tm_begin("sdf_c");
  for (u32 x = 0; x < grid_size; x += grid_width) {
    for (u32 y = 0; y < grid_size; y += grid_width) {
      sdf_circle_ex(renderer,
        x, y,
        0.40f, 0.65f, 0.55f, 
        0.0f,
        1.5f,
        3.0f
      );
    }
    
  }
  tm_end("sdf_c");
}


#endif
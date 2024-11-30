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
} dn_sdf_shape_t;

typedef enum {
  SDF_COMBINE_OP_UNION = 0,
  SDF_COMBINE_OP_INTERSECTION = 1,
  SDF_COMBINE_OP_SUBTRACTION = 2,
} dn_sdf_combine_op_t;

typedef enum {
  SDF_SMOOTH_KERNEL_NONE = 0,
  SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC = 1,
} dn_sdf_smoothing_kernel_t;

typedef enum {
  SDF_RENDERER_STATE_NONE,
  SDF_RENDERER_STATE_COMBINATION,
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
  Vector3 color;
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
DN_API void                     dn_sdf_oriented_box_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy);
DN_API void                     dn_sdf_grid(dn_sdf_renderer_t* renderer, u32 grid_width, u32 grid_size);
#endif

#ifdef DN_SDF_IMPLEMENTATION
dn_sdf_renderer_t dn_sdf_renderer_create(u32 buffer_size) {
  dn_sdf_renderer_t renderer = {
    .state = SDF_RENDERER_STATE_NONE,

    .vertices = dn_gpu_backed_buffer_create({
      .name = "dn_sdf_renderer_tVertices",
      .kind = GPU_BUFFER_KIND_ARRAY,
      .usage = GPU_BUFFER_USAGE_STATIC,
      .capacity = buffer_size,
      .element_size = sizeof(dn_sdf_vertex_t)
    }),
    .instances = dn_gpu_backed_buffer_create({
      .name = "dn_sdf_renderer_tInstances",
      .kind = GPU_BUFFER_KIND_ARRAY,
      .usage = GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(dn_sdf_instance_t)
    }),
    .combinations = dn_gpu_backed_buffer_create({
      .name = "dn_sdf_renderer_tCombinations",
      .kind = GPU_BUFFER_KIND_STORAGE,
      .usage = GPU_BUFFER_USAGE_DYNAMIC,
      .capacity = buffer_size,
      .element_size = sizeof(u32)
    }),
    .shape_data = dn_gpu_backed_buffer_create({
      .name = "dn_sdf_renderer_tShapeData",
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
    .pipeline = dn_gpu_pipeline_create({
      .blend = {
        .fn = GPU_BLEND_FUNC_ADD,
        .source = GPU_BLEND_MODE_SRC_ALPHA,
        .destination = GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA,
      },
      .raster = {
        .shader = dn_gpu_shader_find("shape"),
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
  dn_gpu_backed_buffer_sync(&renderer->instances);
  dn_gpu_backed_buffer_sync(&renderer->shape_data);
  dn_gpu_backed_buffer_sync(&renderer->combinations);

  dn_gpu_bind_pipeline(command_buffer, renderer->pipeline);
  dn_gpu_apply_bindings(command_buffer, renderer->bindings);
  dn_gpu_command_buffer_draw(command_buffer, {
    .mode = GPU_DRAW_MODE_INSTANCE,
    .vertex_offset = 0,
    .num_vertices = 6,
    .num_instances = dn_gpu_backed_buffer_size(&renderer->instances)
  });
  
  dn_gpu_backed_buffer_clear(&renderer->instances);
  dn_gpu_backed_buffer_clear(&renderer->shape_data);
  dn_gpu_backed_buffer_clear(&renderer->combinations);
}

void dn_sdf_renderer_push_instance(dn_sdf_renderer_t* renderer, dn_sdf_shape_t shape) {
  if (renderer->state == SDF_RENDERER_STATE_NONE) {
    dn_sdf_instance_t instance = {
      .shape = shape,
      .buffer_index = dn_gpu_backed_buffer_size(&renderer->shape_data),
    };
    dn_gpu_backed_buffer_push(&renderer->instances, &instance, 1);
  }
}

void dn_sdf_renderer_push_header(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness) {
  dn_gpu_backed_buffer_push(&renderer->shape_data, &r, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &g, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &b, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &px, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &py, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &rotation, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &edge_thickness, 1);
}

dn_sdf_combine_header_t* dn_sdf_combination_begin(dn_sdf_renderer_t* renderer) {
  // First, push an instance that points into the combination buffer.
  dn_sdf_instance_t instance = {
    .shape = SDF_SHAPE_COMBINE,
    .buffer_index = dn_gpu_backed_buffer_size(&renderer->combinations),
  };
  dn_gpu_backed_buffer_push(&renderer->instances, &instance, 1);

  renderer->state = SDF_RENDERER_STATE_COMBINATION;

  // Then, put a combination header in the data buffer; this'll tell the GPU how many combination entries 
  // there are. Since we don't know that yet, return a pointer that gets filled in as you push shapes.
  dn_sdf_combine_header_t* header = (dn_sdf_combine_header_t*)dn_gpu_backed_buffer_push(&renderer->combinations, NULL, 1);
  header->num_sdfs = 0;
  return header;
}

void dn_sdf_combination_append(dn_sdf_renderer_t* renderer, dn_sdf_combine_header_t* header, dn_sdf_shape_t shape, dn_sdf_combine_op_t op, dn_sdf_smoothing_kernel_t kernel) {
  header->num_sdfs++;
  
  dn_sdf_combine_entry_t entry = {
    .buffer_index = dn_gpu_backed_buffer_size(&renderer->shape_data),
    .shape = shape,
    .op = op,
    .kernel = kernel
  };
  dn_gpu_backed_buffer_push(&renderer->combinations, &entry, 4);
}

void dn_sdf_combination_commit(dn_sdf_renderer_t* renderer) {
  renderer->state = SDF_RENDERER_STATE_NONE;
}

void dn_sdf_circle_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius) {
  dn_sdf_renderer_push_instance(renderer, SDF_SHAPE_CIRCLE);
  dn_sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &radius, 1);
}

void dn_sdf_ring_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius) {
  dn_sdf_renderer_push_instance(renderer, SDF_SHAPE_RING);
  dn_sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &inner_radius, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &outer_radius, 1);
}

void dn_sdf_oriented_box_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float dx, float dy) {
  dn_sdf_renderer_push_instance(renderer, SDF_SHAPE_ORIENTED_BOX);
  dn_sdf_renderer_push_header(renderer, px, py, r, g, b, rotation, edge_thickness);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &dx, 1);
  dn_gpu_backed_buffer_push(&renderer->shape_data, &dy, 1);
}

void dn_sdf_grid(dn_sdf_renderer_t* renderer, u32 grid_width, u32 grid_size) {
  dn_time_metric_begin("sdf_c");
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
  dn_time_metric_end("sdf_c");
}


#endif
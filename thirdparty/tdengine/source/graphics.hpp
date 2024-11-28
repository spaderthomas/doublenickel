#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GPU_NEAR_PLANE -100.0
#define GPU_FAR_PLANE 100.0


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
} dn_gpu_command_op_t;

typedef enum {
  GPU_PRIMITIVE_TRIANGLES = 0
} dn_gpu_draw_primitive_t;

typedef enum {
  GPU_DRAW_MODE_ARRAYS = 0,
  GPU_DRAW_MODE_INSTANCE = 1,
} dn_gpu_draw_mode_t;

typedef enum {
  GPU_VERTEX_ATTRIBUTE_FLOAT = 0,
  GPU_VERTEX_ATTRIBUTE_U32 = 1,
} dn_gpu_vertex_attribute_kind_t;

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
} dn_gpu_uniform_kind_t;

typedef enum {
  GPU_BUFFER_KIND_STORAGE = 0,
  GPU_BUFFER_KIND_ARRAY = 1,
} dn_gpu_buffer_kind_t;

typedef enum {
  GPU_BUFFER_USAGE_STATIC = 0,
  GPU_BUFFER_USAGE_DYNAMIC = 1,
  GPU_BUFFER_USAGE_STREAM = 2,
} dn_gpu_buffer_usage_t;

typedef enum {
  GPU_BLEND_FUNC_NONE,
  GPU_BLEND_FUNC_ADD,
  GPU_BLEND_FUNC_SUBTRACT,
  GPU_BLEND_FUNC_REVERSE_SUBTRACT,
  GPU_BLEND_FUNC_MIN,
  GPU_BLEND_FUNC_MAX
} dn_gpu_blend_func_t;

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
} dn_gpu_blend_mode_t;

typedef enum {
  GPU_LOAD_OP_NONE = 0,
  GPU_LOAD_OP_CLEAR = 1
} dn_gpu_load_op_t;

typedef enum {
  GPU_RESOURCE_FRAMEBUFFER = 0,
  GPU_RESOURCE_SHADER = 1,
  GPU_RESOURCE_PROGRAM = 2,
} dn_gpu_resource_id_t;

typedef enum {
  GPU_MEMORY_BARRIER_STORAGE = 0,
  GPU_MEMORY_BARRIER_BUFFER_UPDATE = 1,
} dn_gpu_memory_barrier_t;

typedef enum {
  GPU_SHADER_GRAPHICS = 0,
  GPU_SHADER_COMPUTE = 1,
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
  Matrix4 mat4;
  Matrix3 mat3;
  Matrix2 mat2;
  Vector4 vec4;
  Vector3 vec3;
  Vector2 vec2;
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
  Vector2 size;
} dn_gpu_render_target_descriptor_t;

typedef struct {
  dn_asset_name_t name;
  u32 handle;
  u32 color_buffer;
  Vector2 size;
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
  DN_ALIGN(16) struct {
    dn_gpu_vertex_buffer_binding_t bindings [8];
    u32 count;
  } vertex;

  DN_ALIGN(16) struct {
    dn_gpu_uniform_binding_t bindings [8];
    u32 count;
  } uniforms;

  DN_ALIGN(16) struct {
    dn_gpu_storage_buffer_binding_t bindings [8];
    u32 count;
  } storage;

  // UBO
  // SSBO
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
  Vector2 position;
  Vector2 size;
  bool enabled;
} dn_gpu_scissor_state_t;

typedef struct {
  u32 layer;
  bool world_space;
  Vector2 camera;
  Matrix4 projection;
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

  Array<dn_gpu_command_t> commands;
  u32 vao;
} dn_gpu_command_buffer_t;

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

typedef struct {
  Array<dn_gpu_command_buffer_t, 32> command_buffers;
  Array<dn_gpu_uniform_t, 1024> uniforms;
  Array<dn_gpu_pipeline_t, 64> pipelines;
  Array<dn_gpu_buffer_t, 128>  gpu_buffers;
  Array<dn_gpu_render_target_t, 32>  targets;
  Array<dn_gpu_shader_t, 128> shaders;

  FileMonitor* shader_monitor;
  dn_fixed_array<dn_path_t, 16> search_paths;
} dn_gpu_t;
dn_gpu_t dn_gpu;

///////////////
// FUNCTIONS //
///////////////
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
DN_API void                      dn_gpu_set_camera(dn_gpu_command_buffer_t* command_buffer, Vector2 camera);
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
DN_API tstring                   dn_gpu_error_read();
DN_API void                      dn_gpu_error_log_one();
DN_API void                      dn_gpu_error_log_all();
DN_API void                      dn_gpu_set_resource_name(dn_gpu_resource_id_t id, u32 handle, u32 name_len, const char* name);
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

//////////////
// INTERNAL //
//////////////

#endif // GRAPHICS_H



////////////////////
// IMPLEMENTATION //
////////////////////
#ifdef GRAPHICS_IMPLEMENTATION
void dn_gpu_init(dn_gpu_config_t config) {
  dn_log("%s", __func__);
  arr_init(&dn_gpu.command_buffers);
  arr_init(&dn_gpu.uniforms);
  arr_init(&dn_gpu.pipelines);
  arr_init(&dn_gpu.gpu_buffers);
  arr_init(&dn_gpu.targets);
  arr_init(&dn_gpu.shaders);
  dn_fixed_array_init_t(&dn_gpu.search_paths, &standard_allocator);

  auto swapchain = arr_push(&dn_gpu.targets);
  swapchain->handle = 0;
  swapchain->color_buffer = 0;
  swapchain->size = window.content_area;

  dn_log("%s: Initializing shader file monitor", __func__);
  auto reload_all_shaders = [](FileMonitor* file_monitor, FileChange* event, void* userdata) {
    dn_log("SHADER_RELOAD");
    arr_for(dn_gpu.shaders, shader) {
      dn_gpu_shader_reload(shader);
    }
  };
  dn_gpu.shader_monitor = arr_push(&file_monitors);
  dn_gpu.shader_monitor->init(reload_all_shaders, FileChangeEvent::Modified, nullptr);
  dn_gpu.shader_monitor->add_directory(config.shader_path);

  // Add default search paths and shaders
  dn_log("%s: Initializing default shader include directories", __func__);
  const char* default_include_paths [] = {
    dn_paths_resolve("dn_shaders"),
    dn_paths_resolve("dn_shader_includes"),
  };
  for (u32 i = 0; i < DN_ARR_LEN(default_include_paths); i++) {
    dn_path_t* path = dn_fixed_array_reserve_t(&dn_gpu.search_paths, 1);
    dn_path_from_cstr(*path, default_include_paths[i]);
  }

  dn_log("%s: Initializing default shaders", __func__);
  dn_gpu_shader_descriptor_t default_shaders [] = {
    {
      .name = "shape",
      .vertex_shader = dn_paths_resolve_format("dn_shader", "sdf/shape.vertex"),
      .fragment_shader = dn_paths_resolve_format("dn_shader", "sdf/shape.fragment"),
      .kind = GPU_SHADER_GRAPHICS
    },
    {
        .name = "solid",
        .vertex_shader = dn_paths_resolve_format("dn_shader", "solid/solid.vertex"),
        .fragment_shader = dn_paths_resolve_format("dn_shader", "solid/solid.fragment"),
        .kind = GPU_SHADER_GRAPHICS,
    },
    {
        .name = "sprite",
        .vertex_shader = dn_paths_resolve_format("dn_shader", "sprite/sprite.vertex"),
        .fragment_shader = dn_paths_resolve_format("dn_shader", "sprite/sprite.fragment"),
        .kind = GPU_SHADER_GRAPHICS,
    },
    {
        .name = "text",
        .vertex_shader = dn_paths_resolve_format("dn_shader", "text/text.vertex"),
        .fragment_shader = dn_paths_resolve_format("dn_shader", "text/text.fragment"),
        .kind = GPU_SHADER_GRAPHICS,
    },
  };
  for (u32 i = 0; i < DN_ARR_LEN(default_shaders); i++) {
    dn_gpu_shader_create(default_shaders[i]);
  }

  // Add user search paths and shaders
  for (u32 i = 0; i < config.num_search_paths; i++) {
    auto path = dn_fixed_array_reserve_t(&dn_gpu.search_paths, 1);
    dn_path_from_cstr(*path, config.search_paths[i]);
  }

  for (u32 i = 0; i < config.num_shaders; i++) {
    dn_gpu_shader_create(config.shaders[i]);
  }

  for (u32 i = 0; i < config.num_render_targets; i++) {
    dn_gpu_render_target_create(config.render_targets[i]);
  }

}



////////////////////
// COMMAND BUFFER //
////////////////////
dn_gpu_command_buffer_t* dn_gpu_command_buffer_create(dn_gpu_command_buffer_descriptor_t descriptor) {
  auto command_buffer = arr_push(&dn_gpu.command_buffers);
  arr_init(&command_buffer->commands, descriptor.max_commands);
  glGenVertexArrays(1, &command_buffer->vao);
  glBindVertexArray(command_buffer->vao);

  return command_buffer;
}

void dn_gpu_command_buffer_clear_cached_state(dn_gpu_command_buffer_t* command_buffer) {
  command_buffer->pipeline = nullptr;
  zero_memory(&command_buffer->bindings, sizeof(dn_gpu_buffer_binding_t));
  zero_memory(&command_buffer->render_pass, sizeof(dn_gpu_render_pass_t));
  zero_memory(&command_buffer->render, sizeof(dn_gpu_renderer_state_t));
  zero_memory(&command_buffer->scissor, sizeof(dn_gpu_scissor_state_t));
}

void dn_gpu_command_buffer_submit(dn_gpu_command_buffer_t* command_buffer) {
  dn_gpu_command_buffer_clear_cached_state(command_buffer);
  glBindVertexArray(command_buffer->vao);

  arr_for(command_buffer->commands, it) {
    auto& command = *it;
    auto& pipeline = *command_buffer->pipeline;

    switch (command.op) {
      case GPU_COMMAND_OP_BEGIN_RENDER_PASS: {
        switch (command.render_pass.color.load) {
          case GPU_LOAD_OP_CLEAR: {
            dn_gpu_render_target_clear(command.render_pass.color.attachment);
          }
        }

        dn_gpu_render_target_bind(command.render_pass.color.attachment);
        command_buffer->render_pass = command.render_pass;
      } break;

      case GPU_COMMAND_OP_END_RENDER_PASS: {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisable(GL_SCISSOR_TEST);
      } break;

      case GPU_COMMAND_OP_BIND_PIPELINE: {
        glUseProgram(command.pipeline->raster.shader->program);

        auto target = command_buffer->render_pass.color.attachment;
        set_uniform_immediate_f32("master_time", engine.elapsed_time);
        set_uniform_immediate_mat4("projection", HMM_Orthographic_RH_NO(0, target->size.x, 0, target->size.y, GPU_NEAR_PLANE, GPU_FAR_PLANE));
        set_uniform_immediate_vec2("output_resolution", target->size);
        set_uniform_immediate_vec2("native_resolution", window.native_resolution);

        if (command.pipeline->blend.fn == GPU_BLEND_FUNC_NONE) {
          glDisable(GL_BLEND);
        }
        else {
          glEnable(GL_BLEND);
          glBlendEquation(dn_gpu_blend_func_to_gl_blend_func(command.pipeline->blend.fn));
          glBlendFunc(
            dn_gpu_blend_mode_to_gl_blend_mode(command.pipeline->blend.source), 
            dn_gpu_blend_mode_to_gl_blend_mode(command.pipeline->blend.destination)
          );

        }

        command_buffer->pipeline = command.pipeline;
      } break;

      case GPU_COMMAND_OP_BIND_BUFFERS: {
        // VERTEX BUFFERS
        auto& vertex_buffers = command.bindings.vertex;
        auto& pipeline = *command_buffer->pipeline;

        assert(vertex_buffers.count <= pipeline.num_buffer_layouts);

        u32 attribute_index = 0;
        for (u32 buffer_index = 0; buffer_index < vertex_buffers.count; buffer_index++) {
          auto buffer_layout = pipeline.buffer_layouts[buffer_index];
          auto buffer = vertex_buffers.bindings[buffer_index].buffer;

          dn_gpu_buffer_bind(buffer);

          u32 stride = dn_gpu_vertex_layout_calculate_stride(&buffer_layout);

          u64 offset = 0;
          for (u32 i = 0; i < buffer_layout.num_vertex_attributes; i++) {
            glEnableVertexAttribArray(attribute_index);

            auto attribute = buffer_layout.vertex_attributes[i];
            
            switch(attribute.kind) {
              case GPU_VERTEX_ATTRIBUTE_FLOAT: glVertexAttribPointer(attribute_index, attribute.count, GL_FLOAT,        GL_FALSE, stride, dn_gpu_u32_to_gl_void_pointer(offset)); break;
              case GPU_VERTEX_ATTRIBUTE_U32:   glVertexAttribIPointer(attribute_index, attribute.count, GL_UNSIGNED_INT,           stride, dn_gpu_u32_to_gl_void_pointer(offset)); break;
              default: {
                assert(false);
              } break;
            }

            glVertexAttribDivisor(attribute_index, attribute.divisor);

            auto type_info = dn_gpu_vertex_attribute_info(attribute.kind);
            offset += attribute.count * type_info.size;
            attribute_index++;
          }
        }

        // UNIFORMS
        auto& uniforms = command.bindings.uniforms;
        for (u32 i = 0; i < uniforms.count; i++) {
          auto& binding = uniforms.bindings[i];
          auto uniform = binding.uniform;

          i32 index = find_uniform_index(uniform->name);

          switch(binding.uniform->kind) {
            case GPU_UNIFORM_MATRIX4: glUniformMatrix4fv(index, 1, GL_FALSE, (const float*)&binding.data.mat4); break;
            case GPU_UNIFORM_MATRIX3: glUniformMatrix3fv(index, 1, GL_FALSE, (const float*)&binding.data.mat3); break;
            case GPU_UNIFORM_MATRIX2: glUniformMatrix2fv(index, 1, GL_FALSE, (const float*)&binding.data.mat2); break;
            case GPU_UNIFORM_VECTOR4: glUniform4fv(index, 1, (const float*)&binding.data.vec4); break;
            case GPU_UNIFORM_VECTOR3: glUniform3fv(index, 1, (const float*)&binding.data.vec3); break;
            case GPU_UNIFORM_VECTOR2: glUniform2fv(index, 1, (const float*)&binding.data.vec2); break;
            case GPU_UNIFORM_F32:     glUniform1fv(index, 1, (const float*)&binding.data.f32); break;
            case GPU_UNIFORM_TEXTURE: glActiveTexture(GL_TEXTURE0 + binding.binding_index); glBindTexture(GL_TEXTURE_2D, binding.data.texture); break;
            case GPU_UNIFORM_ENUM:    glUniform1iv(index, 1, (const i32*)&binding.data.i32); break;

          }
        }

        auto& storage = command.bindings.storage;
        for (u32 i = 0; i < storage.count; i++) {
          auto& binding = storage.bindings[i];

          assert(binding.buffer->kind == GPU_BUFFER_KIND_STORAGE);
          glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding.base, binding.buffer->handle);
        }

        command_buffer->bindings = command.bindings;
      } break;

      case GPU_COMMAND_OP_SET_SCISSOR: {
        if (command.scissor.enabled != command_buffer->scissor.enabled) {
          if (command.scissor.enabled) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(
              command.scissor.position.x, command.scissor.position.y, 
              command.scissor.size.x, command.scissor.size.y);
          }
          else {
            glDisable(GL_SCISSOR_TEST);
          }
        }

        command_buffer->scissor = command.scissor;
      } break;

      case GPU_COMMAND_OP_SET_WORLD_SPACE: {
        command_buffer->render.world_space = command.render.world_space;
      } break;
      case GPU_COMMAND_OP_SET_CAMERA: {
        command_buffer->render.camera = command.render.camera;
      } break;
      case GPU_COMMAND_OP_SET_LAYER: {
        command_buffer->render.layer = command.render.layer;
      } break;

      case GPU_COMMAND_OP_DRAW: {      
        auto view_transform = command_buffer->render.world_space ? 
          HMM_Translate(HMM_V3(-command_buffer->render.camera.x, -command_buffer->render.camera.y, 0.f)) :
          HMM_M4D(1.0);

        set_uniform_immediate_mat4("view", view_transform);
        set_uniform_immediate_vec2("camera", command_buffer->render.camera);

        auto primitive = dn_gpu_draw_primitive_to_gl_draw_primitive(pipeline.raster.primitive);
        switch (command.draw.mode) {
          case GPU_DRAW_MODE_ARRAYS: glDrawArrays(primitive, command.draw.vertex_offset, command.draw.num_vertices); break;
          case GPU_DRAW_MODE_INSTANCE: glDrawArraysInstanced(primitive, command.draw.vertex_offset, command.draw.num_vertices, command.draw.num_instances); break;
        }
      } break;
    }
  }

  glBindVertexArray(0);
  dn_gpu_command_buffer_clear_cached_state(command_buffer);
  arr_clear(&command_buffer->commands);
}


void dn_gpu_command_buffer_draw(dn_gpu_command_buffer_t* command_buffer, dn_gpu_draw_call_t draw_call) {
  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_DRAW,
    .draw = draw_call
  });
}


//////////////
// PIPELINE //
//////////////
dn_gpu_pipeline_t* dn_gpu_pipeline_create(dn_gpu_pipeline_descriptor_t descriptor) {
  dn_gpu_pipeline_t* pipeline = arr_push(&dn_gpu.pipelines);
  pipeline->raster = descriptor.raster;
  pipeline->blend = descriptor.blend;
  copy_memory(descriptor.buffer_layouts, pipeline->buffer_layouts, descriptor.num_buffer_layouts * sizeof(dn_gpu_buffer_layout_t));
  pipeline->num_buffer_layouts = descriptor.num_buffer_layouts;

  return pipeline;
}

void dn_gpu_bind_pipeline(dn_gpu_command_buffer_t* command_buffer, dn_gpu_pipeline_t* pipeline) {
  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_BIND_PIPELINE,
    .pipeline = pipeline
  });
}


/////////////
// BINDING //
/////////////
void dn_gpu_begin_render_pass(dn_gpu_command_buffer_t* command_buffer, dn_gpu_render_pass_t render_pass) {
  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_BEGIN_RENDER_PASS,
    .render_pass = render_pass
  });
}

void dn_gpu_end_render_pass(dn_gpu_command_buffer_t* command_buffer) {
  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_END_RENDER_PASS,
  });
}

void dn_gpu_apply_bindings(dn_gpu_command_buffer_t* command_buffer, dn_gpu_buffer_binding_t bindings) {
  if (is_memory_equal(&command_buffer->bindings, &bindings, sizeof(dn_gpu_buffer_binding_t))) return;

  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_BIND_BUFFERS,
    .bindings = bindings
  });
}


//////////////////
// RENDER STATE //
//////////////////
void dn_gpu_set_layer(dn_gpu_command_buffer_t* command_buffer, u32 layer) {
  if (command_buffer->render.layer == layer) return;

  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_SET_LAYER,
    .render = {
      .layer = layer
    }
  });
}

void dn_gpu_set_world_space(dn_gpu_command_buffer_t* command_buffer, bool world_space) {
  if (command_buffer->render.world_space == world_space) return;

  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_SET_WORLD_SPACE,
    .render = {
      .world_space = world_space
    }
  });
}

void dn_gpu_set_camera(dn_gpu_command_buffer_t* command_buffer, Vector2 camera) {
  if (is_memory_equal(&command_buffer->render.camera,  &camera, sizeof(Vector2))) return;

  arr_push(&command_buffer->commands, {
    .op = GPU_COMMAND_OP_SET_CAMERA,
    .render = {
      .camera = camera
    }
  });
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
  auto uniform = arr_push(&dn_gpu.uniforms);
  dn_asset_copy_name(descriptor.name, uniform->name);
  uniform->kind = descriptor.kind;
  
  return uniform;
}


/////////////////
// GPU BUFFERS //
/////////////////
dn_gpu_buffer_t* dn_gpu_buffer_create(dn_gpu_buffer_descriptor_t descriptor) {
  auto buffer = arr_push(&dn_gpu.gpu_buffers);
  dn_asset_copy_name(descriptor.name, buffer->name);
  buffer->kind = descriptor.kind;
  buffer->usage = descriptor.usage;
  buffer->size = descriptor.capacity * descriptor.element_size;
  glGenBuffers(1, &buffer->handle);
  
  dn_gpu_buffer_sync(buffer, nullptr, buffer->size);

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
  auto data = bump_allocator.alloc<u8>(size);
  dn_gpu_buffer_sync(buffer, data, size);
}

dn_gpu_backed_buffer_t dn_gpu_backed_buffer_create(dn_gpu_buffer_descriptor_t descriptor) {
  dn_gpu_backed_buffer_t backed_buffer;
  backed_buffer.gpu_buffer = dn_gpu_buffer_create(descriptor);
  dn_fixed_array_init(&backed_buffer.buffer, descriptor.capacity, descriptor.element_size, &standard_allocator);
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
    case GPU_SHADER_GRAPHICS: dn_log("%s: GPU_SHADER_GRAPHICS (%s) (%s)", __func__, descriptor.vertex_shader, descriptor.fragment_shader); break;
    case GPU_SHADER_COMPUTE: dn_log("%s: GPU_SHADER_COMPUTE (%s)", __func__, descriptor.compute_shader); break;
  }
  
  auto shader = arr_push(&dn_gpu.shaders);
  dn_gpu_shader_init(shader, descriptor);
  dn_assets_add(shader->name, shader);
  
  return shader;  
}

dn_gpu_shader_t* dn_gpu_shader_find(const char* name) {
  arr_for(dn_gpu.shaders, shader) {
    if (!strncmp(shader->name, name, DN_MAX_PATH_LEN)) return shader;
  }

  return nullptr;
}

////////////////////
// SHADER LOADING //
////////////////////
tstring build_shader_source(const char* file_path) {
  auto shader_file = copy_string(file_path, &bump_allocator);
  auto error = bump_allocator.alloc<char>(256);
  
  dn_preprocessor_context_t context = {
    .file_path = file_path,
    .include_dirs = {
      .data = dn_fixed_array_at_t(&dn_gpu.search_paths, 0),
      .count = dn_gpu.search_paths.size
    }
  };
  dn_preprocess(&context);

  if (!context.result) {
    dn_log("shader preprocessor error; shader = %s, err = %s", shader_file, context.error);
    return copy_string("YOUR_SHADER_FAILED_TO_COMPILE", &bump_allocator);
  }
  
  auto source = copy_string(context.result, &bump_allocator);
  
  return source;
}

void check_shader_compilation(u32 shader, const char* file_path) {
  i32 success;
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    static constexpr u32 error_size = 512;
    auto compilation_status = bump_allocator.alloc<char>(error_size);
    
    glGetShaderInfoLog(shader, error_size, NULL, compilation_status);

    dn_log("shader compile error; shader = %s, err = %s", file_path, compilation_status);
  }
}

void check_shader_linkage(u32 program, const char* file_path) {
  i32 success;
  
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    static constexpr u32 error_size = 512;
    auto compilation_status = bump_allocator.alloc<char>(error_size);
    
    glGetProgramInfoLog(program, error_size, NULL, compilation_status);
    dn_log("shader link error; shader = %s, err = %s", file_path, compilation_status);
  }
}


////////////////
// GPU SHADER //
////////////////
void dn_gpu_shader_init(dn_gpu_shader_t* shader, dn_gpu_shader_descriptor_t descriptor) {
  switch (descriptor.kind) {
    case GPU_SHADER_COMPUTE: {
      shader->kind = GPU_SHADER_COMPUTE;
      copy_string(descriptor.name, shader->name, DN_ASSET_NAME_LEN);
      shader->compute.path = copy_string(descriptor.compute_shader);
      auto source = build_shader_source(shader->compute.path);

      u32 num_shaders = 1;
      shader->compute.shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader->compute.shader, num_shaders, &source, NULL);
      glCompileShader(shader->compute.shader);
      check_shader_compilation(shader->compute.shader, shader->compute.path);

      shader->program = glCreateProgram();
      glAttachShader(shader->program, shader->compute.shader);
      glLinkProgram(shader->program);
      check_shader_linkage(shader->program, shader->compute.path);

    } break;
    case GPU_SHADER_GRAPHICS: {
      shader->kind = GPU_SHADER_GRAPHICS;
      copy_string(descriptor.name, shader->name, DN_ASSET_NAME_LEN);
      shader->graphics.vertex_path = copy_string(descriptor.vertex_shader);
      shader->graphics.fragment_path = copy_string(descriptor.fragment_shader);
      

      const char* paths[] = {
        descriptor.vertex_shader,
        descriptor.fragment_shader
      };

      shader->program = glCreateProgram();
      
      for (u32 index = 0; index < 2; index++) {
        auto file_path = paths[index];
        auto source = build_shader_source(file_path);
        if (!source) return;
        
        // Compile the shader
        unsigned int shader_kind = (index == 0) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        unsigned int shader_id = glCreateShader(shader_kind);
        if (shader_kind == GL_VERTEX_SHADER) {
          shader->graphics.vertex_shader = shader_id;
        }
        else if (shader_kind == GL_FRAGMENT_SHADER) {
          shader->graphics.fragment_shader = shader_id;
        }

        u32 num_shaders = 1;
        glShaderSource(shader_id, num_shaders, &source, NULL);
        glCompileShader(shader_id);
        check_shader_compilation(shader_id, file_path);

        glAttachShader(shader->program, shader_id);
      }
        
      // Link into a shader program
      glLinkProgram(shader->program);
      check_shader_linkage(shader->program, shader->graphics.vertex_path);
      dn_gpu_set_resource_name(GPU_RESOURCE_PROGRAM, shader->program, strlen(shader->name), shader->name);
    } break;
  }
}

void dn_gpu_shader_reload(dn_gpu_shader_t* shader) {
  glDeleteProgram(shader->program);

  switch (shader->kind) {
    case GPU_SHADER_GRAPHICS: {
      glDeleteShader(shader->graphics.vertex_shader);
      glDeleteShader(shader->graphics.fragment_shader);
      dn_gpu_shader_init(shader, {
        .name = shader->name,
        .vertex_shader = shader->graphics.vertex_path,
        .fragment_shader = shader->graphics.fragment_path,
        .compute_shader = NULL,
        .kind = GPU_SHADER_GRAPHICS,
      });
    } break;
    case GPU_SHADER_COMPUTE: {
      glDeleteShader(shader->compute.shader);
      dn_gpu_shader_init(shader, {
        .name = shader->name,
        .vertex_shader = NULL,
        .fragment_shader = NULL,
        .compute_shader = shader->compute.path,
        .kind = GPU_SHADER_COMPUTE,
      });
    } break;
    default: {
      DN_ASSERT(false);
    }
  }
}


///////////////////
// RENDER TARGET //
///////////////////
dn_gpu_render_target_t* dn_gpu_render_target_create(dn_gpu_render_target_descriptor_t descriptor) {
  auto target = arr_push(&dn_gpu.targets);
  dn_asset_copy_name(descriptor.name, target->name);
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

  dn_assets_add(target->name, target);


  return target;
}

void dn_gpu_destroy_target(dn_gpu_render_target_t* target) {
  glDeleteTextures(1, &target->color_buffer);
  glDeleteFramebuffers(1, &target->handle);
}

dn_gpu_render_target_t* dn_gpu_render_target_find(const char* name) {
  for (u32 i = 0; i < dn_gpu.targets.size; i++) {
    dn_gpu_render_target_t* target = dn_gpu.targets[i];
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
  return dn_gpu.targets[0];
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
  glfwSwapBuffers(window.handle);
}

/////////////////////
// ENUM CONVERSION //
/////////////////////
u32 dn_gpu_draw_primitive_to_gl_draw_primitive(dn_gpu_draw_primitive_t primitive) {
  switch (primitive) {
    case GPU_PRIMITIVE_TRIANGLES: return GL_TRIANGLES;
  }

  assert(false);
  return 0;
}

u32 dn_gpu_blend_func_to_gl_blend_func(dn_gpu_blend_func_t func) {
  switch (func) {
    case GPU_BLEND_FUNC_NONE:             return 0; break;
    case GPU_BLEND_FUNC_ADD:              return GL_FUNC_ADD; break;
    case GPU_BLEND_FUNC_SUBTRACT:         return GL_FUNC_SUBTRACT; break;
    case GPU_BLEND_FUNC_REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT; break;
    case GPU_BLEND_FUNC_MIN:              return GL_MIN; break;
    case GPU_BLEND_FUNC_MAX:              return GL_MAX; break;
  };

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_blend_mode_to_gl_blend_mode(dn_gpu_blend_mode_t mode) {
  switch (mode) {
    case GPU_BLEND_MODE_ZERO: return GL_ZERO; break;
    case GPU_BLEND_MODE_ONE: return GL_ONE; break;
    case GPU_BLEND_MODE_SRC_COLOR: return GL_SRC_COLOR; break;
    case GPU_BLEND_MODE_ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR; break;
    case GPU_BLEND_MODE_DST_COLOR: return GL_DST_COLOR; break;
    case GPU_BLEND_MODE_ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR; break;
    case GPU_BLEND_MODE_SRC_ALPHA: return GL_SRC_ALPHA; break;
    case GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA; break;
    case GPU_BLEND_MODE_DST_ALPHA: return GL_DST_ALPHA; break;
    case GPU_BLEND_MODE_ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA; break;
    case GPU_BLEND_MODE_CONSTANT_COLOR: return GL_CONSTANT_COLOR; break;
    case GPU_BLEND_MODE_ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR; break;
    case GPU_BLEND_MODE_CONSTANT_ALPHA: return GL_CONSTANT_ALPHA; break;
    case GPU_BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA: return GL_ONE_MINUS_CONSTANT_ALPHA; break;
  }

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_resource_id_to_gl_id(dn_gpu_resource_id_t id) {
  switch (id) {
    case GPU_RESOURCE_FRAMEBUFFER: return GL_FRAMEBUFFER; break;
    case GPU_RESOURCE_SHADER: return GL_SHADER; break;
    case GPU_RESOURCE_PROGRAM: return GL_PROGRAM; break;
  }
  
  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_memory_barrier_to_gl_barrier(dn_gpu_memory_barrier_t barrier) {
  switch (barrier) {
    case GPU_MEMORY_BARRIER_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
    case GPU_MEMORY_BARRIER_BUFFER_UPDATE: return GL_BUFFER_UPDATE_BARRIER_BIT; break;
  }
  
  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_buffer_kind_to_gl_buffer_kind(dn_gpu_buffer_kind_t kind) {
  switch (kind) {
    case GPU_BUFFER_KIND_STORAGE: return GL_SHADER_STORAGE_BUFFER; break;
    case GPU_BUFFER_KIND_ARRAY: return GL_ARRAY_BUFFER; break;
  }

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_buffer_usage_to_gl_buffer_usage(dn_gpu_buffer_usage_t usage) {
  switch (usage) {
    case GPU_BUFFER_USAGE_STATIC: return GL_STATIC_DRAW; break;
    case GPU_BUFFER_USAGE_DYNAMIC: return GL_DYNAMIC_DRAW; break;
    case GPU_BUFFER_USAGE_STREAM: return GL_STREAM_DRAW; break;
  }

  DN_ASSERT(false);
  return 0;
}

u32 dn_gpu_buffer_kind_to_gl_barrier(dn_gpu_buffer_kind_t kind) {
  switch (kind) {
    case GPU_BUFFER_KIND_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
    case GPU_BUFFER_KIND_ARRAY: return GL_BUFFER_UPDATE_BARRIER_BIT; break;
  }

  DN_ASSERT(false);
  return 0;
}

dn_gpu_vertex_attr_info_t dn_gpu_vertex_attribute_info(dn_gpu_vertex_attribute_kind_t kind) {
  switch (kind) {
    case GPU_VERTEX_ATTRIBUTE_FLOAT: {
      return {
        .size = sizeof(GLfloat),
        .value = GL_FLOAT,
      };
    } break;
    case GPU_VERTEX_ATTRIBUTE_U32: {
      return {
        .size = sizeof(GLuint),
        .value = GL_UNSIGNED_INT,
      };
    } break;
  }

  DN_ASSERT(false);
  return {0};
}

void* dn_gpu_u32_to_gl_void_pointer(u32 value) {
  return (void*)(uintptr_t)value;
}

u32 dn_gpu_vertex_layout_calculate_stride(dn_gpu_buffer_layout_t* layout) {
  assert(layout);

  u32 stride = 0;

  for (u32 i = 0; i < layout->num_vertex_attributes; i++) {
    auto attribute = layout->vertex_attributes[i];
    auto type_info = dn_gpu_vertex_attribute_info(attribute.kind);
    stride += attribute.count * type_info.size;
  }

  return stride;
}


///////////////////
// OPENGL ERRORS //
///////////////////
void dn_gpu_error_clear() {
  while (glGetError() != GL_NO_ERROR) {}
}

tstring dn_gpu_error_read() {
  auto error = glGetError();
  if (error == GL_INVALID_ENUM) {
    return copy_string("GL_INVALID_ENUM", &bump_allocator);
  }
  else if (error == GL_INVALID_OPERATION) {
    return copy_string("GL_INVALID_OPERATION", &bump_allocator);
  }
  else if (error == GL_OUT_OF_MEMORY) {
    return copy_string("GL_OUT_OF_MEMORY", &bump_allocator);
  }
  else if (error == GL_NO_ERROR) {
    return copy_string("GL_NO_ERROR", &bump_allocator);
  }

  return nullptr;
}

void dn_gpu_error_log_one() {
  dn_log(dn_gpu_error_read());
}

void dn_gpu_error_log_all() {
  while (true) {
    auto error = dn_gpu_error_read();
    if (!error) break;
    if (!strcmp(error, "GL_NO_ERROR")) break;

    dn_log(error);
  }
}

void gl_error_callback(GLenum source, GLenum type, GLuint id,GLenum severity, GLsizei length,const GLchar *msg, const void *data) {
  constexpr u32 GL_DEBUG_SEVERITY_NOTHING_EVER = GL_DEBUG_SEVERITY_HIGH - 1;
  constexpr u32 minimum_severity = GL_DEBUG_SEVERITY_MEDIUM;
  
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


    dn_log("%d: %s of %s severity, raised from %s: %s\n",
            id, _type, _severity, _source, msg);
  int x = 0;
}

void dn_gpu_set_resource_name(dn_gpu_resource_id_t id, u32 handle, u32 name_len, const char* name) {
  glObjectLabel(dn_gpu_resource_id_to_gl_id(id), handle, name_len, name);
}
#endif
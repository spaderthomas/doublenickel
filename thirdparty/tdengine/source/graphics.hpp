#ifndef GRAPHICS_H
#define GRAPHICS_H

#define GPU_NEAR_PLANE -100.0
#define GPU_FAR_PLANE 100.0

#define MAX_UNIFORM_NAME 64

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
  char name [MAX_UNIFORM_NAME];
  GpuUniformKind kind;
} GpuUniformDescriptor;

typedef struct {
  char name [MAX_UNIFORM_NAME];
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
  TD_ALIGN(16) struct {
    GpuVertexBufferBinding bindings [8];
    u32 count;
  } vertex;

  TD_ALIGN(16) struct {
    GpuUniformBinding bindings [8];
    u32 count;
  } uniforms;

  TD_ALIGN(16) struct {
    GpuStorageBufferBinding bindings [8];
    u32 count;
  } storage;

  // UBO
  // SSBO
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
  Matrix4 projection;
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
	u32 size;
	u32 value;
} GpuVertexAttributeInfo;

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

typedef struct {
  GpuCommandOp kind;
  union {
    GpuPipeline*       pipeline;
    GpuBufferBinding  bindings;
    GpuRenderPass     render_pass;
    GpuRendererState  render;
    GpuScissorState   scissor;
    GpuDrawCall       draw;
  };
} GpuCommand;

typedef struct {
  u32 max_commands;
} GpuCommandBufferDescriptor;

typedef struct {
  GpuPipeline* pipeline;
  GpuBufferBinding bindings;
  GpuRenderPass render_pass;
  GpuRendererState render;
  GpuScissorState scissor;

  Array<GpuCommand> commands;
  u32 vao;
} GpuCommandBuffer;


///////////////
// FUNCTIONS //
///////////////
DN_API GpuCommandBuffer* gpu_command_buffer_create(GpuCommandBufferDescriptor descriptor);
DN_API void              gpu_command_buffer_draw(GpuCommandBuffer* command_buffer, GpuDrawCall draw_call);
DN_API void              gpu_command_buffer_submit(GpuCommandBuffer* command_buffer);
DN_API void              gpu_bind_pipeline(GpuCommandBuffer* command_buffer, GpuPipeline* pipeline);
DN_API void              gpu_begin_render_pass(GpuCommandBuffer* command_buffer, GpuRenderPass render_pass);
DN_API void              gpu_end_render_pass(GpuCommandBuffer* command_buffer);
DN_API void              gpu_apply_bindings(GpuCommandBuffer* command_buffer, GpuBufferBinding bindings);
DN_API void              gpu_bind_render_state(GpuCommandBuffer* command_buffer, GpuRendererState render);
DN_API void              gpu_set_layer(GpuCommandBuffer* command_buffer, u32 layer);
DN_API void              gpu_set_world_space(GpuCommandBuffer* command_buffer, bool world_space);
DN_API void              gpu_set_camera(GpuCommandBuffer* command_buffer, Vector2 camera);
DN_API GpuPipeline*      gpu_pipeline_create(GpuPipelineDescriptor descriptor);
DN_API GpuUniform*       gpu_uniform_create(GpuUniformDescriptor descriptor);
DN_API GpuBuffer*        gpu_buffer_create(GpuBufferDescriptor descriptor);
DN_API void              gpu_buffer_bind(GpuBuffer* buffer);
DN_API void              gpu_buffer_bind_base(GpuBuffer* buffer, u32 base);
DN_API void              gpu_buffer_sync(GpuBuffer* buffer, void* data, u32 size);
DN_API void              gpu_buffer_sync_subdata(GpuBuffer* buffer, void* data, u32 byte_size, u32 byte_offset);
DN_API void              gpu_buffer_zero(GpuBuffer* buffer, u32 size);
DN_API GpuBackedBuffer   gpu_backed_buffer_create(GpuBufferDescriptor descriptor);
DN_API u32               gpu_backed_buffer_size(GpuBackedBuffer* buffer);
DN_API void              gpu_backed_buffer_clear(GpuBackedBuffer* buffer);
DN_API u8*               gpu_backed_buffer_push(GpuBackedBuffer* buffer, void* data, u32 num_elements);
DN_API void              gpu_backed_buffer_sync(GpuBackedBuffer* buffer);
DN_API GpuShader*        gpu_shader_create(GpuShaderDescriptor descriptor);
DN_API GpuShader*        gpu_shader_find(const char* name);
DN_API GpuRenderTarget*  gpu_render_target_create(GpuRenderTargetDescriptor descriptor);
DN_API GpuRenderTarget*  gpu_acquire_swapchain();
DN_API void              gpu_render_target_bind(GpuRenderTarget* target);
DN_API void              gpu_render_target_clear(GpuRenderTarget* target);
DN_API void              gpu_render_target_blit(GpuRenderTarget* source, GpuRenderTarget* destination);
DN_API void              gpu_memory_barrier(GpuMemoryBarrier barrier);
DN_API void              gpu_dispatch_compute(GpuBuffer* buffer, u32 size);
DN_API void              gpu_swap_buffers();
DN_API void              gpu_error_clear();
DN_API tstring           gpu_error_read();
DN_API void              gpu_error_log_one();
DN_API void              gpu_error_log_all();
DN_API void              gpu_set_resource_name(GpuResourceId id, u32 handle, u32 name_len, const char* name);

//////////////
// INTERNAL //
//////////////
typedef struct {
  Array<GpuCommandBuffer, 32> command_buffers;
  Array<GpuUniform, 1024> uniforms;
  Array<GpuPipeline, 64> pipelines;
	Array<GpuBuffer, 128>  gpu_buffers;
  Array<GpuRenderTarget, 32>  targets;
	Array<GpuShader, 128> shaders;

	FileMonitor* shader_monitor;
} GpuInfo;
GpuInfo td_gpu;

void                   gpu_init();
void                   gpu_command_buffer_clear_cached_state(GpuCommandBuffer* command_buffer);
u32                    gpu_vertex_layout_calculate_stride(GpuBufferLayout* layout);
u32                    gpu_draw_primitive_to_gl_draw_primitive(GpuDrawPrimitive primitive);
GpuVertexAttributeInfo gpu_vertex_attribute_info(GpuVertexAttributeKind kind);
void*                  gpu_u32_to_gl_void_pointer(u32 value);
u32                    gpu_buffer_kind_to_gl_buffer_kind(GpuBufferKind kind);
u32                    gpu_buffer_usage_to_gl_buffer_usage(GpuBufferUsage usage);
u32                    gpu_buffer_kind_to_gl_barrier(GpuBufferKind kind);
u32                    gpu_blend_func_to_gl_blend_func(GpuBlendFunction func);
u32                    gpu_blend_mode_to_gl_blend_mode(GpuBlendMode mode);
u32                    gpu_resource_id_to_gl_id(GpuResourceId id);
u32                    gpu_memory_barrier_to_gl_barrier(GpuMemoryBarrier barrier);
#endif // GRAPHICS_H



////////////////////
// IMPLEMENTATION //
////////////////////
#ifdef GRAPHICS_IMPLEMENTATION


////////////////////
// COMMAND BUFFER //
////////////////////
GpuCommandBuffer* gpu_command_buffer_create(GpuCommandBufferDescriptor descriptor) {
  auto command_buffer = arr_push(&td_gpu.command_buffers);
  arr_init(&command_buffer->commands, descriptor.max_commands);
  glGenVertexArrays(1, &command_buffer->vao);
  glBindVertexArray(command_buffer->vao);

  return command_buffer;
}

void gpu_command_buffer_clear_cached_state(GpuCommandBuffer* command_buffer) {
  command_buffer->pipeline = nullptr;
  zero_memory(&command_buffer->bindings, sizeof(GpuBufferBinding));
  zero_memory(&command_buffer->render_pass, sizeof(GpuRenderPass));
  zero_memory(&command_buffer->render, sizeof(GpuRendererState));
  zero_memory(&command_buffer->scissor, sizeof(GpuScissorState));
}

void gpu_command_buffer_submit(GpuCommandBuffer* command_buffer) {
  gpu_command_buffer_clear_cached_state(command_buffer);
  glBindVertexArray(command_buffer->vao);

  arr_for(command_buffer->commands, it) {
    auto& command = *it;
    auto& pipeline = *command_buffer->pipeline;

    switch (command.kind) {
      case GPU_COMMAND_OP_BEGIN_RENDER_PASS: {
        switch (command.render_pass.color.load) {
          case GPU_LOAD_OP_CLEAR: {
            gpu_render_target_clear(command.render_pass.color.attachment);
          }
        }

        gpu_render_target_bind(command.render_pass.color.attachment);
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
          glBlendEquation(gpu_blend_func_to_gl_blend_func(command.pipeline->blend.fn));
          glBlendFunc(
            gpu_blend_mode_to_gl_blend_mode(command.pipeline->blend.source), 
            gpu_blend_mode_to_gl_blend_mode(command.pipeline->blend.destination)
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

          gpu_buffer_bind(buffer);

          u32 stride = gpu_vertex_layout_calculate_stride(&buffer_layout);

          u64 offset = 0;
          for (u32 i = 0; i < buffer_layout.num_vertex_attributes; i++) {
            glEnableVertexAttribArray(attribute_index);

            auto attribute = buffer_layout.vertex_attributes[i];
            
            switch(attribute.kind) {
              case GPU_VERTEX_ATTRIBUTE_FLOAT: glVertexAttribPointer(attribute_index, attribute.count, GL_FLOAT,        GL_FALSE, stride, gpu_u32_to_gl_void_pointer(offset)); break;
              case GPU_VERTEX_ATTRIBUTE_U32:   glVertexAttribIPointer(attribute_index, attribute.count, GL_UNSIGNED_INT,           stride, gpu_u32_to_gl_void_pointer(offset)); break;
              default: {
                assert(false);
              } break;
            }

            glVertexAttribDivisor(attribute_index, attribute.divisor);

            auto type_info = gpu_vertex_attribute_info(attribute.kind);
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

        auto primitive = gpu_draw_primitive_to_gl_draw_primitive(pipeline.raster.primitive);
        switch (command.draw.mode) {
          case GPU_DRAW_MODE_ARRAYS: glDrawArrays(primitive, command.draw.vertex_offset, command.draw.num_vertices); break;
          case GPU_DRAW_MODE_INSTANCE: glDrawArraysInstanced(primitive, command.draw.vertex_offset, command.draw.num_vertices, command.draw.num_instances); break;
        }
      } break;
    }
  }

  glBindVertexArray(0);
  gpu_command_buffer_clear_cached_state(command_buffer);
  arr_clear(&command_buffer->commands);
}


void gpu_command_buffer_draw(GpuCommandBuffer* command_buffer, GpuDrawCall draw_call) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_DRAW,
    .draw = draw_call
  });
}


//////////////
// PIPELINE //
//////////////
GpuPipeline* gpu_pipeline_create(GpuPipelineDescriptor descriptor) {
  GpuPipeline* pipeline = arr_push(&td_gpu.pipelines);
  pipeline->raster = descriptor.raster;
  pipeline->blend = descriptor.blend;
  copy_memory(descriptor.buffer_layouts, pipeline->buffer_layouts, descriptor.num_buffer_layouts * sizeof(GpuBufferLayout));
  pipeline->num_buffer_layouts = descriptor.num_buffer_layouts;

  return pipeline;
}

void gpu_bind_pipeline(GpuCommandBuffer* command_buffer, GpuPipeline* pipeline) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_BIND_PIPELINE,
    .pipeline = pipeline
  });
}


/////////////
// BINDING //
/////////////
void gpu_begin_render_pass(GpuCommandBuffer* command_buffer, GpuRenderPass render_pass) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_BEGIN_RENDER_PASS,
    .render_pass = render_pass
  });
}

void gpu_end_render_pass(GpuCommandBuffer* command_buffer) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_END_RENDER_PASS,
  });
}

void gpu_apply_bindings(GpuCommandBuffer* command_buffer, GpuBufferBinding bindings) {
  if (is_memory_equal(&command_buffer->bindings, &bindings, sizeof(GpuBufferBinding))) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_BIND_BUFFERS,
    .bindings = bindings
  });
}


//////////////////
// RENDER STATE //
//////////////////
void gpu_set_layer(GpuCommandBuffer* command_buffer, u32 layer) {
  if (command_buffer->render.layer == layer) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_SET_LAYER,
    .render = {
      .layer = layer
    }
  });
}

void gpu_set_world_space(GpuCommandBuffer* command_buffer, bool world_space) {
  if (command_buffer->render.world_space == world_space) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_SET_WORLD_SPACE,
    .render = {
      .world_space = world_space
    }
  });
}

void gpu_set_camera(GpuCommandBuffer* command_buffer, Vector2 camera) {
  if (is_memory_equal(&command_buffer->render.camera,  &camera, sizeof(Vector2))) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_SET_CAMERA,
    .render = {
      .camera = camera
    }
  });
}

void gpu_bind_render_state(GpuCommandBuffer* command_buffer, GpuRendererState render) {
  gpu_set_layer(command_buffer, render.layer);
  gpu_set_camera(command_buffer, render.camera);
  gpu_set_world_space(command_buffer, render.world_space);
}


//////////////
// UNIFORMS //
//////////////
GpuUniform* gpu_uniform_create(GpuUniformDescriptor descriptor) {
  auto uniform = arr_push(&td_gpu.uniforms);
  copy_string_n(descriptor.name, MAX_UNIFORM_NAME, uniform->name, MAX_UNIFORM_NAME);
  uniform->kind = descriptor.kind;
  
  return uniform;
}


/////////////////
// GPU BUFFERS //
/////////////////
GpuBuffer* gpu_buffer_create(GpuBufferDescriptor descriptor) {
	auto buffer = arr_push(&td_gpu.gpu_buffers);
  copy_string_n(descriptor.name, 64, buffer->name, 64);
	buffer->kind = descriptor.kind;
	buffer->usage = descriptor.usage;
	buffer->size = descriptor.capacity * descriptor.element_size;
	glGenBuffers(1, &buffer->handle);
	
	gpu_buffer_sync(buffer, nullptr, buffer->size);

	return buffer;
}

void gpu_memory_barrier(GpuMemoryBarrier barrier) {
	glMemoryBarrier(gpu_memory_barrier_to_gl_barrier(barrier));
}

void gpu_buffer_bind(GpuBuffer* buffer) {
	glBindBuffer(gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), buffer->handle);
}

void gpu_buffer_bind_base(GpuBuffer* buffer, u32 base) {
	glBindBufferBase(gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), base, buffer->handle);
}

void gpu_buffer_sync(GpuBuffer* buffer, void* data, u32 size) {
	gpu_buffer_bind(buffer);
	glBufferData(gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), size, data, gpu_buffer_usage_to_gl_buffer_usage(buffer->usage));
	glMemoryBarrier(gpu_buffer_kind_to_gl_barrier(buffer->kind));
}

void gpu_buffer_sync_subdata(GpuBuffer* buffer, void* data, u32 byte_size, u32 byte_offset) {
	gpu_buffer_bind(buffer);
	glBufferSubData(gpu_buffer_kind_to_gl_buffer_kind(buffer->kind), byte_offset, byte_size, data);
	glMemoryBarrier(gpu_buffer_kind_to_gl_barrier(buffer->kind));
}

void gpu_buffer_zero(GpuBuffer* buffer, u32 size) {
	auto data = bump_allocator.alloc<u8>(size);
	gpu_buffer_sync(buffer, data, size);
}

GpuBackedBuffer gpu_backed_buffer_create(GpuBufferDescriptor descriptor) {
  GpuBackedBuffer backed_buffer;
  backed_buffer.gpu_buffer = gpu_buffer_create(descriptor);
  dn_fixed_array_init(&backed_buffer.buffer, descriptor.capacity, descriptor.element_size);
  return backed_buffer;
}

void gpu_backed_buffer_clear(GpuBackedBuffer* buffer) {
  buffer->buffer.size = 0;
}

u32 gpu_backed_buffer_size(GpuBackedBuffer* buffer) {
  return buffer->buffer.size;
}

u8* gpu_backed_buffer_push(GpuBackedBuffer* buffer, void* data, u32 num_elements) {
  return dn_fixed_array_push(&buffer->buffer, data, num_elements);
}

void gpu_backed_buffer_sync(GpuBackedBuffer* buffer) {
  gpu_buffer_sync(buffer->gpu_buffer, buffer->buffer.data, buffer->buffer.size * buffer->buffer.vertex_size);
}


////////////////
// GPU SHADER //
////////////////
GpuShader* gpu_shader_create(GpuShaderDescriptor descriptor) {
	auto shader = arr_push(&td_gpu.shaders);
	shader->init(descriptor);
	return shader;
}

GpuShader* gpu_shader_find(const char* name) {
	arr_for(td_gpu.shaders, shader) {
		if (!strncmp(shader->name, name, TD_MAX_PATH_LEN)) return shader;
	}

	return nullptr;
}


///////////////////
// RENDER TARGET //
///////////////////
GpuRenderTarget* gpu_render_target_create(GpuRenderTargetDescriptor descriptor) {
	auto target = arr_push(&td_gpu.targets);
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

	return target;
}

void gpu_destroy_target(GpuRenderTarget* target) {
	glDeleteTextures(1, &target->color_buffer);
	glDeleteFramebuffers(1, &target->handle);
}

void gpu_render_target_bind(GpuRenderTarget* target) {
	if (!target) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, target->handle);
	glViewport(0, 0, target->size.x, target->size.y);
}

GpuRenderTarget* gpu_acquire_swapchain() {
	return td_gpu.targets[0];
}

void gpu_render_target_clear(GpuRenderTarget* target) {
	if (!target) return;
	
	gpu_render_target_bind(target);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void gpu_render_target_blit(GpuRenderTarget* source, GpuRenderTarget* destination) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source->handle);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination->handle);
	glBlitFramebuffer(0, 0, source->size.x, source->size.y, 0, 0, destination->size.x, destination->size.y,  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT);
}

void gpu_swap_buffers() {
	glfwSwapBuffers(window.handle);
}

/////////////////////
// ENUM CONVERSION //
/////////////////////
u32 gpu_draw_primitive_to_gl_draw_primitive(GpuDrawPrimitive primitive) {
  switch (primitive) {
    case GPU_PRIMITIVE_TRIANGLES: return GL_TRIANGLES;
  }

  assert(false);
  return 0;
}

u32 gpu_blend_func_to_gl_blend_func(GpuBlendFunction func) {
  switch (func) {
    case GPU_BLEND_FUNC_NONE:             return 0; break;
    case GPU_BLEND_FUNC_ADD:              return GL_FUNC_ADD; break;
    case GPU_BLEND_FUNC_SUBTRACT:         return GL_FUNC_SUBTRACT; break;
    case GPU_BLEND_FUNC_REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT; break;
    case GPU_BLEND_FUNC_MIN:              return GL_MIN; break;
    case GPU_BLEND_FUNC_MAX:              return GL_MAX; break;
  };

  TD_ASSERT(false);
  return 0;
}

u32 gpu_blend_mode_to_gl_blend_mode(GpuBlendMode mode) {
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

  TD_ASSERT(false);
  return 0;
}

u32 gpu_resource_id_to_gl_id(GpuResourceId id) {
  switch (id) {
    case GPU_RESOURCE_FRAMEBUFFER: return GL_FRAMEBUFFER; break;
    case GPU_RESOURCE_SHADER: return GL_SHADER; break;
    case GPU_RESOURCE_PROGRAM: return GL_PROGRAM; break;
  }
	
  TD_ASSERT(false);
  return 0;
}

u32 gpu_memory_barrier_to_gl_barrier(GpuMemoryBarrier barrier) {
  switch (barrier) {
    case GPU_MEMORY_BARRIER_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
    case GPU_MEMORY_BARRIER_BUFFER_UPDATE: return GL_BUFFER_UPDATE_BARRIER_BIT; break;
  }
	
  TD_ASSERT(false);
  return 0;
}

u32 gpu_buffer_kind_to_gl_buffer_kind(GpuBufferKind kind) {
  switch (kind) {
    case GPU_BUFFER_KIND_STORAGE: return GL_SHADER_STORAGE_BUFFER; break;
    case GPU_BUFFER_KIND_ARRAY: return GL_ARRAY_BUFFER; break;
  }

  TD_ASSERT(false);
	return 0;
}

u32 gpu_buffer_usage_to_gl_buffer_usage(GpuBufferUsage usage) {
  switch (usage) {
    case GPU_BUFFER_USAGE_STATIC: return GL_STATIC_DRAW; break;
    case GPU_BUFFER_USAGE_DYNAMIC: return GL_DYNAMIC_DRAW; break;
    case GPU_BUFFER_USAGE_STREAM: return GL_STREAM_DRAW; break;
  }

  TD_ASSERT(false);
	return 0;
}

u32 gpu_buffer_kind_to_gl_barrier(GpuBufferKind kind) {
  switch (kind) {
    case GPU_BUFFER_KIND_STORAGE: return GL_SHADER_STORAGE_BARRIER_BIT; break;
    case GPU_BUFFER_KIND_ARRAY: return GL_BUFFER_UPDATE_BARRIER_BIT; break;
  }

  TD_ASSERT(false);
	return 0;
}

GpuVertexAttributeInfo gpu_vertex_attribute_info(GpuVertexAttributeKind kind) {
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

  TD_ASSERT(false);
  return {0};
}

void* gpu_u32_to_gl_void_pointer(u32 value) {
  return (void*)(uintptr_t)value;
}

u32 gpu_vertex_layout_calculate_stride(GpuBufferLayout* layout) {
  assert(layout);

  u32 stride = 0;

  for (u32 i = 0; i < layout->num_vertex_attributes; i++) {
    auto attribute = layout->vertex_attributes[i];
    auto type_info = gpu_vertex_attribute_info(attribute.kind);
    stride += attribute.count * type_info.size;
  }

  return stride;
}


//////////////
// INTERNAL //
//////////////
void gpu_init() {
  arr_init(&td_gpu.command_buffers);
  arr_init(&td_gpu.uniforms);
  arr_init(&td_gpu.pipelines);
	arr_init(&td_gpu.gpu_buffers);
  arr_init(&td_gpu.targets);
	arr_init(&td_gpu.shaders);

	auto swapchain = arr_push(&td_gpu.targets);
	swapchain->handle = 0;
	swapchain->color_buffer = 0;
	swapchain->size = window.content_area;

	auto reload_all_shaders = [](FileMonitor* file_monitor, FileChange* event, void* userdata) {
		tdns_log.write("SHADER_RELOAD");
		arr_for(td_gpu.shaders, shader) {
			shader->reload();
		}
	};
	td_gpu.shader_monitor = arr_push(&file_monitors);
	td_gpu.shader_monitor->init(reload_all_shaders, FileChangeEvent::Modified, nullptr);
	td_gpu.shader_monitor->add_directory(dn_paths_resolve("shaders"));
}


///////////////////
// OPENGL ERRORS //
///////////////////
void gpu_error_clear() {
	while (glGetError() != GL_NO_ERROR) {}
}

tstring gpu_error_read() {
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

void gpu_error_log_one() {
	tdns_log.write(gpu_error_read());
}

void gpu_error_log_all() {
	while (true) {
		auto error = gpu_error_read();
		if (!error) break;
		if (!strcmp(error, "GL_NO_ERROR")) break;

		tdns_log.write(error);
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


    tdns_log.write("%d: %s of %s severity, raised from %s: %s\n",
            id, _type, _severity, _source, msg);
	int x = 0;
}

void gpu_set_resource_name(GpuResourceId id, u32 handle, u32 name_len, const char* name) {
	glObjectLabel(gpu_resource_id_to_gl_id(id), handle, name_len, name);
}
#endif // GRAPHICS_IMPL
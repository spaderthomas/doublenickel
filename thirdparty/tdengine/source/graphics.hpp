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
  FixedArray buffer;
  GpuBuffer* gpu_buffer;
} GpuBackedBuffer;



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
FM_LUA_EXPORT GpuCommandBuffer* _gpu_command_buffer_create(GpuCommandBufferDescriptor descriptor);
FM_LUA_EXPORT void              _gpu_command_buffer_draw(GpuCommandBuffer* command_buffer, GpuDrawCall draw_call);
FM_LUA_EXPORT void              _gpu_command_buffer_submit(GpuCommandBuffer* command_buffer);
FM_LUA_EXPORT void              _gpu_bind_pipeline(GpuCommandBuffer* command_buffer, GpuPipeline* pipeline);
FM_LUA_EXPORT void              _gpu_begin_render_pass(GpuCommandBuffer* command_buffer, GpuRenderPass render_pass);
FM_LUA_EXPORT void              _gpu_end_render_pass(GpuCommandBuffer* command_buffer);
FM_LUA_EXPORT void              _gpu_apply_bindings(GpuCommandBuffer* command_buffer, GpuBufferBinding bindings);
FM_LUA_EXPORT void              _gpu_bind_render_state(GpuCommandBuffer* command_buffer, GpuRendererState render);
FM_LUA_EXPORT void              _gpu_set_layer(GpuCommandBuffer* command_buffer, u32 layer);
FM_LUA_EXPORT void              _gpu_set_world_space(GpuCommandBuffer* command_buffer, bool world_space);
FM_LUA_EXPORT void              _gpu_set_camera(GpuCommandBuffer* command_buffer, Vector2 camera);
FM_LUA_EXPORT GpuPipeline*      _gpu_pipeline_create(GpuPipelineDescriptor descriptor);
FM_LUA_EXPORT GpuUniform*       _gpu_uniform_create(GpuUniformDescriptor descriptor);
FM_LUA_EXPORT GpuBuffer*         gpu_buffer_create(GpuBufferDescriptor descriptor);
FM_LUA_EXPORT void               gpu_buffer_bind(GpuBuffer* buffer);
FM_LUA_EXPORT void               gpu_buffer_bind_base(GpuBuffer* buffer, u32 base);
FM_LUA_EXPORT void               gpu_buffer_sync(GpuBuffer* buffer, void* data, u32 size);
FM_LUA_EXPORT void               gpu_buffer_sync_subdata(GpuBuffer* buffer, void* data, u32 byte_size, u32 byte_offset);
FM_LUA_EXPORT void               gpu_buffer_zero(GpuBuffer* buffer, u32 size);
FM_LUA_EXPORT GpuBackedBuffer    gpu_backed_buffer_create(GpuBufferDescriptor descriptor);
FM_LUA_EXPORT u32                gpu_backed_buffer_size(GpuBackedBuffer* buffer);
FM_LUA_EXPORT void               gpu_backed_buffer_clear(GpuBackedBuffer* buffer);
FM_LUA_EXPORT u8*                gpu_backed_buffer_push(GpuBackedBuffer* buffer, void* data, u32 num_elements);
FM_LUA_EXPORT void               gpu_backed_buffer_sync(GpuBackedBuffer* buffer);


//////////////
// INTERNAL //
//////////////
typedef struct {
  Array<GpuCommandBuffer, 32> command_buffers;
  Array<GpuUniform, 1024> uniforms;
  Array<GpuPipeline, 64> pipelines;
	Array<GpuBuffer, 128>  gpu_buffers;
} CommandRenderer;
CommandRenderer command_renderer;

void init_command_renderer();
void test_command_renderer();

void _gpu_command_buffer_clear_cached_state(GpuCommandBuffer* command_buffer);
u32 _gpu_vertex_layout_calculate_stride(GpuBufferLayout* layout);
u32 gpu_draw_primitive_to_gl_draw_primitive(GpuDrawPrimitive primitive);
GlTypeInfo vertex_attribute_kind_to_gl_type_info(GpuVertexAttributeKind kind);
void* u32_to_gl_void_pointer(u32 value);
u32 buffer_kind_to_gl_buffer_kind(GpuBufferKind kind);
u32 buffer_usage_to_gl_buffer_usage(GpuBufferUsage usage);
u32 buffer_kind_to_gl_barrier(GpuBufferKind kind);
u32 blend_func_to_gl_blend_func(GpuBlendFunction func);
u32 blend_mode_to_gl_blend_mode(GpuBlendMode mode);

#endif // GRAPHICS_H



////////////////////
// IMPLEMENTATION //
////////////////////
#ifdef GRAPHICS_IMPLEMENTATION


////////////////////
// COMMAND BUFFER //
////////////////////
GpuCommandBuffer* _gpu_command_buffer_create(GpuCommandBufferDescriptor descriptor) {
  auto command_buffer = arr_push(&command_renderer.command_buffers);
  arr_init(&command_buffer->commands, descriptor.max_commands);
  glGenVertexArrays(1, &command_buffer->vao);
  glBindVertexArray(command_buffer->vao);

  return command_buffer;
}

void _gpu_command_buffer_clear_cached_state(GpuCommandBuffer* command_buffer) {
  command_buffer->pipeline = nullptr;
  zero_memory(&command_buffer->bindings, sizeof(GpuBufferBinding));
  zero_memory(&command_buffer->render_pass, sizeof(GpuRenderPass));
  zero_memory(&command_buffer->render, sizeof(GpuRendererState));
  zero_memory(&command_buffer->scissor, sizeof(GpuScissorState));
}

void _gpu_command_buffer_submit(GpuCommandBuffer* command_buffer) {
  _gpu_command_buffer_clear_cached_state(command_buffer);
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
          glBlendEquation(blend_func_to_gl_blend_func(command.pipeline->blend.fn));
          glBlendFunc(
            blend_mode_to_gl_blend_mode(command.pipeline->blend.source), 
            blend_mode_to_gl_blend_mode(command.pipeline->blend.destination)
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

          u32 stride = _gpu_vertex_layout_calculate_stride(&buffer_layout);

          u64 offset = 0;
          for (u32 i = 0; i < buffer_layout.num_vertex_attributes; i++) {
            glEnableVertexAttribArray(attribute_index);

            auto attribute = buffer_layout.vertex_attributes[i];
            
            switch(attribute.kind) {
              case GPU_VERTEX_ATTRIBUTE_FLOAT: glVertexAttribPointer(attribute_index, attribute.count, GL_FLOAT,        GL_FALSE, stride, u32_to_gl_void_pointer(offset)); break;
              case GPU_VERTEX_ATTRIBUTE_U32:   glVertexAttribIPointer(attribute_index, attribute.count, GL_UNSIGNED_INT,           stride, u32_to_gl_void_pointer(offset)); break;
              default: {
                assert(false);
              } break;
            }

            glVertexAttribDivisor(attribute_index, attribute.divisor);

            auto type_info = vertex_attribute_kind_to_gl_type_info(attribute.kind);
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
  _gpu_command_buffer_clear_cached_state(command_buffer);
  arr_clear(&command_buffer->commands);
}


void _gpu_command_buffer_draw(GpuCommandBuffer* command_buffer, GpuDrawCall draw_call) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_DRAW,
    .draw = draw_call
  });
}


//////////////
// PIPELINE //
//////////////
GpuPipeline* _gpu_pipeline_create(GpuPipelineDescriptor descriptor) {
  GpuPipeline* pipeline = arr_push(&command_renderer.pipelines);
  pipeline->raster = descriptor.raster;
  pipeline->blend = descriptor.blend;
  copy_memory(descriptor.buffer_layouts, pipeline->buffer_layouts, descriptor.num_buffer_layouts * sizeof(GpuBufferLayout));
  pipeline->num_buffer_layouts = descriptor.num_buffer_layouts;

  return pipeline;
}

void _gpu_bind_pipeline(GpuCommandBuffer* command_buffer, GpuPipeline* pipeline) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_BIND_PIPELINE,
    .pipeline = pipeline
  });
}


/////////////
// BINDING //
/////////////
void _gpu_begin_render_pass(GpuCommandBuffer* command_buffer, GpuRenderPass render_pass) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_BEGIN_RENDER_PASS,
    .render_pass = render_pass
  });
}

void _gpu_end_render_pass(GpuCommandBuffer* command_buffer) {
  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_END_RENDER_PASS,
  });
}

void _gpu_apply_bindings(GpuCommandBuffer* command_buffer, GpuBufferBinding bindings) {
  if (is_memory_equal(&command_buffer->bindings, &bindings, sizeof(GpuBufferBinding))) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_BIND_BUFFERS,
    .bindings = bindings
  });
}


//////////////////
// RENDER STATE //
//////////////////
void _gpu_set_layer(GpuCommandBuffer* command_buffer, u32 layer) {
  if (command_buffer->render.layer == layer) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_SET_LAYER,
    .render = {
      .layer = layer
    }
  });
}

void _gpu_set_world_space(GpuCommandBuffer* command_buffer, bool world_space) {
  if (command_buffer->render.world_space == world_space) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_SET_WORLD_SPACE,
    .render = {
      .world_space = world_space
    }
  });
}

void _gpu_set_camera(GpuCommandBuffer* command_buffer, Vector2 camera) {
  if (is_memory_equal(&command_buffer->render.camera,  &camera, sizeof(Vector2))) return;

  arr_push(&command_buffer->commands, {
    .kind = GPU_COMMAND_OP_SET_CAMERA,
    .render = {
      .camera = camera
    }
  });
}

void _gpu_bind_render_state(GpuCommandBuffer* command_buffer, GpuRendererState render) {
  _gpu_set_layer(command_buffer, render.layer);
  _gpu_set_camera(command_buffer, render.camera);
  _gpu_set_world_space(command_buffer, render.world_space);
}


//////////////
// UNIFORMS //
//////////////
GpuUniform* _gpu_uniform_create(GpuUniformDescriptor descriptor) {
  auto uniform = arr_push(&command_renderer.uniforms);
  copy_string_n(descriptor.name, MAX_UNIFORM_NAME, uniform->name, MAX_UNIFORM_NAME);
  uniform->kind = descriptor.kind;
  
  return uniform;
}


/////////////////
// GPU BUFFERS //
/////////////////
GpuBuffer* gpu_buffer_create(GpuBufferDescriptor descriptor) {
	auto buffer = arr_push(&command_renderer.gpu_buffers);
  copy_string_n(descriptor.name, 64, buffer->name, 64);
	buffer->kind = descriptor.kind;
	buffer->usage = descriptor.usage;
	buffer->size = descriptor.capacity * descriptor.element_size;
	glGenBuffers(1, &buffer->handle);
	
	gpu_buffer_sync(buffer, nullptr, buffer->size);

	return buffer;
}

void gpu_memory_barrier(GpuMemoryBarrier barrier) {
	glMemoryBarrier(convert_memory_barrier(barrier));
}

void gpu_buffer_bind(GpuBuffer* buffer) {
	glBindBuffer(buffer_kind_to_gl_buffer_kind(buffer->kind), buffer->handle);
}

void gpu_buffer_bind_base(GpuBuffer* buffer, u32 base) {
	glBindBufferBase(buffer_kind_to_gl_buffer_kind(buffer->kind), base, buffer->handle);
}

void gpu_buffer_sync(GpuBuffer* buffer, void* data, u32 size) {
	gpu_buffer_bind(buffer);
	glBufferData(buffer_kind_to_gl_buffer_kind(buffer->kind), size, data, buffer_usage_to_gl_buffer_usage(buffer->usage));
	glMemoryBarrier(buffer_kind_to_gl_barrier(buffer->kind));
}

void gpu_buffer_sync_subdata(GpuBuffer* buffer, void* data, u32 byte_size, u32 byte_offset) {
	gpu_buffer_bind(buffer);
	glBufferSubData(buffer_kind_to_gl_buffer_kind(buffer->kind), byte_offset, byte_size, data);
	glMemoryBarrier(buffer_kind_to_gl_barrier(buffer->kind));
}

void gpu_buffer_zero(GpuBuffer* buffer, u32 size) {
	auto data = bump_allocator.alloc<u8>(size);
	gpu_buffer_sync(buffer, data, size);
}

GpuBackedBuffer gpu_backed_buffer_create(GpuBufferDescriptor descriptor) {
  GpuBackedBuffer backed_buffer;
  backed_buffer.gpu_buffer = gpu_buffer_create(descriptor);
  fixed_array_init(&backed_buffer.buffer, descriptor.capacity, descriptor.element_size);
  return backed_buffer;
}

void gpu_backed_buffer_clear(GpuBackedBuffer* buffer) {
  buffer->buffer.size = 0;
}

u32 gpu_backed_buffer_size(GpuBackedBuffer* buffer) {
  return buffer->buffer.size;
}

u8* gpu_backed_buffer_push(GpuBackedBuffer* buffer, void* data, u32 num_elements) {
  return fixed_array_push(&buffer->buffer, data, num_elements);
}

void gpu_backed_buffer_sync(GpuBackedBuffer* buffer) {
  gpu_buffer_sync(buffer->gpu_buffer, buffer->buffer.data, buffer->buffer.size * buffer->buffer.vertex_size);
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

u32 blend_func_to_gl_blend_func(GpuBlendFunction func) {
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

u32 blend_mode_to_gl_blend_mode(GpuBlendMode mode) {
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

GlTypeInfo vertex_attribute_kind_to_gl_type_info(GpuVertexAttributeKind kind) {
  GlTypeInfo info;

  if (kind  == GPU_VERTEX_ATTRIBUTE_FLOAT) {
    info.value = GL_FLOAT;
    info.size = sizeof(GLfloat);
    info.floating_point = true;
    info.integral = false;
  }
  else if (kind == GPU_VERTEX_ATTRIBUTE_U32) {
    info.value = GL_UNSIGNED_INT;
    info.size = sizeof(GLuint);
    info.floating_point = false;
    info.integral = true;		
  }
  else {
    assert(false);
  }

  return info;
}

void* u32_to_gl_void_pointer(u32 value) {
  return (void*)(uintptr_t)value;
}

u32 _gpu_vertex_layout_calculate_stride(GpuBufferLayout* layout) {
  assert(layout);

  u32 stride = 0;

  for (u32 i = 0; i < layout->num_vertex_attributes; i++) {
    auto attribute = layout->vertex_attributes[i];
    auto type_info = vertex_attribute_kind_to_gl_type_info(attribute.kind);
    stride += attribute.count * type_info.size;
  }

  return stride;
}


/////////////
// TESTING //
/////////////
void init_command_renderer() {
  arr_init(&command_renderer.command_buffers);
  arr_init(&command_renderer.uniforms);
  arr_init(&command_renderer.pipelines);
	arr_init(&command_renderer.gpu_buffers);
}

void test_command_renderer() {
}
#endif // GRAPHICS_IMPL
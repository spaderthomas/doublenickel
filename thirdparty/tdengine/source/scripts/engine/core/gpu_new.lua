-------------
-- STRUCTS --
-------------

GpuBufferDescriptor = tdengine.class.metatype('dn_gpu_buffer_descriptor_t')
function GpuBufferDescriptor:init(params)
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.usage = tdengine.enum.load(params.usage):to_number()
  self.capacity = params.size
  self.element_size = 1
end

GpuUniformDescriptor = tdengine.class.metatype('dn_gpu_uniform_descriptor_t')
function GpuUniformDescriptor:init(params)
  self.name = params.name
  self.kind = tdengine.enum.load(params.kind):to_number()
end

GpuCommandBufferDescriptor = tdengine.class.metatype('dn_gpu_command_buffer_descriptor_t')
function GpuCommandBufferDescriptor:init(params)
  self.max_commands = params.max_commands
end

GpuVertexAttribute = tdengine.class.metatype('dn_gpu_vertex_attribute_t')
function GpuVertexAttribute:init(params)
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.count = params.count
  self.divisor = params.divisor or 0
end

GpuBufferLayout = tdengine.class.metatype('dn_gpu_buffer_layout_t')
function GpuBufferLayout:init(params)
  local allocator = tdengine.ffi.dn_allocator_find('standard')

  self.num_vertex_attributes = #params.vertex_attributes
  self.vertex_attributes = allocator:alloc_array('dn_gpu_vertex_attribute_t', self.num_vertex_attributes)
  for i = 1, self.num_vertex_attributes, 1 do
    self.vertex_attributes[i - 1] = GpuVertexAttribute:new(params.vertex_attributes[i])
  end
end

GpuRasterState = tdengine.class.metatype('dn_gpu_raster_state_t')
function GpuRasterState:init(params)
  self.shader = tdengine.gpu.find(params.shader)
  self.primitive = tdengine.enum.load(params.primitive):to_number()
end

GpuPipelineDescriptor = tdengine.class.metatype('dn_gpu_pipeline_descriptor_t')
function GpuPipelineDescriptor:init(params)
  local allocator = tdengine.ffi.dn_allocator_find('standard')

  self.raster = dn_gpu_raster_state_t:new(params.raster)

  self.num_buffer_layouts = #params.buffer_layouts
  self.buffer_layouts = allocator:alloc_array('dn_gpu_buffer_layout_t', self.num_buffer_layouts)
  for i = 1, self.num_buffer_layouts do
    self.buffer_layouts[i - 1] = GpuBufferLayout:new(params.buffer_layouts[i])
  end
end

GpuRenderPass = tdengine.class.metatype('dn_gpu_render_pass_t')
function GpuRenderPass:init(params)
  dbg()
  if params.color then
    params.color.attachment = tdengine.enum.load(params.color.attachment)
    self.color.attachment = tdengine.ffi.dn_gpu_render_target_find(params.color.attachment:to_string())
    self.color.load = tdengine.enum.load(params.color.load):to_number()
  end
end

GpuUniformBinding = tdengine.class.metatype('dn_gpu_uniform_binding_t')
function GpuUniformBinding:init(params)
  self.uniform = params.uniform
  self.binding_index = params.binding_index or 0

  if     GpuUniformKind.Matrix4:match(self.uniform.kind) then self.data.mat4 = params.value
  elseif GpuUniformKind.Matrix3:match(self.uniform.kind) then self.data.mat3 = params.value
  elseif GpuUniformKind.Matrix2:match(self.uniform.kind) then self.data.mat2 = params.value
  elseif GpuUniformKind.Vector4:match(self.uniform.kind) then self.data.vec4 = params.value
  elseif GpuUniformKind.Vector3:match(self.uniform.kind) then self.data.vec3 = params.value
  elseif GpuUniformKind.Vector2:match(self.uniform.kind) then self.data.vec2 = params.value
  elseif GpuUniformKind.F32:match(self.uniform.kind)     then self.data.f32 = params.value
  elseif GpuUniformKind.I32:match(self.uniform.kind)     then self.data.i32 = params.value
  elseif GpuUniformKind.Enum:match(self.uniform.kind)    then self.data.i32 = tdengine.enum.load(params.value):to_number()
  elseif GpuUniformKind.Texture:match(self.uniform.kind) then self.data.texture = params.value
  else   tdengine.debug.assert(false) end
end

GpuVertexBufferBinding = tdengine.class.metatype('dn_gpu_vertex_buffer_binding_t')
function GpuVertexBufferBinding:init(buffer)
  if type(buffer) == 'cdata' then
    self.buffer = buffer
  else
    self.buffer = tdengine.gpu.find(buffer)
  end
end

GpuStorageBufferBinding = tdengine.class.metatype('dn_gpu_storage_buffer_binding_t')
function GpuStorageBufferBinding:init(params)
  if type(params.buffer) == 'cdata' then
    self.buffer = params.buffer
  else
    self.buffer = tdengine.gpu.find(params.buffer)
  end

  self.base = params.base
end

GpuBufferBinding = tdengine.class.metatype('dn_gpu_buffer_binding_t')
function GpuBufferBinding:init(params)
  local allocator = tdengine.ffi.dn_allocator_find('standard')

  if params.vertex then
    self.vertex.count = #params.vertex
    self.vertex.bindings = allocator:alloc_array('dn_gpu_vertex_buffer_binding_t', self.vertex.count)
    for i = 1, self.vertex.count do
      self.vertex.bindings[i - 1] = GpuVertexBufferBinding:new(params.vertex[i])
    end
  end

  if params.uniforms then
    self.uniforms.count = #params.uniforms
    self.uniforms.bindings = allocator:alloc_array('dn_gpu_uniform_binding_t', self.uniforms.count)
    for i = 1, self.uniforms.count do
      self.uniforms.bindings[i - 1] = GpuUniformBinding:new(params.uniforms[i])
    end
  end

  if params.storage then
    self.storage.count = #params.storage
    self.storage.bindings = allocator:alloc_array('dn_gpu_storage_binding_t', self.storage.count)
    for i = 1, self.storage.count do
      self.storage.bindings[i - 1] = GpuStorageBufferBinding:new(params.storage[i])
    end
  end
end


----------------
-- GPU MODULE --
----------------
local self = tdengine.gpu
function tdengine.gpu.init()
  self.render_targets = {}
  self.buffers = {}
  self.shaders = {}
  self.resolutions = {}
  self.assets = {}
end

function tdengine.gpu.render()
  tdengine.ffi.dn_time_metric_begin('render')

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_render_scene)
  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_scene_rendered)

  local swapchain = tdengine.ffi.dn_gpu_acquire_swapchain()
  tdengine.ffi.dn_gpu_render_target_bind(swapchain)
  tdengine.ffi.dn_gpu_render_target_clear(swapchain)
  tdengine.app:on_swapchain_ready()
  tdengine.ffi.dn_gpu_swap_buffers()

  tdengine.ffi.dn_time_metric_end('render')
end


function tdengine.gpu.build(gpu_info)
  self.add_resolutions(gpu_info.resolutions)
  self.add_render_targets(gpu_info.render_targets)
  self.add_buffers(gpu_info.buffers)
  self.add_shaders(gpu_info.shaders)
end


function tdengine.gpu.find(id)
  return self.assets[id:to_qualified_string()]
end
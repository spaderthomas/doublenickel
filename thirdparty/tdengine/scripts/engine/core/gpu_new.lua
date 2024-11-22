-------------
-- STRUCTS --
-------------
VertexAttribute = tdengine.class.metatype('VertexAttribute')
function VertexAttribute:init(params)
  self.count = params.count
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.divisor = params.divisor or 0
end


GpuColorAttachment = tdengine.class.metatype('GpuColorAttachment')
function GpuColorAttachment:init(params)
  self.read = params.read and tdengine.gpus.find(params.read) or nil
  self.write = tdengine.gpus.find(params.write)
  self.load_op = tdengine.enum.load(params.load_op):to_number()
end


GpuShaderDescriptor = tdengine.class.metatype('GpuShaderDescriptor')
function GpuShaderDescriptor:init(params)
  params.kind = tdengine.enum.load(params.kind)
  self.kind = params.kind:to_number()
  self.name = params.name

  if params.kind == tdengine.enums.GpuShaderKind.Graphics then
    self.vertex_shader = params.vertex_shader
    self.fragment_shader = params.fragment_shader
  elseif params.kind == tdengine.enums.GpuShaderKind.Compute then
    self.compute_shader = params.compute_shader
  end
end

GpuRenderTargetDescriptor = tdengine.class.metatype('GpuRenderTargetDescriptor')
function GpuRenderTargetDescriptor:init(params)
  if params.resolution then
    self.size = tdengine.gpus.find(params.resolution)
  else
    self.size = Vector2:new(params.size.x, params.size.y)
  end
end

GpuGraphicsPipelineDescriptor = tdengine.class.metatype('GpuGraphicsPipelineDescriptor')
function GpuGraphicsPipelineDescriptor:init(params)
  self.color_attachment = GpuColorAttachment:new(params.color_attachment)
  self.command_buffer = tdengine.gpus.find(params.command_buffer)
end
-- 
GpuCommandBufferBatchedDescriptor = tdengine.class.metatype('GpuCommandBufferBatchedDescriptor')
function GpuCommandBufferBatchedDescriptor:init(params) 
  local allocator = tdengine.ffi.ma_find('bump')

  self.max_vertices = params.max_vertices
  self.max_draw_calls = params.max_draw_calls

  self.num_vertex_attributes = #params.vertex_attributes
  self.vertex_attributes = allocator:alloc_array('VertexAttribute', self.num_vertex_attributes)
  for i = 1, self.num_vertex_attributes, 1 do
    self.vertex_attributes[i - 1] = VertexAttribute:new(params.vertex_attributes[i])
  end
end

GpuBufferDescriptor = tdengine.class.metatype('GpuBufferDescriptor')
function GpuBufferDescriptor:init(params)
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.usage = tdengine.enum.load(params.usage):to_number()
  -- self.size = params.size or 0
  self.capacity = params.size
  self.element_size = 1
end





GpuUniformDescriptor = tdengine.class.metatype('GpuUniformDescriptor')
function GpuUniformDescriptor:init(params)
  self.name = params.name
  self.kind = tdengine.enum.load(params.kind):to_number()
end

GpuCommandBufferDescriptor = tdengine.class.metatype('GpuCommandBufferDescriptor')
function GpuCommandBufferDescriptor:init(params)
  self.max_commands = params.max_commands
end

GpuDrawCall = tdengine.class.metatype('GpuDrawCall')
function GpuDrawCall:init(params)
  self.mode = tdengine.enum.load(params.mode):to_number()
  self.vertex_offset = params.vertex_offset or 0
  self.num_vertices = params.num_vertices or 0
  self.num_instances = params.num_instances or 0
end

GpuVertexAttribute = tdengine.class.metatype('GpuVertexAttribute')
function GpuVertexAttribute:init(params)
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.count = params.count
  self.divisor = params.divisor or 0
end

GpuBufferLayout = tdengine.class.metatype('GpuBufferLayout')
function GpuBufferLayout:init(params)
  local allocator = tdengine.ffi.ma_find('standard')

  self.num_vertex_attributes = #params.vertex_attributes
  self.vertex_attributes = allocator:alloc_array('GpuVertexAttribute', self.num_vertex_attributes)
  for i = 1, self.num_vertex_attributes, 1 do
    self.vertex_attributes[i - 1] = GpuVertexAttribute:new(params.vertex_attributes[i])
  end
end

GpuRasterState = tdengine.class.metatype('GpuRasterState')
function GpuRasterState:init(params)
  self.shader = tdengine.gpus.find(params.shader)
  self.primitive = tdengine.enum.load(params.primitive):to_number()
end

GpuPipeline = tdengine.class.metatype('GpuPipeline')
function GpuPipeline:init(params)
  local allocator = tdengine.ffi.ma_find('standard')

  self.raster = GpuRasterState:new(params.raster)

  self.num_buffer_layouts = #params.buffer_layouts
  self.buffer_layouts = allocator:alloc_array('GpuBufferLayout', self.num_buffer_layouts)
  for i = 1, self.num_buffer_layouts do
    self.buffer_layouts[i - 1] = GpuBufferLayout:new(params.buffer_layouts[i])
  end
end


GpuRenderPass = tdengine.class.metatype('GpuRenderPass')
function GpuRenderPass:init(params)
  if params.color then
    self.color.attachment = tdengine.gpus.find(params.color.attachment)
    self.color.load = tdengine.enum.load(params.color.load):to_number()
  end
end

GpuUniformBinding = tdengine.class.metatype('GpuUniformBinding')
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

GpuVertexBufferBinding = tdengine.class.metatype('GpuVertexBufferBinding')
function GpuVertexBufferBinding:init(buffer)
  if type(buffer) == 'cdata' then
    self.buffer = buffer
  else
    self.buffer = tdengine.gpus.find(buffer)
  end
end

GpuStorageBufferBinding = tdengine.class.metatype('GpuStorageBufferBinding')
function GpuStorageBufferBinding:init(params)
  if type(params.buffer) == 'cdata' then
    self.buffer = params.buffer
  else
    self.buffer = tdengine.gpus.find(params.buffer)
  end

  self.base = params.base
end

GpuBufferBinding = tdengine.class.metatype('GpuBufferBinding')
function GpuBufferBinding:init(params)
  local allocator = tdengine.ffi.ma_find('standard')

  if params.vertex then
    self.vertex.count = #params.vertex
    self.vertex.bindings = allocator:alloc_array('GpuVertexBufferBinding', self.vertex.count)
    for i = 1, self.vertex.count do
      self.vertex.bindings[i - 1] = GpuVertexBufferBinding:new(params.vertex[i])
    end
  end

  if params.uniforms then
    self.uniforms.count = #params.uniforms
    self.uniforms.bindings = allocator:alloc_array('GpuUniformBinding', self.uniforms.count)
    for i = 1, self.uniforms.count do
      self.uniforms.bindings[i - 1] = GpuUniformBinding:new(params.uniforms[i])
    end
  end

  if params.storage then
    self.storage.count = #params.storage
    self.storage.bindings = allocator:alloc_array('GpuStorageBufferBinding', self.storage.count)
    for i = 1, self.storage.count do
      self.storage.bindings[i - 1] = GpuStorageBufferBinding:new(params.storage[i])
    end
  end

end



-------------
-- CLASSES --
------------
UniformBinding = tdengine.class.define('UniformBinding')
function UniformBinding:init(name, value, kind)
  self.name = name
  self.value = value
  self.kind = kind
end

function UniformBinding:bind()
  if UniformKind.Matrix4:match(self.kind) then
    tdengine.ffi.set_uniform_mat4(self.name, self.value)
  elseif UniformKind.Matrix3:match(self.kind) then
    tdengine.ffi.set_uniform_mat3(self.name, self.value)
  elseif UniformKind.Vector4:match(self.kind) then
    tdengine.ffi.set_uniform_vec4(self.name, self.value)
  elseif UniformKind.Vector3:match(self.kind) then
    tdengine.ffi.set_uniform_vec3(self.name, self.value)
  elseif UniformKind.Vector2:match(self.kind) then
    tdengine.ffi.set_uniform_vec2(self.name, self.value)
  elseif UniformKind.F32:match(self.kind) then
    tdengine.ffi.set_uniform_f32(self.name, self.value)
  elseif UniformKind.I32:match(self.kind) then
    tdengine.ffi.set_uniform_i32(self.name, self.value)
  elseif UniformKind.Enum:match(self.kind) then
    tdengine.ffi.set_uniform_enum(self.name, self.value)
  elseif UniformKind.RenderTarget:match(self.kind) then
    tdengine.ffi.set_uniform_texture(self.name, tdengine.gpus.find(self.value).color_buffer)
  elseif UniformKind.PipelineOutput:match(self.kind) then
    tdengine.ffi.set_uniform_texture(self.name, tdengine.gpus.find(self.value).color_attachment.read)
  elseif self.kind == tdengine.enums.UniformKind.Enum then
    tdengine.ffi.set_uniform_enum(self.name, self.value)
  end
end


SsboBinding = tdengine.class.define('SsboBinding')
function SsboBinding:init(index, buffer_id)
  self.index = index
  self.ssbo = tdengine.gpus.find(buffer_id)
end

function SsboBinding:bind()
  tdengine.ffi.gpu_buffer_bind_base(self.ssbo, self.index)
end


GpuDrawConfiguration = tdengine.class.define('GpuDrawConfiguration')
function GpuDrawConfiguration:init(params)
  self.shader = tdengine.gpus.find(params.shader)

  self.uniforms = tdengine.data_types.Array:new()
  for binding in tdengine.iterator.values(params.uniforms) do
    self.uniforms:add(UniformBinding:new(binding))
  end

  self.ssbos = tdengine.data_types.Array:new()
  for binding in tdengine.iterator.values(params.ssbos) do
    self.ssbos:add(SsboBinding:new(binding.index, binding.id))
  end
end

function GpuDrawConfiguration:add_uniform(name, value, kind)
  for uniform in self.uniforms:iterate_values() do
    if uniform.name == name then
      uniform.value = value
      return
    end
  end

  self.uniforms:add(UniformBinding:new(name, value, kind))
end

function GpuDrawConfiguration:bind()
  tdengine.ffi.set_active_shader_ex(self.shader)
  tdengine.ffi.set_draw_mode(tdengine.enums.DrawMode.Triangles)

  for uniform in self.uniforms:iterate_values() do
    uniform:bind()
  end

  for ssbo in self.ssbos:iterate_values() do
    ssbo:bind()
  end
end


PreconfiguredPostProcess = tdengine.class.define('PreconfiguredPostProcess')
function PreconfiguredPostProcess:init(pipeline, draw_configuration)
  self.pipeline = pipeline
  self.draw_configuration = draw_configuration
end

function PreconfiguredPostProcess:render()
  tdengine.ffi.gpu_graphics_pipeline_bind(self.pipeline)
  self.draw_configuration:bind()

  local size = self.pipeline.color_attachment.write.size
  ffi.C.push_quad(
    0, size.y,
    size.x, size.y,
    nil,
    1.0)


    tdengine.ffi.gpu_graphics_pipeline_submit(self.pipeline)
    -- tdengine.gpu.apply_ping_pong(self.graphics_pipeline)
end

local todo = [[
- Draw an SDF circle using the other buffer
  - Make a GL_ARRAY_BUFFER from Lua to hold the vertex data (position and UV, just reuse Vertex)
  - Upload vertices for a quad to the buffer (once at startup)
  - Create another GL_ARRAY_BUFFER to hold the instance-specific data (SdfVertex)
  - Create a VAO which first specifies the vertex layout, then the instance data layout. It uses the divisor to tell OpenGL to only advance the instance data once per instance, rather than once per vertex.
  - Every frame, update a CPU buffer with instance data. Sync that to the GPU buffer when you're ready to render.
  - Call glDrawArraysInstanced, telling it to start at instance 0, draw 6 vertices per instance, and however many instances you have


- Render component should draw to the correct pipeline
- Reimplement all of the post processing stuff
  - Reimplement ping-pong
- Figure out the actual minimum number of command buffers you need
- Make the benchmark timer API better (e.g. local timer = tdengine.ffi.tm_begin(Timer.Render); timer:end())
- Fully remove gpu.lua
- Merge into the base engine...?
]]

local done = [[
- find_resource() -> find()
  - Move the named assets thing into the right place in the user folder
- Properly clear render targets on load
- Clean up push_vertex() so the call stack isn't four deep
- Make sure the old draw API works and mawe sure the GPU setup for that is included in the base engine
  - Why is the grid totally filling up the vertex buffer? Are the sizes correct?
- What is a store op?
- UniformBinding is still a mess of unimplemented and messy uniforms; fix those up
  - Can I just move all the union-uniform stuff into Lua? I think that'd mean moving all of your current draw stuff into Lua, which 
  isn't necessarily a problem. It's more that your whole immediate mode API doesn't make as much sense here. Or does it...? That's
  something else. What is actually my bottleneck in drawing? I definitely want, for example, all the grid draw calls to get batched
  together. I want *some* kind of auto-batching.
- Rename GraphicsPipeline enum -> GraphicsPipeline

]]

----------------
-- GPU MODULE --
----------------
local self = tdengine.gpus
function tdengine.gpus.init()
  self.render_targets = {}
  self.graphics_pipelines = {}
  self.command_buffers = {}
  self.buffers = {}
  self.shaders = {}
  self.resolutions = {}
  self.draw_configurations = {}
end

function tdengine.gpus.update()
  for pipeline in tdengine.iterator.values(self.graphics_pipelines) do
    tdengine.ffi.gpu_graphics_pipeline_begin_frame(pipeline)
  end
end

function tdengine.gpus.render()
    tdengine.ffi.tm_begin('render')

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_render_scene)
  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_scene_rendered)

  local swapchain = tdengine.ffi.gpu_acquire_swapchain()
  tdengine.ffi.gpu_render_target_bind(swapchain)
  tdengine.ffi.gpu_render_target_clear(swapchain)
  tdengine.app:on_swapchain_ready()
  tdengine.ffi.gpu_swap_buffers()

  tdengine.ffi.tm_end('render')
end

function tdengine.gpus.bind_entity(entity)
  local pipeline = self.find(GraphicsPipeline.Editor)

  local render = entity:find_component('Render')
  if render then
    pipeline = render.pipeline
  end

  tdengine.ffi.gpu_graphics_pipeline_bind(pipeline)
end


function tdengine.gpus.build(gpu_info)
  self.add_resolutions(gpu_info.resolutions)
  self.add_render_targets(gpu_info.render_targets)
  self.add_buffers(gpu_info.buffers)
  self.add_shaders(gpu_info.shaders)
  self.add_command_buffers(gpu_info.command_buffers)
  self.add_graphics_pipelines(gpu_info.graphics_pipelines)
  self.add_draw_configurations(gpu_info.draw_configurations)
end

function tdengine.gpus.find(id)
  if not tdengine.enum.is_enum(id) then 
    log.warn('Tried to find GPU resource, but ID passed in was not an enum; id = %s', tostring(id))
    return nil
  end

  local resource_map
  if tdengine.enums.RenderTarget:match(id) then
    resource_map = self.render_targets
  elseif tdengine.enums.GraphicsPipeline:match(id) then
    resource_map = self.graphics_pipelines
  elseif tdengine.enums.CommandBuffer:match(id) then
    resource_map = self.command_buffers
  elseif tdengine.enums.Buffer:match(id) then
    resource_map = self.buffers
  elseif tdengine.enums.Shader:match(id) then
    resource_map = self.shaders
  elseif tdengine.enums.Resolution:match(id) then
    resource_map = self.resolutions
  elseif tdengine.enums.DrawConfiguration:match(id) then
    resource_map = self.draw_configurations
  end

  local string_id = tdengine.enum.load(id):to_string()
  local resource = resource_map[string_id]
  if not resource then
    dbg()
    log.warn('Could not find GPU resource; id = %s', string_id)
  end

  return resource
end


-------------------
-- RENDER TARGET -- 
-------------------
function tdengine.gpus.add_render_target(id, descriptor)
  self.render_targets[id:to_string()] = tdengine.ffi.gpu_render_target_create(descriptor)
end

function tdengine.gpus.add_render_targets(targets)
  for target in tdengine.iterator.values(targets) do
		self.add_render_target(
			target.id,
			GpuRenderTargetDescriptor:new(target.descriptor)
		)
	end
end

-----------------------
-- GRAPHICS PIPELINE --
-----------------------
function tdengine.gpus.add_graphics_pipeline(id, descriptor)
  self.graphics_pipelines[id:to_string()] = tdengine.ffi.gpu_graphics_pipeline_create(descriptor)
end

function tdengine.gpus.add_graphics_pipelines(pipelines)
	for pipeline in tdengine.iterator.values(pipelines) do
		self.add_graphics_pipeline(
			pipeline.id,
			GpuGraphicsPipelineDescriptor:new(pipeline.descriptor)
		)
	end
end

--------------------
-- COMMAND BUFFER --
--------------------
function tdengine.gpus.add_command_buffer(id, descriptor)
  self.command_buffers[id:to_string()] = tdengine.ffi.gpu_create_command_buffer(descriptor)
end

function tdengine.gpus.add_command_buffers(command_buffers)
  for buffer in tdengine.iterator.values(command_buffers) do
    self.add_command_buffer(
      buffer.id,
      GpuCommandBufferBatchedDescriptor:new(buffer.descriptor)
    )
  end
end

----------------
-- GPU BUFFER --
----------------
function tdengine.gpus.add_buffer(id, descriptor)
  self.buffers[id:to_string()] = tdengine.ffi.gpu_buffer_create(descriptor)
end

function tdengine.gpus.add_buffers(buffers)
  for buffer in tdengine.iterator.values(buffers) do
		self.add_buffer(buffer.id, GpuBufferDescriptor:new(buffer.descriptor))
	end
end

------------
-- SHADER --
------------
function tdengine.gpus.add_shader(id, descriptor)
  self.shaders[id:to_string()] = tdengine.ffi.gpu_shader_create(descriptor)
end

function tdengine.gpus.add_shaders(shaders)
  for shader in tdengine.iterator.values(shaders) do
		self.add_shader(
			shader.id,
			GpuShaderDescriptor:new(shader.descriptor)
		)
	end
end

----------------
-- RESOLUTION --
----------------
function tdengine.gpus.add_resolution(id, size)
  self.resolutions[id:to_string()] = Vector2:new(size.x, size.y)
end

function tdengine.gpus.add_resolutions(resolutions)
  for resolution in tdengine.iterator.values(resolutions) do
    self.add_resolution(resolution.id, resolution.size)
	end
end

------------
-- SHADER --
------------
function tdengine.gpus.add_draw_configuration(id, draw_configuration)
  self.draw_configurations[id:to_string()] = draw_configuration
end

function tdengine.gpus.add_draw_configurations(draw_configurations)
  for draw_configuration in tdengine.iterator.values(draw_configurations) do
		self.add_draw_configuration(
			draw_configuration.id,
			GpuDrawConfiguration:new(draw_configuration)
		)
	end
end

-------------
-- STRUCTS --
-------------
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
    self.size = tdengine.gpu.find(params.resolution)
  else
    self.size = Vector2:new(params.size.x, params.size.y)
  end
end

GpuBufferDescriptor = tdengine.class.metatype('GpuBufferDescriptor')
function GpuBufferDescriptor:init(params)
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.usage = tdengine.enum.load(params.usage):to_number()
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

GpuVertexAttribute = tdengine.class.metatype('GpuVertexAttribute')
function GpuVertexAttribute:init(params)
  self.kind = tdengine.enum.load(params.kind):to_number()
  self.count = params.count
  self.divisor = params.divisor or 0
end

GpuBufferLayout = tdengine.class.metatype('GpuBufferLayout')
function GpuBufferLayout:init(params)
  local allocator = tdengine.ffi.dn_allocator_find('standard')

  self.num_vertex_attributes = #params.vertex_attributes
  self.vertex_attributes = allocator:alloc_array('GpuVertexAttribute', self.num_vertex_attributes)
  for i = 1, self.num_vertex_attributes, 1 do
    self.vertex_attributes[i - 1] = GpuVertexAttribute:new(params.vertex_attributes[i])
  end
end

GpuRasterState = tdengine.class.metatype('GpuRasterState')
function GpuRasterState:init(params)
  self.shader = tdengine.gpu.find(params.shader)
  self.primitive = tdengine.enum.load(params.primitive):to_number()
end

GpuPipeline = tdengine.class.metatype('GpuPipeline')
function GpuPipeline:init(params)
  local allocator = tdengine.ffi.dn_allocator_find('standard')

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
    self.color.attachment = tdengine.gpu.find(params.color.attachment)
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
    self.buffer = tdengine.gpu.find(buffer)
  end
end

GpuStorageBufferBinding = tdengine.class.metatype('GpuStorageBufferBinding')
function GpuStorageBufferBinding:init(params)
  if type(params.buffer) == 'cdata' then
    self.buffer = params.buffer
  else
    self.buffer = tdengine.gpu.find(params.buffer)
  end

  self.base = params.base
end

GpuBufferBinding = tdengine.class.metatype('GpuBufferBinding')
function GpuBufferBinding:init(params)
  local allocator = tdengine.ffi.dn_allocator_find('standard')

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
- Make the benchmark timer API better (e.g. local timer = tdengine.ffi.dn_time_metric_begin(Timer.Render); timer:end())
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

  local swapchain = tdengine.ffi.gpu_acquire_swapchain()
  tdengine.ffi.gpu_render_target_bind(swapchain)
  tdengine.ffi.gpu_render_target_clear(swapchain)
  tdengine.app:on_swapchain_ready()
  tdengine.ffi.gpu_swap_buffers()

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


-------------------
-- RENDER TARGET -- 
-------------------
function tdengine.gpu.add_render_target(id, descriptor)
  local target = tdengine.ffi.gpu_render_target_create(descriptor)
  self.render_targets[id:to_string()] = target
  self.assets[id:to_qualified_string()] = target
end

function tdengine.gpu.add_render_targets(targets)
  for target in tdengine.iterator.values(targets) do
		self.add_render_target(
			target.id,
			GpuRenderTargetDescriptor:new(target.descriptor)
		)
	end
end

----------------
-- GPU BUFFER --
----------------
function tdengine.gpu.add_buffer(id, descriptor)
  local buffer = tdengine.ffi.gpu_buffer_create(descriptor)
  self.buffers[id:to_string()] = buffer
  self.assets[id:to_qualified_string()] = buffer
end

function tdengine.gpu.add_buffers(buffers)
  for buffer in tdengine.iterator.values(buffers) do
		self.add_buffer(buffer.id, GpuBufferDescriptor:new(buffer.descriptor))
	end
end

------------
-- SHADER --
------------
function tdengine.gpu.add_shader(id, descriptor)
  local shader = tdengine.ffi.gpu_shader_create(descriptor)
  self.shaders[id:to_string()] = shader
  self.assets[id:to_qualified_string()] = shader
end

function tdengine.gpu.add_shaders(shaders)
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
function tdengine.gpu.add_resolution(id, size)
  local resolution = Vector2:new(size.x, size.y)
  self.resolutions[id:to_string()] = resolution
  self.assets[id:to_qualified_string()] = resolution
end

function tdengine.gpu.add_resolutions(resolutions)
  for resolution in tdengine.iterator.values(resolutions) do
    self.add_resolution(resolution.id, resolution.size)
	end
end
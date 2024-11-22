DeferredRenderer = tdengine.subsystem.define('DeferredRenderer')

function DeferredRenderer:init()
  self.render_enabled = true
  self.max_lights = 16;
  self.lights = nil
  self.shape_buffers = {}

  tdengine.time_metric.add('sdf')
end

function DeferredRenderer:on_start_game()
  self.sdf_renderer = ffi.new('SdfRenderer [1]');
  self.sdf_renderer = tdengine.ffi.sdf_renderer_create(1024 * 1024)

  self.command_buffer = tdengine.gpu.command_buffer_create(GpuCommandBufferDescriptor:new({
    max_commands = 1024
  }))

  self.render_pass = GpuRenderPass:new({
    color = {
      attachment = RenderTarget.Color,
      load = GpuLoadOp.Clear
    }
  })
end

function DeferredRenderer:sdf_example()
  local num_instances = 10
  local grid_width = 26
  local grid_size = math.sqrt(num_instances) * grid_width

  tdengine.ffi.tm_begin('sdf')
  for x = 0, grid_size, grid_width do
    for y = 0, grid_size, grid_width do
      ffi.C.sdf_circle_ex(
        self.sdf_renderer,
        x + grid_size, y + grid_size,
        x / grid_size, y / grid_size, 1.0,
        0.0,
        1.5,
        10.0
      )
    end
  end

  local header = tdengine.ffi.sdf_combination_begin(self.sdf_renderer)

  for x = 0, grid_size, grid_width do
    for y = 0, grid_size, grid_width do
      ffi.C.sdf_combination_append(
        self.sdf_renderer, header,
        ffi.C.SDF_SHAPE_CIRCLE, 
        ffi.C.SDF_COMBINE_OP_UNION,
        ffi.C.SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC
      )

      ffi.C.sdf_circle_ex(
        self.sdf_renderer,
        x, y,
        1.0, 1.0, 1.0,
        0.0,
        1.5,
        10.0
      )
    end
  end
  tdengine.ffi.sdf_combination_commit(self.sdf_renderer)

  tdengine.ffi.tm_end('sdf')
end

function DeferredRenderer:on_scene_rendered()
  -- self:sdf_example()
  tdengine.gpu.begin_render_pass(self.command_buffer, self.render_pass)
  tdengine.gpu.set_world_space(self.command_buffer, true)
  tdengine.gpu.set_camera(self.command_buffer, tdengine.editor.find('EditorCamera').offset:to_ctype())
  tdengine.ffi.sdf_renderer_draw(self.sdf_renderer, self.command_buffer)
  tdengine.gpu.end_render_pass(self.command_buffer)
  tdengine.gpu.command_buffer_submit(self.command_buffer)

end

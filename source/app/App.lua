Resolution = tdengine.enum.define(
	'Resolution', 
	{
		Native = 0,
		Upscaled = 1
	}
)

Shader = tdengine.enum.define(
  'Shader',
  {
		Shape = 1,
		Sdf = 2,
		SdfNormal = 3,
		Solid = 5,
		Sprite = 6,
		Text = 7,
		Blit = 9,
	}
)


RenderTarget = tdengine.enum.define(
  'RenderTarget',
  {
    Native = 0,
    Upscaled = 1,
	}
)

Buffer = tdengine.enum.define(
	'Buffer',
	{
		Lights = 0,
	}
)


local App = tdengine.define_app()

function App:init()
	self.native_resolution = tdengine.vec2(320, 180)
	self.output_resolution = tdengine.vec2(1024, 576)

end

function App:on_init_game()
	tdengine.ffi.create_window(
		'SDF Clock', 
		self.native_resolution.x, self.native_resolution.y, 
		tdengine.enum.bitwise_or(
			tdengine.enums.WindowFlags.Windowed,
			tdengine.enums.WindowFlags.Border
		)
	)

	tdengine.ffi.set_window_icon(tdengine.ffi.resolve_format_path('image', 'logo/icon.png'):to_interned())
	tdengine.ffi.set_target_fps(144)

	tdengine.gpu.build(tdengine.module.read_from_named_path('gpu_info'))

	self.sdf_renderer = ffi.new('SdfRenderer [1]');
  self.sdf_renderer = tdengine.ffi.sdf_renderer_create(1024 * 1024)

  self.command_buffer = tdengine.ffi._gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
    max_commands = 1024
  }))

  self.render_pass = GpuRenderPass:new({
    color = {
      attachment = RenderTarget.Native,
      load = GpuLoadOp.Clear
    }
  })

end

function App:on_start_game()
	tdengine.editor.configure(EditorConfig:new({
		grid_enabled = false,
		grid_size = 12,
		hide_dialogue_editor = true,
		game_views = {
			GameView:new(
				'Native View',
				tdengine.gpu.find(RenderTarget.Native),
				tdengine.enums.GameViewSize.ExactSize, self.native_resolution,
				tdengine.enums.GameViewPriority.Main),
			GameView:new(
				'Upscaled View',
				tdengine.gpu.find(RenderTarget.Upscaled),
				tdengine.enums.GameViewSize.ExactSize, self.output_resolution,
				tdengine.enums.GameViewPriority.Standard)
		},
		scene = 'sdf_clock',
		layout = 'sdf_clock',
		render_pass = self.render_pass,
		command_buffer = self.command_buffer
	}))
end

function App:on_scene_rendered()
	tdengine.ffi._gpu_begin_render_pass(self.command_buffer, self.render_pass)
  tdengine.ffi._gpu_set_world_space(self.command_buffer, true)
  tdengine.ffi._gpu_set_camera(self.command_buffer, tdengine.editor.find('EditorCamera').offset:to_ctype())
  tdengine.ffi.sdf_renderer_draw(self.sdf_renderer, self.command_buffer)
  tdengine.ffi._gpu_end_render_pass(self.command_buffer)
  tdengine.ffi._gpu_command_buffer_submit(self.command_buffer)

	tdengine.ffi.gpu_render_target_blit(
    tdengine.gpu.find(RenderTarget.Native),
    tdengine.gpu.find(RenderTarget.Upscaled)
	)
end

function App:on_swapchain_ready()
	tdengine.ffi.render_imgui()
end


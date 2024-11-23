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

	tdengine.gpus.build(tdengine.module.read_from_named_path('gpu_info'))

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
  tdengine.editor.find('SceneEditor'):load('sdf_clock')

	tdengine.ffi.use_editor_layout('sdf_clock')

	tdengine.editor.find('EditorUtility').style.grid.size = 12
	tdengine.editor.find('EditorUtility').enabled.grid = false

	tdengine.editor.find('DialogueEditor').hidden = true

	local game_views = tdengine.editor.find('GameViewManager')
  game_views:add_view(GameView:new(
		'Native View',
		tdengine.gpus.find(RenderTarget.Native),
		tdengine.enums.GameViewSize.ExactSize, self.native_resolution,
		tdengine.enums.GameViewPriority.Main))

  game_views:add_view(GameView:new(
		'Upscaled View',
		tdengine.gpus.find(RenderTarget.Upscaled),
		tdengine.enums.GameViewSize.ExactSize, self.output_resolution,
		tdengine.enums.GameViewPriority.Standard))

end

function App:on_scene_rendered()
	tdengine.ffi._gpu_begin_render_pass(self.command_buffer, self.render_pass)
  tdengine.ffi._gpu_set_world_space(self.command_buffer, true)
  tdengine.ffi._gpu_set_camera(self.command_buffer, tdengine.editor.find('EditorCamera').offset:to_ctype())
  tdengine.ffi.sdf_renderer_draw(self.sdf_renderer, self.command_buffer)
  tdengine.ffi._gpu_end_render_pass(self.command_buffer)
  tdengine.ffi._gpu_command_buffer_submit(self.command_buffer)

	tdengine.ffi.gpu_render_target_blit(
    tdengine.gpus.find(RenderTarget.Native),
    tdengine.gpus.find(RenderTarget.Upscaled)
	)
end

function App:on_swapchain_ready()
	tdengine.ffi.render_imgui()
end


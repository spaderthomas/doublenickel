local App = tdengine.define_app()

function App:init()
	self.upscaled_view = 'Game (2x)'
	self.native_resolution = tdengine.vec2(320, 180)
	self.output_resolution = tdengine.vec2(1280, 720)
	self.gbuffer_resolution = tdengine.vec2(1024, 576)

	self.window_flags = tdengine.enum.bitwise_or(
		tdengine.enums.WindowFlags.Windowed,
		tdengine.enums.WindowFlags.Border
	)
end

function App:on_init_game()
	tdengine.ffi.create_window('Skeletal Particles', self.native_resolution.x, self.native_resolution.y, self.window_flags)
	tdengine.ffi.set_window_icon(tdengine.ffi.resolve_format_path('image', 'logo/icon.png'):to_interned())
	tdengine.ffi.set_target_fps(144)

	tdengine.gpus.build(tdengine.module.read_from_named_path('gpu_info'))
end

function App:on_start_game()
  tdengine.editor.find('SceneEditor'):load('deferred_render')

	tdengine.ffi.use_editor_layout('gbuffer')

	tdengine.editor.find('EditorUtility').style.grid.size = 12
	tdengine.editor.find('EditorUtility').enabled.grid = false

	tdengine.editor.find('DialogueEditor').hidden = true

	local game_views = tdengine.editor.find('GameViewManager')
  game_views:add_view(GameView:new(
		'Game',
		tdengine.gpus.find(RenderTarget.Color),
		tdengine.enums.GameViewSize.ExactSize, self.native_resolution,
		tdengine.enums.GameViewPriority.Main))

  game_views:add_view(GameView:new(
		'Scene',
		tdengine.gpus.find(RenderTarget.UpscaledLitScene),
		tdengine.enums.GameViewSize.ExactSize, self.gbuffer_resolution,
		tdengine.enums.GameViewPriority.Standard))

	game_views:add_view(GameView:new(
		'Color Buffer',
		tdengine.gpus.find(RenderTarget.Color),
		tdengine.enums.GameViewSize.ExactSize, self.gbuffer_resolution,
		tdengine.enums.GameViewPriority.Standard))

	game_views:add_view(GameView:new(
		'Normal Buffer',
		tdengine.gpus.find(RenderTarget.UpscaledNormals),
		tdengine.enums.GameViewSize.ExactSize, self.gbuffer_resolution,
		tdengine.enums.GameViewPriority.Standard))

	game_views:add_view(GameView:new(
		'Light Map',
		tdengine.gpus.find(RenderTarget.LightMap),
		tdengine.enums.GameViewSize.ExactSize, self.gbuffer_resolution,
		tdengine.enums.GameViewPriority.Standard))
end

function App:on_end_frame()
	-- local skeleton_viewer = tdengine.entity.find('SkeletonViewer') 
	-- if skeleton_viewer then
	-- 	for system in tdengine.iterator.values(skeleton_viewer.animation.particle_systems) do
	-- 		tdengine.gpu.bind_render_pass('fluid')
	-- 		tdengine.ffi.lf_draw(system.handle)
	-- 		tdengine.gpu.submit_render_pass('fluid')
	-- 	end
	-- end
end
  
function App:on_render_scene()  
	-- tdengine.gpu.bind_render_pass('scene')
	-- tdengine.gpu.submit_render_pass('scene')
  -- tdengine.gpu.apply_ping_pong('scene')
end 
 
function App:on_swapchain_ready()
	if tdengine.is_packaged_build then 
		--local swapchain = tdengine.ffi.gpu_acquire_swapchain()
		--tdengine.ffi.blit_render_target(self.renderer.command_buffers.post_processing, self.renderer.targets.scene, swapchain)
	end

	tdengine.ffi.render_imgui()
end

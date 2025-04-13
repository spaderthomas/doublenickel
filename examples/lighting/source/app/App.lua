Shader = doublenickel.enum.define(
  'Shader',
  {
  }
)

RenderTarget = doublenickel.enum.define(
  'RenderTarget',
  {
    Native = 0,
    Upscaled = 1,
  }
)

Buffer = doublenickel.enum.define(
  'Buffer',
  {
    Lights = 0,
  }
)

--[===[
- Get the collider editor working again (draw the grid to the correct framebuffer, draw colliders to the same, make them draggable)
- Make render targets for the color buffer and normal buffer
- Render SDF shapes to the color buffer
- Render SDF shapes to the normal buffer
- Add a render pass which copies (for instance) the color buffer to a final render target and applies a basic shader (double the red component)
- Add some C code for a point light, load this header into LuaJIT, and then make an entity that holds this struct
- Make a GPU buffer which holds an array of lights
- Change the aforementioned shader to do some debug rendering of the lights

]===]

local App = doublenickel.define_app()

function App:on_init_game()
  self.native_resolution = doublenickel.vec2(320, 180)
  self.output_resolution = doublenickel.vec2(1024, 576)

  local dn_config = AppConfig:new({
    target_fps = 144,
    window = WindowConfig:new({
      title = 'Lighting',
      native_resolution = Vector2:new(320, 180),
      flags = doublenickel.enum.bitwise_or(
        doublenickel.enums.WindowFlags.Windowed,
        doublenickel.enums.WindowFlags.Border
      ),
      icon = dn.paths_resolve_format('dn_image', 'logo/icon.png'),
    }),
    gpu = GpuConfig:new({
      shader_path = dn.paths_resolve('shaders'),
      search_paths = {
          dn.paths_resolve('shader_includes')
      },
      shaders = {
      },
      render_targets = {
        {
          name = RenderTarget.Native,
          size = self.native_resolution,
        },
        {
          name = RenderTarget.Upscaled,
          size = self.output_resolution,
        }
      }
    }),
  })

  dn.app_configure(dn_config)

  doublenickel.asset.register_cast(RenderTarget, 'dn_gpu_render_target_t')
  doublenickel.asset.register_cast(Shader, 'dn_gpu_shader_t')
  
  self.sdf = ffi.new('dn_sdf_renderer_t [1]');
  self.sdf = dn.sdf_renderer_create(1024 * 1024)

  self.command_buffer = dn.gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
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
  doublenickel.editor.configure(EditorConfig:new({
    grid_enabled = true,
    grid_size = 24,
    hide_dialogue_editor = true,
    game_views = {
      GameView:new(
        'Native View',
        RenderTarget.Native,
        doublenickel.enums.GameViewSize.ExactSize, self.native_resolution,
        doublenickel.enums.GameViewPriority.Main),
      GameView:new(
        'Upscaled View',
        RenderTarget.Upscaled,
        doublenickel.enums.GameViewSize.ExactSize, self.output_resolution,
        doublenickel.enums.GameViewPriority.Standard)
    },
    scene = 'default',
    layout = 'default',
  }))
end

function App:on_scene_rendered()
  dn.gpu_begin_render_pass(self.command_buffer, self.render_pass)
  dn.gpu_set_world_space(self.command_buffer, true)
  dn.gpu_set_camera(self.command_buffer, doublenickel.editor.find('EditorCamera').offset:to_ctype())
  dn.sdf_renderer_draw(self.sdf, self.command_buffer)
  dn.gpu_end_render_pass(self.command_buffer)
  dn.gpu_command_buffer_submit(self.command_buffer)

  dn.gpu_render_target_blit(
    doublenickel.asset.find(RenderTarget.Native),
    doublenickel.asset.find(RenderTarget.Upscaled)
  )
end

function App:on_swapchain_ready()
  imgui.Imgui_Impl_glfw_opengl3:Render()
end


Shader = dn.enum.define(
  'Shader',
  {
    Sample = 0,
    Shape = 1,
    Sdf = 2,
    SdfNormal = 3,
    Solid = 5,
    Sprite = 6,
    Text = 7,
    Blit = 9,
  }
)

Font = dn.enum.define(
  'Font',
  {
    Tiny5 = 0,
  }
)


RenderTarget = dn.enum.define(
  'RenderTarget',
  {
    Native = 0,
    Upscaled = 1,
  }
)

local App = dn.define_app()

function App:on_init_game()
  self.native_resolution = dn.vec2(320, 180)
  self.output_resolution = dn.vec2(1024, 576)

  -----------------------
  -- APP CONFIGURATION --
  -----------------------
  dn.paths.load(dn.ffi.paths_resolve_format('dn_app_file', 'data/paths.lua'))

  local dn_config = AppConfig:new({
    window = WindowConfig:new({
      title = 'SDF Clock',
      native_resolution = Vector2:new(320, 180),
      flags = dn.enum.bitwise_or(
        WindowFlags.Windowed,
        WindowFlags.Border
      ),
      icon = dn.ffi.paths_resolve_format('dn_image', 'logo/icon.png'),
      target_fps = 144,
    }),

    audio = AudioConfig:new({
      dirs = {
        dn.ffi.paths_resolve('audio')
      },
    }),

    font = FontConfig:new({
      fonts = {
        {
          id = Font.Tiny5,
          file_path = dn.ffi.paths_resolve_format('font', 'Tiny5-Regular.ttf'),
          sizes = { 16, 24, 32 },
          imgui = false
        },
      }
    }),

    gpu = GpuConfig:new({
      shader_path = dn.ffi.paths_resolve('shaders'),
      search_paths = {
          dn.ffi.paths_resolve('shader_includes')
      },
      shaders = {
        {
          name = Shader.Sample,
          kind = GpuShaderKind.Graphics,
          vertex_shader = dn.ffi.paths_resolve_format('shader', 'shader.vertex'),
          fragment_shader = dn.ffi.paths_resolve_format('shader', 'shader.fragment'),
        }
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

  -- With our configuration done, we can call into C to initialize the framework. This is the thrust of initialization.
  dn.ffi.app_configure(dn_config)


  -----------------------
  -- GPU CONFIGURATION --
  -----------------------
  dn.asset.register_cast(RenderTarget, 'dn_gpu_render_target_t')
  dn.asset.register_cast(Shader, 'dn_gpu_shader_t')
  
  self.sdf_renderer = dn.ffi.sdf_renderer_create(1024 * 1024)

  self.command_buffer = dn.ffi.gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
    max_commands = 1024
  }))

  self.render_pass = GpuRenderPass:new({
    color = {
      attachment = RenderTarget.Native,
      load = GpuLoadOp.Clear
    }
  })

end

-- App:on_init_game()'s purpose is to initialize the framework. App:on_start_game()'s purpose is to initialize the game itself. Now that
-- the framework is fully initialized, and all resources have been loaded, you can do whatever you'd like.
function App:on_start_game()
  -- dn opens your game in a framebuffer within an editor window. You can configure that editor window here.
  dn.editor.configure(EditorConfig:new({
    grid_enabled = false,
    grid_size = 12,

    -- There's a full featured dialogue editor, but if you don't plan to use it, hide it.
    hide_dialogue_editor = true,

    -- What framebuffers should be drawn in the editor, and at what size?
    game_views = {
      GameView:new(
        'Native View',
        RenderTarget.Native,
        dn.enums.GameViewSize.ExactSize, self.native_resolution,
        dn.enums.GameViewPriority.Main),
      GameView:new(
        'Upscaled View',
        RenderTarget.Upscaled,
        dn.enums.GameViewSize.ExactSize, self.output_resolution,
        dn.enums.GameViewPriority.Standard)
    },

    -- What scene should be opened by default?
    scene = 'sdf_clock',

    -- What editor layout should be opened by default?
    layout = 'sdf_clock',

    -- What GPU resources should the editor use to draw widgets?
    render_pass = self.render_pass,
    command_buffer = self.command_buffer
  }))
end

-- First, the framework calls `on_render_scene()` for anything which has that lifecycle method. Then, once everything
-- has rendered to the appropriate framebuffers, we call `on_scene_rendered()`. 
--
-- This is where I do post processing and compositing.
function App:on_scene_rendered()
  dn.ffi.gpu_begin_render_pass(self.command_buffer, self.render_pass)
  dn.ffi.gpu_set_world_space(self.command_buffer, true)
  dn.ffi.gpu_set_camera(self.command_buffer, dn.editor.find('EditorCamera').offset:to_ctype())
  dn.ffi.sdf_renderer_draw(self.sdf_renderer, self.command_buffer)
  dn.ffi.gpu_end_render_pass(self.command_buffer)
  dn.ffi.gpu_command_buffer_submit(self.command_buffer)

  dn.ffi.gpu_render_target_blit(
    dn.asset.find(RenderTarget.Native),
    dn.asset.find(RenderTarget.Upscaled)
  )
end

-- Finally, once the framework has the swapchain ready, you can draw to it. In an editor build, everything is rendered
-- to editor windows via ImGui
function App:on_swapchain_ready()
  imgui.Imgui_Impl_glfw_opengl3:Render()
end


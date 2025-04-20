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

local App = dn.app.define()

function App:init()
  self.native_resolution = Vector2:new(320, 180)
  self.output_resolution = Vector2:new(1024, 576)
end

function App:configure()
  dn.paths.load(dn.ffi.paths_resolve_format('dn_install_file', 'data/paths.lua'))

  dn.app.configure(AppConfig:new({
    window = WindowConfig:new({
      title = 'SDF Clock',
      native_resolution = self.native_resolution,
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

    scene = SceneConfig:new({
      directory = dn.ffi.paths_resolve('scenes'),
      persistent = 'persistent'
    }),

    layout = LayoutConfig:new({
      directory = dn.ffi.paths_resolve('layouts')
    }),

    asset = AssetConfig:new({
      casts = {
        { enum = RenderTarget, ctype = 'dn_gpu_render_target_t' },
        { enum = Shader,       ctype = 'dn_gpu_shader_t' },
      }
    }),
  }))
  
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

  dn.editor.configure(EditorConfig:new({
    views = {
      GameView:new(
        'Native View',
        RenderTarget.Native,
        dn.enums.GameViewSize.ExactSize, self.native_resolution,
        dn.enums.GameViewPriority.Main
      ),
      GameView:new(
        'Upscaled View',
        RenderTarget.Upscaled,
        dn.enums.GameViewSize.ExactSize, self.output_resolution,
        dn.enums.GameViewPriority.Standard
      )
    },
    scene = 'sdf_clock',
    layout = 'sdf_clock',
    -- render_pass = self.render_pass,
    -- command_buffer = self.command_buffer
  }))
end

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

function App:on_swapchain_ready()
  imgui.Imgui_Impl_glfw_opengl3:Render()
end


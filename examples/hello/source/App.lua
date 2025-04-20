RenderTarget = dn.enum.define(
  'RenderTarget',
  {
    Native = 0,
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
      title = 'Hello, DN!',
      native_resolution = self.native_resolution,
      flags = dn.enum.bitwise_or(
        WindowFlags.Windowed,
        WindowFlags.Border
      ),
      icon = dn.ffi.paths_resolve_format('dn_image', 'logo/icon.png'),
      target_fps = 144,
    }),

    gpu = GpuConfig:new({
      render_targets = {
        {
          name = RenderTarget.Native,
          size = self.native_resolution,
        },
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
    },
    scene = 'hello',
    layout = 'hello',
    render_pass = self.render_pass,
    command_buffer = self.command_buffer
  }))
end

function App:on_swapchain_ready()
  imgui.Imgui_Impl_glfw_opengl3:Render()
end


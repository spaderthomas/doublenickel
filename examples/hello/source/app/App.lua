-- Assets can be referred to by string or by enum 
RenderTarget = doublenickel.enum.define(
  'RenderTarget',
  {
    Native = 0,
  }
)

-- This is your application's entry point
local App = doublenickel.define_app()

-- This is the first user code to be called. You provide parameters to initialize the framework's core subsystems. Mostly,
-- this involves specifying paths and telling the framework what resources to create (e.g. shaders)
function App:on_init_game()
  self.native_resolution = Vector2:new(1280, 960)

  -----------------------
  -- APP CONFIGURATION --
  -----------------------
  doublenickel.paths.load(dn.ffi.paths_resolve_format('dn_app_file', 'data/paths.lua'))

  -- The main configuration struct for the framework
  dn.ffi.app_configure(AppConfig:new({
    -- WINDOW
    --
    -- This is the first time that dn.ffi.paths_resolve_*() shows up. This subsystem of doublenickel is how you should deal
    -- with all paths in your game. It builds absolute paths to resources at runtime. It can also handle format strings,
    -- for e.g. a specific audio file in the well-known audio directory.
    --
    -- The way this system works is by providing named paths. The framework is hardcoded to look for a file called 
    -- "paths.lua" in your app's data directory. That file will provide a hierarchy of paths, and names for those paths.
    -- The framework will build the corresponding absolute paths, which you can then refer to by name.
    window = WindowConfig:new({
      title = 'Hello, DN!',
      native_resolution = self.native_resolution,
      flags = doublenickel.enum.bitwise_or(
        WindowFlags.Windowed,
        WindowFlags.Border
      ),
      icon = dn.ffi.paths_resolve_format('dn_image', 'logo/icon.png'),
      target_fps = 144,
    }),

    -- GPU
    --
    -- Shaders are preprocessed to allow for #includes. Add your include directories here. The shader search path
    -- isn't used to locate shaders (since, remember, everything is an absolute path). Rather, it's a directory for
    -- a file monitor to watch. When files change in this directory, your shaders will be hotloaded.
    gpu = GpuConfig:new({
      render_targets = {
        {
          name = RenderTarget.Native,
          size = self.native_resolution,
        },
      }
    }),

    layout = LayoutConfig:new({
      file = dn.ffi.paths_resolve('layouts')
    }),

    dialogue = {},
    scenes = {},
    state = {},
    actions = {},
    animations = {},
    backgrounds = {},
    characters = {},
  }))

  -----------------------
  -- GPU CONFIGURATION --
  -----------------------

  -- Any named resources we create are placed into a big table. You can get a pointer to any asset by giving its name to the table.
  -- However, since it returns a void*, you end up needing to do a lot of ugly casting. For a minor and purely aesthetic convenience,
  -- you can tell the framework which enums correspond to which types, and it will call ffi.cast() before returning the pointer.
  --
  -- This is entirely optional, inconsequential, and only works if you use enums instead of strings to identify resources.
  doublenickel.asset.register_cast(RenderTarget, 'dn_gpu_render_target_t')
  
  -- Every app needs a command buffer to write GPU commands into.
  self.command_buffer = dn.ffi.gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
    max_commands = 1024
  }))

  -- A render pass defines the attachments your draw calls will write to.
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
  -- doublenickel opens your game in a framebuffer within an editor window. You can configure that editor window here.
  doublenickel.editor.configure(EditorConfig:new({
    grid_enabled = true,
    grid_size = 12,

    -- There's a full featured dialogue editor, but if you don't plan to use it, hide it.
    hide_dialogue_editor = true,

    -- What framebuffers should be drawn in the editor, and at what size?
    game_views = {
      GameView:new(
        'Native View',
        RenderTarget.Native,
        GameViewSize.ExactSize, self.native_resolution
      )
    },

    -- What scene should be opened by default?
    scene = 'hello',

    -- What editor layout should be opened by default?
    layout = 'hello',

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
  -- dn.ffi.gpu_begin_render_pass(self.command_buffer, self.render_pass)
  -- dn.ffi.gpu_set_world_space(self.command_buffer, true)
  -- dn.ffi.gpu_set_camera(self.command_buffer, doublenickel.editor.find('EditorCamera').offset:to_ctype())
  -- dn.ffi.sdf_renderer_draw(self.sdf_renderer, self.command_buffer)
  -- dn.ffi.gpu_end_render_pass(self.command_buffer)
  -- dn.ffi.gpu_command_buffer_submit(self.command_buffer)

  -- dn.ffi.gpu_render_target_blit(
  --   doublenickel.asset.find(RenderTarget.Native),
  --   doublenickel.asset.find(RenderTarget.Upscaled)
  -- )
end

-- Finally, once the framework has the swapchain ready, you can draw to it. In an editor build, everything is rendered
-- to editor windows via ImGui
function App:on_swapchain_ready()
  imgui.Imgui_Impl_glfw_opengl3:Render()
end


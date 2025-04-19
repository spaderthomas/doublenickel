-- First, I'll define enums for most resources in the game. You can refer to things by string constants, but I much prefer
-- to use enums. There's nothing special about the names of the enums or their values.
Shader = doublenickel.enum.define(
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

Font = doublenickel.enum.define(
  'Font',
  {
    Tiny5 = 0,
  }
)


RenderTarget = doublenickel.enum.define(
  'RenderTarget',
  {
    Native = 0,
  }
)

Buffer = doublenickel.enum.define(
  'Buffer',
  {
    Lights = 0,
  }
)

-- Then, tell doublenickel what the app class will be for your program
local App = doublenickel.define_app()

-- After all of the framework scripts have been loaded, this is the first user code to be called. The framework is not
-- initialized yet; there is no window, and therefore no GPU context, and therefore very few subsystems we can properly
-- initialize.
--
-- It's the job of the app to configure each framework subsystem, pointing it at directories it should find data and specifying
-- which resources to create. That's what we'll do in App:on_init_game()
function App:on_init_game()
  self.native_resolution = doublenickel.vec2(320, 180)
  self.output_resolution = doublenickel.vec2(1024, 576)

  -----------------------
  -- APP CONFIGURATION --
  -----------------------
  
  -- The main configuration struct for the framework
  local dn_config = AppConfig:new({
    target_fps = 144,
    -- WINDOW
    --
    -- This is the first time that dn.paths_resolve_*() shows up. This subsystem of doublenickel is how you should deal
    -- with all paths in your game. It builds absolute paths to resources at runtime. It can also handle format strings,
    -- for e.g. a specific audio file in the well-known audio directory.
    --
    -- The way this system works is by providing named paths. The framework is hardcoded to look for a file called 
    -- "paths.lua" in your app's data directory. That file will provide a hierarchy of paths, and names for those paths.
    -- The framework will build the corresponding absolute paths, which you can then refer to by name.
    window = WindowConfig:new({
      title = 'Hello, DN!',
      native_resolution = Vector2:new(320, 180),
      flags = doublenickel.enum.bitwise_or(
        doublenickel.enums.WindowFlags.Windowed,
        doublenickel.enums.WindowFlags.Border
      ),
      icon = dn.paths_resolve_format('dn_image', 'logo/icon.png'),
    }),

    -- AUDIO
    --
    -- Just provide a list of directories as Lua strings, and the framework will scan them for audio files and load 
    -- them into memory.
    audio = AudioConfig:new({
      dirs = {
        dn.paths_resolve('audio')
      },
    }),

    -- FONT
    --
    -- Fonts are baked at startup, so if you'd like to use a font size, you need to declare it here.
    font = FontConfig:new({
      fonts = {
        {
          id = Font.Tiny5,
          file_path = dn.paths_resolve_format('font', 'Tiny5-Regular.ttf'),
          sizes = { 16, 24, 32 },
          imgui = false
        },
      }
    }),

    -- GPU
    --
    -- Shaders are preprocessed to allow for #includes. Add your include directories here. The shader search path
    -- isn't used to locate shaders (since, remember, everything is an absolute path). Rather, it's a directory for
    -- a file monitor to watch. When files change in this directory, your shaders will be hotloaded.
    gpu = GpuConfig:new({
      shader_path = dn.paths_resolve('shaders'),
      search_paths = {
          dn.paths_resolve('shader_includes')
      },
      shaders = {},
      render_targets = {
        {
          name = RenderTarget.Native,
          size = self.native_resolution,
        },
      }
    }),

    -- STEAM
    -- 
    -- Steam support is mostly unported, but this is a placeholder.
    steam = SteamConfig:new({
      app_id = 480
    }),

    -- IMAGE
    --
    -- A list of directories from which images will be loaded and packed into texture atlases.
    image = ImageConfig:new({
      dirs = {
        dn.paths_resolve('images')
      },
    }),
  })

  -- With our configuration done, we can call into C to initialize the framework. This is the thrust of initialization.
  dn.app_configure(dn_config)


  -----------------------
  -- GPU CONFIGURATION --
  -----------------------

  -- Any named resources we create are placed into a big table. You can get a pointer to any asset by giving its name to the table.
  -- However, since it returns a void*, you end up needing to do a lot of ugly casting. For a minor and purely aesthetic convenience,
  -- you can tell the framework which enums correspond to which types, and it will call ffi.cast() before returning the pointer.
  --
  -- This is entirely optional, inconsequential, and only works if you use enums instead of strings to identify resources.
  doublenickel.asset.register_cast(RenderTarget, 'dn_gpu_render_target_t')
  doublenickel.asset.register_cast(Shader, 'dn_gpu_shader_t')
  
  -- The framework provides a pre-configured SDF renderer for drawing primitives. It works immediate mode; it owns a CPU buffer,
  -- which it syncs to the GPU every frame, and it builds a dn_gpu_pipeline_t with default shaders.
  -- self.sdf_renderer = ffi.new('dn_sdf_renderer_t [1]');
  -- self.sdf_renderer = dn.sdf_renderer_create(1024 * 1024)

  -- Every app needs a command buffer to write GPU commands into.
  self.command_buffer = dn.gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
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
    grid_enabled = false,
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
  -- dn.gpu_begin_render_pass(self.command_buffer, self.render_pass)
  -- dn.gpu_set_world_space(self.command_buffer, true)
  -- dn.gpu_set_camera(self.command_buffer, doublenickel.editor.find('EditorCamera').offset:to_ctype())
  -- dn.sdf_renderer_draw(self.sdf_renderer, self.command_buffer)
  -- dn.gpu_end_render_pass(self.command_buffer)
  -- dn.gpu_command_buffer_submit(self.command_buffer)

  -- dn.gpu_render_target_blit(
  --   doublenickel.asset.find(RenderTarget.Native),
  --   doublenickel.asset.find(RenderTarget.Upscaled)
  -- )
end

-- Finally, once the framework has the swapchain ready, you can draw to it. In an editor build, everything is rendered
-- to editor windows via ImGui
function App:on_swapchain_ready()
  imgui.Imgui_Impl_glfw_opengl3:Render()
end


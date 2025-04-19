function dn.define_app()
  local class = dn.subsystem.define('App')
  return class
end

----------------------
-- C CONFIGURATIONS --
----------------------
WindowConfig = dn.class.metatype('dn_window_config_t')
function WindowConfig:init(params)
  local default = dn.ffi.window_config_default();
  self.title = String:new(params.title) or default.title
  self.icon = String:new(params.icon) or default.icon
  self.native_resolution = params.native_resolution or default.native_resolution
  self.flags = params.flags or default.flags
  self.display_mode = params.display_mode or default.display_mode
  self.target_fps = params.target_fps or default.target_fps
end

AudioConfig = dn.class.metatype('dn_audio_config_t')
function AudioConfig:init(params)
  local allocator = dn.ffi.allocator_find('bump')

  self.num_dirs = #params.dirs
  self.dirs = allocator:alloc_array('dn_path_t', #params.dirs)
  for i = 0, #params.dirs - 1 do
    self.dirs[i] = params.dirs[i + 1]
  end
end

FontDescriptor = dn.class.metatype('dn_font_descriptor_t')
function FontDescriptor:init(params)
  if dn.enum.is_enum(params.id) then
    self.id = params.id:to_qualified_string()
  else
    self.id = params.id
  end

  self.file_path = params.file_path

  self.sizes = {0}
  for i in dn.iterator.keys(params.sizes) do
    self.sizes[i - 1] = params.sizes[i]
  end
  self.flags = 0
  if params.default       then self.flags = bit.bor(self.flags, ffi.C.DN_FONT_FLAG_DEFAULT) end
  if params.imgui         then self.flags = bit.bor(self.flags, ffi.C.DN_FONT_FLAG_IMGUI) end
end

FontConfig = dn.class.metatype('dn_font_config_t')
function FontConfig:init(params)
  local allocator = dn.ffi.allocator_find('bump')

  self.num_fonts = #params.fonts
  self.fonts = allocator:alloc_array('dn_font_descriptor_t', #params.fonts)
  for i = 0, #params.fonts - 1 do
    self.fonts[i] = FontDescriptor:new(params.fonts[i + 1])
  end
end

GpuConfig = dn.class.metatype('dn_gpu_config_t')
function GpuConfig:init(params)
  local allocator = dn.ffi.allocator_find('bump')

  self.shader_path = params.shader_path or nil

  self.num_search_paths = params.search_paths and #params.search_paths or 0
  self.search_paths = allocator:alloc_array('const char*', self.num_search_paths)
  for i = 0, self.num_search_paths - 1 do
    self.search_paths[i] = params.search_paths[i + 1]
  end

  self.num_shaders = params.shaders and #params.shaders or 0
  self.shaders = allocator:alloc_array('dn_gpu_shader_descriptor_t', self.num_shaders)
  for i = 0, self.num_shaders - 1 do
    self.shaders[i] = GpuShaderDescriptor:new(params.shaders[i + 1])
  end

  self.num_render_targets = params.render_targets and #params.render_targets or 0
  self.render_targets = allocator:alloc_array('dn_gpu_render_target_descriptor_t', self.num_render_targets)
  for i = 0, self.num_render_targets - 1 do
    self.render_targets[i] = GpuRenderTargetDescriptor:new(params.render_targets[i + 1])
  end
end

SteamConfig = dn.class.metatype('dn_steam_config_t')
function SteamConfig:init(params)
  self.app_id = params.app_id
end

ImageConfig = dn.class.metatype('dn_image_config_t')
function ImageConfig:init(params)
  local allocator = dn.ffi.allocator_find('bump')

  self.num_dirs = #params.dirs
  self.dirs = allocator:alloc_array('dn_string_t', #params.dirs)
  for i = 0, #params.dirs - 1 do
    self.dirs[i] = String:new(params.dirs[i + 1])
  end
end

FrameworkConfig = dn.class.metatype('dn_app_config_t')
function FrameworkConfig:init(params)
  self.window = params.window or dn.ffi.window_config_default()
  self.audio = params.audio or dn.ffi.audio_config_default()
  self.font = params.font or dn.ffi.font_config_default()
  self.gpu = params.gpu or ffi.new('dn_gpu_config_t')
  self.asset = params.asset or ffi.new('dn_asset_config_t')
  self.steam = params.steam or ffi.new('dn_steam_config_t')
  self.image = params.image or ffi.new('dn_image_config_t')
end

-----------------------
-- LUA CONFIGURATION --
-----------------------

LayoutConfig = dn.class.define('LayoutConfig')
function LayoutConfig:init(params)
  params = params or {}
  self.directory = params.directory
end


AppConfig = dn.class.define('AppConfig')
function AppConfig:init(params)
  self.framework = FrameworkConfig:new(params)
  self.layout = LayoutConfig:new(params.layout)
end

function AppConfig:apply()
  ffi.C.dn_app_configure(self.framework)
end
local self = dn.editor

----------------
-- PUBLIC API --
----------------
EditorConfig = dn.class.define('EditorConfig')
function EditorConfig:init(params)
  self.grid_enabled = params.grid_enabled
  self.grid_size = params.grid_size or 12
  self.hide_dialogue_editor = params.hide_dialogue_editor
  self.game_views = params.game_views or {}
  self.scene = params.scene or 'default'
  self.layout = params.layout or 'default'

  if params.render_pass then
    dn.ffi.trace('dn.ffi.editor.configure', 'Configuring editor to use the app\'s render pass and command buffer')
    self.render_pass = params.render_pass
    self.command_buffer = params.command_buffer
  else
    dn.ffi.trace('dn.ffi.editor.configure', 'Creating the editor to use its own render pass and command buffer')
    dn.ffi.gpu_render_target_create(GpuRenderTargetDescriptor:new({
      name = DnRenderTargets.Editor,
      size = Vector2:new(1024, 576)
    }))
    self.render_pass = GpuRenderPass:new({
      color = {
        attachment = DnRenderTargets.Editor,
        load = GpuLoadOp.Clear
      }
    })
    self.command_buffer = dn.ffi.gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
      max_commands = 1024
    }))

    table.insert(self.game_views, GameView:new(
      'Editor View',
      DnRenderTargets.Editor,
      GameViewSize.ExactSize, Vector2:new(1024, 576),
      GameViewPriority.Main
    ))
  end

  dn.asset.register_cast(DnRenderTargets, 'dn_gpu_render_target_t')

  self.fonts = {
    regular = params.fonts and params.fonts.regular or 'inconsolata',
    bold = params.fonts and params.fonts.bold or 'inconsolata-extrabold',
    size = params.fonts and params.fonts.size or 16,
  }
end

function dn.editor.init()
  dn.ffi.log('dn.editor.init')

  EditorFonts = dn.enum.define(
    'EditorFonts',
    {
      Regular = 0,
      Bold = 1,
    }
  )

  DnRenderTargets = dn.enum.define(
    'DnRenderTargets',
    {
      Editor = 0
    }
  )

  self.focus_state = {}
  self.hover_state = {}
  self.window_stack = dn.data_types.stack:new()

  dn.editor.entities = {}
  for name, class in pairs(dn.editor.types) do
    dn.editor.entities[name] = class:new()
  end

  dn.editor.colors = {
    main_list = dn.colors.white:copy(),
    cdata_member = dn.colors.red:copy(),
    lua_member = dn.colors.green:copy(),
    scalar = dn.colors.zomp:copy(),
  }
end

function dn.editor.update()
  dn.ffi.trace('dn.ffi.editor.update')

  dn.ffi.gpu_begin_render_pass(self.config.command_buffer, self.config.render_pass)
  for name, editor in dn.iterator.pairs(dn.editor.entities) do
    dn.ffi.trace('dn.ffi.editor.update_entity', name)
    editor:update()
    editor:draw()
  end

  dn.ffi.sdf_circle_ex(self.sdf, 0, 0, 1.0, 0.0, 0.0, 0.0, 1.0, 100.0)
  dn.ffi.trace('dn.ffi.editor.update', 'Rendering the editor')

  local camera = self.find('EditorCamera').offset:to_ctype()
  dn.ffi.gpu_set_world_space(self.config.command_buffer, true)
  dn.ffi.gpu_set_camera(self.config.command_buffer, camera)
  dn.ffi.coord_set_camera(camera.x, camera.y)
  dn.ffi.sdf_renderer_draw(self.sdf, self.config.command_buffer)
  dn.ffi.gpu_end_render_pass(self.config.command_buffer)
  dn.ffi.gpu_command_buffer_submit(self.config.command_buffer)
end

function dn.editor.configure(config)
  dn.ffi.trace('dn.ffi.editor.configure')

  self.config = config

  -- self.sdf = ffi.new('dn_sdf_renderer_t [1]');
  self.sdf = dn.ffi.sdf_renderer_create(1024 * 1024)

  dn.editor.find('EditorUtility').enabled.grid = config.grid_enabled
  dn.editor.find('EditorUtility').style.grid.size = config.grid_size
  dn.editor.find('DialogueEditor').hidden = config.hide_dialogue_editor

  for view in dn.iterator.values(config.game_views) do
    dn.editor.find('GameViewManager'):add_view(view)
  end

  dn.editor.find('SceneEditor'):load(config.scene)
  dn.ffi.imgui_load_layout(config.layout)
end

function dn.editor.define(name)
  local class = dn.class.define(name)
  class:include_lifecycle()
  class:include_update()

  dn.editor.types[name] = class

  return class
end

function dn.editor.find(name)
  return dn.editor.entities[name]
end

function dn.editor.begin_window(name, flags)
  flags = flags or 0
  imgui.Begin(name)
  self:set_window_focus(name, imgui.IsWindowFocused())
  self:set_window_hover(name, imgui.IsWindowHovered())
  self.window_stack:push(name)
end

function dn.editor.end_window()
  self.window_stack:pop()
  imgui.End()
end

function dn.editor.push_font(font)
  if EditorFonts.Regular:match(font) then
    imgui.PushFont(self.config.fonts.regular, self.config.fonts.size)
  elseif EditorFonts.Bold:match(font) then
    imgui.PushFont(self.config.fonts.bold, self.config.fonts.size)
  end
end

function dn.editor.begin_child(name, x, y, flags)
  imgui.BeginChild(name, imgui.ImVec2(x, y), true, flags)

  -- If a child window is focused (or hovered), it will mark the parent (i.e. the part of the parent window
  -- that is *not* inside the child) as unfocused. Since I'm using this for only allowing a window's hotkeys
  -- when it or its child region is focused, I don't want this.
  --
  -- Therefore, if you begin a child region, we lump its focus in with the parent. This means there's no way
  -- to differentiate between which one is actually capturing focus, but that'd be a very simple API change,
  -- since all the ImGui calls are wrapped up here.
  local parent = self.window_stack:peek()

  local current_focus = self:is_window_focused(parent)
  self:set_window_focus(parent, current_focus or imgui.IsWindowFocused())

  local current_hover = self:is_window_hovered(parent)
  self:set_window_hover(parent, current_hover or imgui.IsWindowHovered())
end

function dn.editor.end_child()
  imgui.EndChild()
end

function dn.editor.is_window_focused(name)
  name = name or self.window_stack:peek()
  return self.focus_state[name]
end

function dn.editor.set_window_focus(name, focus)
  self.focus_state[name] = focus
end

function dn.editor.is_window_hovered(name)
  name = name or self.window_stack:peek()
  return self.hover_state[name]
end

function dn.editor.set_window_hover(name, hover)
  self.hover_state[name] = hover
end


function dn.editor.center_next_window(size)
  local screen = dn.vec2(dn.ffi.window_get_content_area())
  local position = screen:scale(.5):subtract(size:scale(.5))
  imgui.SetNextWindowPos(position:unpack())
  imgui.SetNextWindowSize(size:unpack())
end


---------------------
-- EDITOR METADATA --
---------------------
dn.editor.MetadataKind = dn.enum.define(
  'EditorMetadata',
  {
    Ignore = 0,
    Field = 1,
    Callback = 2
  }
)

FieldMetadata = dn.class.define('FieldMetadata')

function FieldMetadata:init(field, metadata)
  metadata = metadata or {}

  self.field = field
  self.slider_min = metadata.slider_min or -100
  self.slider_max = metadata.slider_max or 100
  self.read_only = ternary(metadata.read_only, true, false) 
end

FieldMetadata.Presets = {
  Float_01 = {
    slider_min = 0,
    slider_max = 1
  },
  ReadOnly = {
    read_only = true
  }
}

dn.editor.FieldMetadata = FieldMetadata
dn.editor.default_metadata = FieldMetadata:new()


local function ensure_editor_sentinel(t)
  if not t[dn.editor.sentinel] then
    t[dn.editor.sentinel] = {}
  end
end

local function ensure_editor_metadata(t, metadata_kind)
  ensure_editor_sentinel(t)
  if not t[dn.editor.sentinel][metadata_kind:to_string()] then
    t[dn.editor.sentinel][metadata_kind:to_string()] = {}
  end
end

local function find_metadata(t, metadata_kind) 
  ensure_editor_metadata(t, metadata_kind)
  return t[dn.editor.sentinel][metadata_kind:to_string()]
end

function dn.editor.ignore_field(t, field)
  ensure_editor_metadata(t, dn.editor.MetadataKind.Ignore)
  local metadata = find_metadata(t, dn.editor.MetadataKind.Ignore)
  metadata[field] = true
end

function dn.editor.is_ignoring_field(t, field)
  -- Globally ignore the intrusive editor table
  if field == dn.editor.sentinel then return true end

  if not t[dn.editor.sentinel] then return false end
  if not t[dn.editor.sentinel].ignore then return false end

  return t[dn.editor.sentinel].ignore[field]
end

function dn.editor.set_field_metadata(t, field, metadata)
  local table_metadata = find_metadata(t, dn.editor.MetadataKind.Field)
  table_metadata[field] = FieldMetadata:new(field, metadata)
end

function dn.editor.set_field_metadatas(t, metadatas)
  for field, metadata in pairs(metadatas) do
    dn.editor.set_field_metadata(t, field, metadata)
  end
end


function dn.editor.get_field_metadata(t, field)
  local metadata = table.get_or_nil(t, dn.editor.sentinel, dn.editor.MetadataKind.Field:to_string(), field)
  if metadata then 
    return metadata, true
  end
 
  return dn.editor.default_metadata, false
end


----------------------
-- EDITOR CALLBACKS --
----------------------
function dn.editor.set_editor_callbacks(t, callbacks)
  ensure_editor_sentinel(t)
  t[dn.editor.sentinel].callbacks = callbacks
end

function dn.editor.run_editor_callback(t, callback, ...)
  if not t[dn.editor.sentinel] then return end
  if not t[dn.editor.sentinel].callbacks then return end

  local callback = t[dn.editor.sentinel].callbacks[callback]
  if not callback then return end

  return callback(...)
end

dn.editor.layers = {
  grid = 90,
  colliders = 110,
  collider_overlay = 120,
}

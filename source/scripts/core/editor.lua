local self = doublenickel.editor

----------------
-- PUBLIC API --
----------------
EditorConfig = doublenickel.class.define('EditorConfig')
function EditorConfig:init(params)
  self.grid_enabled = params.grid_enabled
  self.grid_size = params.grid_size or 12
  self.hide_dialogue_editor = params.hide_dialogue_editor
  self.game_views = params.game_views or {}
  self.scene = params.scene or 'default'
  self.layout = params.layout or 'default'

  if params.render_pass then
    self.render_pass = params.render_pass
    self.command_buffer = params.command_buffer
  else
    dn.gpu_render_target_create(GpuRenderTargetDescriptor:new({
      name = DnRenderTargets.Editor,
      size = Vector2:new(1024, 576)
    }))
    self.render_pass = GpuRenderPass:new({
      color = {
        attachment = DnRenderTargets.Editor,
        load = GpuLoadOp.Clear
      }
    })
    self.command_buffer = dn.gpu_command_buffer_create(GpuCommandBufferDescriptor:new({
      max_commands = 1024
    }))

    table.insert(self.game_views, GameView:new(
      'Editor View',
      DnRenderTargets.Editor,
      GameViewSize.ExactSize, Vector2:new(1024, 576),
      GameViewPriority.Main
    ))
  end

  doublenickel.asset.register_cast(DnRenderTargets, 'dn_gpu_render_target_t')

  self.fonts = {
    regular = params.fonts and params.fonts.regular or 'inconsolata',
    bold = params.fonts and params.fonts.bold or 'inconsolata-extrabold',
    size = params.fonts and params.fonts.size or 16,
  }
end

function doublenickel.editor.init()
  doublenickel.dn_log('doublenickel.editor.init')

  EditorFonts = doublenickel.enum.define(
    'EditorFonts',
    {
      Regular = 0,
      Bold = 1,
    }
  )

  DnRenderTargets = doublenickel.enum.define(
    'DnRenderTargets',
    {
      Editor = 0
    }
  )

  self.focus_state = {}
  self.hover_state = {}
  self.window_stack = doublenickel.data_types.stack:new()

  doublenickel.editor.entities = {}
  for name, class in pairs(doublenickel.editor.types) do
    doublenickel.editor.entities[name] = class:new()
  end

  doublenickel.editor.colors = {
    main_list = doublenickel.colors.white:copy(),
    cdata_member = doublenickel.colors.red:copy(),
    lua_member = doublenickel.colors.green:copy(),
    scalar = doublenickel.colors.zomp:copy(),
  }
end

function doublenickel.editor.update()
  dn.trace('dn.editor.update')

  dn.gpu_begin_render_pass(self.config.command_buffer, self.config.render_pass)
  for name, editor in doublenickel.iterator.pairs(doublenickel.editor.entities) do
    dn.trace('dn.editor.update', name)
    editor:update()
    editor:draw()
  end

  dn.gpu_set_world_space(self.config.command_buffer, true)
  local camera = self.find('EditorCamera').offset:to_ctype()
  dn.gpu_set_camera(self.config.command_buffer, camera)
  dn.coord_set_camera(camera.x, camera.y)
  dn.sdf_renderer_draw(self.sdf, self.config.command_buffer)
  dn.gpu_end_render_pass(self.config.command_buffer)
  dn.gpu_command_buffer_submit(self.config.command_buffer)
end

function doublenickel.editor.configure(config)
  self.config = config

  self.sdf = ffi.new('dn_sdf_renderer_t [1]');
  self.sdf = dn.sdf_renderer_create(1024 * 1024)

  doublenickel.editor.find('EditorUtility').enabled.grid = config.grid_enabled
  doublenickel.editor.find('EditorUtility').style.grid.size = config.grid_size
  doublenickel.editor.find('DialogueEditor').hidden = config.hide_dialogue_editor

  for view in doublenickel.iterator.values(config.game_views) do
    doublenickel.editor.find('GameViewManager'):add_view(view)
  end

  doublenickel.editor.find('SceneEditor'):load(config.scene)
  dn.imgui_load_layout(config.layout)
end

function doublenickel.editor.define(name)
  local class = doublenickel.class.define(name)
  class:include_lifecycle()
  class:include_update()

  doublenickel.editor.types[name] = class

  return class
end

function doublenickel.editor.find(name)
  return doublenickel.editor.entities[name]
end

function doublenickel.editor.begin_window(name, flags)
  flags = flags or 0
  imgui.Begin(name)
  self:set_window_focus(name, imgui.IsWindowFocused())
  self:set_window_hover(name, imgui.IsWindowHovered())
  self.window_stack:push(name)
end

function doublenickel.editor.end_window()
  self.window_stack:pop()
  imgui.End()
end

function doublenickel.editor.push_font(font)
  if EditorFonts.Regular:match(font) then
    imgui.PushFont(self.config.fonts.regular, self.config.fonts.size)
  elseif EditorFonts.Bold:match(font) then
    imgui.PushFont(self.config.fonts.bold, self.config.fonts.size)
  end
end

function doublenickel.editor.begin_child(name, x, y, flags)
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

function doublenickel.editor.end_child()
  imgui.EndChild()
end

function doublenickel.editor.is_window_focused(name)
  name = name or self.window_stack:peek()
  return self.focus_state[name]
end

function doublenickel.editor.set_window_focus(name, focus)
  self.focus_state[name] = focus
end

function doublenickel.editor.is_window_hovered(name)
  name = name or self.window_stack:peek()
  return self.hover_state[name]
end

function doublenickel.editor.set_window_hover(name, hover)
  self.hover_state[name] = hover
end


function doublenickel.editor.center_next_window(size)
  local screen = doublenickel.vec2(dn.window_get_content_area())
  local position = screen:scale(.5):subtract(size:scale(.5))
  imgui.SetNextWindowPos(position:unpack())
  imgui.SetNextWindowSize(size:unpack())
end


---------------------
-- EDITOR METADATA --
---------------------
doublenickel.editor.MetadataKind = doublenickel.enum.define(
  'EditorMetadata',
  {
    Ignore = 0,
    Field = 1,
    Callback = 2
  }
)

FieldMetadata = doublenickel.class.define('FieldMetadata')

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

doublenickel.editor.FieldMetadata = FieldMetadata
doublenickel.editor.default_metadata = FieldMetadata:new()


local function ensure_editor_sentinel(t)
  if not t[doublenickel.editor.sentinel] then
    t[doublenickel.editor.sentinel] = {}
  end
end

local function ensure_editor_metadata(t, metadata_kind)
  ensure_editor_sentinel(t)
  if not t[doublenickel.editor.sentinel][metadata_kind:to_string()] then
    t[doublenickel.editor.sentinel][metadata_kind:to_string()] = {}
  end
end

local function find_metadata(t, metadata_kind) 
  ensure_editor_metadata(t, metadata_kind)
  return t[doublenickel.editor.sentinel][metadata_kind:to_string()]
end

function doublenickel.editor.ignore_field(t, field)
  ensure_editor_metadata(t, doublenickel.editor.MetadataKind.Ignore)
  local metadata = find_metadata(t, doublenickel.editor.MetadataKind.Ignore)
  metadata[field] = true
end

function doublenickel.editor.is_ignoring_field(t, field)
  -- Globally ignore the intrusive editor table
  if field == doublenickel.editor.sentinel then return true end

  if not t[doublenickel.editor.sentinel] then return false end
  if not t[doublenickel.editor.sentinel].ignore then return false end

  return t[doublenickel.editor.sentinel].ignore[field]
end

function doublenickel.editor.set_field_metadata(t, field, metadata)
  local table_metadata = find_metadata(t, doublenickel.editor.MetadataKind.Field)
  table_metadata[field] = FieldMetadata:new(field, metadata)
end

function doublenickel.editor.set_field_metadatas(t, metadatas)
  for field, metadata in pairs(metadatas) do
    doublenickel.editor.set_field_metadata(t, field, metadata)
  end
end


function doublenickel.editor.get_field_metadata(t, field)
  local metadata = table.get_or_nil(t, doublenickel.editor.sentinel, doublenickel.editor.MetadataKind.Field:to_string(), field)
  if metadata then 
    return metadata, true
  end
 
  return doublenickel.editor.default_metadata, false
end


----------------------
-- EDITOR CALLBACKS --
----------------------
function doublenickel.editor.set_editor_callbacks(t, callbacks)
  ensure_editor_sentinel(t)
  t[doublenickel.editor.sentinel].callbacks = callbacks
end

function doublenickel.editor.run_editor_callback(t, callback, ...)
  if not t[doublenickel.editor.sentinel] then return end
  if not t[doublenickel.editor.sentinel].callbacks then return end

  local callback = t[doublenickel.editor.sentinel].callbacks[callback]
  if not callback then return end

  return callback(...)
end

doublenickel.editor.layers = {
  grid = 90,
  colliders = 110,
  collider_overlay = 120,
}

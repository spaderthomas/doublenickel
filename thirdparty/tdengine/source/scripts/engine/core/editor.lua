function tdengine.editor.define(name)
  local class = tdengine.class.define(name)
  class:include_lifecycle()
  class:include_update()

  tdengine.editor.types[name] = class

  return class
end

function tdengine.editor.find(name)
  return tdengine.editor.entities[name]
end

function tdengine.editor.begin_window(name, flags)
  tdengine.editor.impl:begin_window(name, flags)
end

function tdengine.editor.end_window()
  tdengine.editor.impl:end_window()
end

function tdengine.editor.begin_child(name, x, y, flags)
  tdengine.editor.impl:begin_child(name, x, y, flags)
end

function tdengine.editor.end_child()
  tdengine.editor.impl:end_child()
end

function tdengine.editor.is_window_focused(name)
  return tdengine.editor.impl:is_window_focused(name)
end

function tdengine.editor.set_window_focus(name, focus)
  return tdengine.editor.impl:set_window_focus(name, focus)
end

function tdengine.editor.is_window_hovered(name)
  return tdengine.editor.impl:is_window_hovered(name)
end

function tdengine.editor.center_next_window(size)
  local screen = tdengine.vec2(tdengine.screen_dimensions())
  local position = screen:scale(.5):subtract(size:scale(.5))
  imgui.SetNextWindowPos(position:unpack())
  imgui.SetNextWindowSize(size:unpack())
end

-- EDITOR METADATA
tdengine.editor.MetadataKind = tdengine.enum.define(
  'EditorMetadata',
  {
    Ignore = 0,
    Field = 1,
    Callback = 2
  }
)

FieldMetadata = tdengine.class.define('FieldMetadata')

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

tdengine.editor.FieldMetadata = FieldMetadata
tdengine.editor.default_metadata = FieldMetadata:new()


local function ensure_editor_sentinel(t)
  if not t[tdengine.editor.sentinel] then
    t[tdengine.editor.sentinel] = {}
  end
end

local function ensure_editor_metadata(t, metadata_kind)
  ensure_editor_sentinel(t)
  if not t[tdengine.editor.sentinel][metadata_kind:to_string()] then
    t[tdengine.editor.sentinel][metadata_kind:to_string()] = {}
  end
end

local function find_metadata(t, metadata_kind) 
  ensure_editor_metadata(t, metadata_kind)
  return t[tdengine.editor.sentinel][metadata_kind:to_string()]
end

function tdengine.editor.ignore_field(t, field)
  ensure_editor_metadata(t, tdengine.editor.MetadataKind.Ignore)
  t[tdengine.editor.sentinel].ignore[field] = true
end

function tdengine.editor.is_ignoring_field(t, field)
  -- Globally ignore the intrusive editor table
  if field == tdengine.editor.sentinel then return true end

  if not t[tdengine.editor.sentinel] then return false end
  if not t[tdengine.editor.sentinel].ignore then return false end

  return t[tdengine.editor.sentinel].ignore[field]
end



function tdengine.editor.set_field_metadata(t, field, metadata)
  local table_metadata = find_metadata(t, tdengine.editor.MetadataKind.Field)
  table_metadata[field] = FieldMetadata:new(field, metadata)
end

function tdengine.editor.set_field_metadatas(t, metadatas)
  for field, metadata in pairs(metadatas) do
    tdengine.editor.set_field_metadata(t, field, metadata)
  end
end


function tdengine.editor.get_field_metadata(t, field)
  local metadata = table.get_or_nil(t, tdengine.editor.sentinel, tdengine.editor.MetadataKind.Field:to_string(), field)
  if metadata then 
    return metadata, true
  end
 
  return tdengine.editor.default_metadata, false
end


function tdengine.editor.set_editor_callbacks(t, callbacks)
  ensure_editor_sentinel(t)
  t[tdengine.editor.sentinel].callbacks = callbacks
end

function tdengine.editor.run_editor_callback(t, callback, ...)
  if not t[tdengine.editor.sentinel] then return end
  if not t[tdengine.editor.sentinel].callbacks then return end

  local callback = t[tdengine.editor.sentinel].callbacks[callback]
  if not callback then return end

  return callback(...)
end

tdengine.editor.layers = {
  grid = 90,
  colliders = 110,
  collider_overlay = 120,
}


-- All of the actual functionality is in this inner class; initting the editor
-- just means instantiating one of these and sticking it in a well known place
local EditorImpl = tdengine.class.define('EditorImpl')


function tdengine.editor.init()
  tdengine.editor.impl = EditorImpl:new()

  tdengine.editor.entities = {}
  for name, class in pairs(tdengine.editor.types) do
    tdengine.editor.entities[name] = class:new()
  end

  tdengine.editor.colors = {
    main_list = tdengine.colors.white:copy(),
    cdata_member = tdengine.colors.red:copy(),
    lua_member = tdengine.colors.green:copy(),
    scalar = tdengine.colors.zomp:copy(),
  }
end

function tdengine.editor.update()
  local pipeline = tdengine.gpus.find(GraphicsPipeline.Editor)
  tdengine.ffi.gpu_graphics_pipeline_bind(pipeline)

  for editor in tdengine.iterator.values(tdengine.editor.entities) do
    editor:update()
    editor:draw()
  end

  tdengine.ffi.gpu_graphics_pipeline_submit(pipeline)
end

function EditorImpl:init()
  self.focus_state = {}
  self.hover_state = {}
  self.window_stack = tdengine.data_types.stack:new()
end

function EditorImpl:begin_window(name, flags)
  flags = flags or 0
  imgui.Begin(name)
  self:set_window_focus(name, imgui.IsWindowFocused())
  self:set_window_hover(name, imgui.IsWindowHovered())
  self.window_stack:push(name)
end

function EditorImpl:end_window()
  self.window_stack:pop()
  imgui.End()
end

function EditorImpl:begin_child(name, x, y, flags)
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

function EditorImpl:end_child()
  imgui.EndChild()
end

function EditorImpl:is_window_focused(name)
  name = name or self.window_stack:peek()
  return self.focus_state[name]
end

function EditorImpl:set_window_focus(name, focus)
  self.focus_state[name] = focus
end

function EditorImpl:is_window_hovered(name)
  name = name or self.window_stack:peek()
  return self.hover_state[name]
end

function EditorImpl:set_window_hover(name, hover)
  self.hover_state[name] = hover
end

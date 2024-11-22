-----------------
-- BUTTON INFO --
-----------------
-- Use this to wrap up all the textures and other configurations a button might need into one
-- struct for use with any GUI button APIs
local ButtonInfo = tdengine.class.define('ButtonInfo')
tdengine.gui.ButtonInfo = ButtonInfo
function ButtonInfo:init(regular, hovered, held, text_inactive, text_active)
  self.regular = regular or tdengine.colors.clear
  self.hovered = hovered or tdengine.colors.red
  self.held = held or tdengine.colors.blue
  self.text_active = text_active or tdengine.colors.white
  self.text_inactive = text_inactive or tdengine.colors.white
end

-----------------------
-- PRIVATE UTILITIES --
-----------------------
local GuiItem = tdengine.class.define('GuiItem')

GuiItem.kinds = {
  quad = 'quad',
  circle = 'circle',
  image = 'image',
  text = 'text',
  line = 'line',
  scissor = 'scissor',
  end_scissor = 'end_scissor'
}

function GuiItem:init(kind)
  self.kind = kind or self.kinds.quad
  self.size = tdengine.vec2(100, 100)

  self.color = tdengine.colors.white

  self.background = ''

  self.image = ''

  self.text = ''
  self.font = ''
  self.wrap = 0
  self.precise = false

  self.position = tdengine.vec2()
end

function GuiItem:add_offset(offset)
  if self.kind == GuiItem.kinds.line then
    self.a = self.a:add(offset)
    self.b = self.b:add(offset)
  else
    self.position = self.position:add(offset)
  end
end

function GuiItem:subtract_offset(offset)
  self:add_offset(offset:scale(-1))
end

-- I usually want two versions of any widget: One that draws in region-space, and one that draws in canvas-space.
-- I want to share the generic initialization and defaults between the two functions, so I put it here.
local function make_quad(sx, sy, color)
  local item = GuiItem:new(GuiItem.kinds.quad)
  item.color = color or tdengine.colors.red
  item.size = tdengine.vec2(sx, sy)

  return item
end

local function make_line(thickness, color)
  local item = GuiItem:new(GuiItem.kinds.line)
  item.color = color
  item.thickness = thickness
  item.a = tdengine.vec2()
  item.b = tdengine.vec2()

  return item
end

local function make_image(image, sx, sy)
  local item = GuiItem:new(GuiItem.kinds.image)
  item.image = image

  if sx == 0 or sy == 0 or not sx or not sy then
    sx, sy = tdengine.sprite_size(image)
  end
  item.size = tdengine.vec2(sx, sy)

  return item
end

local function make_text(text, font, color, wrap, precise)
  local item = GuiItem:new(GuiItem.kinds.text)
  item.text = text
  item.font = font or 'game'
  item.color = color or tdengine.colors.white
  item.wrap = wrap or 0
  item.precise = ternary(precise, true, false)
  item.prepared = tdengine.ffi.prepare_text_ex(item.text, 0, 0, item.font, item.wrap, item.color:to_vec4(), item.precise)

  if item.precise then
    item.size = tdengine.vec2(item.prepared.width, item.prepared.height)
  else
    item.size = tdengine.vec2(item.prepared.width, item.prepared.height_imprecise)
  end

  return item
end


local function setup_animation_interpolation(interpolator, direction)
  local native_resolution = tdengine.window.get_native_resolution()

  local start = tdengine.vec2()
  if direction == tdengine.gui.animation_direction.left then
    start.x = native_resolution.x * -1
  elseif direction == tdengine.gui.animation_direction.right then
    start.x = native_resolution.x
  elseif direction == tdengine.gui.animation_direction.bottom then
    start.y = native_resolution.y * -1
  elseif direction == tdengine.gui.animation_direction.top then
    start.y = native_resolution.y
  end

  interpolator:set_start(start)
  interpolator:set_target(tdengine.vec2())
  interpolator:reset()
end

----------------
-- GUI REGION --
----------------
local GuiRegion = tdengine.class.define('GuiRegion')

tdengine.gui.style_fields = {
  padding_h = 'padding_h',
  padding_v = 'padding_v',
  padding_button = 'padding_button',
  scroll_area_width = 'scroll_area_width',
  scroll_bar_size = 'scroll_bar_size',
  scroller_min_height = 'scroller_min_height',
}

GuiRegion.default_style = {
  [tdengine.gui.style_fields.padding_h] = 10,
  [tdengine.gui.style_fields.padding_v] = 10,
  [tdengine.gui.style_fields.padding_button] = 8,
  [tdengine.gui.style_fields.scroll_area_width] = 20,
  [tdengine.gui.style_fields.scroll_bar_size] = 4,
  [tdengine.gui.style_fields.scroller_min_height] = 40,
}

tdengine.gui.animation_direction = {
  left = 'left',
  right = 'right',
  bottom = 'bottom',
  top = 'top',
}

tdengine.enum.define(
  'MenuDirection',
  {
    Vertical = 0,
    Horizontal = 1,
    Any = 1,
  }
)

GuiRegion.padding = 10

function GuiRegion:init(position, size)
  self.size = size:copy()
  self.content_area = size:copy()
  self:set_position(position)
  self.unanimated_position = position

  self.style = table.deep_copy(self.default_style)

  self.label = ''

  self.point_stack = tdengine.data_types.stack:new()
  self.last_item = tdengine.vec2()

  self.center_next_item = false
  self.scroll_speed = 200

  self.cache_draw_calls = false
  self.cached_draw_list = tdengine.data_types.array:new()
end

function GuiRegion:set_position(position)
  self.position = position:copy()
  self.point = position:copy()
  self.default_point = position:copy()
  self.last_point = tdengine.vec2()
end

function GuiRegion:reset_flags()
  self.center_next_item = false
  self.center_next_item_vertically = false
end

function GuiRegion:advance_point(size)
  if self.frozen_point then return end

  self.last_point = self.point:copy()
  self.last_point.x = self.last_point.x + size.x
  self.last_item = size:copy()
  self.point.x = self.default_point.x

  if self.flipped then
    self.point.y = self.point.y + size.y + self.style[tdengine.gui.style_fields.padding_v]
  else
    self.point.y = self.point.y - size.y - self.style[tdengine.gui.style_fields.padding_v]
  end
end

function GuiRegion:calc_item_position(item_size)
  -- Calculate the position of a new item. A new item starts wherever the current point is, and its position can
  -- be affected by some configuration APIs (right now, center_next_item()). The position is then adjusted by
  -- the region's camera.

  -- @hack: Breaks with anything that is not drawing from the left side of region
  local position = self.point:copy()
  if item_size.x == -1 then
    item_size.x = self.content_area.x
  end

  if item_size.y == -1 then
    item_size.y = self.content_area.y - self.style[tdengine.gui.style_fields.padding_v] * 2
  end

  self:apply_formatting(position, item_size)
  self:apply_camera(position)

  return position
end

function GuiRegion:calc_canvas_item_position(item_position, item_size)
  -- Calculate the position of a new item, except don't start at the point. This is used when you are using the
  -- API canvas-style (i.e. laying out things by hand, like in a node graph). You still want the hand-placed
  -- element to abide by the region's camera, but you don't want the library to do any layout.
  --
  -- You can also think of this as specifying an item's position in the world coordinates of the region.
  item_position = item_position:add(self.position)
  self:apply_camera(item_position)

  return item_position
end

function GuiRegion:apply_formatting(position, item_size)
  -- @hack: When you center something, the point moves farther than just the item because it also needs to
  -- move past the bit at the left it used for centering. advance_point() is written to just advance a
  -- given size and I'm not sure if it's right for it to need to know this stuff...
  if self.center_next_item then
    local offset = (self.content_area.x / 2) - (item_size.x / 2)
    position.x = self.point.x + offset
    if not self.frozen_point then
      self.point.x = self.point.x + offset
    end
  else
    position.x = self.point.x + self.style[tdengine.gui.style_fields.padding_h]
  end


  if self.center_next_item_vertically then
    position.y = self.point.y - (self.content_area.y / 2) + (item_size.y / 2)
  else
    position.y = position.y - self.style[tdengine.gui.style_fields.padding_v]
  end

  if self.flipped then
    position.y = position.y + item_size.y + self.style[tdengine.gui.style_fields.padding_v]
  end
end

function GuiRegion:apply_camera(position)
  if self.is_scroll then
    local scroll_data = tdengine.gui.scroll[self.label]
    position.y = position.y + scroll_data.offset
  end

  if self.is_drag then
    local drag = tdengine.gui.find_drag_data(self.label)
    position.x = position.x + drag.offset.x
    position.y = position.y + drag.offset.y
  end
end

function GuiRegion:find_size_remaining()
  local size_used = self:find_size_used()
  local size_remaining = tdengine.vec2(self.content_area.x - size_used.x,
    self.content_area.y - size_used.y - self.style[tdengine.gui.style_fields.padding_v])
  return size_remaining
end

function GuiRegion:find_size_used()
  local size_used = tdengine.vec2()
  size_used.x = self.point.x - self.position.x
  if self.flipped then
    size_used.y = self.point.y - (self.position.y - self.size.y)
  else
    size_used.y = self.position.y - self.point.y
  end

  return size_used
end

function GuiRegion:is_mouse_inside()
  if not tdengine.input.is_mkb_mode() then return false end
  return tdengine.physics.is_point_inside(self.input:mouse(), self.position, self.size)
end

--
-- GUI LAYOUT
-- This is how you make GUIs in this game. In an entity, create one of these every frame, and then use the API
-- to specify the widgets. There are two APIs:
--   1. A total immediate mode one, which looks just like ImGui
--   2. A still-immediate-but-less-so one, where you specify a flex behavior for the main and cross axis, then
--      spit out widgets, and then the layout code will lay them out
--
-- I think I prefer (1), but I wrote them both while experimenting, so I'm keeping them around. I think there are
-- strengths for both.
--
local Gui = tdengine.class.define('Gui')
tdengine.gui.Gui = Gui

Gui.mouse_state = {
  idle = 'idle',
  hovered = 'hovered',
  pressed = 'pressed',
  held = 'held'
}

function Gui:init()
  self.native_resolution = tdengine.window.get_native_resolution()
  self.scale = table.deep_copy(self.native_resolution)

  self.active_regions = tdengine.data_types.stack:new()
  self.finished_regions = tdengine.data_types.stack:new()
  self.regions = tdengine.data_types.queue:new()

  self.draw_list = tdengine.data_types.array:new()
  self.scissor = tdengine.data_types.stack:new()
  self.layer = tdengine.layers.ui

  -- To animate hiding regions, we cache all draw calls submitted between begin_region()
  -- and end_region()
  self.cache_draw_calls = false
  self.cache_region = nil

  self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.Game)

  self.precise_text = true
end

---
-- REGIONS
--
function Gui:begin_frame()
  local position = tdengine.vec2(0, 1080)
  local size = self.native_resolution:copy()
  self:begin_region_impl(position, size)

  return true
end

function Gui:end_frame()
  self:end_region() -- End the fullscreen region we push by default
  self:render()
end

function Gui:begin_region_px(px, py, sx, sy)
  -- Begin a region at an absolute offset and size from the current parent's top left corner. Unused.
  if not self:ensure_region() then return false end

  local parent = self.active_regions:peek()

  local child_position = tdengine.vec2(px, py)
  child_position = child_position:add(parent.position)
  local child_size = tdengine.vec2(sx, sy)

  return self:begin_region_impl(child_position, child_size)
end

function Gui:begin_region_rel(px, py, sx, sy)
  -- Begin a region at an offset and size relative to the current parent. For example, px = 0.25 and sx = 0.50 would
  -- create a subregion that begins 1/4th of the parent's total width from its left edge and is half as wide as the
  -- parent.
  --
  -- The most common one, because it lets you size things without having to go back and revise exact pixels if you make
  -- something up the chain a little bigger or smaller.
  if not self:ensure_region() then return false end

  local parent = self.active_regions:peek()


  -- The child's position is just the parent's position, plus whatever fraction of the parent's size given by px and py
  local relative_position = tdengine.vec2(px, py)
  local offset = parent.size:pairwise_mult(relative_position)
  offset.y = offset.y * -1
  local child_position = parent.position:add(offset)

  -- The child's size is the fraction of the parent's size given by sx and sy
  local relative_size = tdengine.vec2(sx, sy)
  local child_size = parent.size:pairwise_mult(relative_size)

  return self:begin_region_impl(child_position, child_size)
end

function Gui:begin_region_shrink(shrink_x, shrink_y)
  -- The same as begin_region_rel, except we calculate px and py to ensure that the child is centered
  if not self:ensure_region() then return false end

  local parent = self.active_regions:peek()
  return self:begin_region_shrink_px(parent.size.x * shrink_x, parent.size.y * shrink_y)
end

function Gui:begin_region_shrink_px(sx, sy)
  -- The same as begin_region_shrink, except it takes pixel units instead of fractions (and will therefore not
  -- dynamically adjust if you change the parent's size)
  if not self:ensure_region() then return false end

  local parent = self.active_regions:peek()

  local child_size = tdengine.vec2(sx, sy)
  local child_position = tdengine.vec2(
    parent.position.x + (parent.size.x - child_size.x) / 2,
    parent.position.y - (parent.size.y - child_size.y) / 2
  )

  return self:begin_region_impl(child_position, child_size)
end

function Gui:begin_region_widget(sx, sy)
  -- This one's a little funky; it creates a region that's some fraction of the remaining size of the parent(*), but
  -- it treats it like a widget. That is, it starts the region wherever the current point is in the parent,
  -- and it advances the point by the size of the region. In these ways, the region behaves like a widget,
  -- hence the name.
  local parent = self.active_regions:peek()
  local size_remaining = parent:find_size_remaining()

  local child_size = tdengine.vec2(
    size_remaining.x * sx,
    size_remaining.y * sy - parent.style[tdengine.gui.style_fields.padding_v]
  )

  local child_position = parent:calc_item_position(child_size)

  -- local child_position = tdengine.vec2(
  --   parent.point.x,
  --   parent.point.y - parent.style[tdengine.gui.style_fields.padding_v]
  -- )

  parent:advance_point(child_size)

  return self:begin_region_impl(child_position, child_size)
end

function Gui:begin_region_offset(px, py, sx, sy)
  -- Begin a region at an arbitrary offset and size within the parent region
  local parent = self.active_regions:peek()

  local size = tdengine.vec2(sx, sy)
  local position = tdengine.vec2(px, py)
  position = parent:calc_canvas_item_position(position, size)

  return self:begin_region_impl(position, size, true)
end

function Gui:begin_region_impl(absolute_position, absolute_size)
  -- If any size parameter wasn't specified, take up the entire region provided by the parent
  if absolute_size.y == -1 then
    local parent = self.active_regions:peek()
    absolute_size.y = parent.content_area.y
  end

  if absolute_size.x == -1 then
    local parent = self.active_regions:peek()
    absolute_size.x = parent.content_area.x
  end

  local region = GuiRegion:new(absolute_position, absolute_size)
  self.active_regions:push(region)
  self.regions:push(region)
  --self:draw_region(tdengine.color(0.00, 1.00, 1.00, 0.05))

  return true
end

function Gui:end_region()
  local region = self.active_regions:peek()

  -- If we were caching this region's draw calls to animate it as it hides, finalize that list
  if region.animate_hide then
    local animation = self:find_or_add_animation_data(region)
    animation.hide_data = {
      draw_list = region.cached_draw_list,
    }

    self.cache_draw_calls = false
  end

  self:update_scroll(region)
  self:update_drag(region)
  self:update_scissor(region)

  self.active_regions:pop()
end

function Gui:enable_debug_drawing()
  -- By default, any debug drawing calls will just draw. But, it's really common that you want to sometimes
  -- have the debug view and sometimes not, and it's a pain in the ass to comment out all the draw_region()
  -- calls. We want some function like enable_draw_regions()
  --
  -- But also, if anyone were to use this code, they'd be really confused if their debug regions weren't showing
  -- up because of some esoteric function in the API. So, if you want the ability to toggle debug region
  -- drawing, you do this:
  --
  -- layout:use_debug_drawing()
  -- layout:enable_draw_regions() -- Comment out only this line to toggle
  self.debug_render_enabled = true
end

function Gui:enable_draw_region()
  self.draw_region_enabled = true
end

function Gui:draw_region(color)
  if self.debug_render_enabled and not self.draw_region_enabled and not force then return end
  self:draw_region_force(color)
end

function Gui:draw_region_force(color)
  local region = self.active_regions:peek()

  local item = GuiItem:new(GuiItem.kinds.quad)
  item.position = region.position:copy()
  item.size = region.size:copy()
  item.color = color or tdengine.colors.red_light_trans
  self:push_draw_command(item)
end

function Gui:set_region_label(label)
  local region = self.active_regions:peek()
  region.label = label
end

function Gui:flip_region(label)
  local region = self.active_regions:peek()
  region.flipped = true
  region.point = region.position:copy()
  region.point.y = region.point.y - region.size.y
end

function Gui:animate_region(interpolator, direction)
  local region = self.active_regions:peek()
  local animation = self:find_or_add_animation_data(region, interpolator)

  animation.show_direction = direction

  animation.active = true
  if not animation.was_active then
    -- This is the first frame the animation has played; set everything up
    animation.time_begin = tdengine.elapsed_time
    region.unanimated_position = region.position
    setup_animation_interpolation(animation.interpolator, direction)
  end

  animation.interpolator:update()
  local interp = animation.interpolator:get_value()
  local position = tdengine.vec2(
    region.unanimated_position.x + interp.x,
    region.unanimated_position.y + interp.y)
  region:set_position(position)
end

function Gui:animate_region_hide(direction)
  local region = self.active_regions:peek()

  local animation = self:find_animation_data(region)
  animation.hide_direction = direction or animation.show_direction

  region.animate_hide = true
  region.cache_draw_calls = true
  self.cache_draw_calls = true
  self.cache_region = region
end

function Gui:scroll_region()
  local region = self.active_regions:peek()
  if region.is_scroll then return end

  -- We need to know the size of everything we added to the region, so we can't actually calculate
  -- anything right here (which feels right to me to be called near the begin_region(), like you're
  -- configuring the region you just started).
  --
  -- We do, however, need to adjust the content area (i.e. the area available for the user to draw
  -- widgets) to account for the fact that we're going to draw a scrollbar.
  --
  -- Probably what we want to do is only account for the scroll bar if we actually needed it last frame,
  -- because as is this will incorrectly adjust for the scroll bar even when we don't need one.
  region.content_area.x = region.content_area.x - region.style.scroll_area_width
  region.is_scroll = true
  tdengine.gui.find_scroll_data(region.label)

  -- Scrolling always implies scissoring, so the extra material isn't visible
  self:scissor_region()
end

function Gui:interpolate_scroll_to_percent(percent)
  -- This is hacky. I use this for the dialogue box; when an item is added, I want to smoothly scroll
  -- to the new bottom. However, since the new bottom is only known after that new item is drawn, I have
  -- to defer the calculation for a frame. This marks the region's scroll data to, when it updates
  -- its scroll values, initialize an interpolator that goes from its current offset to this percentage
  -- of the new overage.
  local region = self.active_regions:peek()
  if not region.is_scroll then return end
  if not region.label then return end

  local scroll = tdengine.gui.find_scroll_data(region.label)
  scroll.need_init_interpolate = true
  scroll.interpolate_percent = percent
end

function Gui:show_scroller_at_bottom(percent)
  -- @hack SCROLL_VISUAL_INTERP
  local region = self.active_regions:peek()
  if not region.is_scroll then return end
  if not region.label then return end

  local scroll = tdengine.gui.find_scroll_data(region.label)
  scroll.display_at_bottom = true
end

function Gui:drag_region()
  local region = self.active_regions:peek()
  region.is_drag = true
end

function Gui:scissor_region()
  local region = self.active_regions:peek()
  if region.is_scissor then return end

  region.is_scissor = true

  -- We have to use an indirection here; instead of getting the position and size from the region and
  -- storing it in the draw command, we store the region itself. That's because when animating a region,
  -- we modify the position in place. Except that happens *after* we've called begin_region(), so there'd
  -- be no way to go back, find the draw command to start scissor for that region, and update the position.
  local item = GuiItem:new(GuiItem.kinds.scissor)
  item.region = region
  self:push_draw_command(item)
end

-------------------------------------
-- IMMEDIATE MODE LAYOUT UTILITIES --
-------------------------------------
function Gui:use_precise_text()
  self.precise_text = true
end

function Gui:use_imprecise_text()
  self.precise_text = false
end

function Gui:set_point(px, py)
  local region = self.active_regions:peek()
  region.default_point = tdengine.vec2(px, py)
  region.point = region.default_point:copy()
end

function Gui:freeze_point()
  local region = self.active_regions:peek()
  region.frozen_point = true
end

function Gui:push_point()
  local region = self.active_regions:peek()
  region.pushed_point = region.point:copy()
end

function Gui:pop_point()
  local region = self.active_regions:peek()
  if not region.pushed_point then return end
  region.point = region.pushed_point:copy()
end

function Gui:unfreeze_point()
  local region = self.active_regions:peek()
  region.frozen_point = false
end

function Gui:set_style_field(field, value)
  local region = self.active_regions:peek()
  value = value or region.default_style[field]
  region.style[field] = value
end

function Gui:get_style_field(field)
  local region = self.active_regions:peek()
  return region.style[field]
end

function Gui:disable_padding()
  self:set_style_field(tdengine.gui.style_fields.padding_h, 0)
  self:set_style_field(tdengine.gui.style_fields.padding_v, 0)
end

function Gui:same_line()
  local region = self.active_regions:peek()
  region.point = region.last_point:copy()
  region.point.x = region.point.x + region.style[tdengine.gui.style_fields.padding_h]
  region.point.y = region.point.y
end

function Gui:center_next_item()
  local region = self.active_regions:peek()
  region.center_next_item = true
end

function Gui:center_next_item_vertically()
  local region = self.active_regions:peek()
  region.center_next_item_vertically = true
end

function Gui:is_item_controller_hovered()
  local menu = tdengine.gui.find_last_menu()
  return tdengine.input.is_controller_mode() and menu.current == menu.items_this_frame - 1
end

function Gui:is_next_item_controller_hovered()
  local menu = tdengine.gui.find_last_menu()
  return tdengine.input.is_controller_mode() and menu.current == menu.items_this_frame
end

function Gui:is_item_hovered()
  if tdengine.input.is_controller_mode() then
    return self:is_item_controller_hovered()
  else
    local item = self.draw_list:back()
    local position = item.position
    local size = item.size

    local mouse = self.input:mouse()
    local right = mouse.x > position.x
    local left = mouse.x < position.x + size.x
    local below = mouse.y < position.y
    local above = mouse.y > position.y - size.y
    return right and left and above and below
  end
end

function Gui:get_region_drag()
  local region = self.active_regions:peek()
  if not region.is_drag then return tdengine.vec2() end

  local drag = tdengine.gui.find_drag_data(region.label)
  return drag.offset
end

function Gui:get_region_size()
  local region = self.active_regions:peek()
  return region.size
end

function Gui:get_region_position()
  local region = self.active_regions:peek()
  return region.position
end

function Gui:get_region_available()
  local region = self.active_regions:peek()
  return tdengine.vec2(
    region.content_area.x - region.style[tdengine.gui.style_fields.padding_h] * 2,
    region.content_area.y - region.style[tdengine.gui.style_fields.padding_v] * 2
  )
end

function Gui:get_world_cursor()
  return self.input:mouse()
      :subtract(self:get_region_position())
      :subtract(self:get_region_drag())
end

-------------
-- WIDGETS --
-------------
-- PRIMITIVES
function Gui:quad(sx, sy, color)
  local region = self.active_regions:peek()

  local item = make_quad(sx, sy, color)
  item.position = region:calc_item_position(item.size)

  self:add_regular_item(item)
end

function Gui:canvas_quad(px, py, sx, sy, color)
  local region = self.active_regions:peek()

  local item = make_quad(sx, sy, color)
  item.position = region:calc_canvas_item_position(tdengine.vec2(px, py))

  self:add_canvas_item(item)
end

function Gui:canvas_line(ax, ay, bx, by, thickness, color, debg)
  local region = self.active_regions:peek()

  local item = make_line(thickness, color)
  item.a = region:calc_canvas_item_position(tdengine.vec2(ax, ay))
  item.b = region:calc_canvas_item_position(tdengine.vec2(bx, by))
  item.dbg = true

  self:add_canvas_item(item)
end

-- IMAGE
function Gui:image(image, sx, sy)
  local region = self.active_regions:peek()

  local item = make_image(image, sx, sy)
  item.position = region:calc_item_position(item.size)

  self:add_regular_item(item)
end

function Gui:canvas_image(image, px, py, sx, sy)
  local region = self.active_regions:peek()

  local item = make_image(image, sx, sy)
  item.position = region:calc_canvas_item_position(tdengine.vec2(px, py))

  self:add_canvas_item(item)
end

-- TEXT
function Gui:text(text, font, color, wrap)
  local region = self.active_regions:peek()

  wrap = wrap or
      region.content_area.x - region:find_size_used().x - (region.style[tdengine.gui.style_fields.padding_h] * 2)
  local item = make_text(text, font, color, wrap, self.precise_text)
  item.position = region:calc_item_position(item.size)

  self:add_regular_item(item)
end

function Gui:canvas_text(text, px, py, font, color, wrap)
  local region = self.active_regions:peek()

  wrap = wrap or region.size.x - (region.style[tdengine.gui.style_fields.padding_h] * 2)
  local item = make_text(text, font, color, wrap, self.precise_text)
  item.position = region:calc_canvas_item_position(tdengine.vec2(px, py))

  self:add_canvas_item(item)
end

-- IMAGE BUTTON
function Gui:image_button(label, button_info, sx, sy, font)
  local region = self.active_regions:peek()

  local image = GuiItem:new(GuiItem.kinds.image)
  image.size = tdengine.vec2(sx, sy)
  image.position = region:calc_item_position(image.size)

  return self:image_button_ex(label, button_info, sx, sy, font, image)
end

function Gui:canvas_image_button(label, button_info, px, py, sx, sy, font)
  local region = self.active_regions:peek()

  local image = GuiItem:new(GuiItem.kinds.image)
  image.size = tdengine.vec2(sx, sy)
  image.position = region:calc_canvas_item_position(tdengine.vec2(px, py))

  return self:image_button_ex(label, button_info, sx, sy, font, image)
end

function Gui:image_button_ex(label, button_info, sx, sy, font, image)
  local region = self.active_regions:peek()

  local text = self:prepare_button_text(label, font)
  local state = self:item_mouse_state(image)
  self:check_button_state(image, text, button_info, state)

  self:center_button_text(image, text)
  self:finish_button(image, text)

  return state == self.mouse_state.pressed
end

-- TEXT BUTTON
function Gui:button(text, font)
  return self:button_ex(text, tdengine.gui.ButtonInfo:new(), font)
end

function Gui:button_ex(label, button_info, font, sx, sy)
  local region = self.active_regions:peek()

  local text = self:prepare_button_text(label, font)
  local frame = self:build_button_frame(text.prepared.width, text.prepared.height, sx, sy)
  local state = self:item_mouse_state(frame)
  self:check_button_state(frame, text, button_info, state)

  self:center_button_text(frame, text)
  self:finish_button(frame, text)

  return state == self.mouse_state.pressed
end

-- MENUS
function Gui:begin_menu()
  local region = self.active_regions:peek()

  local i = 1
  while region and region.label == '' do
    region = self.active_regions:peek(i)
    i = i + 1
  end
  if not region then return end

  local menu = tdengine.gui.find_menu_data(region.label)
  menu.label = region.label
  tdengine.gui.last_menu = menu

  local is_menu_stacked = false
  for _, stacked_menu in tdengine.gui.menu_stack:iterate() do
    is_menu_stacked = is_menu_stacked or stacked_menu == menu.label
  end

  if not is_menu_stacked then
    for id, other in pairs(tdengine.gui.menu) do
      other.active = false
    end
    menu.active = true
    tdengine.gui.menu_stack:add(menu.label)
  end
end

function Gui:end_menu()
  local region = self.active_regions:peek()
  local menu = tdengine.gui.find_menu_data(region.label)

  -- Assign like this so the
  menu.size_last_frame:assign(menu.size_this_frame)
  menu.size_this_frame:assign(region:find_size_used()) -- @hack: Region != menu
end

function Gui:scroll_menu()
  local region = self.active_regions:peek()
  local menu = tdengine.gui.find_menu_data(region.label)

  self:scroll_region()

  if tdengine.input.is_controller_mode() then
    region.scroll_locked_to_menu = true

    -- Normally, we update scrolling at the end of the frame, since we need to know the size drawn into the region
    -- to determine the size of the scrollbar. However, when we lock the scroll to the menu, the scroll is entirely
    -- dependent on the selected menu item. We need to synchronize those before we issue draw commands, or there
    -- will be a one frame delay (and therefore popping) when we move through the menu.
    local scroll = tdengine.gui.find_scroll_data(region.label)
    scroll.percent = menu.current / menu.items_last_frame
    scroll.offset = menu.size_last_frame.y * scroll.percent

    if scroll.offset == 26 then
      print(menu.current, menu.items_last_frame, menu.size_last_frame.y)
    end
  end
end

function Gui:set_menu_direction(direction)
  local menu = tdengine.gui.find_last_menu()
  menu.direction = direction
end

function Gui:clear_menu_item()
  local menu = tdengine.gui.find_last_menu()
  menu.current = 0
end

function Gui:menu_item(label, button_info, font, sx, sy)
  local region = self.active_regions:peek()
  local menu = tdengine.gui.find_last_menu()
  if not menu then return self:button_ex(label, button_info, font, sx, sy) end

  local item_index = menu.items_this_frame
  menu.items_this_frame = menu.items_this_frame + 1

  local text = self:prepare_button_text(label, font)
  local frame = self:build_button_frame(text.prepared.width, text.prepared.height, sx, sy)

  local state = self.mouse_state.idle
  if tdengine.input.get_input_device() == tdengine.input.device_kinds.controller then
    local selected = menu.active and tdengine.input.was_digital_pressed('Action_MenuSelect')
    local hovered = menu.active and (item_index == menu.current)

    if hovered and not selected then
      state = self.mouse_state.hovered
    elseif hovered and selected then
      state = self.mouse_state.pressed
    end
  else
    frame.text = label
    state = self:item_mouse_state(frame)
  end

  button_info = button_info or tdengine.gui.ButtonInfo:new()
  self:check_button_state(frame, text, button_info, state)

  self:center_button_text(frame, text)
  self:finish_button(frame, text)

  return state == self.mouse_state.pressed
end

function Gui:image_menu_item(label, button_info, font, sx, sy)
  local region = self.active_regions:peek()
  local menu = tdengine.gui.find_last_menu()

  local item_index = menu.items_this_frame
  menu.items_this_frame = menu.items_this_frame + 1

  local text = self:prepare_button_text(label, font)
  local frame = self:build_button_frame(text.prepared.width, text.prepared.height, sx, sy)
  local image = GuiItem:new(GuiItem.kinds.image)
  image.size = frame.size
  image.position = frame.position

  local state = self.mouse_state.idle
  if tdengine.input.get_input_device() == tdengine.input.device_kinds.controller then
    local selected = menu.active and tdengine.input.was_digital_pressed('Action_MenuSelect')
    local hovered = menu.active and (item_index == menu.current)

    if hovered and not selected then
      state = self.mouse_state.hovered
    elseif hovered and selected then
      state = self.mouse_state.pressed
      self:clear_menu_item()
    end
  else
    state = self:item_mouse_state(image)
  end

  button_info = button_info or tdengine.gui.ButtonInfo:new()
  self:check_button_state(image, text, button_info, state)

  self:center_button_text(image, text)
  self:finish_button(image, text)

  return state == self.mouse_state.pressed
end

-- OTHER WIDGETS
function Gui:background(image, opacity)
  local region = self.active_regions:peek()

  local item = GuiItem:new(GuiItem.kinds.image)
  item.image = image
  item.size = region.size:copy()
  item.position = region.position:copy()
  item.opacity = opacity or 1

  self:push_draw_command(item)
end

function Gui:dummy(sx, sy)
  local region = self.active_regions:peek()
  local size = tdengine.vec2(sx, sy)
  region:advance_point(size)
  region:reset_flags()
end

--------------------
-- INTERNAL: MAIN --
--------------------
function Gui:render()
  tdengine.ffi.set_world_space(false)
  for index, item in self.draw_list:iterate() do
    tdengine.ffi.set_layer(self.layer + index)
    self:draw_item(item)
  end
end

function Gui:draw_item(item)
  if item.kind == GuiItem.kinds.quad then
    tdengine.ffi.draw_quad_l(item.position, item.size, item.color)
  elseif item.kind == GuiItem.kinds.line then
    tdengine.ffi.draw_line(item.a.x, item.a.y, item.b.x, item.b.y, item.thickness, item.color)
  elseif item.kind == GuiItem.kinds.circle then
    tdengine.draw_circle_l(item.position, item.radius, item.color)
  elseif item.kind == GuiItem.kinds.image then
    tdengine.ffi.draw_image_l(item.image, item.position, item)
    --tdengine.ffi.draw_quad_l(item.position, item.size, tdengine.colors.red_light_trans)
  elseif item.kind == GuiItem.kinds.text then
    local highlight_color = tdengine.color(1.00, 0.00, 0.00, 0.50)

    local draw_bounding_box = false
    if draw_bounding_box then
      if item.precise then
        tdengine.ffi.draw_quad_l(item.position, tdengine.vec2(item.prepared.width, item.prepared.height), highlight_color)
      else
        tdengine.ffi.draw_quad_l(item.position, tdengine.vec2(item.prepared.width, item.prepared.height_imprecise),
          highlight_color)
      end
    end

    item.prepared.color = tdengine.color_to_vec4(item.color)
    item.prepared.position.x = item.position.x
    item.prepared.position.y = item.position.y
    tdengine.ffi.draw_prepared_text(item.prepared)
  elseif item.kind == GuiItem.kinds.scissor then
    self.scissor:push(item)
    self:apply_scissor(item)
  elseif item.kind == GuiItem.kinds.end_scissor then
    self.scissor:pop()
    if self.scissor:is_empty() then
      tdengine.ffi.end_scissor()
    else
      local scissor = self.scissor:peek()
      self:apply_scissor(scissor)
    end
  end
end

-------------------------
-- INTERNAL: UTILITIES --
-------------------------
function Gui:push_draw_command(item)
  self.draw_list:add(item)

  if self.cache_draw_calls then
    self.cache_region.cached_draw_list:add(item)
  end
end

function Gui:add_regular_item(item)
  local region = self.active_regions:peek()

  self:push_draw_command(item)
  region:advance_point(item.size)
  region:reset_flags()
end

function Gui:add_canvas_item(item)
  self:push_draw_command(item)
end

function Gui:ensure_region()
  if self.active_regions:size() == 0 then
    log.warn('Gui:begin_region_px(): no parent region; did you forget to begin_frame()?')
    return false
  end

  return true
end

function Gui:apply_scissor(item)
  item.position = item.region.position:copy()
  item.size = item.region.size:copy()
  item.position.y = item.position.y - item.size.y
  tdengine.ffi.begin_scissor(item.position.x, item.position.y, item.size.x, item.size.y)
end

function Gui:item_mouse_state(item)
  local region = self.active_regions:peek()
  if not region:is_mouse_inside() then return self.mouse_state.idle end

  if not item then item = region end

  local inside = tdengine.physics.is_point_inside(self.input:mouse(), item.position, item.size)
  local pressed = self.input:pressed(glfw.keys.MOUSE_BUTTON_1)
  local down = self.input:down(glfw.keys.MOUSE_BUTTON_1)

  if inside and pressed then
    return self.mouse_state.pressed
  elseif inside and down then
    return self.mouse_state.held
  elseif inside then
    return self.mouse_state.hovered
  else
    return self.mouse_state.idle
  end
end

--------------------------------
-- INTERNAL: BUTTON UTILITIES --
--------------------------------
function Gui:prepare_button_text(label, font)
  local region = self.active_regions:peek()
  local wrap = region.content_area.x - region.style[tdengine.gui.style_fields.padding_h] * 2
  return make_text(label, font, tdengine.colors.white, wrap, self.precise_text)
end

function Gui:check_button_state(frame, text, button_info, state)
  if state == self.mouse_state.hovered then
    text.color = button_info.text_active
    if frame.kind == frame.kinds.image then frame.image = button_info.hovered end
    if frame.kind == frame.kinds.quad then frame.color = button_info.hovered end
  elseif state == self.mouse_state.pressed then
    text.color = button_info.text_active
    if frame.kind == frame.kinds.image then frame.image = button_info.held end
    if frame.kind == frame.kinds.quad then frame.color = button_info.held end
  else
    text.color = button_info.text_inactive
    if frame.kind == frame.kinds.image then frame.image = button_info.regular end
    if frame.kind == frame.kinds.quad then frame.color = button_info.regular end
  end
end

function Gui:finish_button(frame, text)
  self:push_draw_command(frame)
  self:push_draw_command(text)

  local region = self.active_regions:peek()
  region:advance_point(frame.size)
  region:reset_flags()
end

function Gui:center_button_text(frame, text)
  local height = text.prepared.height
  if not text.precise then height = text.prepared.height_imprecise end
  text.position = tdengine.vec2(
    frame.position.x + (frame.size.x - text.prepared.width) / 2,
    frame.position.y - (frame.size.y - height) / 2
  )
end

function Gui:build_button_frame(contents_x, contents_y, sx, sy)
  local region = self.active_regions:peek()
  local frame = GuiItem:new(GuiItem.kinds.quad)

  -- Frame size can be specified by the size parameter; if it isn't, just make the frame as large as needed to
  -- hold the contents. -1 signals to use up all space in the respective axis.
  if not sx then
    frame.size.x = contents_x + region.style[tdengine.gui.style_fields.padding_h] * 2
  elseif sx == -1 then
    frame.size.x = region.content_area.x - region.style[tdengine.gui.style_fields.padding_h] * 2
  else
    frame.size.x = sx
  end

  -- I don't support -1 for y because I've never had a use for it yet.
  if not sy then
    frame.size.y = contents_y + region.style[tdengine.gui.style_fields.padding_button] * 2
  else
    frame.size.y = sy
  end

  frame.position = region:calc_item_position(frame.size)

  return frame
end

-----------------------
-- INTERNAL: UPDATES --
-----------------------
function Gui:update_scroll(region)
  if not region.is_scroll then return end
  if not region.label then return end

  local scroll = tdengine.gui.find_scroll_data(region.label)

  -- Figure out how much spaced we used rendering widgets, and compare that to the size of the region to
  -- find how much we went over. This is how we calculate the size and offset of the scrollbar.
  --
  -- Since the paradigm is to always add padding upon rendering the next widget, we add one extra padding into
  -- the "how much size did we use" equation. That just means the bottommost widget will have padding below it
  -- when we're scrolled all the way down
  local size_used = region:find_size_used()
  size_used.y = size_used.y + region.style[tdengine.gui.style_fields.padding_v]
  scroll.overage = size_used.y - region.size.y
  scroll.overage = math.max(scroll.overage, 0)

  -- Adjust the scroll amount in cases where it's calculated. Right now, that either means when it's locked to
  -- the menu item you have selected, or when it's interpolating.
  if region.scroll_locked_to_menu then
    -- CASE: Scrolling is locked to the currently selected menu item
    local menu = tdengine.gui.find_menu_data(region.label)
    scroll.percent = menu.current / menu.items_this_frame
    scroll.offset = scroll.percent * menu.size_this_frame.y
  elseif scroll.need_init_interpolate then
    -- CASE: During the frame, we were told to interpolate the scroll to some value. Set it up.
    scroll.interpolator = tdengine.interpolation.SmoothDamp:new({
      start = scroll.offset,
      target = scroll.overage * scroll.interpolate_percent,
      epsilon = 1,
      velocity = .15
    })
    scroll.need_init_interpolate = false
    --scroll.interpolate_percent = nil
    scroll.interpolating = true
  elseif scroll.interpolating then
    -- CASE: We're actively interpolating.
    scroll.interpolator:update()
    if scroll.interpolator:is_done() then
      scroll.interpolating = false
      scroll.display_at_bottom = false
    end

    scroll.offset = scroll.interpolator:get_value()
    scroll.percent = scroll.offset / scroll.overage
  end

  -- Then, update the scroll amount based on input. When we're locked to the menu, there is no free scrolling.
  local is_scrolling_input_accepted = true
  is_scrolling_input_accepted = is_scrolling_input_accepted and not region.scroll_locked_to_menu

  if is_scrolling_input_accepted then
    -- Calculate a frame offset based on the inputs; this is slightly different per input mode, but we're
    -- calculating the same number in both cases.
    local frame_offset = 0
    if tdengine.input.is_controller_mode() then
      -- CASE: CONTROLLER
      -- @hack: You can see that in the mouse and keyboard code, we actually check to see whether the mouse is inside the
      -- region. I don't have an easy way to ask if a region is active in controller mode. There is no current situation in
      -- the game where I need more than two scroll regions on screen at once, so I'm just leaving it for now.
      if tdengine.input.is_digital_active('Action_MenuScrollDown') then
        frame_offset = region.scroll_speed
      elseif tdengine.input.is_digital_active('Action_MenuScrollUp') then
        frame_offset = region.scroll_speed * -1
      end
    elseif tdengine.input.is_mkb_mode() then
      -- CASE: MKB
      if region:is_mouse_inside() then
        frame_offset = self.input:scroll().y * region.scroll_speed * -1
      end
    end

    -- Now, if there was some new scrolling input, apply it. We always interpolate scrolling, so we're just checking
    -- if we're already interpolating or if we need to start fresh.
    if math.abs(frame_offset) > 0 then
      -- You shouldn't be able to scroll beyond the farthest rendered widget, which is given by the overage
      local offset = tdengine.math.clamp(scroll.offset + frame_offset, 0, scroll.overage)
      local interpolate_percent = scroll.overage > 0 and offset / scroll.overage or 0

      if scroll.interpolating then
        scroll.interpolator:set_start(scroll.interpolator:get_value())
        scroll.interpolator:set_target(offset)
      else
        scroll.need_init_interpolate = true
        scroll.interpolate_percent = interpolate_percent
      end
    end
  end

  -- If there is an overage (i.e. we drew more widgets than could fit in the region), render a scrollbar
  if scroll.overage > 0 then
    -- The bar itself is directly to the right of the content area, and is as tall as the entire region
    local scroll_bar = GuiItem:new(GuiItem.kinds.image)
    scroll_bar.image = 'scroll_bar_long.png'
    scroll_bar.size = tdengine.vec2(region.style.scroll_bar_size, region.size.y)
    scroll_bar.position = tdengine.vec2(
      region.position.x + region.content_area.x + ((region.style.scroll_area_width - region.style.scroll_bar_size) / 2),
      region.position.y
    )

    self:push_draw_command(scroll_bar)

    local scroller = GuiItem:new(GuiItem.kinds.image)
    scroller.image = 'circle-16.png'
    scroller.size = tdengine.vec2(16, 16)

    -- All we're doing is taking the percentage we're scrolled (scroll.offset / overage) and projecting that
    -- onto the range of values the scroll bar position can take i.e. [0, scroll_bar.size.y - scroller.size.y]
    local max_scroller_offset = scroll_bar.size.y - scroller.size.y
    scroller.position = tdengine.vec2(
      scroll_bar.position.x - (scroller.size.x / 2) + (region.style.scroll_bar_size / 2),
      scroll_bar.position.y - scroll.percent * max_scroller_offset
    )

    -- @hack SCROLL_VISUAL_INTERP
    if scroll.display_at_bottom then
      scroller.position.y = scroll_bar.position.y - max_scroller_offset
    end

    self:push_draw_command(scroller)
  end
end

function Gui:update_drag(region)
  if not region.is_drag then return end
  if not region.label then return end

  local data = tdengine.gui.find_drag_data(region.label)

  local delta = tdengine.vec2()
  if tdengine.input.is_controller_mode() then
    if tdengine.input.is_digital_active('Action_MenuScrollDown') then
      delta.y = region.scroll_speed + 4
    elseif tdengine.input.is_digital_active('Action_MenuScrollUp') then
      delta.y = (region.scroll_speed + 4) * -1
    end

    if tdengine.input.is_digital_active('Action_MenuScrollLeft') then
      delta.x = region.scroll_speed + 4
    elseif tdengine.input.is_digital_active('Action_MenuScrollRight') then
      delta.x = (region.scroll_speed + 4) * -1
    end
  elseif tdengine.input.is_mkb_mode() then
    local inside = tdengine.physics.is_point_inside(self.input:mouse(), region.position, region.size)
    local mouse_down = self.input:down(glfw.keys.MOUSE_BUTTON_1)
    if inside and mouse_down then
      delta = self.input:mouse_delta()
    end
  end
  data.offset:assign(data.offset:add(delta))
end

function Gui:update_scissor(region)
  if not region.is_scissor then return end

  local item = GuiItem:new(GuiItem.kinds.end_scissor)
  self:push_draw_command(item)
end

local function update_animations()
  for label, animation in pairs(tdengine.gui.animation) do
    local stopped_this_frame = animation.was_active and not animation.active
    local animate_hide = not animation.active and animation.hide_data

    animation.was_active = animation.active
    animation.active = false

    -- PLAY HIDE ANIMATION
    if animate_hide then
      -- UPDATE THE INTERPOLATION
      if stopped_this_frame then
        setup_animation_interpolation(animation.interpolator, animation.hide_direction)
        animation.interpolator:reverse()
        animation.interpolator:reset()
      end

      animation.interpolator:update()
      if animation.interpolator:is_done() then
        animation.animate_hide = nil
        animation.hide_data = nil
        return
      end

      -- REBUILD PREPARED TEXT
      -- prepare_text() returns a pointer to temporary storage. But here, we want to cache a list of draw calls
      -- and redraw them across many frames. That leaves us with two options:
      -- 1. (VERY HARD) Figure out a way to allocate prepared text such that it survives across frame boundaries
      --    but isn't memory leaked when were done with it.
      -- 2. (VERY EASY) Assume, as always, that any pointer to prepared text is invalid and re-prepare the text
      --    each frame. This is """"slow""" but dead simple in every possible way. (Also it's not slow)
      for index, item in animation.hide_data.draw_list:iterate() do
        if item.kind == GuiItem.kinds.text then
          item.prepared = tdengine.ffi.prepare_text_ex(item.text, 0, 0, item.font, item.wrap,
            tdengine.color_to_vec4(item.color), item.precise)
        end
      end

      -- RENDER CACHED DRAW LIST AT INTERPOLATED POSITION
      -- We're interpolating the region's position from where it was when it was last rendered to its position
      -- offscreen. The offset is what's being interpolated.
      local offset = animation.interpolator:get_value()

      -- Rendering a draw list requires a little state for scissor regions, so make a dummy layout instance. Otherwise,
      -- this is copy and pasted from Gui:render() (except applying the offset)
      local layout = tdengine.gui.Gui:new()

      tdengine.ffi.end_world_space(true)()
      for index, item in animation.hide_data.draw_list:iterate() do
        tdengine.ffi.set_layer(tdengine.layers.ui + index)

        item:add_offset(offset)
        layout:draw_item(item)
        item:subtract_offset(offset)
      end
    end
  end
end

local function update_menus()
  -- Reset the menu state for each region
  for id, menu in pairs(tdengine.gui.menu) do
    if menu.items_this_frame == 0 then
      menu.active = false
      tdengine.gui.menu_stack:remove_value(menu.label)
    end

    menu.items_last_frame = menu.items_this_frame
    menu.items_this_frame = 0
  end


  -- Update the menu state based on this frame's inputs
  local menu = tdengine.gui.find_active_menu()

  if menu and menu.items_last_frame > 0 then
    local menu_item_forward = function() menu.current = (menu.current + 1) % menu.items_last_frame end
    local menu_item_backward = function() menu.current = (menu.current - 1) % menu.items_last_frame end

    local direction = menu.direction or tdengine.enums.MenuDirection.Vertical
    local is_vertical = direction == tdengine.enums.MenuDirection.Vertical or
        direction == tdengine.enums.MenuDirection.Any
    local is_horizontal = direction == tdengine.enums.MenuDirection.Horizontal or
        direction == tdengine.enums.MenuDirection.Any

    if is_vertical then
      if tdengine.input.was_digital_pressed('Action_MenuUp') then
        menu_item_backward()
      elseif tdengine.input.was_digital_pressed('Action_MenuDown') then
        menu_item_forward()
      end
    end
    if is_horizontal then
      if tdengine.input.was_digital_pressed('Action_MenuLeft') then
        menu_item_backward()
      elseif tdengine.input.was_digital_pressed('Action_MenuRight') then
        menu_item_forward()
      end
    end
  end
end



-----------------------
-- INTERNAL: BUFFERS --
-----------------------
function Gui:find_or_add_animation_data(region, interpolator)
  if region.label == '' then
    log.warn(
      'Tried to animate region, but no label was set; call set_region_label() before animating, so we can uniquely identify the region across frames')
    return
  end

  local data = tdengine.gui.animation[region.label]

  -- If there is no entry, add one
  if not data or not data.interpolator then
    interpolator = interpolator or tdengine.interpolation.SmoothDamp2:new()

    tdengine.gui.animation[region.label] = {
      time_begin = tdengine.elapsed_time,
      active = false,
      was_active = false,
      interpolator = interpolator
    }
  end


  return tdengine.gui.animation[region.label]
end

function Gui:find_animation_data(region)
  if region.label == '' then return end

  return tdengine.gui.animation[region.label]
end

function tdengine.gui.find_drag_data(label)
  local data = tdengine.gui.drag[label]
  if not data then
    tdengine.gui.drag[label] = {
      offset = tdengine.vec2()
    }
  end

  return tdengine.gui.drag[label]
end

function tdengine.gui.find_scroll_data(label)
  local data = tdengine.gui.scroll[label]
  if not data then
    tdengine.gui.scroll[label] = {
      percent = 0,
      offset = 0
    }
  end

  return tdengine.gui.scroll[label]
end

function tdengine.gui.find_menu_data(label)
  local data = tdengine.gui.menu[label]
  if not data then
    tdengine.gui.menu[label] = {
      size_this_frame = tdengine.vec2(),
      size_last_frame = tdengine.vec2(),
      current = 0,
      items_this_frame = 0,
      items_last_frame = 0,
    }
  end

  return tdengine.gui.menu[label]
end

function tdengine.gui.find_last_menu()
  return tdengine.gui.last_menu
  --return tdengine.gui.menu[tdengine.gui.menu_stack:back()]
end

function tdengine.gui.find_active_menu()
  for id, menu in pairs(tdengine.gui.menu) do
    if menu.active then return menu end
  end

  local menu = tdengine.gui.find_last_menu()
  if menu then
    menu.active = true
    return menu
  end

  return nil
end

--------------------------
-- INTERNAL: HIGH LEVEL --
--------------------------
function tdengine.gui.init()
  tdengine.gui.menu_stack = tdengine.data_types.array:new()
end

function tdengine.gui.update()
  update_animations()
  update_menus()
end

function tdengine.gui.reset()
  table.clear(tdengine.gui.animation)
  table.clear(tdengine.gui.menu)
  table.clear(tdengine.gui.drag)
  table.clear(tdengine.gui.scroll)
  tdengine.gui.menu_stack = tdengine.data_types.array:new()
end

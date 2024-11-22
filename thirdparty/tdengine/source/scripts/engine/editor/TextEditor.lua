local editor_state = {
  idle = 'idle',
  dragging = 'dragging',
  selection = 'selection',
}


local TextEditor = tdengine.editor.define('TextEditor')
TextEditor.editor_actions = {
  insert = 'insert',
  delete = 'delete'
}

TextEditor.editor_fields = {
  'size',
}

function TextEditor:init(params)
  params = params or {}
  self.size = params.size or tdengine.vec2(0, 0)
  self.text = params.text or nil
  self.point = 0
  self.line_breaks = {}
  self.count_lines = 0

  self.is_input_dirty = false
  self.last_action = nil

  self.show_grid = false

  self.frame = 0
  self.blink_speed = 20
  self.blink_acc = 0
  self.blink = true

  self.repeat_delay = .3
  self.repeat_time = {}

  self.time_since_click = 0
  self.selection_delay = 8 / 60
  self.selection_begin = 0
  self.selection_end = 0
  self.selection_color = tdengine.colors.spring_green:to_u32()

  self.max_chars_per_line = nil

  self.state = editor_state.idle

  -- @hack
  -- Man, I'll be honest, I forgot how fucked the text editor is. I mean, the code actually is readable,
  -- but figuring out how big shit is is just so nasty. The engine tells me that the width of a character in
  -- the editor font is 8 pixels, which seems reasonable, but it is inaccurate.
  --
  -- I think what's going on is that I used to, for ImGui fonts, check with ImGui itself to see what the width
  -- was, but I did not want to bother with that after I rewrote stuff. However, it looks like there is a small
  -- but meaningful difference b/w what ImGui thinks the width of a character is and what the engine thinks.
  --
  -- So I just hardcoded 7.63 as I remember seeing this value before & it makes the cursor right :^). Ditto for
  -- 20, except here I actually used to call GetTextLineHeightWithSpacing(), except that returned 4.
  self.character_size = tdengine.vec2(7.63, 20)

	self.input = ContextualInput:new(tdengine.enums.InputContext.Editor, tdengine.enums.CoordinateSystem.Game)

  self.imgui_ignore = {
    last_action = true
  }
end

------------
-- UPDATE --
------------
function TextEditor:update()
  if tdengine.editor.find('DialogueEditor').hidden then return end

  tdengine.editor.begin_window('Dialogue Node Text')

  self.frame = self.frame + 1
  self.time_since_click = self.time_since_click + tdengine.dt
  self.last_action = nil

  if not self.text then
    tdengine.editor.end_window()
    return
  end

  -- Calculate how many characters a line can hold -- after we
  -- begin the content region
  if not self.max_chars_per_line then
    self.max_chars_per_line = calc_max_chars_per_line(self)
  end

  if self.is_input_dirty then
    update_line_breaks(self)
    self.is_input_dirty = false
  end

  local focus = tdengine.editor.is_window_focused()
  local hover = tdengine.editor.is_window_hovered()

  if self.state == editor_state.idle then
    for code = glfw.keys.SPACE, glfw.keys.GRAVE_ACCENT, 1 do
      self:handle_check_repeat(tdengine.dt, code, self.handle_alpha, self, string.char(code))
      if code == glfw.keys.GRAVE_ACCENT then break end
    end

    self:handle_check_repeat(tdengine.dt, glfw.keys.BACKSPACE, self.handle_backspace, self)
    self:handle_check_repeat(tdengine.dt, glfw.keys.LEFT, self.handle_left_arrow, self)
    self:handle_check_repeat(tdengine.dt, glfw.keys.RIGHT, self.handle_right_arrow, self)
    self:handle_check_repeat(tdengine.dt, glfw.keys.DOWN, self.handle_down_arrow, self)
    self:handle_check_repeat(tdengine.dt, glfw.keys.UP, self.handle_up_arrow, self)

    if hover then handle_click(self) end

    -- Do this before we render the blinker, so the line size is accurate
    if self.is_input_dirty then
      update_line_breaks(self)
      self.is_input_dirty = false
    end

    update_blink(self)
  elseif self.state == editor_state.dragging then
    if focus or hover then
      self.selection_end = screen_to_point(self, tdengine.vec2(imgui.GetMousePos()))
    end

    if self.selection_begin == self.selection_end then
      draw_blink(self, self.selection_begin)
    else
      draw_selection(self, self.selection_begin, self.selection_end, self.selection_color)
    end

    -- Mouse is up, we're done dragging. Every click starts as a drag. We decide it's not IF:
    --  1. The selection is one character
    --  2. The mouse was pressed for less than self.selection_delay
    if not self.input:down(glfw.keys.MOUSE_BUTTON_1) then
      if self.selection_begin == self.selection_end then
        self.state = editor_state.idle
      elseif self.time_since_click > self.selection_delay then
        ensure_selection_order(self)
        self.state = editor_state.selection
      else
        self.state = editor_state.idle
      end
    end
  elseif self.state == editor_state.selection then
    if hover then handle_click(self) end

    if self.selection_begin == self.selection_end then
      draw_blink(self, self.selection_begin)
    else
      draw_selection(self, self.selection_begin, self.selection_end, self.selection_color)
    end
  end

  if self.show_grid then show_grid(self) end

  -- Draw the text
  for i = 1, (#self.line_breaks - 1) do
    local low = self.line_breaks[i]
    local high = self.line_breaks[i + 1] - 1
    if high > #self.text then dbg() end
    imgui.Text(self.text:sub(low, high))
  end

  tdengine.editor.end_window()
end

---------------
-- UTILITIES --
---------------
function TextEditor:set_text(text)
  self.text = text
  self.point = 1
  self.line_breaks = {}
  self.is_input_dirty = true
  self.selection_begin = 1
  self.selection_end = 2
end

--------------------
-- INPUT HANDLING --
--------------------
function TextEditor:handle_check_repeat(dt, key, f, ...)
  -- Always do it on the first press
  if self.input:pressed(key) then
    self.repeat_time[key] = self.repeat_delay
    f(...)
    return
  end

  -- Otherwise, check if we've hit the repeat threshold
  if self.input:down(key) then
    -- @hack: self.repeat_time[key] is nil occasionally when pressing a chord
    if not self.repeat_time[key] then self.repeat_time[key] = self.repeat_delay end
    self.repeat_time[key] = self.repeat_time[key] - dt
    if self.repeat_time[key] <= 0 then f(...) end
  end
end

function TextEditor:handle_enter()
  -- Insert: Point tells us the index of the newly inserted character
  -- e.g. if point = 6, then text[6] = c
  self.last_action = {
    kind = self.editor_actions.insert,
    index = self.point
  }

  self.is_input_dirty = true
  -- The point defines where the next character will go. For example, if the point is at 1, then
  -- the next character inserted will be at index 1.
  self.text = self.text:sub(1, self.point - 1) .. '\n' .. self.text:sub(self.point, #self.text)
  self:move_cursor_right()
end

function TextEditor:handle_backspace()
  -- If the point is N, then we will delete character N - 1
  if self.point == 1 then return end

  -- Index tells us which character was deleted
  -- e.g. if point = 6, then text[5] was removed
  self.last_action = {
    kind = self.editor_actions.delete,
    index = self.point
  }
  self.is_input_dirty = true

  -- Minus two because (A) we want the character before point and (B) sub is inclusive
  -- For example: point = 4 -> we want to delete character 3 -> substring [1, 2]
  local before = self.text:sub(1, self.point - 2)
  if self.point == #self.text then
    self.text = before
    self:move_cursor_left()
    return
  end

  local after = self.text:sub(self.point, #self.text)
  self.text = before .. after
  self:move_cursor_left()
end

function TextEditor:handle_left_arrow()
  self:move_cursor_left()
end

function TextEditor:handle_right_arrow()
  self:move_cursor_right()
end

function TextEditor:handle_down_arrow()
  local offset = offset_into_line(self, self.point)
  local line = point_to_line(self, self.point)

  -- Down on the bottom line moves to the end of the line
  if line == #self.line_breaks - 1 then
    self.point = #self.text + 1
    return
  end

  -- Otherwise, move into the same offset one line down
  local next_line_start = self.line_breaks[line + 1]
  self.point = math.min(#self.text + 1, next_line_start + offset)
end

function TextEditor:handle_up_arrow()
  local offset = offset_into_line(self, self.point)
  local line = point_to_line(self, self.point)

  if line == 1 then
    self.point = 1; return
  end

  local prev_line_start = self.line_breaks[line - 1]
  self.point = math.max(1, prev_line_start + offset)
end

function TextEditor:handle_alpha(c)
  if self.input:down(glfw.keys.LEFT_CONTROL) then return end
  if self.input:down(glfw.keys.RIGHT_CONTROL) then return end

  -- Insert: Point tells us the index of the newly inserted character
  -- e.g. if point = 6, then text[6] = c
  self.last_action = {
    kind = self.editor_actions.insert,
    index = self.point
  }

  self.is_input_dirty = true
  local unshifted_byte = string.byte(c);
  local shifted_byte = tdengine.ffi.shift_key(unshifted_byte)
  local c = string.char(shifted_byte)
  -- The point defines where the next character will go. For example, if the point is at 1, then
  -- the next character inserted will be at index 1.
  self.text = self.text:sub(1, self.point - 1) .. c .. self.text:sub(self.point, #self.text)
  self:move_cursor_right()
end

function handle_click(editor)
  if imgui.IsMouseClicked(0) then
    editor.point = screen_to_point(editor, tdengine.vec2(imgui.GetMousePos()))
    editor.selection_begin = editor.point
    editor.selection_end = editor.point
    editor.time_since_click = 0
    editor.state = editor_state.dragging
  end
end

--------------
-- MOVEMENT --
--------------
function TextEditor:move_cursor_right()
  self.point = math.min(self.point + 1, #self.text + 1)
end

function TextEditor:move_cursor_left()
  self.point = math.max(self.point - 1, 1)
end

----------------------------------------
-- COORDINATES, IN THAT WEIRD C STYLE --
----------------------------------------
function point_to_line(editor, point)
  for line = 1, #editor.line_breaks - 1 do
    local size = line_size(editor, line)
    if point <= size then return line end
    point = point - size
  end
  return #editor.line_breaks - 1
end

function screen_to_line(editor, screen)
  return math.floor(screen.y / editor.character_size.y)
end

function point_to_screen(editor, point)
  local screen = tdengine.vec2(imgui.GetCursorScreenPos())

  local max_length, max_height = imgui.GetContentRegionAvail()

  local remaining = point - 1
  for i = 1, #editor.line_breaks - 1 do
    local line_size = editor.line_breaks[i + 1] - editor.line_breaks[i]
    if remaining == line_size then
      if line_size ~= editor.max_chars_per_line then
        -- Case 1: We're on the end of the line, but the line is not full.
        -- The screen position is the end of this line
        screen.x = screen.x + editor.character_size.x * (remaining)
        return screen
      else
        -- Case 1: We're on the end of the line, and also the line is full.
        -- The screen position is the start of the next line
        screen.y = screen.y + editor.character_size.y
        return screen
      end
    elseif remaining > line_size then
      -- Case 2: The total is not on this line. Just advance downward,
      -- removing however many characters this line has
      remaining = remaining - line_size
      screen.y = screen.y + editor.character_size.y
    elseif remaining < line_size then
      -- Case 2: Same as above, but must return. Special case needed to avoid overflowing line break array
      screen.x = screen.x + editor.character_size.x * (remaining)
      return screen
    end
  end

  return screen
end

function screen_to_point(editor, screen)
  if #editor.text == 0 then return 1 end
  if #editor.text == 1 then return 1 end

  -- Find the coordinates relative to the beginning of the text editor window
  local screen_begin = tdengine.vec2(imgui.GetCursorScreenPos())
  local window_coordinates = screen:subtract(screen_begin)
  window_coordinates = window_coordinates:clampl(0)

  local point = 1

  -- Advance by whole lines, but not past the last line
  local line = screen_to_line(editor, window_coordinates)
  line = math.min(line, editor.count_lines - 1)
  point = point + count_chars_for_lines(editor, line)

  -- Advance inside the line you clicked on
  local x = math.floor(window_coordinates.x / editor.character_size.x)
  x = math.min(x, line_size(editor, line + 1))
  point = point + x

  return point
end

function offset_into_line(editor, point)
  local line = point_to_line(editor, point)
  local line_begin = editor.line_breaks[line]
  return point - line_begin
end

function count_chars_for_lines(editor, line_max)
  local count = 0
  for i = 0, line_max - 1, 1 do
    local line_size = editor.line_breaks[i + 2] - editor.line_breaks[i + 1]
    count = count + line_size
  end
  return count
end

function line_size(editor, line)
  return editor.line_breaks[line + 1] - editor.line_breaks[line]
end

function update_line_breaks(editor)
  editor.line_breaks = {}

  local point = 1
  local max_length, max_height = imgui.GetContentRegionAvail()
  while point <= #editor.text do
    table.insert(editor.line_breaks, point)
    point = point + editor.max_chars_per_line
  end
  table.insert(editor.line_breaks, #editor.text + 1)

  editor.count_lines = #editor.line_breaks - 1
end

function show_grid(editor)
  local line_color = tdengine.color32(255, 40, 200, 100)
  local canvas = tdengine.vec2(imgui.GetCursorScreenPos())
  local window = tdengine.vec2(imgui.GetWindowSize())
  for x = 0, window.x, editor.character_size.x do
    local top = tdengine.vec2(x, 0):add(canvas)
    local bottom = tdengine.vec2(x, window.y):add(canvas)
    imgui.GetWindowDrawList():AddLine(top.x, top.y, bottom.x, bottom.y, line_color)
  end
  for y = 0, window.y, editor.character_size.y do
    local top = tdengine.vec2(0, y):add(canvas)
    local bottom = tdengine.vec2(window.x, y):add(canvas)
    imgui.GetWindowDrawList():AddLine(top.x, top.y, bottom.x, bottom.y, line_color)
  end
end

function adjust_cursor_height(top_left, dim)
  top_left.y = top_left.y + 2
  dim.y = dim.y - 4
end

function update_blink(editor)
  editor.blink_acc = editor.blink_acc + 1
  if editor.blink_acc == editor.blink_speed then
    editor.blink = not editor.blink
    editor.blink_acc = 0
  end

  if editor.blink then
    draw_blink(editor, point)
  end
end

function draw_blink(editor, point)
  local tl = point_to_screen(editor, editor.point)
  local dim = tdengine.vec2(8, 20)
  adjust_cursor_height(tl, dim)
  imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(tl.x, tl.y), imgui.ImVec2(tl.x + dim.x, tl.y + dim.y), editor.selection_color)
end

function draw_selection(editor, selection_begin, selection_end, color)
  local chx = editor.character_size.x
  local chy = editor.character_size.y

  local hl_areas = {}
  local line_min = point_to_line(editor, selection_begin)
  local line_max = point_to_line(editor, selection_end)
  local line_max = math.min(line_max, #editor.line_breaks - 1)
  local line_begin = selection_begin
  for line = line_min, line_max do
    local line_end
    if line == line_max then
      line_end = selection_end
    else
      line_end = editor.line_breaks[line + 1]
    end

    local count = line_end - line_begin

    local hl_area = {
      top_left = point_to_screen(editor, line_begin),
      dim = tdengine.vec2(count * chx, chy)
    }
    table.insert(hl_areas, hl_area)

    line_begin = line_end
  end

  for i, hl in pairs(hl_areas) do
    adjust_cursor_height(hl.top_left, hl.dim)
    imgui.GetWindowDrawList():AddRectFilled(imgui.ImVec2(hl.top_left.x, hl.top_left.y), imgui.ImVec2(hl.top_left.x + hl.dim.x), hl.top_left.y + hl.dim.y, color)
  end
end

function ensure_selection_order(editor)
  if editor.selection_begin > editor.selection_end then
    local temp = editor.selection_begin
    editor.selection_begin = editor.selection_end
    editor.selection_end = temp
  end
end

function calc_max_chars_per_line(editor)
  local point = 0
  local max_length, max_height = imgui.GetContentRegionAvail()
  for i = 1, 1000 do
    point = point + editor.character_size.x

    local cstart = point + editor.character_size.x
    local cend = point + (editor.character_size.x * 2)
    if (cend > max_length) then
      return i - 1
    end
  end

  return 50
end

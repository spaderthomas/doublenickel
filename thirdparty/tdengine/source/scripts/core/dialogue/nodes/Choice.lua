local Choice = tdengine.node('Choice')

Choice.editor_fields = {
  'text',
  'only_show_once',
  'once_ever',
  'unlock',
  'mute'
}

Choice.imgui_ignore = {
  shown = true,
  unlock = true,
  roll = true,
  input = true,
  history = true
}

function Choice:init()
  self.text = ''
  self.only_show_once = true
  self.once_ever = false
  self.shown = false
  self.mute = false

  self.unlock = {}
  self.unlock.combinator = tdengine.branch_combinators.op_and
  self.unlock.branches = {}

  self.input = ContextualInput:new()

  self.history = tdengine.dialogue.history
end

----------------
-- PROCESSING --
----------------
function Choice:advance(graph)
  self.shown = true
end

function Choice:process()
  return dialogue_state.advancing
end

-----------
-- INPUT --
-----------
function Choice:handle_input()
  local up = false
  up = up or self.input:pressed(glfw.keys.UP)
  up = up or self.input:pressed(glfw.keys.W)
  up = up or self.input:pressed(glfw.keys.I)
  if up then
    self.hovered = math.max(self.hovered - 1, 1)
  end

  local down = false
  down = down or self.input:pressed(glfw.keys.DOWN)
  down = down or self.input:pressed(glfw.keys.S)
  down = down or self.input:pressed(glfw.keys.K)
  if down then
    self.hovered = math.min(self.hovered + 1, #self.choices)
  end
end

function Choice:short_text()
  local max_size = 16
  if string.len(self.text) < max_size then
    return string.sub(self.text, 0, max_size)
  else
    return string.sub(self.text, 0, max_size - 3) .. '...'
  end
end

function Choice:get_combinator()
  return tdengine.branch_combinators.op_and
end

function Choice:has_unlock_condition()
  return #self.unlock > 0
end
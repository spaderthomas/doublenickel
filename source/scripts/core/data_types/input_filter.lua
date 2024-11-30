InputFilter = tdengine.class.define('InputFilter')

function InputFilter:init()
  self.input = ContextualInput:new(tdengine.enums.InputContext.Editor)

  self.buffer = ''
  self.backspace = InputBuffer:new()
  self.backspace:set_delay(.3)
  self.backspace:set_speed(.03)
  self.backspace:set_key(glfw.keys.BACKSPACE)
  self.backspace:set_callback(function() self:do_backspace() end)
end

function InputFilter:update()
  for code = glfw.keys.SPACE, glfw.keys.Z, 1 do
    if self.input:pressed(code) then
      local c = string.char(code)
      self.buffer = self.buffer .. c
    end
  end

  self.backspace:update()
end

function InputFilter:do_backspace()
  self.buffer = self.buffer:sub(1, #self.buffer - 1)
end

function InputFilter:pass(str)
  return string.find(string.lower(str), string.lower(self.buffer))
end

function InputFilter:clear(str)
  self.buffer = ''
end

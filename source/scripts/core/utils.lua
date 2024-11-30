function tdengine.platform()
  local separator = package.config:sub(1, 1)
  if separator == '\\' then
    return 'Windows'
  elseif separator == '/' then
    return 'Unix'
  else
    return ''
  end
end

function split_ex(str, separator)
  output = {}

  for match in string.gmatch(str, separator) do
    table.insert(output, match)
  end

  return output
end

function split(str, separator)
  return split_ex(str, "([^" .. separator .. "]+)")
end

function string.split(str, separator)
  return split_ex(str, "([^" .. separator .. "]+)")
end

function string.split_ex(str, separator)
  return split_ex(str, separator)
end

KeyRepeat = tdengine.class.define('KeyRepeat')
function KeyRepeat:init(params)
  self.repeat_time = {}
  self.repeat_delay = params.repeat_delay
end

function KeyRepeat:check(key)
  -- Always do it on the first press
  if tdengine.input.pressed(key) then
    self.repeat_time[key] = self.repeat_delay
    return true
  end

  -- Otherwise, check if we've hit the repeat threshold
  if tdengine.input.down(key) then
    -- @hack: self.repeat_time[key] is nil occasionally when pressing a chord
    if not self.repeat_time[key] then self.repeat_time[key] = self.repeat_delay end
    self.repeat_time[key] = self.repeat_time[key] - tdengine.dt
    if self.repeat_time[key] <= 0 then
      return true
    end
  end

  return false
end

function KeyRepeat:check_fn(key, f, ...)
  if self:check(key) then
    f(...)
    return true
  end

  return false
end


function tdengine.uuid()
  local random = math.random
  local template = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'
  local sub = function(c)
    local v = (c == 'x') and random(0, 0xf) or random(8, 0xb)
    return string.format('%x', v)
  end
  return string.gsub(template, '[xy]', sub)
end

function tdengine.uuid_imgui()
  local random = math.random
  local template = '##xxxxxxxx-xxxx-4xxx-yxxx'
  local sub = function(c)
    local v = (c == 'x') and random(0, 0xf) or random(8, 0xb)
    return string.format('%x', v)
  end

  local subbed = string.gsub(template, '[xy]', sub)
  return subbed
end

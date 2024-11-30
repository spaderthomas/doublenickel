function tdengine.callback.register(name, fn)
  tdengine.callback.data[name] = fn
end

function tdengine.callback.find(name)
  return tdengine.callback.data[name]
end

function tdengine.callback.run(name, ...)
  local fn = tdengine.callback.data[name]
  if fn then
    local value = fn(...)
    return value
  end
end

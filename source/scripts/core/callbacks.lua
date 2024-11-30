function doublenickel.callback.register(name, fn)
  doublenickel.callback.data[name] = fn
end

function doublenickel.callback.find(name)
  return doublenickel.callback.data[name]
end

function doublenickel.callback.run(name, ...)
  local fn = doublenickel.callback.data[name]
  if fn then
    local value = fn(...)
    return value
  end
end

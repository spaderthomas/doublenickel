function dn.callback.register(name, fn)
  dn.callback.data[name] = fn
end

function dn.callback.find(name)
  return dn.callback.data[name]
end

function dn.callback.run(name, ...)
  local fn = dn.callback.data[name]
  if fn then
    local value = fn(...)
    return value
  end
end

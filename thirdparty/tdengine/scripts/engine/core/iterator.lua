function tdengine.iterator.values(t, filter)
  local function iterator()
    if not t then coroutine.yield(nil) end

    for key, value in pairs(t) do
      if not filter then
        coroutine.yield(value)
      elseif filter(key, value) then
        coroutine.yield(value)
      end
    end
  end

  return coroutine.wrap(iterator)
end

function tdengine.iterator.keys(t, filter)
  local function iterator()
    if not t then coroutine.yield(nil) end
    
    for key, value in pairs(t) do
      if not filter then
        coroutine.yield(key)
      elseif filter(key, value) then
        coroutine.yield(key)
      end
    end
  end

  return coroutine.wrap(iterator)
end
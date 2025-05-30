function dn.iterator.values(t, filter)
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

function dn.iterator.keys(t, filter)
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

function dn.iterator.pairs(t, filter)
  local function iterator()
    if not t then coroutine.yield(nil) end
    
    for key, value in pairs(t) do
      if not filter then
        coroutine.yield(key, value)
      elseif filter(key, value) then
        coroutine.yield(key, value)
      end
    end
  end

  return coroutine.wrap(iterator)
end

function dn.iterator.carray(array, length)
  local function iterator()
    for i = 0, length - 1 do
      coroutine.yield(array[i])
    end
  end

  return coroutine.wrap(iterator)
end
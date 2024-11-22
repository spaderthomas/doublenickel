local Stack = tdengine.class.define('Stack')
tdengine.data_types.stack = Stack

function Stack:init()
  self.stack = {}
  self.visited = {}
end

function Stack:peek(n)
  if n then
    if n < #self.stack then
      return self.stack[#self.stack - n]
    end
  else
    return self.stack[#self.stack]
  end
end

function Stack:pop()
  local out = self:peek()
  self.stack[#self.stack] = nil
  return out
end

function Stack:push(item)
  if item == nil then dbg() end
  table.insert(self.stack, item)
  self.visited[dumb_hash(item)] = true
end

function Stack:size()
  return #self.stack
end

function Stack:clear()
  self.stack = {}
end

function Stack:push_unique(item)
  if item == nil then return end
  local hash = dumb_hash(item)
  if self.visited[hash] then return end

  self:push(item)
end

function Stack:is_empty()
  return #self.stack == 0
end

function stack_create(stack)
  stack.visited = {}
end

function stack_pop(stack)
  local out = stack[#stack]
  stack[#stack] = nil
  return out
end

function stack_empty(stack)
  return #stack == 0
end

function stack_push(stack, item)
  if item == nil then dbg() end
  table.insert(stack, item)
  stack.visited[dumb_hash(item)] = true
end

function stack_push_unique(stack, item)
  if item == nil then return end
  local hash = dumb_hash(item)
  if stack.visited[hash] then return end

  stack_push(stack, item)
end

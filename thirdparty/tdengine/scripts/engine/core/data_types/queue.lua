Queue = tdengine.class.define('Queue')
tdengine.data_types.queue = Queue

function Queue:init(capacity)
  self.array = tdengine.data_types.array:new()
  self.capacity = capacity
end

function Queue:push(item)
  if self.capacity and self.array:size() == self.capacity then
    self:pop()
  end

  self.array:add(item)
end

function Queue:pop()
  local item = self.array:at(1)
  self.array:remove(1)
  return item
end

function Queue:peek_at(index)
  return self.array:at(index)
end

function Queue:size()
  return self.array:size()
end

function Queue:is_empty()
  return self.array:is_empty()
end

function Queue:clear()
  return self.array:clear()
end

function Queue:remove(item)
  self.array:remove_value(item)
end

function Queue:iterate()
  return self.array:iterate()
end

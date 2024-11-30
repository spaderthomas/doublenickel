tdengine.array = {}

function tdengine.array.add(array, item)
  array[#array + 1] = item
end

function tdengine.array.remove(array, index)
  for i = index, #array, 1 do
    array[i] = array[i + 1]
  end
end

function delete(array, value)
  local len = #array

  for index, v in pairs(array) do
    if v == value then
      array[index] = nil
    end
  end

  local next_available = 0
  for check = 1, len do
    if array[check] ~= nil then
      next_available = next_available + 1
      array[next_available] = array[check]
    end
  end

  for remove = next_available + 1, len do
    array[remove] = nil
  end
end

Array = tdengine.class.define('Array')
tdengine.data_types.array = Array
tdengine.data_types.Array = Array

function Array:init()
  self.data = {}
end

function Array:add(item)
  self.data[#self.data + 1] = item
end

function Array:remove(index)
  for i = index, #self.data, 1 do
    self.data[i] = self.data[i + 1]
  end
end

function Array:insert(value, index)
  if index > self:size() then
    return self:add(value)
  end

  for i = #self.data, index, -1 do
    self.data[i + 1] = self.data[i]
  end
  self.data[index] = value
end

function Array:remove_value(value)
  local len = self:size()

  for index, v in pairs(self.data) do
    if v == value then
      self.data[index] = nil
    end
  end

  local next_available = 0
  for check = 1, len do
    if self.data[check] ~= nil then
      next_available = next_available + 1
      self.data[next_available] = self.data[check]
    end
  end

  for remove = next_available + 1, len do
    self.data[remove] = nil
  end
end

function Array:at(index)
  return self.data[index]
end

function Array:back()
  return self.data[self:size()]
end

function Array:clear()
  self.data = {}
end

function Array:size()
  return #self.data
end

function Array:is_empty()
  return #self.data == 0
end

function Array:concatenate(other)
  for index, value in other:iterate() do
    self:add(value)
  end
end

function Array:iterate()
  local index = 0

  local function iterator()
    index = index + 1
    local item = self.data[index]
    if item then
      return index, item
    end
  end

  return iterator
end

function Array:reverse_iterate()
  local index = self:size() + 1

  local function iterator()
    index = index - 1
    local item = self.data[index]
    if item then
      return index, item
    end
  end

  return iterator
end

function Array:iterate_values()
  local function iterator()
    for _, value in self:iterate() do
      coroutine.yield(value)
    end
  end

  return coroutine.wrap(iterator)
end

function Array:__tostring()
  return print_table(self.data)
end

local function test_array()
  local a = tdengine.data_types.array:new()
  a:add(69)
  a:add(420)
  a:add(9001)
  a:insert(100, 7)
  for index, item in a:iterate() do
    print(index, item)
  end
end

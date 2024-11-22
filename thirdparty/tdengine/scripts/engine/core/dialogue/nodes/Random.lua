local Random = tdengine.node('Random')

function Random:init()
  self.children_chosen = {}
end

function Random:advance(graph)
  -- If we've picked every option already, start over
  if #self.children_chosen == #self.children then
    table.clear(self.children_chosen)
  end

  -- Generate random indices until you get one you haven't picked yet. Obviously, this
  -- isn't truly random, but in the general case that I want this (where you have a few
  -- flavor options and you want to look a *little* responsive), this is way better
  -- than picking the same option a few times just because muh random.
  local index
  while true do
    index = math.random(#self.children)
    if not self.children_chosen[index] then
      self.children_chosen[index] = true
      break
    end
  end

  local uuid = self.children[index]
  return graph[uuid]
end

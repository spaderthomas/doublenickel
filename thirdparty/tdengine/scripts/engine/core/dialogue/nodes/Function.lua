local Function = tdengine.node('Function')

Function.editor_fields = {
  'name',
  'params'
}

function Function:init()
  self.name = 'debug'
  self.params = {}
end

function Function:enter(graph)
  self.fn = coroutine.create(tdengine.callback.find(self.name))
  self:run_one()
  if self.is_done then
    return dialogue_state.advancing
  else
    return dialogue_state.processing
  end
end

function Function:process(graph)
  self:run_one()
  if self.is_done then
    return dialogue_state.advancing
  else
    return dialogue_state.processing
  end
end

function Function:advance(graph)
  if self.value and type(self.value) == 'number' then
    local uuid = self.children[self.value]
    return graph[uuid]
  elseif self.value and type(self.value) == 'boolean' then
    local index = ternary(self.value, 1, 2)
    local uuid = self.children[index]
    return graph[uuid]
  else
    return simple_advance(self, graph)
  end
end

function Function:short_text()
  return short_text(self.name)
end

function Function:run_one()
  local success, value = coroutine.resume(self.fn, self.params)
  if not success then
    tdengine.handle_error(value)
  else
    self.value = value
  end

  self.is_done = coroutine.status(self.fn) == 'dead'
end

local Jump = tdengine.node('Jump')

Jump.editor_fields = {
  'target',
  'target_dialogue',
}

Jump.default_target = 'JumpTarget'

function Jump:init()
  self.target = Jump.default_target
  self.target_dialogue = ''
end

function Jump:short_text()
  return self.target
end

function Jump:has_target_dialogue()
  return #self.target_dialogue > 0
end

function Jump:advance(graph)
  local controller = tdengine.dialogue.controller

  local need_dialogue_change = false
  need_dialogue_change = need_dialogue_change or self:has_target_dialogue()
  need_dialogue_change = need_dialogue_change and self.target_dialogue ~= controller.current_dialogue
  if need_dialogue_change then
    -- Ask the controller to load the target dialogue, then return the
    -- target node within as the current node
    controller:change_dialogue(self.target_dialogue)
    local node = tdengine.dialogue.find_node(controller.data, self.target)
    return node
  else
    -- We've already got the graph, so find the node in it
    local node = tdengine.dialogue.find_node(graph, self.target)
    return node
  end
end

local Call = tdengine.node('Call')

function Call:init()
  -- What node should I jump to?
  self.target = 'FILL_IN_NODE_UUID_OR_LABEL'

  -- What scene should I look for the target node in?
  self.target_dialogue = ''
end

function Call:short_text()
  return self.target
end

function Call:has_target_dialogue()
  return #self.target_dialogue > 0
end

function Call:advance(graph)
  local controller = tdengine.dialogue.controller
  local stack = tdengine.dialogue.stack

  -- When we finally encounter a Return node, it should go to this node's child (which
  -- exists in the currently loaded dialogue)
  local return_node = self:get_child()
  stack:push(return_node, controller.current_dialogue)

  -- Then, push the called node onto the stack. This might be in another dialogue, so figure that out
  -- and then load it if necessary
  local need_dialogue_change = false
  need_dialogue_change = need_dialogue_change or self:has_target_dialogue()
  need_dialogue_change = need_dialogue_change and self.target_dialogue ~= controller.current_dialogue
  if need_dialogue_change then
    -- Ask the controller to load the target dialogue, then return the
    -- target node within as the current node
    controller:change_dialogue(self.target_dialogue)
    local node = tdengine.dialogue.find_node(controller.nodes, self.target)
    return node
  else
    -- We've already got the graph, so find the node in it
    local node = tdengine.dialogue.find_node(graph, self.target)
    return node
  end
end

Call.editor_fields = {
  'target',
  'target_dialogue',
}

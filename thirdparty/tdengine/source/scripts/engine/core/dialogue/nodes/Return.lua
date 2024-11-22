local Return = tdengine.node('Return')

function Return:init()
end

function Return:advance(graph)
  local stack = tdengine.dialogue.stack
  local target = stack:pop()

  local controller = tdengine.dialogue.controller
  local need_dialogue_change = false
  need_dialogue_change = need_dialogue_change or #target.dialogue > 0
  need_dialogue_change = need_dialogue_change and target.dialogue ~= controller.current_dialogue
  if need_dialogue_change then
    -- Ask the controller to load the target dialogue, then return the
    -- target node within as the current node
    controller:change_dialogue(target.dialogue)
    return tdengine.dialogue.find_node(controller.nodes, target.node_uuid)
  else
    -- We've already got the graph, so find the node in it
    return tdengine.dialogue.find_node(graph, target.node_uuid)
  end
end

Return.editor_fields = {
}

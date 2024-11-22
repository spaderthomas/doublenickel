DialogueStack = tdengine.class.define('DialogueStack')

function DialogueStack:init(params)
  self.stack = tdengine.data_types.stack:new()
  self.graph = {}
end

function DialogueStack:set_graph(graph)
  self.graph = graph
end

function DialogueStack:push(node_uuid, target_dialogue)
  local controller = tdengine.dialogue.controller
  local item = {
    node_uuid = node_uuid,
    dialogue = target_dialogue or controller.current_dialogue
  }
  self.stack:push(item)
end

function DialogueStack:pop()
  return self.stack:pop()
end

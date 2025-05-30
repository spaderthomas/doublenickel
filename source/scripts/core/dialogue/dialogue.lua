local self = dn.dialogue

function dn.dialogue.init()
  self.cache:init()
  self.update_all_metrics()

  local on_unknown_callback = function()
    log.warn('debug callback')
  end
  dn.callback.register('debug', on_unknown_callback)

  local info = dn.ffi.paths_resolve('character_info')
  if dn.ffi.os_does_path_exist(info) then
    dn.dialogue.characters = dn.module.read_from_named_path('character_info')
  else
    dn.dialogue.characters = {}
  end


  self.controller = DialogueController:new()
  self.history = History:new()
  self.stack = DialogueStack:new()
end

-------------------
-- SERIALIZATION --
-------------------
function dn.dialogue.load(name_or_path)
  local cache_data = self.cache:find(name_or_path)
  if not cache_data then return end

  local data = {}
  data.metadata = table.deep_copy(cache_data.metadata)
  data.nodes = {}

  for uuid, node_data in pairs(cache_data.nodes) do
    local node = dn.create_node(node_data.kind)
    node:deserialize(node_data)
    data.nodes[uuid] = node
  end

  return data
end

function dn.dialogue.save(name, data, pretty)
  local serialized_data = {}
  serialized_data.metadata = table.deep_copy(data.metadata)
  serialized_data.nodes = {}

  for uuid, node in pairs(data.nodes) do
    serialized_data.nodes[uuid] = node:serialize()
  end

  local file_path = dn.ffi.paths_resolve_format('dialogue_source', name)
  dn.module.write(file_path, serialized_data, dn.module.WriteOptions.Pretty)

  -- The cache always reflects what is on disk; so, if we update the copy on disk at runtime,
  -- then we need to update the cache.
  --
  -- You could just use the serialized node list you built directly above.
  self.cache:reload_single(name)
end

function dn.dialogue.play(dialogue)
  local controller = self.controller
  controller:play_dialogue(dialogue)
end

function dn.dialogue.stop()
  local controller = self.controller
  controller:stop_dialogue()
end

function dn.dialogue.play_at_tool(dialogue, tool)
  self.play(dialogue)

  local controller = self.controller
  controller.state = dialogue_state.beginning_tool
  controller.tool = tool
end

----------
-- FIND --
----------
function dn.dialogue.find_node(graph, fid)
  for id, node in pairs(graph) do
    local match = false
    match = match or fid == id
    match = match or fid == node.label

    if match then
      return node
    end
  end
end

function dn.dialogue.list()
  local dialogues = {}
  
  local directory = dn.ffi.paths_resolve('dialogues')
	for entry in dn.filesystem.iterate_directory(directory) do
    table.insert(dialogues, entry.file_path)
	end
  return dialogues
end

-----------
-- CACHE --
-----------
--- I want to do dialogue validation on every keypress. That involves validating references to nodes
--- across dialogues. I can't load every dialogue every keypress just to do this, so I keep a cache.
--- This does make it tricky to have two instances of the editor open, but that should probably just
--- be a feature of the engine (many dialogues open at once).
---
function dn.dialogue.cache:init()
  dn.dialogue.data = {}

  local dialogues = dn.dialogue.list()
  for index, dialogue_name in pairs(dialogues) do
    self:reload_single(dialogue_name)
  end
end

function dn.dialogue.cache:iterate()
  local function iterator(t, k)
    local name, data = next(t, k)
    return name, data
  end

  return iterator, dn.dialogue.data, nil
end

function dn.dialogue.cache:reload_single(name_or_path)
  local file_name = dn.dialogue.clean_name(name_or_path)
  local file_path = dn.ffi.paths_resolve_format('dialogue_source', file_name)
  dn.dialogue.data[file_name] = dn.module.read(file_path)
end

function dn.dialogue.cache:find(name_or_path)
  local dialogue_name = dn.dialogue.clean_name(name_or_path)
  return dn.dialogue.data[dialogue_name]
end

function dn.dialogue.cache:find_node(target_node, target_dialogue)
  if target_dialogue then
    local dialogue = dn.dialogue.data[target_dialogue]
    if not dialogue then return nil end
    return dn.dialogue.find_node(dialogue, target_node)
  else
    -- Look in every dialogue. Probably this would be a performance issue if you did it on
    -- every keystroke for large projects
    for _, dialogue in pairs(self.data) do
      local node = dn.dialogue.find_node(dialogue, target_node)
      if node then return node end
    end
  end
end

-------------
-- METRICS --
-------------
function dn.dialogue.find_metric(dialogue)
  dialogue = dialogue:gsub('.lua', '')
  return self.metrics.dialogues[dialogue]
end

function dn.dialogue.update_all_metrics()
  self.metrics = self.calculate_all_metrics()
end

function dn.dialogue.update_single_metrics(dialogue)
  dialogue = dialogue:gsub('.lua', '')
  self.metrics.dialogues[dialogue] = self.calculate_single_metrics(dialogue)
end

function dn.dialogue.calculate_all_metrics()
  local metrics = {
    dialogues = {},
    nodes = 0,
    branches = 0,
    words = 0,
  }

  local dialogues = self.list()
  for index, dialogue in pairs(dialogues) do
    local metric = self.calculate_single_metrics(dialogue)
    metrics.dialogues[dialogue] = metric
    metrics.nodes = metrics.nodes + metric.nodes
    metrics.words = metrics.words + metric.words
    metrics.branches = metrics.branches + metric.branches
  end

  return metrics
end

function dn.dialogue.calculate_single_metrics(dialogue)
  local metrics = {
    nodes = 0,
    words = 0,
    branches = 0,
  }

  dialogue = dialogue:gsub('.lua', '')
  local nodes = self.cache:find(dialogue).nodes

  for uuid, node in pairs(nodes) do
    metrics.nodes = metrics.nodes + 1

    if node.text then
      local words = split(node.text, ' ')
      metrics.words = metrics.words + #words
    end

    if node.kind == self.node_kind.Branch then
      metrics.branches = metrics.branches + 1
    end
  end

  return metrics
end

---------------
-- CALLBACKS --
---------------



---------------
-- UTILITIES --
---------------
function dn.dialogue.clean_name(name_or_path)
  return dn.strip_extension(dn.extract_filename(name_or_path))
end

function find_entry_node(graph)
  for id, node in pairs(graph) do
    local is_entry_point = node.is_entry_point or false
    if is_entry_point then
      return node
    end
  end

  return nil
end

function find_tool_node(graph, tool)
  for uuid, node in pairs(graph) do
    if node.kind == self.node_kind.Tool and node.tool == tool then
      return node
    end
  end

  return nil
end

function simple_advance(node, graph)
  local child_id = node.children[1]
  return graph[child_id]
end

function stat_name(branch)
  return split(branch.variable, '.')[2]
end

function short_text(text, max_size)
  if not text then return '' end

  max_size = max_size or 16
  if string.len(text) < max_size then
    return string.sub(text, 0, max_size)
  else
    return string.sub(text, 0, max_size - 3) .. '...'
  end
end

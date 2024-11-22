-- Graph validation
local invalid_graph_error = {
  all_choices_can_return_nil = 'All Choice nodes could be nil',
  bad_if_node = 'If: Invalid placement',
  branch_is_missing = 'Node is missing a branch path',
  branch_too_many = 'Branch: too many branches',
  branch_no_checks = 'Branch does not check any condition',
  choice_multi_children = 'Choice nodes that do not roll can only have one child',
  choice_list_wrong_child = 'ChoiceList child is not Choice',
  default_label = 'Forgot to fill in a label',
  duplicate_label = 'Duplicate label',
  illegal_split = 'Node branches illegally',
  invalid_next_dialogue = 'Next dialogue does not exist',
  invalid_choice = 'Illegal Choice node',
  label_does_not_exist = 'Target node does not exist',
  laziness = 'I was too lazy to make a real error for this one',
  no_child = 'Node is missing child',
  no_entry_node = 'No entry node',
  no_skill_check = 'Skill check node does not roll a skill check',
  none = 'none',
  not_all_choices_valid = 'Not all paths in choice list return Choice node',
  open_notes_password = 'Bad children for OpenNotesPassword',
  return_has_child = 'Node child will never be hit',
  switch_has_child = 'Node child will never be hit',
  text_is_empty = 'Node text is empty',
  too_many_children = "Node has too many children",
  unknown_node_type = 'Unknown node type',
  var_not_exist = 'Variable does not exist',
}

local invalid_graph_messages = {
  [invalid_graph_error.all_choices_can_return_nil] =
  [[When you present choices, they may be gated behind conditions. If a condition is false, it may not return a Choice node at all. However, _at least one_ Choice must be valid, or else the game has nothing to present. There is a combination of variables here where all branches are false and no Choice node is returned.]],

  [invalid_graph_error.bad_if_node] =
  [[If nodes may only be used when it is OK if they evaluate to false and do not return a node. For instance, when gating a choice behind some condition. The reason: If the node evaluates to false, and we do not display what is behind it, what else can we display? There is no alternative node.

If you want to switch between 2 different nodes based on a condition, use a Branch node.]],

  [invalid_graph_error.branch_is_missing] =
  [[Any node that branches, either by rolling or doing a state check, must have a path for both the true and false case.]],

  [invalid_graph_error.branch_too_many] =
  [[Branch nodes may only have two children (a branch for true and a branch for false).]],

  [invalid_graph_error.branch_no_checks] =
  [[A branch node must check at least one condition.]],

  [invalid_graph_error.choice_multi_children] =
  [[Choice nodes may only have more than one child if they have a branching operation after selection.]],

  [invalid_graph_error.choice_list_wrong_child] =
  [[All children of a ChoiceList node must be Choice nodes.]],

  [invalid_graph_error.default_label] =
  [[You either left the target label empty, or didn't change it from the default.]],

  [invalid_graph_error.duplicate_label] =
  [[You're using the same label on two nodes in the same dialogue.]],

  [invalid_graph_error.illegal_split] =
  [[A node may only have multiple children if it is (a) a Branch node or (b) it leads into Choice nodes (or conditional nodes that branch into Choice nodes).]],

  [invalid_graph_error.invalid_next_dialogue] =
  [[If you want to switch to another dialogue, it has to exist.]],

  [invalid_graph_error.invalid_choice] =
  [[Choice nodes may only be the children of ChoiceList nodes.]],

  [invalid_graph_error.no_child] =
  [[Every node must have a child, except Jump and Switch nodes. These nodes are special, because they _do_ have a child -- it's just not explicitly in the graph. The reason: If a node does not have a child, what does the game do? ]],

  [invalid_graph_error.no_entry_node] =
  [[Every dialogue graph _must_ have an entry node. This is the first node displayed when the dialogue begins. Dialogues _may_ be started elsewhere -- for instance, if another dialogue uses a Switch node that jumps to a specific point. However, if we just load the dialogue, we must know where to start]],

  [invalid_graph_error.no_skill_check] =
  [[This node needs to roll, but it doesn't roll anything]],

  [invalid_graph_error.not_all_choices_valid] =
  [[A branch path evaluates to a node that is not a Choice node. If you have choices, you must ONLY have choices. For example: It does not make sense to have 3 Choice nodes and 1 Text node -- what does the engine do with that Text node while it's displaying choices?]],

  [invalid_graph_error.open_notes_password] =
  [[An OpenNotesPassword node must have a NotesPassword child for each password that's accepted, and then a non-NotesPassword child for when the user cancels entering the password.]],

  [invalid_graph_error.return_has_child] =
  [[Jump nodes look up a separate node and move to it. They will not move to their child node. Any child node of a Jump node will not be hit.]],

  [invalid_graph_error.label_does_not_exist] =
  [[The node you're trying to return to does not exist. The node must exist in this dialogue -- if you're trying to change dialogues, then use a Switch node.]],

  [invalid_graph_error.switch_has_child] =
  [[This node will look up a separate node and move to it instead of just going to its child. Any child node will not be hit. ]],

  [invalid_graph_error.text_is_empty] =
  [[This node's text is empty. That's bad.]],

  [invalid_graph_error.too_many_children] =
  [[Only nodes which branch in some way (i.e. by rolling, or checking state fields, or presenting choices) may have more than one child.]],

  [invalid_graph_error.unknown_node_type] =
  [[Unknown node type.]],

  [invalid_graph_error.var_not_exist] =
  [[Field used by this node does not exist in state.]],

}

-----------
-- UTILS --
-----------
local function error_message(error_kind, node)
  return { kind = error_kind, node = node.uuid }
end


-----------------
-- NODE TRAITS --
-----------------
function validate_bifurcating_node(node, context, errors)
  if #node.children < 2 then
    table.insert(errors, error_message(invalid_graph_error.branch_is_missing, node))
  end
  if #node.children > 2 then
    table.insert(errors, error_message(invalid_graph_error.branch_too_many, node))
  end
end

function validate_one_child(node, context, errors)
  if #node.children == 0 then
    table.insert(errors, error_message(invalid_graph_error.no_child, node))
  elseif #node.children > 1 then
    table.insert(errors, error_message(invalid_graph_error.too_many_children, node))
  end
end

function validate_zero_children(node, context, errors)
  if #node.children ~= 0 then
    table.insert(errors, error_message(invalid_graph_error.switch_has_child, node))
  end
end

function validate_nonzero_children(node, context, errors)
  if #node.children == 0 then
    table.insert(errors, error_message(invalid_graph_error.no_child, node))
  end
end

function validate_no_choice_children(node, context, errors)
  for index, uuid in pairs(node.children) do
    local child = context.graph[uuid]
    if child.kind == tdengine.dialogue.node_kind.Choice then
      table.insert(errors, error_message(invalid_graph_error.invalid_choice, node))
    end
  end
end

function validate_has_roll(node, context, errors)
  if #node.branches == 0 then
    table.insert(errors, error_message(invalid_graph_error.no_skill_check, node))
  end
end

function validate_has_condition(node, context, errors)
  if #node.branches == 0 then
    table.insert(errors, error_message(invalid_graph_error.branch_no_checks, node))
  end
end

function validate_all_children_match(node, context, errors, node_kind, error_kind)
  -- Choice list can have as many children as you like, as long as they're all choices.
  for index, child_uuid in pairs(node.children) do
    local child = context.graph[child_uuid]
    if child.kind ~= node_kind then
      table.insert(errors, {
        kind = error_kind,
        node = node.uuid,
        child_kind = child_uuid
      })
    end
  end
end

----------------
-- NODE KINDS --
----------------
function validate_label(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
  validate_no_duplicate_label(node, context, errors)
end

function validate_jump(node, context, errors)
  validate_zero_children(node, context, errors)
  validate_target(node, context, errors)
  validate_target_exists(node, context, errors)
end

function validate_call(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
  validate_target(node, context, errors)
  validate_target_exists(node, context, errors)
end

function validate_return(node, context, errors)
  validate_zero_children(node, context, errors)
end

function validate_end(node, context, errors)
  validate_zero_children(node, context, errors)
end

function validate_find_note(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_function(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_choice_list(node, context, errors)
  validate_all_children_match(node, context, errors, tdengine.dialogue.node_kind.Choice,
    invalid_graph_error.choice_list_wrong_child)
end

function validate_choice(node, context, errors)
  validate_text_not_empty(node, context, errors)
  validate_no_choice_children(node, context, errors)

  if node:get_roll() then
    validate_bifurcating_node(node, context, errors)
  else
    validate_one_child(node, context, errors)
  end
end

function validate_text(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_branch(node, context, errors)
  validate_bifurcating_node(node, context, errors)
  validate_no_choice_children(node, context, errors)
  validate_has_condition(node, context, errors)
end

function validate_skill_check(node, context, errors)
  validate_bifurcating_node(node, context, errors)
  validate_no_choice_children(node, context, errors)
  validate_has_roll(node, context, errors)
end

function validate_set(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_increment(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_continue(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_random(node, context, errors)
  validate_nonzero_children(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_play_sound(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_terminal_input(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_password_input(node, context, errors)
  validate_bifurcating_node(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_notes_password_input(node, context, errors)
  local found_default = false
  local found_error = false

  for index, child_uuid in pairs(node.children) do
    local child = context.graph[child_uuid]
    if child.kind ~= tdengine.dialogue.node_kind.NotePassword then
      if found_default then
        -- Already have a default
        found_error = true
      end
      found_default = true
    end
  end

  if found_error or not found_default then
    table.insert(errors, {
      kind = invalid_graph_error.open_notes_password,
      node = node.uuid,
      child_kind = child_uuid
    })
  end
end

function validate_note_password(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_open_page(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

function validate_unknown(node, context, errors)
  local err = error_message(invalid_graph_error.unknown_node_type, node)
  err.kind = node.kind
  if node.kind == nil then err.kind = 'nil' end
  table.insert(errors, err)
end

function validate_tithonus(node, context, errors)
  validate_one_child(node, context, errors)
  validate_all_children_match(node, context, errors, tdengine.dialogue.node_kind.ChoiceList, invalid_graph_error
    .laziness)
end

function validate_tool(node, context, errors)
  validate_one_child(node, context, errors)
end

function validate_knowledge_map(node, context, errors)
  validate_one_child(node, context, errors)
  validate_no_choice_children(node, context, errors)
end

-----------------
-- NODE FIELDS --
-----------------
function validate_node_fields(node, context, errors)
  if not node:uses_state() then return end

  local err = {
    kind = invalid_graph_error.var_not_exist,
    node = node.uuid,
    fields = {}
  }

  local validate_single_field = function(field)
    local exist = index_string(tdengine.state.data, field)
    if exist == nil then
      table.insert(err.fields, field)
    end
  end

  if node.branches then
    for index, branch in pairs(node.branches) do
      validate_single_field(branch.variable)
    end
  elseif node.variable then
    validate_single_field(node.variable)
  end

  if #err.fields > 0 then table.insert(errors, err) end
end

function validate_target_exists(node, context, errors)
  if context.symbols[node.target] then
    return
  end

  local err = error_message(invalid_graph_error.label_does_not_exist, node)
  table.insert(errors, err)
end

function validate_target(node, context, errors)
  if node.target == tdengine.dialogue.node_type.Jump.default_target then
    local err = error_message(invalid_graph_error.default_label, node)
    table.insert(errors, err)
  end

  if node.target == '' then
    local err = error_message(invalid_graph_error.default_label, node)
    table.insert(errors, err)
  end
end

function validate_dialogue_exists(node, context, errors)
  if tdengine.dialogue.cache:find(node.target) then return end

  local err = error_message(invalid_graph_error.invalid_next_dialogue, node)
  err.target = node.target
  table.insert(errors, err)
end

function validate_text_not_empty(node, context, errors)
  if #node.text == 0 then
    local err = error_message(invalid_graph_error.text_is_empty, node)
    table.insert(errors, err)
  end
end

function validate_no_duplicate_label(node, context, errors)
  for uuid, other in pairs(context.graph) do
    if node.uuid == uuid then goto continue end
    if node.label == other.label then
      local err = error_message(invalid_graph_error.duplicate_label, node)
      table.insert(errors, err)
    end
    ::continue::
  end
end

----------------
-- VALIDATION --
----------------
function build_validation_context(graph)
  local context = {
    graph = graph
  }

  context.symbols = {}
  for uuid, node in pairs(graph) do
    context.symbols[uuid] = true
    if node.label then context.symbols[node.label] = true end
  end

  for name, data in tdengine.dialogue.cache:iterate() do
    local nodes = data.nodes
    for uuid, node in pairs(nodes) do
      if node.label and node.export then
        context.symbols[uuid] = true
        context.symbols[node.label] = true
      end
    end
  end

  return context
end

function validate_graph(graph)
  local errors = {}

  -- Ensure that there's an entry point to the graph
  local entry = find_entry_node(graph)
  if not entry then
    local err = {
      kind = invalid_graph_error.no_entry_node
    }
    table.insert(errors, err)
    return errors
  end

  -- Collect disconnected components by making a list of all nodes that have a parent,
  -- and then defining a DC'd component as beginning with any node that doesn't.
  local stack = {}
  stack_create(stack)

  local has_parent = {}
  for uuid, node in pairs(graph) do
    has_parent[uuid] = false
  end

  for _, node in pairs(graph) do
    for index, uuid in pairs(node.children) do
      has_parent[uuid] = true
    end
  end

  for uuid, node in pairs(graph) do
    if not has_parent[uuid] then
      stack_push(stack, node)
    end
  end

  local context = build_validation_context(graph)

  local dummy = function() end

  -- Do a BFS on each disconnected component
  while not stack_empty(stack) do
    current = stack_pop(stack)

    validate_node_fields(current, context, errors)

    if current.kind == 'ActiveSkillCheck' then
      validate_skill_check(current, context, errors)
    elseif current.kind == 'Branch' then
      validate_branch(current, context, errors)
    elseif current.kind == 'Call' then
      validate_call(current, context, errors)
    elseif current.kind == 'ChoiceList' then
      validate_choice_list(current, context, errors)
    elseif current.kind == 'Choice' then
      validate_choice(current, context, errors)
    elseif current.kind == 'Continue' then
      validate_continue(current, context, errors)
    elseif current.kind == 'DevNote' then
      dummy()
    elseif current.kind == 'DevMarker' then
      dummy()
    elseif current.kind == 'End' then
      validate_end(current, context, errors)
    elseif current.kind == 'FindNote' then
      validate_find_note(current, context, errors)
    elseif current.kind == 'Function' then
      validate_function(current, context, errors)
    elseif current.kind == 'Increment' then
      validate_increment(current, context, errors)
    elseif current.kind == 'Jump' then
      validate_jump(current, context, errors)
    elseif current.kind == 'KnowledgeMap' then
      validate_knowledge_map(current, context, errors)
    elseif current.kind == 'Label' then
      validate_label(current, context, errors)
    elseif current.kind == 'NotePassword' then
      validate_note_password(current, context, errors)
    elseif current.kind == 'OpenPage' then
      validate_open_page(current, context, errors)
    elseif current.kind == 'OpenPasswordInput' then
      validate_password_input(current, context, errors)
    elseif current.kind == 'OpenTerminalInput' then
      validate_terminal_input(current, context, errors)
    elseif current.kind == 'OpenNotesPassword' then
      validate_notes_password_input(current, context, errors)
    elseif current.kind == 'PlaySound' then
      validate_play_sound(current, context, errors)
    elseif current.kind == 'Random' then
      validate_random(current, context, errors)
    elseif current.kind == 'Return' then
      validate_return(current, context, errors)
    elseif current.kind == 'Set' then
      validate_set(current, context, errors)
    elseif current.kind == 'Switch' then
      validate_switch(current, context, errors)
    elseif current.kind == 'Text' then
      validate_text(current, context, errors)
    elseif current.kind == 'Tithonus' then
      validate_tithonus(current, context, errors)
    elseif current.kind == 'Tool' then
      validate_tool(current, context, errors)
    elseif current.kind == 'Unreachable' then
      dummy()
    elseif current.kind == 'Wait' then
      dummy()
    else
      validate_unknown(current, context, errors)
    end

    if current.children then
      for index, child_uuid in pairs(current.children) do
        local child = context.graph[child_uuid]
        stack_push_unique(stack, child)
      end
    end
  end

  -- We now have all the errors; populate the table with human readable error messages
  for i, err in pairs(errors) do
    err['why?'] = invalid_graph_messages[err.kind]
  end

  return errors
end

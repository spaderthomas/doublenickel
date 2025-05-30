------------
-- ENUMS  --
------------
dn.branch_ops = {
  eq = 1,
  geq = 2,
  leq = 3,
}

dn.branch_op_names = {
  [dn.branch_ops.eq] = '==',
  [dn.branch_ops.geq] = '>=',
  [dn.branch_ops.leq] = '<=',
}

dn.branch_op_symbols = {
  [dn.branch_ops.eq] = '==',
  [dn.branch_ops.geq] = '>=',
  [dn.branch_ops.leq] = '<=',
}

dn.branch_combinators = {
  op_and = 1,
  op_or  = 2
}

dn.branch_combinator_names = {
  [dn.branch_combinators.op_and] = 'and',
  [dn.branch_combinators.op_or]  = 'or',
}

dn.branch_compatibility = {
  ['number'] = {
    dn.branch_ops.eq,
    dn.branch_ops.geq,
    dn.branch_ops.leq,
  },
  ['boolean'] = {
    dn.branch_ops.eq,
  },
  ['string'] = {
    dn.branch_ops.eq,
  }
}


--------------
-- DEFAULTS --
--------------
function make_default_branch()
  local default = {
    variable = 'Fallback.FallbackState',
    op = dn.branch_ops.eq,
    value = true
  }
  return default
end


--------------------------
-- BRANCH COMPATIBILITY --
--------------------------
function get_valid_ops_for_value(value)
  return dn.branch_compatibility[type(value)]
end


--------------------------
-- EVALUATION FUNCTIONS --
--------------------------
function evaluate_node(node, graph)
  -- Base case: If we hit nil, or a node that is not conditional, then we've
  -- evaluated as far as we can.
  if not node then return nil end
  if not node:is_conditional() then return node end

  -- Evaluate this node
  local branches = node:get_branches()
  local combinator = node:get_combinator()
  local pass, outcomes = evaluate_branches(branches, combinator)

  -- @hack: We only care about the outcome (i.e. what the actual value being checked was, be it roll or
  -- value of state) when we want to display it to the user, and we only do that for choices.
  --
  -- Choices only have one roll, and therefore one outcome.
  local outcome = outcomes[1].value

  -- Recurse to the child we evaluated to
  local index = ternary(pass, 1, 2)
  local child = graph[node.children[index]]
  local result_node = evaluate_node(child, graph)
  return result_node, pass, outcome
end

function evaluate_branches(branches, combinator)
  if #branches == 0 then
    return true, {}
  end

  local outcomes = {}
  for index, branch in ipairs(branches) do
    local outcome = {}

    local pass, value = evaluate_single_branch(branch)
    outcome.pass = pass
    outcome.value = value
    table.insert(outcomes, outcome)
  end

  local has_false = false
  local has_true = false
  for index, outcome in pairs(outcomes) do
    if outcome.pass == false then has_false = true end
    if outcome.pass == true then has_true = true end
  end

  if combinator == dn.branch_combinators.op_and then
    return (not has_false), outcomes
  elseif combinator == dn.branch_combinators.op_or then
    return has_true, outcomes
  end
end

function evaluate_single_branch(branch)
  local pass = false
  local outcome = nil
  local roll = 0
  local stat = 0
  if branch.op == dn.branch_ops.eq then
    --local outcome = dn.state.find(branch.variable)
    outcome = index_string(dn.state.data, branch.variable)
    pass = branch.value == outcome
  elseif branch.op == dn.branch_ops.geq then
    outcome = index_string(dn.state.data, branch.variable)
    pass = outcome >= branch.value
  elseif branch.op == dn.branch_ops.leq then
    outcome = index_string(dn.state.data, branch.variable)
    pass = outcome < branch.value
  end

  return pass, outcome
end
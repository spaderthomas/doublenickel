local Branch = tdengine.node('Branch')

Branch.editor_fields = {
  'branches',
  'combinator',
  'description'
}

function Branch:init()
  self.branches = {}
  self.combinator = tdengine.branch_combinators.op_and
  self.description = ''
end

function Branch:short_text()
  if #self.description > 0 then
    return self.description
  end

  if #self.branches == 1 then
    return short_text(self.branches[1].variable)
  end


  return '...'
end

function Branch:advance(graph)
  return evaluate_node(self, graph)
end

--------------
-- VIRTUAL  --
--------------
function Branch:is_conditional()
  return true
end

function Branch:is_bifurcate()
  return true
end

function Branch:uses_state()
  return true
end

function Branch:get_branches()
  return self.branches
end

function Branch:get_combinator()
  return self.combinator
end

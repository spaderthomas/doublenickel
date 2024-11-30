------------------------------
-- BRANCH COMBINATOR PICKER --
------------------------------
imgui.extensions.BranchCombinator = function(current_op)
  local change = false
  local next_op = current_op
  local combo_label = '##branch_combinator'
  local op_display_name = tdengine.branch_combinator_names[current_op]

  imgui.PushItemWidth(200)
  if imgui.BeginCombo(combo_label, op_display_name) then
    -- Iterate through all the ops that are in the engine
    for op_name, op_id in pairs(tdengine.branch_combinators) do
      -- Translate them to a human readable name
      local op_display_name = tdengine.branch_combinator_names[op_id]
      local op_selected = op_id == current_op

      -- Update the branch with the op's integer identifier if chosen
      if imgui.Selectable(op_display_name, op_selected) then
        change = true
        next_op = op_id
      end
    end
    imgui.EndCombo()
  end

  imgui.PopItemWidth()

  return change, next_op
end

imgui.extensions.AlignedBranchCombinator = function(current_op, table_editor)
  local cursor = imgui.GetCursorPosX()
  local padding = imgui.internal.table_editor_padding(table_editor)
  local color = imgui.internal.table_editor_depth_color(1)

  imgui.extensions.VariableName('combinator', color)
  imgui.SameLine()
  imgui.SetCursorPosX(cursor + padding)
  return imgui.extensions.BranchCombinator(current_op)
end

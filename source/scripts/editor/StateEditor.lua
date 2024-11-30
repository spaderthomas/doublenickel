local StateEditor = tdengine.editor.define('StateEditor')
function StateEditor:init(params)
  self.state_editor = imgui.extensions.TableEditor(tdengine.state.data)
  self.dirty = false
  self.timer = Timer:new(1)

  self.new_category = ''

  self.ids = {
    add = '##state_editor:add',
  }
end

function StateEditor:update(dt)
  if tdengine.editor.find('DialogueEditor').hidden then return end
  
  -- This flag only applies to whether we're dirty this frame
  self.dirty = false

  tdengine.editor.begin_window('Dialogue State')

  imgui.InputText(self.ids.add, self, 'new_category')
  imgui.SameLine()
  if imgui.Button('Add') then
    tdengine.state.data[self.new_category] = {}
    self.new_category = ''
  end

  -- If the underlying table changes, we're holding a stale reference
  if self.state_editor.editing ~= tdengine.state.data then
    self.state_editor = imgui.extensions.TableEditor(tdengine.state.data)
  end

  self.timer:update()
  if self.timer:is_done() then
    self.timer:reset()

    -- Reconcile any added / removed state fields to the default state
    local default_state = tdengine.state.read_file('default')
    if make_keys_match(tdengine.state.data, default_state) then
      self.dirty = true
      tdengine.state.write('default', default_state)
    end
  end

  self.state_editor:draw()
  tdengine.editor.end_window()
end

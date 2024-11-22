function tdengine.action.init()
  tdengine.action.event_kind = {
    press = tdengine.ffi.KeyAction_Press,
    down = tdengine.ffi.KeyAction_Down,
  }

  tdengine.action.data = tdengine.module.read_from_named_path('action_info')
  for action_set_name, action_set in pairs(tdengine.action.data.action_sets) do
    tdengine.ffi.register_action_set(action_set_name)
    if action_set.default then
      --tdengine.ffi.activate_action_set(action_set_name)
    end

    for _, action in pairs(action_set.actions) do
      local key_controls = tdengine.action.data.keyboard_controls[action]
      local key = key_controls.key
      local event = tdengine.action.event_kind[key_controls.event]

      tdengine.ffi.register_action(action, key, event, action_set_name)
    end
  end
end

function tdengine.action.activate_set(action_set)
  return tdengine.ffi.activate_action_set(action_set)
end

function tdengine.action.get_active_set()
  return ffi.string(tdengine.ffi.get_active_action_set())
end

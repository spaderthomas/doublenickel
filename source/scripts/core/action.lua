function dn.action.init()
  dn.action.event_kind = {
    press = dn.unported.KEY_ACTION_PRESS,
    down = dn.unported.KEY_ACTION_DOWN,
  }

  dn.action.data = dn.module.read_from_named_path('action_info')
  for action_set_name, action_set in pairs(dn.action.data.action_sets) do
    dn.unported.register_action_set(action_set_name)
    if action_set.default then
      --dn.unported.activate_action_set(action_set_name)
    end

    for _, action in pairs(action_set.actions) do
      local key_controls = dn.action.data.keyboard_controls[action]
      local key = key_controls.key
      local event = dn.action.event_kind[key_controls.event]

      dn.unported.register_action(action, key, event, action_set_name)
    end
  end
end

function dn.action.activate_set(action_set)
  return dn.unported.activate_action_set(action_set)
end

function dn.action.get_active_set()
  return ffi.string(dn.unported.get_active_action_set())
end

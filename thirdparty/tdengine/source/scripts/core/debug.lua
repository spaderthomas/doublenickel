local do_once = {
  info = nil,
  args = nil,
  current = 0,
  target = 0
}

function tdengine.debug.assert(expression)
  if not expression then dbg() end
end

function tdengine.debug.do_once(f, ...)
  tdengine.debug.do_n_times(f, 1, ...)
end

function tdengine.debug.do_n_times(f, n, ...)
  local info = debug.getinfo(f)

  -- Is this the first time we called this specific function with these
  -- specific args?
  local is_new_thing
  if not do_once.info then
    is_new_thing = true
  else
    local line_match = do_once.info.linedefined == info.linedefined
    local file_match = do_once.info.source == info.source
    local args_match = table_eq_shallow(do_once.args, { ... })
    local all_match = line_match and file_match and args_match
    is_new_thing = not all_match
  end

  -- If this so, reset the counter
  if is_new_thing then
    do_once.info = info
    do_once.args = { ... }
    do_once.current = 0
    do_once.target = n
  end

  if do_once.current < do_once.target then
    f(...)
    do_once.current = do_once.current + 1
  end
end

function d()
  local controller = tdengine.dialogue.controller
  controller.state = 'idle'
end

function tdengine.debug.open_debugger(stack_depth)
  stack_depth = stack_depth or 0
  dbg(false, stack_depth + 1) -- @refactor
end

x = true
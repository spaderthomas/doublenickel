function doublenickel.state.init()
  -- doublenickel.state.load_file('default')
end

function doublenickel.state.load(state)
  for key, fields in pairs(state) do
    doublenickel.state.data[key] = doublenickel.state.data[key] or {}
    for field, value in pairs(fields) do
      doublenickel.state.data[key][field] = value
    end
  end
end

function doublenickel.state.load_file(file_name)
  local state = doublenickel.state.read_file(file_name)
  doublenickel.state.load(state)
end

function doublenickel.state.write(file_name, state)
  for key, value in pairs(doublenickel.state.data) do
    value.__editor = nil
  end

  local file_path = dn.ffi.paths_resolve_format('state', file_name)
  state = state or doublenickel.state.data

  doublenickel.module.write(file_path, state, doublenickel.module.WriteOptions.Pretty)
end

function doublenickel.state.read_file(file_name)
  local file_path = dn.ffi.paths_resolve_format('state', file_name)
  return doublenickel.state.read(file_path)
end

function doublenickel.state.read(file_path)
  return dofile(file_path)
end

function doublenickel.state.find(full_variable)
  if not full_variable then return false end

  local parent = doublenickel.state.extract_parent(full_variable)
  local key = doublenickel.state.extract_key(full_variable)

  return parent[key]
end

function doublenickel.state.set(full_variable, value)
  local parent = doublenickel.state.extract_parent(full_variable)
  local key = doublenickel.state.extract_key(full_variable)

  if parent == nil then
    log.error('doublenickel.state.set(): Tried to set %s, but parent field does not exist', full_variable)
    return
  end

  if parent[key] == nil then
    log.error('doublenickel.state.set(): Tried to set %s, but no such field exists', full_variable)
    return
  end

  -- Sanity check the types. I wish I was using a real type system, I guess.
  local parent_type = type(parent[key])
  local value_type = type(value)
  if parent_type ~= value_type then
    log.error('doublenickel.state.set(): Tried to set %s (%s) with a value of type %s', full_variable, parent_type,
      value_type)
    return
  end

  parent[key] = value
end

function doublenickel.state.increment(full_variable, step)
  local parent = doublenickel.state.extract_parent(full_variable)
  local key = doublenickel.state.extract_key(full_variable)

  local parent_type = type(parent[key])
  if parent_type ~= 'number' then
    log.error('doublenickel.state.increment(): Tried to increment %s (%s), but it is not a number', full_variable,
      parent_type)
    return
  end

  parent[key] = parent[key] + step
end

function doublenickel.state.extract_key(full_variable)
  local keys = string.split(full_variable, '.')
  return keys[#keys]
end

function doublenickel.state.extract_parent(full_variable)
  local parent = doublenickel.state.data
  local keys = string.split(full_variable, '.')
  for i, key in pairs(keys) do
    if i == #keys then break end
    parent = parent[key]
  end

  return parent
end

function doublenickel.state.get_type(full_variable)
  local state = doublenickel.state.find(full_variable)
  return type(state)
end

function doublenickel.state.is_number(full_variable)
  return doublenickel.state.get_type(full_variable) == 'number'
end

function doublenickel.state.is_boolean(full_variable)
  return doublenickel.state.get_type(full_variable) == 'boolean'
end

function doublenickel.state.is_string(full_variable)
  return doublenickel.state.get_type(full_variable) == 'string'
end

function doublenickel.state.get_sorted_fields()
  -- @hack, supreme laziness furthers
  local copy = table.deep_copy(doublenickel.state.data)
  copy.stats = nil
  local sorted_state = table.flatten(copy)
  table.sort(sorted_state)
  return sorted_state
end

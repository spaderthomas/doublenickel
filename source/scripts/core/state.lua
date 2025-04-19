function dn.state.init()
  -- dn.state.load_file('default')
end

function dn.state.load(state)
  for key, fields in pairs(state) do
    dn.state.data[key] = dn.state.data[key] or {}
    for field, value in pairs(fields) do
      dn.state.data[key][field] = value
    end
  end
end

function dn.state.load_file(file_name)
  local state = dn.state.read_file(file_name)
  dn.state.load(state)
end

function dn.state.write(file_name, state)
  for key, value in pairs(dn.state.data) do
    value.__editor = nil
  end

  local file_path = dn.ffi.paths_resolve_format('state', file_name)
  state = state or dn.state.data

  dn.module.write(file_path, state, dn.module.WriteOptions.Pretty)
end

function dn.state.read_file(file_name)
  local file_path = dn.ffi.paths_resolve_format('state', file_name)
  return dn.state.read(file_path)
end

function dn.state.read(file_path)
  return dofile(file_path)
end

function dn.state.find(full_variable)
  if not full_variable then return false end

  local parent = dn.state.extract_parent(full_variable)
  local key = dn.state.extract_key(full_variable)

  return parent[key]
end

function dn.state.set(full_variable, value)
  local parent = dn.state.extract_parent(full_variable)
  local key = dn.state.extract_key(full_variable)

  if parent == nil then
    log.error('dn.state.set(): Tried to set %s, but parent field does not exist', full_variable)
    return
  end

  if parent[key] == nil then
    log.error('dn.state.set(): Tried to set %s, but no such field exists', full_variable)
    return
  end

  -- Sanity check the types. I wish I was using a real type system, I guess.
  local parent_type = type(parent[key])
  local value_type = type(value)
  if parent_type ~= value_type then
    log.error('dn.state.set(): Tried to set %s (%s) with a value of type %s', full_variable, parent_type,
      value_type)
    return
  end

  parent[key] = value
end

function dn.state.increment(full_variable, step)
  local parent = dn.state.extract_parent(full_variable)
  local key = dn.state.extract_key(full_variable)

  local parent_type = type(parent[key])
  if parent_type ~= 'number' then
    log.error('dn.state.increment(): Tried to increment %s (%s), but it is not a number', full_variable,
      parent_type)
    return
  end

  parent[key] = parent[key] + step
end

function dn.state.extract_key(full_variable)
  local keys = string.split(full_variable, '.')
  return keys[#keys]
end

function dn.state.extract_parent(full_variable)
  local parent = dn.state.data
  local keys = string.split(full_variable, '.')
  for i, key in pairs(keys) do
    if i == #keys then break end
    parent = parent[key]
  end

  return parent
end

function dn.state.get_type(full_variable)
  local state = dn.state.find(full_variable)
  return type(state)
end

function dn.state.is_number(full_variable)
  return dn.state.get_type(full_variable) == 'number'
end

function dn.state.is_boolean(full_variable)
  return dn.state.get_type(full_variable) == 'boolean'
end

function dn.state.is_string(full_variable)
  return dn.state.get_type(full_variable) == 'string'
end

function dn.state.get_sorted_fields()
  -- @hack, supreme laziness furthers
  local copy = table.deep_copy(dn.state.data)
  copy.stats = nil
  local sorted_state = table.flatten(copy)
  table.sort(sorted_state)
  return sorted_state
end

function tdengine.state.init()
  tdengine.state.load_file('default')
end

function tdengine.state.load(state)
  for key, fields in pairs(state) do
    tdengine.state.data[key] = tdengine.state.data[key] or {}
    for field, value in pairs(fields) do
      tdengine.state.data[key][field] = value
    end
  end
end

function tdengine.state.load_file(file_name)
  local state = tdengine.state.read_file(file_name)
  tdengine.state.load(state)
end

function tdengine.state.write(file_name, state)
  for key, value in pairs(tdengine.state.data) do
    value.__editor = nil
  end

  local file_path = tdengine.ffi.resolve_format_path('state', file_name):to_interned()
  state = state or tdengine.state.data

  tdengine.module.write(file_path, state, tdengine.module.WriteOptions.Pretty)
end

function tdengine.state.read_file(file_name)
  local file_path = tdengine.ffi.resolve_format_path('state', file_name):to_interned()
  return tdengine.state.read(file_path)
end

function tdengine.state.read(file_path)
  return dofile(file_path)
end

function tdengine.state.find(full_variable)
  if not full_variable then return false end

  local parent = tdengine.state.extract_parent(full_variable)
  local key = tdengine.state.extract_key(full_variable)

  return parent[key]
end

function tdengine.state.set(full_variable, value)
  local parent = tdengine.state.extract_parent(full_variable)
  local key = tdengine.state.extract_key(full_variable)

  if parent == nil then
    log.error('tdengine.state.set(): Tried to set %s, but parent field does not exist', full_variable)
    return
  end

  if parent[key] == nil then
    log.error('tdengine.state.set(): Tried to set %s, but no such field exists', full_variable)
    return
  end

  -- Sanity check the types. I wish I was using a real type system, I guess.
  local parent_type = type(parent[key])
  local value_type = type(value)
  if parent_type ~= value_type then
    log.error('tdengine.state.set(): Tried to set %s (%s) with a value of type %s', full_variable, parent_type,
      value_type)
    return
  end

  parent[key] = value
end

function tdengine.state.increment(full_variable, step)
  local parent = tdengine.state.extract_parent(full_variable)
  local key = tdengine.state.extract_key(full_variable)

  local parent_type = type(parent[key])
  if parent_type ~= 'number' then
    log.error('tdengine.state.increment(): Tried to increment %s (%s), but it is not a number', full_variable,
      parent_type)
    return
  end

  parent[key] = parent[key] + step
end

function tdengine.state.extract_key(full_variable)
  local keys = string.split(full_variable, '.')
  return keys[#keys]
end

function tdengine.state.extract_parent(full_variable)
  local parent = tdengine.state.data
  local keys = string.split(full_variable, '.')
  for i, key in pairs(keys) do
    if i == #keys then break end
    parent = parent[key]
  end

  return parent
end

function tdengine.state.get_type(full_variable)
  local state = tdengine.state.find(full_variable)
  return type(state)
end

function tdengine.state.is_number(full_variable)
  return tdengine.state.get_type(full_variable) == 'number'
end

function tdengine.state.is_boolean(full_variable)
  return tdengine.state.get_type(full_variable) == 'boolean'
end

function tdengine.state.is_string(full_variable)
  return tdengine.state.get_type(full_variable) == 'string'
end

function tdengine.state.get_sorted_fields()
  -- @hack, supreme laziness furthers
  local copy = table.deep_copy(tdengine.state.data)
  copy.stats = nil
  local sorted_state = table.flatten(copy)
  table.sort(sorted_state)
  return sorted_state
end

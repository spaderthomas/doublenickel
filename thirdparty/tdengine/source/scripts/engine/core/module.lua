function tdengine.write_file_to_return_table(filepath, t, pretty)
  if t == nil then dbg() end

  if pretty == nil then pretty = false end

  local serialized_data = table.serialize(t, pretty)
  local file = assert(io.open(filepath, 'w'))
  if not file then
    log.warn('tdengine.write_file_to_return_table(): cannot open file, file = %s', filepath)
  end

  file:write('return ')
  file:write(serialized_data)
  file:close()
end

tdengine.module.WriteOptions = tdengine.enum.define(
  'ModuleWriteOptions',
  {
    Compact = 1,
    Pretty = 2,
  }
)

function tdengine.module.write_to_named_path(name, data, pretty)
  local file_path = tdengine.ffi.resolve_named_path(name):to_interned()
  local is_pretty = pretty == tdengine.module.WriteOptions.Pretty

  tdengine.write_file_to_return_table(file_path, data, pretty)
end

function tdengine.module.write(file_path, data, pretty)
  local is_pretty = pretty == tdengine.module.WriteOptions.Pretty

  tdengine.write_file_to_return_table(file_path, data, pretty)
end

function tdengine.module.read(file_path)
  return dofile(file_path)
end

function tdengine.module.read_from_named_path(name)
  local file_path = tdengine.ffi.resolve_named_path(name):to_interned()
  return tdengine.module.read(file_path)
end

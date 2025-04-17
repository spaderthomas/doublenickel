function doublenickel.write_file_to_return_table(filepath, t, pretty)
  if t == nil then dbg() end

  if pretty == nil then pretty = false end

  local serialized_data = table.serialize(t, pretty)
  local file = assert(io.open(filepath, 'w'))
  if not file then
    log.warn('doublenickel.write_file_to_return_table(): cannot open file, file = %s', filepath)
  end

  file:write('return ')
  file:write(serialized_data)
  file:close()
end

doublenickel.module.WriteOptions = doublenickel.enum.define(
  'ModuleWriteOptions',
  {
    Compact = 1,
    Pretty = 2,
  }
)

function doublenickel.module.write_to_named_path(name, data, pretty)
  local file_path = dn.paths_resolve(name)
  local is_pretty = pretty == doublenickel.module.WriteOptions.Pretty

  doublenickel.write_file_to_return_table(file_path, data, pretty)
end

function doublenickel.module.write(file_path, data, pretty)
  local is_pretty = pretty == doublenickel.module.WriteOptions.Pretty

  doublenickel.write_file_to_return_table(file_path, data, pretty)
end

function doublenickel.module.read(file_path)
  print(file_path)
  return dofile(file_path)
end

function doublenickel.module.read_from_named_path(name)
  local file_path = dn.paths_resolve(name)
  return doublenickel.module.read(file_path)
end

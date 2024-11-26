function tdengine.extract_filename(path)
  return path:match("([^/\\]+)$")
end

function tdengine.is_extension(path, extension)
  local ext_len = string.len(extension)
  local path_len = string.len(path)
  if ext_len > path_len then return false end

  local last = string.sub(path, path_len - ext_len + 1, path_len)
  return last == extension
end

function tdengine.has_extension(path)
  return string.find(path, '%.')
end

function tdengine.strip_extension(path)
  local extension = tdengine.has_extension(path)
  if not extension then return path end

  return path:sub(1, extension - 1)
end

function tdengine.filesystem.iterate_directory(path)
  local function iterator()
    local entry_list = tdengine.ffi.dn_os_scan_directory(path)
    for entry in tdengine.iterator.carray(entry_list.entries, entry_list.num_entries) do
      coroutine.yield(entry)
    end
  end

  return coroutine.wrap(iterator)
end

function tdengine.filesystem.collect_directory(path)
  local files = {}
  for file in tdengine.filesystem.iterate_directory(path) do
    table.insert(files, file.path:to_interned())
  end

  return files
end

function tdengine.filesystem.collect_named_directory(name)
  local path = tdengine.ffi.dn_paths_resolve(name):to_interned()
  return tdengine.filesystem.collect_directory(path)
end

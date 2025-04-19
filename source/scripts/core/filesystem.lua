function dn.extract_filename(path)
  return path:match("([^/\\]+)$")
end

function dn.is_extension(path, extension)
  local ext_len = string.len(extension)
  local path_len = string.len(path)
  if ext_len > path_len then return false end

  local last = string.sub(path, path_len - ext_len + 1, path_len)
  return last == extension
end

function dn.has_extension(path)
  return string.find(path, '%.')
end

function dn.strip_extension(path)
  local extension = dn.has_extension(path)
  if not extension then return path end

  return path:sub(1, extension - 1)
end

function dn.filesystem.iterate_directory(path)
  local function iterator()
    local entry_list = dn.ffi.os_scan_directory(path)
    for entry in dn.iterator.carray(entry_list.data, entry_list.count) do
      coroutine.yield(entry)
    end
  end

  return coroutine.wrap(iterator)
end

function dn.filesystem.collect_directory(path)
  local files = {}
  for entry in dn.filesystem.iterate_directory(path) do
    table.insert(files, entry.file_name)
  end

  return files
end

function dn.filesystem.collect_named_directory(name)
  local path = dn.ffi.paths_resolve(name)
  return dn.filesystem.collect_directory(path)
end

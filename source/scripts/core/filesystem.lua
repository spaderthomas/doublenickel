function doublenickel.extract_filename(path)
  return path:match("([^/\\]+)$")
end

function doublenickel.is_extension(path, extension)
  local ext_len = string.len(extension)
  local path_len = string.len(path)
  if ext_len > path_len then return false end

  local last = string.sub(path, path_len - ext_len + 1, path_len)
  return last == extension
end

function doublenickel.has_extension(path)
  return string.find(path, '%.')
end

function doublenickel.strip_extension(path)
  local extension = doublenickel.has_extension(path)
  if not extension then return path end

  return path:sub(1, extension - 1)
end

function doublenickel.filesystem.iterate_directory(path)
  local function iterator()
    local entry_list = dn.os_scan_directory(path)
    for entry in doublenickel.iterator.carray(entry_list.data, entry_list.count) do
      coroutine.yield(entry)
    end
  end

  return coroutine.wrap(iterator)
end

function doublenickel.filesystem.collect_directory(path)
  local files = {}
  for entry in doublenickel.filesystem.iterate_directory(path) do
    table.insert(files, entry.file_name:to_interned())
  end

  return files
end

function doublenickel.filesystem.collect_named_directory(name)
  local path = dn.paths_resolve(name):to_interned()
  return doublenickel.filesystem.collect_directory(path)
end

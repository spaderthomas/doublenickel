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
    local entries = tdengine.ffi.dn_os_scan_directory(path)
    local num_entries = tdengine.ffi.dn_dynamic_array_size(entries)
    for i = 0, num_entries - 1 do
      coroutine.yield(entries + i)
    end
  end

  return coroutine.wrap(iterator)
end
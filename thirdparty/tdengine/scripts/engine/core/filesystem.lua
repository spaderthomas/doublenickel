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

function tdengine.scandir(dir)
  local platform = tdengine.platform()
  if platform == 'Windows' then
    local dir = string.format('%s/*', dir)
    return tdengine.scandir_impl(dir)
  end

  local command = 'ls -a "' .. dir .. '"'

  local i, t, popen = 0, {}, io.popen
  local pfile = popen(command)
  for filename in pfile:lines() do
    if filename ~= '.' and filename ~= '..' then
      i = i + 1
      t[i] = filename
    end
  end
  pfile:close()
  return t
end

function tdengine.create_dir(dir)
  local command = string.format('mkdir "%s"', dir)
  os.execute(command)
end

function tdengine.does_path_exist(path)
  return tdengine.ffi.does_path_exist(path)
end

function tdengine.is_regular_file(path)
  return tdengine.ffi.is_regular_file(path)
end

function tdengine.is_directory(path)
  return tdengine.ffi.is_directory(path)
end

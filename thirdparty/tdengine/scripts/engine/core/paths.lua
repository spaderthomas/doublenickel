local self = tdengine.paths

local NamedPath = tdengine.class.define('NamedPath')
tdengine.paths.NamedPath = NamedPath

function NamedPath:init(name, path)
	self.name = name or ''
	self.path = path or ''
end

local function collect_paths(paths, full_parent)
	local collected_paths = tdengine.data_types.Array:new()

	for name, data in pairs(paths) do
		local full_path = ''
		if full_parent then
			full_path = string.format('%s/%s', full_parent, data.path)
		else
			full_path = data.path
		end

		collected_paths:add(NamedPath:new(name, full_path))

		if data.children then
			collected_paths:concatenate(collect_paths(data.children, full_path))
		end
	end

	return collected_paths
end

function tdengine.paths.init()
	local file_path = tdengine.ffi.resolve_named_path('path_info'):to_interned()
	local path_info = tdengine.module.read(file_path)
	self.paths = tdengine.data_types.Array:new()

	local install_paths = collect_paths(path_info.install_paths)
	for index, path in install_paths:iterate() do
		tdengine.ffi.add_install_path(path.name, path.path)
	end

	local write_paths = collect_paths(path_info.write_paths)
	for index, path in write_paths:iterate() do
		tdengine.ffi.add_write_path(path.name, path.path)
	end

	self.paths:concatenate(install_paths)
	self.paths:concatenate(write_paths)
end

function tdengine.paths.iterate()
	local named_paths = tdengine.ffi.find_all_named_paths()
	local i = -1

	local function iterator()
		i = i + 1
		if i < named_paths.size then
			local item = named_paths.named_paths + i
			return ffi.string(item.name), ffi.string(item.path)
		end
	end

	return iterator
end

function tdengine.paths.iterate_no_format()
	local index = 0

	local function iterator()
		index = index + 1
		local item = self.paths:at(index)
		if item then
			local fixed_path = item.path
			fixed_path = fixed_path:gsub('%%', '%%%%')
			return item.name, fixed_path
		end
	end

	return iterator
end

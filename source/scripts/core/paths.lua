local self = dn.paths

local NamedPath = dn.class.define('NamedPath')
dn.paths.NamedPath = NamedPath

function NamedPath:init(name, path, named_parent)
	self.name = name or ''
	self.full_path = path or ''
	self.parent = named_parent or nil
end

local function collect_paths(paths, full_parent)
	local collected_paths = dn.data_types.Array:new()
	if not paths then return collected_paths end

	for name, data in pairs(paths) do
		local path = NamedPath:new(name)
		if full_parent then
			path.full_path = string.format('%s/%s', full_parent, data.path or data)
		elseif data.named_parent then
			path.named_parent = data.named_parent
			path.full_path = data.path
		else
			path.full_path = data.path
		end

		collected_paths:add(path)

		if data.children then
			collected_paths:concatenate(collect_paths(data.children, path.full_path))
		end
	end

	return collected_paths
end

function dn.paths.init()
end

function dn.paths.load(file_path)
	local path_info = dn.module.read(file_path)
	self.paths = dn.data_types.Array:new()

	local install_paths = collect_paths(path_info.install_paths)
	for path in install_paths:iterate_values() do
		path.named_parent = path.named_parent or 'dn_install'
		dn.ffi.paths_add_subpath(path.name, path.named_parent, path.full_path)
	end

	local write_paths = collect_paths(path_info.write_paths)
	for path in write_paths:iterate_values() do
		path.named_parent = path.named_parent or 'dn_write'
		dn.ffi.paths_add_subpath(path.name, path.named_parent, path.full_path)
	end

	local app_paths = collect_paths(path_info.app_paths)
	for path in app_paths:iterate_values() do
		path.named_parent = path.named_parent or 'dn_app'
		dn.ffi.paths_add_subpath(path.name, path.named_parent, path.full_path)
	end

	self.paths:concatenate(install_paths)
	self.paths:concatenate(write_paths)
	self.paths:concatenate(app_paths)
end

function dn.paths.iterate()
	local named_paths = dn.ffi.paths_find_all()
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

function dn.paths.iterate_no_format()
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

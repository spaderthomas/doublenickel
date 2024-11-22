local self = tdengine.paths

local NamedPath = tdengine.class.define('NamedPath')
tdengine.paths.NamedPath = NamedPath

function NamedPath:init(name, path, named_parent)
	self.name = name or ''
	self.full_path = path or ''
	self.parent = named_parent or nil
end

-- local PathRegistry = tdengine.class.define('PathRegistry')
-- function PathRegistry:init(descriptor)
-- 	self.path_info = descriptor.path_info
-- 	self.dialogue = descriptor.dialogue
-- 	self.saves = descriptor.saves
-- 	self.shaders = descriptor.shaders
-- 	self.scenes = descriptor.scenes
-- 	self.layouts = descriptor.layouts
-- 	self.ffi = descriptor.ffi
-- 	self.backgrounds = descriptor.background_info
-- 	self.font_info = descriptor.font_info
-- 	self.animation_info = descriptor.animation_info
-- 	self.texture_info = descriptor.texture_info
-- 	self.character_info = descriptor.character_info
-- end

local function collect_paths(paths, full_parent)
	local collected_paths = tdengine.data_types.Array:new()

	for name, data in pairs(paths) do
		local path = NamedPath:new(name)
		if full_parent then
			path.full_path = string.format('%s/%s', full_parent, data.path)
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

function tdengine.paths.init(path_info)
	local file_path = tdengine.ffi.resolve_named_path('path_info'):to_interned()
	local path_info = tdengine.module.read(file_path)
	self.paths = tdengine.data_types.Array:new()

	local install_paths = collect_paths(path_info.install_paths)
	for path in install_paths:iterate_values() do
		if path.named_parent then
			tdengine.ffi.add_named_subpath(path.name, path.named_parent, path.full_path)
		else
			tdengine.ffi.add_install_path(path.name, path.full_path)
		end
	end

	local write_paths = collect_paths(path_info.write_paths)
	for index, path in write_paths:iterate() do
		tdengine.ffi.add_write_path(path.name, path.full_path)
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

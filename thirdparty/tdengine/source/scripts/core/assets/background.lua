function tdengine.background.load()
	local info = tdengine.ffi.dn_paths_resolve('background_info'):to_interned()
  if tdengine.ffi.dn_os_does_path_exist(info) then
		tdengine.background.data = tdengine.module.read_from_named_path('background_info')
	else
		tdengine.background.data = {}
  end
end

function tdengine.background.save()
	tdengine.module.write_to_named_path('background_info', tdengine.background.data, tdengine.module.WriteOptions.Pretty)
end

function tdengine.background.find(name)
	return tdengine.background.data[name]
end

function tdengine.background.add(name, file)
	tdengine.background.data[name] = {
		mod_time = 0,
		size = {
			x = 0,
			y = 0
		},
		source = file,
		tiles = {}
	}
end

function tdengine.background.remove(name)
	tdengine.background.data[name] = nil
end

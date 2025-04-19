function dn.background.init()
  dn.background.data = {}
	-- local info = dn.ffi.paths_resolve('background_info')
  -- if dn.ffi.os_does_path_exist(info) then
	-- 	dn.background.data = dn.module.read_from_named_path('background_info')
	-- else
	-- 	dn.background.data = {}
  -- end
end

function dn.background.save()
	dn.module.write_to_named_path('background_info', dn.background.data, dn.module.WriteOptions.Pretty)
end

function dn.background.find(name)
	return dn.background.data[name]
end

function dn.background.add(name, file)
	dn.background.data[name] = {
		mod_time = 0,
		size = {
			x = 0,
			y = 0
		},
		source = file,
		tiles = {}
	}
end

function dn.background.remove(name)
	dn.background.data[name] = nil
end

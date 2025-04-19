function doublenickel.background.init()
  doublenickel.background.data = {}
	-- local info = dn.paths_resolve('background_info')
  -- if dn.os_does_path_exist(info) then
	-- 	doublenickel.background.data = doublenickel.module.read_from_named_path('background_info')
	-- else
	-- 	doublenickel.background.data = {}
  -- end
end

function doublenickel.background.save()
	doublenickel.module.write_to_named_path('background_info', doublenickel.background.data, doublenickel.module.WriteOptions.Pretty)
end

function doublenickel.background.find(name)
	return doublenickel.background.data[name]
end

function doublenickel.background.add(name, file)
	doublenickel.background.data[name] = {
		mod_time = 0,
		size = {
			x = 0,
			y = 0
		},
		source = file,
		tiles = {}
	}
end

function doublenickel.background.remove(name)
	doublenickel.background.data[name] = nil
end

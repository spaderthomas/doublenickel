local self = dn.layout

function dn.layout.init()
end

function dn.layout.configure(config)
  if config.directory then
    dn.ffi.paths_add_ex(dn.paths.USER_LAYOUTS, config.directory)
    dn.ffi.paths_add_ex(dn.paths.USER_LAYOUT, config.directory .. '/%s.ini')
  end
end

function dn.layout.load(name)
  ffi.C.dn_imgui_load_layout(String:new(name))
end

function dn.layout.save(name)
  ffi.C.dn_imgui_save_layout(String:new(name))
end

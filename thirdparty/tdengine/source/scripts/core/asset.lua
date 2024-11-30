local self = tdengine.asset

function tdengine.asset.init()
  self.casts = {}
end

function tdengine.asset.register_cast(enum, ctype)
  self.casts[enum.name] = tdengine.ffi.ptr_type(ctype)
end

function tdengine.asset.find(id)
  local ctype = 'dn_asset_data_t'
  if tdengine.enum.is_enum(id) then
    ctype = self.casts[id.__enum] or cast
    id = id:to_qualified_string()
  end

  local asset = tdengine.ffi.dn_assets_find(id)
  return ffi.cast(ctype, asset)
end
local self = doublenickel.asset

function doublenickel.asset.init()
  self.casts = {}
end

function doublenickel.asset.register_cast(enum, ctype)
  self.casts[enum.name] = doublenickel.ffi.ptr_type(ctype)
end

function doublenickel.asset.find(id)
  local ctype = 'dn_asset_data_t'
  if doublenickel.enum.is_enum(id) then
    ctype = self.casts[id.__enum] or cast
    id = id:to_qualified_string()
  end

  local asset = doublenickel.ffi.dn_assets_find(id)
  return ffi.cast(ctype, asset)
end
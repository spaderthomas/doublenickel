local self = doublenickel.asset

function doublenickel.asset.init()
  self.casts = {}
end

function doublenickel.asset.register_cast(enum, ctype)
  self.casts[enum.name] = dn.reflection.ptr_type(ctype)
end

function doublenickel.asset.find(id)
  local ctype = 'dn_asset_data_t'
  if doublenickel.enum.is_enum(id) then
    ctype = self.casts[id.__enum] or ctype
    id = id:to_qualified_string()
  end

  local asset = dn.ffi.asset_registry_find(id)
  return ffi.cast(ctype, asset)
end
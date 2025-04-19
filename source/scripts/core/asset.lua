local self = dn.asset

function dn.asset.init()
  self.casts = {}
end

function dn.asset.register_cast(enum, ctype)
  self.casts[enum.name] = dn.reflection.ptr_type(ctype)
end

function dn.asset.find(id)
  local ctype = 'dn_asset_data_t'
  if dn.enum.is_enum(id) then
    ctype = self.casts[id.__enum] or ctype
    id = id:to_qualified_string()
  end

  local asset = dn.ffi.asset_registry_find(id)
  return ffi.cast(ctype, asset)
end
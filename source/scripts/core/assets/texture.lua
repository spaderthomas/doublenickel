function dn.texture.init()
  -- dn.texture.data = dn.module.read_from_named_path('texture_info')
end

function dn.texture.save()
  dn.module.write_to_named_path('texture_info', dn.texture.data, dn.module.WriteOptions.Pretty)
end

function dn.texture.delete(id)
  dn.texture.data.atlases[id] = nil
end

function dn.texture.find(id)
  if not dn.texture.data.atlases[id] then
    dn.texture.data.atlases[id] = {
      name = id,
      hash = 0,
      mod_time = 0,
      directories = {}
    }
  end
  return dn.texture.data.atlases[id]
end

function dn.internal.clear_texture_cache()
  for name, atlas in pairs(dn.texture.data.atlases) do
    atlas.mod_time = 0
    atlas.hash = 0
  end

  dn.texture.data.files = {}
end

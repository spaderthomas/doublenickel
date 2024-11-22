function tdengine.texture.load()
  tdengine.texture.data = tdengine.module.read_from_named_path('texture_info')
end

function tdengine.texture.save()
  tdengine.module.write_to_named_path('texture_info', tdengine.texture.data, tdengine.module.WriteOptions.Pretty)
end

function tdengine.texture.delete(id)
  tdengine.texture.data.atlases[id] = nil
end

function tdengine.texture.find(id)
  if not tdengine.texture.data.atlases[id] then
    tdengine.texture.data.atlases[id] = {
      name = id,
      hash = 0,
      mod_time = 0,
      directories = {}
    }
  end
  return tdengine.texture.data.atlases[id]
end

function tdengine.internal.clear_texture_cache()
  for name, atlas in pairs(tdengine.texture.data.atlases) do
    atlas.mod_time = 0
    atlas.hash = 0
  end

  tdengine.texture.data.files = {}
end

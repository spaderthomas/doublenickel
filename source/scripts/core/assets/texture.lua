function doublenickel.texture.init()
  -- doublenickel.texture.data = doublenickel.module.read_from_named_path('texture_info')
end

function doublenickel.texture.save()
  doublenickel.module.write_to_named_path('texture_info', doublenickel.texture.data, doublenickel.module.WriteOptions.Pretty)
end

function doublenickel.texture.delete(id)
  doublenickel.texture.data.atlases[id] = nil
end

function doublenickel.texture.find(id)
  if not doublenickel.texture.data.atlases[id] then
    doublenickel.texture.data.atlases[id] = {
      name = id,
      hash = 0,
      mod_time = 0,
      directories = {}
    }
  end
  return doublenickel.texture.data.atlases[id]
end

function doublenickel.internal.clear_texture_cache()
  for name, atlas in pairs(doublenickel.texture.data.atlases) do
    atlas.mod_time = 0
    atlas.hash = 0
  end

  doublenickel.texture.data.files = {}
end

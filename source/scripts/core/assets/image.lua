dn.layers = {
  editor = 5,
  background = 5,
  foreground = 25,
  lab_bottom = 30,
  default_particle = 31,
  page = 32,
  door = 45,
  dialogue = 50,
  dialogue_buttons = 51,
  dialogue_text = 51,
  dialogue_button_text = 52,
  editor_overlay = 90,
  ui = 100,
}

function dn.animation.init()
  dn.animation.data = {}
  -- local info = dn.ffi.paths_resolve('animation_info')
  -- if dn.ffi.os_does_path_exist(info) then
  --   dn.animation.data = dn.module.read_from_named_path('animation_info')
  -- else
  --   dn.animation.data = {}
  -- end

end

function dn.animation.save()
  dn.module.write_to_named_path('animation_info', dn.animation.data, dn.module.WriteOptions.Pretty)
end

function dn.animation.find(animation)
  for name, data in dn.animation.iterate() do
    if name == animation then return data end
  end

  return {
    speed = 1,
    frames = {
      { image = '64.png' },
      { image = '128.png' },
    }
  }
end

function dn.animation.add(name, data)
  dn.animation.data[name] = table.deep_copy(data)
end

function dn.animation.remove(name)
  dn.animation.data[name] = nil
end

function dn.animation.iterate()
  return pairs(dn.animation.data)
end

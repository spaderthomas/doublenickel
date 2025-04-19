doublenickel.layers = {
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

function doublenickel.animation.init()
  doublenickel.animation.data = {}
  -- local info = dn.paths_resolve('animation_info')
  -- if dn.os_does_path_exist(info) then
  --   doublenickel.animation.data = doublenickel.module.read_from_named_path('animation_info')
  -- else
  --   doublenickel.animation.data = {}
  -- end

end

function doublenickel.animation.save()
  doublenickel.module.write_to_named_path('animation_info', doublenickel.animation.data, doublenickel.module.WriteOptions.Pretty)
end

function doublenickel.animation.find(animation)
  for name, data in doublenickel.animation.iterate() do
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

function doublenickel.animation.add(name, data)
  doublenickel.animation.data[name] = table.deep_copy(data)
end

function doublenickel.animation.remove(name)
  doublenickel.animation.data[name] = nil
end

function doublenickel.animation.iterate()
  return pairs(doublenickel.animation.data)
end

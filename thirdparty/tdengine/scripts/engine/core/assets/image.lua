tdengine.layers = {
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

function tdengine.animation.load()
  tdengine.animation.data = tdengine.module.read_from_named_path('animation_info')
end

function tdengine.animation.save()
  tdengine.module.write_to_named_path('animation_info', tdengine.animation.data, tdengine.module.WriteOptions.Pretty)
end

function tdengine.animation.find(animation)
  for name, data in tdengine.animation.iterate() do
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

function tdengine.animation.add(name, data)
  tdengine.animation.data[name] = table.deep_copy(data)
end

function tdengine.animation.remove(name)
  tdengine.animation.data[name] = nil
end

function tdengine.animation.iterate()
  return pairs(tdengine.animation.data)
end

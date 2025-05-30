local PlaySound = dn.node('PlaySound')

PlaySound.editor_fields = {
  'sound',
  'volume'
}

function PlaySound:init()
  self.sound = 'debug.wav'
  self.volume = 1.0
end

function PlaySound:enter(graph)
  self.handle = dn.audio.play(self.sound)
  dn.audio.set_volume(self.handle, self.volume)

  return dialogue_state.advancing
end

function PlaySound:advance(graph)
  return simple_advance(self, graph)
end

function PlaySound:short_text()
  return short_text(self.sound)
end

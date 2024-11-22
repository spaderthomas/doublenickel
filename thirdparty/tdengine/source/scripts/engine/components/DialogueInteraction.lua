local DialogueInteraction = tdengine.component.define('DialogueInteraction')

DialogueInteraction.editor_fields = {
  'dialogue'
}

function DialogueInteraction:init(params)
  self.dialogue = params.dialogue or 'test'
end

function DialogueInteraction:play()
  local interaction = self:get_entity():find_component('Interaction')
  interaction:add_callback(function()
    tdengine.dialogue.play(self.dialogue)
  end)
end

function DialogueInteraction:update()
end

function DialogueInteraction:draw()
end

local Position = tdengine.component.define('Position')

Position.editor_fields = {
  'x',
  'y'
}

function Position:init(params)
  self.x = params.x or 0
  self.y = params.y or 0
end

function Position:update()
end

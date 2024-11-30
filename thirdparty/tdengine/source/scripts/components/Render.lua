Render = tdengine.component.define('Render')

Render.editor_fields = {
  'render_pass'
}

function Render:init(params)
  self.render_pass = params.render_pass or 'scene'
end

function Render:update()
end

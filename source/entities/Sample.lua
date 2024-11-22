SampleEntity = tdengine.entity.define('SampleEntity')

SampleEntity.components = {
  'Collider'
}


function SampleEntity:init() 
  self.__draw_shapes = false
end

function SampleEntity:late_init()
end 

function SampleEntity:deinit()
end

function SampleEntity:play()
  self.positions = {}
  self.jitters = {}
  for i = 0, 20 do
    table.insert(self.positions, tdengine.math.random_float(0, 400))
    table.insert(self.jitters, tdengine.math.random_float(0, 1.0))
  end
end

function SampleEntity:stop()
end

function SampleEntity:update()
end

function SampleEntity:draw()
  self:draw_shapes()


  tdengine.ffi.draw_image_ex('animal-well-320-180.png', 0, -100, 320, 180, 1)


  for i = 1, 20 do
    tdengine.ffi.set_layer(i)
    local yoff = tdengine.math.ranged_sin(tdengine.elapsed_time * self.jitters[i], 0.0, 20.0)
    tdengine.ffi.draw_circle_sdf(self.positions[i], 200 + yoff, 8, tdengine.colors.zomp:to_vec4(), 2)
  end

  local tone_map = function(x)
    -- return math.pow(x, tdengine.math.ranged_sin(tdengine.elapsed_time, 0, 1))
    return math.pow(x, 1.8)
  end

  local step = .025
  local scale = 100
  tdengine.editor.find('EditorUtility'):plot_function(tone_map, 0, 1, step, scale, tdengine.colors.spring_green)

end

-- .75, .82, .92

function SampleEntity:on_load_game()
  -- This is called directly after your fields have been deserialized and assigned.
end

function SampleEntity:on_save_game()
  -- This is called right before your fields are serialized
end


function SampleEntity:draw_shapes()
  if not self.__draw_shapes then return end
  tdengine.ffi.set_world_space(true)
  tdengine.ffi.set_layer(100)

  tdengine.ffi.draw_circle_sdf(0, -100, 100, tdengine.colors.zomp:to_vec4(), 2)
  -- tdengine.ffi.draw_circle_l(tdengine.vec2(0, -100), 100, tdengine.colors.zomp, 2)
  tdengine.ffi.draw_quad(100, 0, 200, 200, tdengine.colors.charcoal:to_vec4())
  --tdengine.ffi.draw_quad_l(tdengine.vec2(100, 0), tdengine.vec2(200, 200), tdengine.colors.charcoal)
  tdengine.ffi.draw_line(500, -200, 300, 0, 2, tdengine.colors.indian_red:to_vec4())
  tdengine.ffi.draw_line(300, -200, 500, 0, 2, tdengine.colors.indian_red:to_vec4())
  tdengine.ffi.draw_image_size('studio-logo.png', 500, 0, 200, 200)
end
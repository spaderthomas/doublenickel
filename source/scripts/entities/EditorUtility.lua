EditorUtility = doublenickel.editor.define('EditorUtility')

function EditorUtility:init()
  self.enabled = {
    grid = true,
    plot = false,
    pallette = false,
  }

  self.style = {
    grid = {
      draw_axes = true,
      draw_body = true,
      size = 50
    },
    plot = {
      point_size = 2,
      derivative_length = 200,
      derivative_width = 4
    },
  }

  self.colors = {
    grid = doublenickel.colors.grid_bg:copy(),
    axis = {
      x = doublenickel.colors.indian_red:copy(),
      y = doublenickel.colors.spring_green:copy(),
    },
    plot = doublenickel.colors.white:alpha(.25),
  }

  self.layers = {
    pallette = 99,
    plot = 120
  }

  self.input = ContextualInput:new(doublenickel.enums.InputContext.Game, doublenickel.enums.CoordinateSystem.World)
end


function EditorUtility:update()
end

function EditorUtility:draw()
  self:draw_grid()
  self:draw_plot()
  self:draw_pallette()
end

function EditorUtility:draw_pallette()
  if not self.enabled.pallette then return end

  local grid_cell = doublenickel.vec2(0, 1)

  doublenickel.ffi.set_world_space(true)
  doublenickel.ffi.set_layer(self.layers.pallette)

  for category, color_names in pairs(doublenickel.colors.pallette) do
    for _, color_name in pairs(color_names) do
      local color = doublenickel.colors[color_name]
      local position = self:grid_to_world2(grid_cell)

      grid_cell.y = grid_cell.y + 1

      local text_color = doublenickel.colors.white
      local readable = color:readable_color()
      if readable == doublenickel.enums.ReadableTextColor.Light then
        text_color = doublenickel.colors.white
      else
        text_color = doublenickel.colors.black
      end

      local label = dn.prepare_text_ex(color_name,
        0, 0,
        'merriweather-bold-32',
        0,
        text_color:to_vec4(),
        true)

      label.position.x = position.x + (self.style.grid.size / 2)
      label.position.y = position.y - (self.style.grid.size - label.height) / 2

      doublenickel.ffi.draw_quad(position.x, position.y, self.style.grid.size * 8, self.style.grid.size, color:to_vec4())
      doublenickel.ffi.draw_prepared_text(label)
    end
    grid_cell.y = grid_cell.y + 1
  end
end

function EditorUtility:draw_grid()
  if not self.enabled.grid then return end

  doublenickel.ffi.set_world_space(true)
  doublenickel.ffi.set_layer(doublenickel.editor.layers.grid)

  local grid_size = self.style.grid.size
  local line_thickness = 1

  local size = doublenickel.gpu.find(RenderTarget.Editor).size
  local camera = doublenickel.editor.find('EditorCamera')
  if doublenickel.tick then
    local game_camera = doublenickel.entity.find('Camera')
    camera = game_camera or camera
  end
  local slop = 300

  local min = doublenickel.vec2(
    camera.offset.x - slop,
    camera.offset.y - slop
  )
  min.x = min.x - doublenickel.math.fmod(min.x, grid_size)
  min.y = min.y - doublenickel.math.fmod(min.y, grid_size)

  local max = doublenickel.vec2(
    camera.offset.x + size.x + doublenickel.math.fmod(size.x, grid_size) + slop,
    camera.offset.y + size.y + doublenickel.math.fmod(size.y, grid_size) + slop
  )

  if self.style.grid.draw_body then
    local i = 0
    -- Draw vertical lines
    for x = min.x, max.x, grid_size do  
      i = i + 1
      doublenickel.ffi.draw_line(x, min.y, x, max.y, line_thickness, self.colors.grid:to_vec4())
    end

    -- Draw horizontal lines
    for y = min.y, max.y, grid_size do
      i = i + 1
      doublenickel.ffi.draw_line(min.x, y, max.x, y, line_thickness, self.colors.grid:to_vec4())
    end


  end


  if self.style.grid.draw_axes then
    doublenickel.ffi.draw_line(1, min.y, 1, max.y, line_thickness,
      self.colors.axis.x:to_vec4())
    doublenickel.ffi.draw_line(min.x, 0, max.x, 0, line_thickness,
      self.colors.axis.y:to_vec4())
  end
end

function EditorUtility:mouse_to_grid()
  local mouse = self.input:mouse()
  return mouse:scale(1 / self.style.grid.size):floor()
end

function EditorUtility:grid_to_world(x, y)
  return doublenickel.vec2(x, y):scale(self.style.grid.size)
end

function EditorUtility:grid_to_world2(grid_cell)
  return self:grid_to_world(grid_cell:unpack())
end

function EditorUtility:draw_plot()
  if not self.enabled.plot then return end

  local radius = 1

  local lague_spiky_volume = doublenickel.math.pi * math.pow(radius, 4.0) / 6.0;
  local lague_spiky = function(x)
    return math.pow(radius - math.abs(x), 2.0) / lague_spiky_volume
  end

  local lague_smooth_volume = doublenickel.math.pi * math.pow(radius, 8.0) / 4.0
  local lague_smooth = function(x)
    return math.pow(radius * radius - x * x, 3) / lague_smooth_volume
  end

  local lague_smooth_derivative = function(x)
    local f = radius * radius - x * x
    local scale = -24 / (doublenickel.math.pi * math.pow(radius, 8))
    return scale * x * f * f
  end

  local muller_spiky_volume = 15.0 / (doublenickel.math.pi * math.pow(radius, 6));
  local muller_spiky = function(x)
    return math.pow(radius - math.abs(x), 3) / muller_spiky_volume
  end

  local muller_smooth_volume = 315 / (64 * doublenickel.math.pi * math.pow(radius, 9))
  local muller_smooth = function(x)
    return math.pow(math.pow(radius, 2) - math.pow(math.abs(x), 2), 3) / muller_smooth_volume
  end

  local muller_viscosity = function(x)
    local scale = 45.0 / (doublenickel.math.pi * math.pow(radius, 6))
    return scale * (radius - math.abs(x))
  end

  local step = .025
  self:plot_function(lague_spiky, -1, 1, step, 500, doublenickel.colors.mindaro)
  self:plot_function(lague_smooth, -1, 1, step, 500, doublenickel.colors.cadet_gray)
  self:plot_function(muller_spiky, -1, 1, step, 500, doublenickel.colors.indian_red)
  self:plot_function(muller_smooth, -1, 1, step, 500, doublenickel.colors.spring_green)
  self:plot_function(muller_viscosity, -1, 1, step, 50, doublenickel.colors.spring_green)

  self:plot_derivative(lague_smooth, lague_smooth_derivative, -1, 1, 500, doublenickel.colors.indian_red:copy())
end

function EditorUtility:plot_function(f, xmin, xmax, step, scale, color)
  scale = scale or 1
  color = color or self.colors.plot

  doublenickel.ffi.set_world_space(true)
  doublenickel.ffi.set_layer(self.layers.plot)

  for x = xmin, xmax, step do
    -- doublenickel.ffi.draw_circle(x * scale, f(x) * scale, self.style.plot.point_size, color:to_vec4())
    doublenickel.ffi.draw_circle_sdf(x * scale, f(x) * scale, self.style.plot.point_size, color:to_vec4(), 2)
  end

  doublenickel.ffi.set_layer(self.layers.plot + 1)

  local x = doublenickel.math.clamp(self.input:mouse().x / scale, xmin, xmax)
  doublenickel.ffi.draw_text_ex(
    string.format('%.3f -> %.3f', x, f(x)), 
    x * scale, f(x) * scale, 
    doublenickel.colors.white:to_vec4(), 
    'editor-16', 
    0)
  doublenickel.ffi.draw_circle_sdf(x * scale, f(x) * scale, self.style.plot.point_size * 1.5, doublenickel.colors.cardinal:to_vec4(), 2)

end

function EditorUtility:plot_derivative(f, df, xmin, xmax, scale, color)
  scale = scale or 1
  color = color or self.colors.plot

  -- Calculate f(x) and df(x), using the mouse as the input point
  local mouse = self.input:mouse()

  local point = doublenickel.vec2()
  point.x = doublenickel.math.clamp(mouse.x / scale, xmin, xmax)
  point.y = f(point.x)
  
  local delta = doublenickel.vec2(1, df(point.x))

  -- Scale everything to screen coordinates
  local scaled_point = point:scale(scale)
  local scaled_delta = delta:scale(scale):normalize():scale(self.style.plot.derivative_length)
  
  -- Draw it
  doublenickel.ffi.set_world_space(true)
  doublenickel.ffi.set_layer(self.layers.plot + 1)

  doublenickel.ffi.draw_circle(scaled_point.x, scaled_point.y, self.style.plot.point_size * 4, color:to_vec4())
  doublenickel.ffi.draw_line(
    scaled_point.x - scaled_delta.x, scaled_point.y - scaled_delta.y,
    scaled_point.x + scaled_delta.x, scaled_point.y + scaled_delta.y,
    self.style.plot.derivative_width, 
    color:to_vec4())

  local label = dn.prepare_text_ex(
    string.format('%.3f', delta.y / delta.x),
    scaled_point.x, scaled_point.y,
    'merriweather-bold-48',
    0,
    doublenickel.colors.white:to_vec4(), 
    true)
  doublenickel.ffi.draw_prepared_text(label)
end

EulerianFluidSystem = tdengine.entity.define('EulerianFluidSystem')

EulerianFluidSystem.editor_fields = {
  'grid_size',
  'render_size',
  'start_disabled',
}

function EulerianFluidSystem:init(params)
  params = params or {}
  self.grid_size = params.grid_size or 100
  self.render_size = params.render_size or 1000
  self.buffer_width = self.grid_size + 2
  self.start_disabled = ternary(params.start_disabled, true, false)
end

function EulerianFluidSystem:play()
  if self.start_disabled then return end

  self:enable()
end

function EulerianFluidSystem:stop(params)
  if not self.handle then return end

  self:disable()
end

function EulerianFluidSystem:enable()
  self.handle = tdengine.ffi.ef_create(self.grid_size)
  tdengine.ffi.ef_init(self.handle)
  tdengine.ffi.ef_set_render_size(self.handle, self.render_size)

  for i = 0, self.grid_size - 1, 1 do
    for j = 0, self.grid_size - 1, 1 do
      tdengine.ffi.ef_set_velocity(self.handle, i, j, 1.0, 1.0)
    end
  end
end

function EulerianFluidSystem:disable()
  if not self.handle then return end
  tdengine.ffi.ef_destroy(self.handle)
  self.handle = nil
end

function EulerianFluidSystem:update()
  if not self.handle then return end

  tdengine.ffi.set_active_shader('fluid_eulerian')
  tdengine.ffi.set_draw_mode(tdengine.enums.DrawMode.Triangles)
  tdengine.ffi.set_world_space(true)
  tdengine.ffi.set_layer(10000)
  tdengine.ffi.ef_bind(self.handle)
  tdengine.ffi.push_quad(0, self.render_size, self.render_size, self.render_size, nil, 1.0)

  tdengine.ffi.ef_set_density_source(self.handle, 1, 1, 100.0)
  --tdengine.ffi.ef_set_density_source(self.handle, 10, 10, 100.0)
  --tdengine.ffi.ef_set_density_source(self.handle, 50, 50, 1000.0)
end

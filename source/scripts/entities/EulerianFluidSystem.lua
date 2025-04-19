EulerianFluidSystem = doublenickel.entity.define('EulerianFluidSystem')

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
  self.handle = dn.unported.ef_create(self.grid_size)
  dn.unported.ef_init(self.handle)
  dn.unported.ef_set_render_size(self.handle, self.render_size)

  for i = 0, self.grid_size - 1, 1 do
    for j = 0, self.grid_size - 1, 1 do
      dn.unported.ef_set_velocity(self.handle, i, j, 1.0, 1.0)
    end
  end
end

function EulerianFluidSystem:disable()
  if not self.handle then return end
  dn.unported.ef_destroy(self.handle)
  self.handle = nil
end

function EulerianFluidSystem:update()
  if not self.handle then return end

  dn.unported.set_active_shader('fluid_eulerian')
  dn.unported.set_draw_mode(doublenickel.enums.DrawMode.Triangles)
  dn.unported.set_world_space(true)
  dn.unported.set_layer(10000)
  dn.unported.ef_bind(self.handle)
  dn.unported.push_quad(0, self.render_size, self.render_size, self.render_size, nil, 1.0)

  dn.unported.ef_set_density_source(self.handle, 1, 1, 100.0)
  --dn.unported.ef_set_density_source(self.handle, 10, 10, 100.0)
  --dn.unported.ef_set_density_source(self.handle, 50, 50, 1000.0)
end

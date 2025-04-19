LagrangianFluidSystem = dn.entity.define('LagrangianFluidSystem')

LagrangianFluidSystem.editor_fields = {
  'bounding_volume',
  'num_particles',
  'start_disabled',
  'fluid'
}

function LagrangianFluidSystem:init(params)
  params = params or {}
  self.bounding_volume = BoundingVolume:new(params.bounding_volume)
  self.num_particles = params.num_particles or 10000
  self.start_disabled = ternary(params.start_disabled, true, false)
  self.fluid = params.fluid or {
    smoothing_radius = 4,
    particle_mass = 1,
    viscosity = 50,
    pressure = 100,
    gravity = 200,
    dt = 1.0 / 60.0
  }

  self.velocity = dn.vec2()

  self.colors = {
    bounding_volume = dn.colors.paynes_gray:copy()
  }
  self.layers = {
    bounding_volume = 100
  }

  self.__editor_controls = {
    draw_bounding_volumes = false
  }
end

function LagrangianFluidSystem:play()
  if not self.start_disabled then
    self:enable()
  end
end

function LagrangianFluidSystem:stop(params)
  if not self.handle then return end

  self:disable()
end

function LagrangianFluidSystem:enable()
  self.handle = dn.unported.lf_create(self.num_particles)
  self:sync()
  dn.unported.lf_init(self.handle)
  --dn.unported.lf_inspect(self.handle)
end

function LagrangianFluidSystem:disable()
  dn.unported.lf_destroy(self.handle)
  self.handle = nil
end

function LagrangianFluidSystem:update()
  if not self.handle then return end

  self:sync()
  --dn.unported.lf_inspect(self.handle)
end

function LagrangianFluidSystem:sync()
  dn.unported.lf_set_velocity(self.handle, self.velocity.x, self.velocity.y)

  dn.unported.lf_set_volume(
    self.handle,
    self.bounding_volume.a.x, self.bounding_volume.a.y,
    self.bounding_volume.b.x, self.bounding_volume.b.y,
    self.bounding_volume.radius)

  dn.unported.lf_set_smoothing_radius(self.handle, self.fluid.smoothing_radius)
  dn.unported.lf_set_particle_mass(self.handle, self.fluid.particle_mass);
  dn.unported.lf_set_viscosity(self.handle, self.fluid.viscosity);
  dn.unported.lf_set_pressure(self.handle, self.fluid.pressure);
  dn.unported.lf_set_gravity(self.handle, self.fluid.gravity);
  dn.unported.lf_set_timestep(self.handle, self.fluid.dt);
end

function LagrangianFluidSystem:draw()
  if not self.__editor_controls.draw_bounding_volumes then return end


  local color = self.colors.bounding_volume:alpha(.2):premultiply():to_vec4()

  dn.unported.set_world_space(true)()
  dn.unported.set_layer(self.layers.bounding_volume)

  dn.unported.draw_circle(self.bounding_volume.a.x, self.bounding_volume.a.y, self.bounding_volume.radius, color)
  dn.unported.draw_circle(self.bounding_volume.b.x, self.bounding_volume.b.y, self.bounding_volume.radius, color)

  local distance = self.bounding_volume.b:subtract(self.pa)
  dn.unported.draw_line(
    ffi.new('dn_vector2_t', self.bounding_volume.a.x, self.bounding_volume.a.y),
    ffi.new('dn_vector2_t', self.bounding_volume.b.x, self.bounding_volume.b.y),
    self.bounding_volume.radius * 2,
    color)

end

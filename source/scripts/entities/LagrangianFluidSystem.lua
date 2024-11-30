LagrangianFluidSystem = doublenickel.entity.define('LagrangianFluidSystem')

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

  self.velocity = doublenickel.vec2()

  self.colors = {
    bounding_volume = doublenickel.colors.paynes_gray:copy()
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
  self.handle = doublenickel.ffi.lf_create(self.num_particles)
  self:sync()
  doublenickel.ffi.lf_init(self.handle)
  --doublenickel.ffi.lf_inspect(self.handle)
end

function LagrangianFluidSystem:disable()
  doublenickel.ffi.lf_destroy(self.handle)
  self.handle = nil
end

function LagrangianFluidSystem:update()
  if not self.handle then return end

  self:sync()
  --doublenickel.ffi.lf_inspect(self.handle)
end

function LagrangianFluidSystem:sync()
  doublenickel.ffi.lf_set_velocity(self.handle, self.velocity.x, self.velocity.y)

  doublenickel.ffi.lf_set_volume(
    self.handle,
    self.bounding_volume.a.x, self.bounding_volume.a.y,
    self.bounding_volume.b.x, self.bounding_volume.b.y,
    self.bounding_volume.radius)

  doublenickel.ffi.lf_set_smoothing_radius(self.handle, self.fluid.smoothing_radius)
  doublenickel.ffi.lf_set_particle_mass(self.handle, self.fluid.particle_mass);
  doublenickel.ffi.lf_set_viscosity(self.handle, self.fluid.viscosity);
  doublenickel.ffi.lf_set_pressure(self.handle, self.fluid.pressure);
  doublenickel.ffi.lf_set_gravity(self.handle, self.fluid.gravity);
  doublenickel.ffi.lf_set_timestep(self.handle, self.fluid.dt);
end

function LagrangianFluidSystem:draw()
  if not self.__editor_controls.draw_bounding_volumes then return end


  local color = self.colors.bounding_volume:alpha(.2):premultiply():to_vec4()

  doublenickel.ffi.set_world_space(true)()
  doublenickel.ffi.set_layer(self.layers.bounding_volume)

  doublenickel.ffi.draw_circle(self.bounding_volume.a.x, self.bounding_volume.a.y, self.bounding_volume.radius, color)
  doublenickel.ffi.draw_circle(self.bounding_volume.b.x, self.bounding_volume.b.y, self.bounding_volume.radius, color)

  local distance = self.bounding_volume.b:subtract(self.pa)
  doublenickel.ffi.draw_line(
    ffi.new('dn_vector2_t', self.bounding_volume.a.x, self.bounding_volume.a.y),
    ffi.new('dn_vector2_t', self.bounding_volume.b.x, self.bounding_volume.b.y),
    self.bounding_volume.radius * 2,
    color)

end

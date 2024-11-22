FluidSimulation = tdengine.entity.define('FluidSimulation')

FluidSimulation.sims = tdengine.enum.define(
  'FluidSimKind',
  {
    Idle = 0,
    Euler = 1,
    Lagrange = 2,
  }
)

FluidSimulation.euler = tdengine.enum.define(
  'EulerState',
  {
    Idle = 0,
    DrawVelocityField = 1,
  }
)

FluidSimulation.lagrange_states = tdengine.enum.define(
  'LagrangeState',
  {
    Idle = 0,
    FollowMouse = 1,
    InterpolateChoose = 2,
    InterpolateWait = 3,
  }
)

function FluidSimulation:init()
  self.state = self.lagrange_states.Idle
  self.sim = self.sims.Idle

  self.hotkeys = {
    lagrange = glfw.keys.L,
    follow_mouse = glfw.keys.ONE,
    interpolate_to = glfw.keys.TWO,
    left = glfw.keys.LEFT,
    right = glfw.keys.RIGHT,
    smaller = glfw.keys.DOWN,
    larger = glfw.keys.UP,

    euler = glfw.keys.E,
    inspect = glfw.keys.TAB,
  }

  self.input = ContextualInput:new(tdengine.enums.InputContext.Game, tdengine.enums.CoordinateSystem.World)
end

function FluidSimulation:update()
  if self.sim == self.sims.Idle then
    self:update_idle()
  elseif self.sim == self.sims.Euler then
    self:update_euler()
  elseif self.sim == self.sims.Lagrange then
    self:update_lagrange()
  end
end

function FluidSimulation:update_idle()
  if not tdengine.find_entity_editor('GameViewManager').hover then return end

  if self.input:pressed(self.hotkeys.lagrange) then
    tdengine.find_entity('LagrangianFluidSystem'):enable()
    self.sim = self.sims.Lagrange
    self.state = self.lagrange_states.Idle
  elseif self.input:pressed(self.hotkeys.euler) then
    local sim = tdengine.find_entity('EulerianFluidSystem')
    sim:enable()

    local camera = tdengine.entity.find('Camera')
    local nx, ny = tdengine.app.native_resolution:unpack()
    local center = tdengine.vec2((nx - sim.render_size) / -2, (ny - sim.render_size) / -2)
    local interpolation = tdengine.interpolation.EaseInOut2:new({ exponent = 3, time = 1 })
    camera:interpolate_to(center, interpolation)
    camera:stop_after_interpolate()

    self.sim = self.sims.Euler
    self.state = self.lagrange_states.Idle
  end
end

function FluidSimulation:update_euler()
  local editor = tdengine.editor.find('EditorUtility')
  local sim = tdengine.entity.find('EulerianFluidSystem')

  if self.input:pressed(self.hotkeys.inspect) then
    tdengine.ffi.ef_inspect(sim.handle)
  end

  local px_per_grid = sim.render_size / sim.buffer_width
  editor.style.grid.size = math.floor(px_per_grid)
  editor.style.grid.draw = true

  local mouse = editor:mouse_to_grid()
  mouse = mouse:subtract(tdengine.vec2(1, 1))
  mouse = mouse:clamp(tdengine.vec2(0, 0), tdengine.vec2(sim.grid_size, sim.grid_size))
end

function FluidSimulation:update_lagrange()
  if not self.sim == self.sims.Lagrange then return end

  if self.input:pressed(self.hotkeys.lagrange) then
    tdengine.find_entity('LagrangianFluidSystem'):disable()
    self.sim = self.sims.Idle
  end


  local particle_system = tdengine.find_entity('LagrangianFluidSystem')

  particle_system.velocity = tdengine.vec2()
  if self.state == self.lagrange_states.Idle then
    if self.input:pressed(self.hotkeys.follow_mouse) then
      self.state = self.lagrange_states.FollowMouse
      return
    elseif self.input:pressed(self.hotkeys.interpolate_to) then
      self.state = self.lagrange_states.InterpolateChoose
      return
    end

    self.speed = 4
    if self.input:down(self.hotkeys.left) then
      local delta = tdengine.vec2(-self.speed, 0)
      particle_system.velocity = delta
      particle_system.bounding_volume.a:update(delta)
    end
    if self.input:down(self.hotkeys.right) then
      local delta = tdengine.vec2(self.speed, 0)
      particle_system.velocity = delta
      particle_system.bounding_volume.a:update(delta)
    end
    if self.input:down(self.hotkeys.smaller) then
      particle_system.bounding_volume.radius = particle_system.bounding_volume.radius - self.speed
    end
    if self.input:down(self.hotkeys.larger) then
      particle_system.bounding_volume.radius = particle_system.bounding_volume.radius + self.speed
    end

  elseif self.state == self.lagrange_states.FollowMouse then
    if self.input:pressed(self.hotkeys.follow_mouse) then
      self.state = self.lagrange_states.Idle
      return
    elseif self.input:pressed(self.hotkeys.interpolate_to) then
      self.state = self.lagrange_states.InterpolateChoose
      return
    end

    local position = self.input:mouse()
    local delta = position:subtract(particle_system.bounding_volume.a)
    particle_system.velocity = delta
    particle_system.bounding_volume.a:assign(position)

  elseif self.state == self.lagrange_states.InterpolateChoose then
    if self.input:pressed(self.hotkeys.follow_mouse) then
      self.state = self.lagrange_states.FollowMouse
      return
    elseif self.input:pressed(self.hotkeys.interpolate_to) then
      self.state = self.lagrange_states.Idle
      return
    end

    local mouse = self.input:mouse()

    tdengine.ffi.set_world_space(true)
    tdengine.ffi.set_layer(100)
    tdengine.ffi.draw_circle_sdf(mouse.x, mouse.y, 12, tdengine.colors.spring_green:alpha(.5):to_vec4(), 2)

    if self.input:pressed(glfw.keys.MOUSE_BUTTON_1) then
      self.interpolation = tdengine.interpolation.EaseInOut2:new({
        start = tdengine.vec2(),
        target = mouse:subtract(particle_system.bounding_volume.a),
        exponent = 3,
        time = 2
      })
      self.base_a = particle_system.bounding_volume.a:copy()
      self.base_b = particle_system.bounding_volume.b:copy()

      self.state = self.lagrange_states.InterpolateWait
    end
  elseif self.state == self.lagrange_states.InterpolateWait then
    if self.interpolation:update() then
      self.state = self.lagrange_states.Idle
    end

    local offset = self.interpolation:get_value()

    local last_a = particle_system.bounding_volume.a:copy()

    particle_system.bounding_volume.a:assign(self.base_a:add(offset))
    particle_system.bounding_volume.b:assign(self.base_b:add(offset))
    particle_system.velocity = particle_system.bounding_volume.a:subtract(last_a)
  end

  tdengine.gpu.bind_render_pass('fluid')
  tdengine.ffi.lf_update(particle_system.handle)
  tdengine.ffi.lf_draw(particle_system.handle)
  tdengine.gpu.submit_render_pass('fluid')

end

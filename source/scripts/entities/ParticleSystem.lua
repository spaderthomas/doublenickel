---------------------
-- PARTICLE SYSTEM --
---------------------
ParticleSystem = doublenickel.entity.define('ParticleSystem')

ParticleSystem.components = {
  'Collider'
}

ParticleSystem.editor_fields = {
  'size',
  'color',
  'layer',
  'image',
  'position_mode',
  'lifetime',
  'spawn_rate',
  'velocity_base',
  'velocity_max',
  'velocity_jitter',
  'jitter_base_velocity',
  'jitter_max_velocity',
  'warmup',
  'particle_kind',
  'particle_data',
  'gravity_source',
  'gravity_intensity',
  'gravity_enabled',
  'size_jitter',
  'jitter_size',
  'master_opacity',
  'opacity_jitter',
  'jitter_opacity',
  'opacity_interpolate_active',
  'opacity_interpolate_time',
  'opacity_interpolate_target',
  'start_disabled'
}

ParticleSystem.save_fields = {
  'emit'
}

function ParticleSystem:init(params)
  -- PARAMETERS
  self.color = params.color or doublenickel.colors.red_light_trans
  self.layer = params.layer or doublenickel.layers.default_particle

  self.position_mode = params.position_mode or doublenickel.ffi.ParticlePositionMode_Bottom
  self.velocity_base = doublenickel.vec2(params.velocity_base)
  self.velocity_max = doublenickel.vec2(params.velocity_max or doublenickel.vec2(4, 4))
  self.velocity_jitter = doublenickel.vec2(params.velocity_jitter)
  self.jitter_base_velocity = ternary(params.jitter_base_velocity, true, false)
  self.jitter_max_velocity = ternary(params.jitter_max_velocity, true, false)

  self.size_jitter = params.size_jitter or 0
  self.jitter_size = ternary(params.jitter_size, true, false)

  self.default_master_opacity = params.master_opacity or 1
  self.master_opacity = params.master_opacity or 1
  self.opacity_jitter = params.opacity_jitter or 0
  self.jitter_opacity = ternary(params.jitter_opacity, true, false)
  self.opacity_interpolate_active = ternary(params.opacity_interpolate_active, true, false)
  self.opacity_interpolate_time = params.opacity_interpolate_time or 0
  self.opacity_interpolate_target = params.opacity_interpolate_target or 0

  self.lifetime = params.lifetime or 4

  self.particles = doublenickel.data_types.array:new()
  self.spawn_rate = params.spawn_rate or 20
  self.spawn_accumulated = 0
  self.despawn = doublenickel.data_types.array:new()
  self.warmup = params.warmup or 0
  self.start_disabled = ternary(params.start_disabled, true, false)

  self:set_particle_kind(doublenickel.enum.load(params.particle_kind) or doublenickel.enums.ParticleKind.Quad,
    params.particle_data)

  self.gravity_source = params.gravity_source or doublenickel.vec2()
  self.gravity_intensity = params.gravity_intensity or 1
  self.gravity_enabled = ternary(params.gravity_enabled, true, false)

  self.collider = self:find_component('Collider')
  self.collider.kind = doublenickel.enums.ColliderKind.Bypass

  -- RUNTIME
  self.emit = not self.start_disabled
  self.interpolation = {
    emission = doublenickel.interpolation.Lerp:new({
      time = 2
    })
  }

  -- FFI
  self.handle = doublenickel.ffi.make_particle_system();
  self:sync()
  if self.start_disabled then
    self:stop_emission()
  end

  -- EDITOR CALLBACKS
  self.__reset = false
  local editor_callbacks = {
    on_change_field = function(field) self:on_change_field(field) end,
    on_draw_colliders = function() self:on_draw_colliders() end
  }
  doublenickel.editor.set_editor_callbacks(self, editor_callbacks)
end

function ParticleSystem:deinit()
  if self.handle then
    doublenickel.ffi.free_particle_system(self.handle)
    self.handle = nil
  end
end

function ParticleSystem:update()
end

function ParticleSystem:draw()
  doublenickel.ffi.update_particles(self.handle)
  if self.interpolate_emission then
    self.interpolation.emission:update()
    self.master_opacity = self.interpolation.emission:get_value()

    if self.interpolation.emission:is_done() then
      if self.clear_after_interpolation then
        doublenickel.ffi.clear_particles(self.handle)
      end

      if self.master_opacity == 0 then
        doublenickel.ffi.stop_particle_emission(self.handle)
      end

      self.clear_after_interpolation = false
      self.interpolate_emission = false
    end
  end

  self:sync()
  doublenickel.ffi.draw_particles(self.handle)
end

function ParticleSystem:sync()
  doublenickel.editor.ignore_field(self, 'handle')
  if not self.handle then return end

  doublenickel.ffi.set_particle_warmup(self.handle, self.warmup)
  doublenickel.ffi.set_particle_lifetime(self.handle, self.lifetime)
  doublenickel.ffi.set_particle_max_spawn(self.handle, 32000)
  doublenickel.ffi.set_particle_spawn_rate(self.handle, self.spawn_rate)
  doublenickel.ffi.set_particle_position(self.handle, self.collider:get_xmin(), self.collider:get_ymin())
  doublenickel.ffi.set_particle_position_mode(self.handle, doublenickel.ffi.ParticlePositionMode_Bottom)
  doublenickel.ffi.set_particle_area(self.handle, self.collider:get_dimension().x, self.collider:get_dimension().y)
  doublenickel.ffi.set_particle_kind(self.handle, doublenickel.ffi.ParticleKind_Quad)
  doublenickel.ffi.set_particle_color(self.handle, self.color.r, self.color.g, self.color.b, self.color.a)
  doublenickel.ffi.set_particle_layer(self.handle, self.layer)
  doublenickel.ffi.set_particle_velocity_fn(self.handle, doublenickel.ffi.InterpolationFn_Linear)
  doublenickel.ffi.set_particle_velocity_base(self.handle, self.velocity_base.x, self.velocity_base.y)
  doublenickel.ffi.set_particle_velocity_max(self.handle, self.velocity_max.x, self.velocity_max.y)
  doublenickel.ffi.set_particle_velocity_jitter(self.handle, self.velocity_jitter.x, self.velocity_jitter.y)
  doublenickel.ffi.set_particle_jitter_base_velocity(self.handle, self.jitter_base_velocity)
  doublenickel.ffi.set_particle_jitter_max_velocity(self.handle, self.jitter_max_velocity)
  doublenickel.ffi.set_particle_size_jitter(self.handle, self.size_jitter)
  doublenickel.ffi.set_particle_jitter_size(self.handle, self.jitter_size)
  doublenickel.ffi.set_particle_master_opacity(self.handle, self.master_opacity)
  doublenickel.ffi.set_particle_opacity_jitter(self.handle, self.opacity_jitter)
  doublenickel.ffi.set_particle_jitter_opacity(self.handle, self.jitter_opacity)
  doublenickel.ffi.set_particle_opacity_interpolation(self.handle, self.opacity_interpolate_active,
    self.opacity_interpolate_time, self.opacity_interpolate_target)

  doublenickel.ffi.set_particle_gravity_source(self.handle, self.gravity_source.x, self.gravity_source.y)
  doublenickel.ffi.set_particle_gravity_intensity(self.handle, self.gravity_intensity)
  doublenickel.ffi.set_particle_gravity_enabled(self.handle, self.gravity_enabled)

  doublenickel.ffi.set_particle_kind(self.handle, self.particle_kind:to_number())
  if self.particle_kind == doublenickel.enums.ParticleKind.Quad then
    doublenickel.ffi.set_particle_size(self.handle, self.particle_data.size.x, self.particle_data.size.y)
  elseif self.particle_kind == doublenickel.enums.ParticleKind.Circle then
    doublenickel.ffi.set_particle_radius(self.handle, self.particle_data.radius)
  elseif self.particle_kind == doublenickel.enums.ParticleKind.Image then
    doublenickel.ffi.set_particle_sprite(self.handle, self.particle_data.sprite)
    doublenickel.ffi.set_particle_size(self.handle, self.particle_data.size.x, self.particle_data.size.y)
  end
end

function ParticleSystem:start_emission()
  doublenickel.ffi.start_particle_emission(self.handle)
  self.master_opacity = self.default_master_opacity
  self.emit = true
end

function ParticleSystem:stop_emission()
  doublenickel.ffi.stop_particle_emission(self.handle)
  self.emit = false
end

function ParticleSystem:fade_show(time, opacity_start)
  if self.emit then return end

  doublenickel.ffi.start_particle_emission(self.handle)

  self.interpolation.emission.start = opacity_start and opacity_start or self.master_opacity
  self.interpolation.emission.target = self.default_master_opacity
  self.interpolation.emission.time = time or 1
  self.interpolation.emission:reset()
  self.interpolate_emission = true

  self.emit = true
  self.clear_after_interpolation = false
end

function ParticleSystem:fade_hide(time, opacity_target)
  if not self.emit then return end

  doublenickel.ffi.stop_particle_emission(self.handle)

  self.interpolation.emission.start = self.master_opacity
  self.interpolation.emission.target = opacity_target and opacity_target or 0
  self.interpolation.emission.time = time or 1
  self.interpolation.emission:reset()
  self.interpolate_emission = true

  self.emit = false
  self.clear_after_interpolation = false
end

function ParticleSystem:clear_after_fade()
  self.clear_after_interpolation = true
end

function ParticleSystem:set_particle_kind(kind, data)
  self.particle_kind = kind
  self:on_change_particle_kind(data)
end

------------
-- EDITOR --
------------
function ParticleSystem:check_stats()
  local stats = doublenickel.ffi.check_particle_system(self.handle)
  return {
    spawned = stats.spawned,
    despawned = stats.despawned,
    alive = stats.alive
  }
end

function ParticleSystem:on_draw_colliders()
  if self.gravity_enabled then
    doublenickel.ffi.set_world_space(true)()
    doublenickel.draw_circle_l(self.gravity_source, math.abs(self.gravity_intensity) + 10, doublenickel.colors.white_trans)
  end
end

function ParticleSystem:on_change_field(field)
  if field == 'particle_kind' then
    self:on_change_particle_kind()
  elseif field == 'emit' then
    self:on_change_emit()
  elseif field == '__reset' then
    self:on_change_reset()
  end
end

function ParticleSystem:on_change_reset()
  self:deinit()
  self.handle = doublenickel.ffi.make_particle_system()
  self:sync()
  self.__reset = false
end

function ParticleSystem:on_change_emit()
  if self.emit then self:start_emission() end
  if not self.emit then self:stop_emission() end
end

function ParticleSystem:on_change_particle_kind(data)
  data = data or {}
  if self.particle_kind == doublenickel.enums.ParticleKind.Quad then
    self.particle_data = {
      size = data.size and doublenickel.vec2(data.size) or doublenickel.vec2(10, 10)
    }
  elseif self.particle_kind == doublenickel.enums.ParticleKind.Circle then
    self.particle_data = {
      radius = data.radius or 10
    }
  elseif self.particle_kind == doublenickel.enums.ParticleKind.Image then
    self.particle_data = {
      sprite = data.sprite or 'debug.png',
      size = data.size and doublenickel.vec2(data.size) or doublenickel.vec2(10, 10)
    }
  end
end
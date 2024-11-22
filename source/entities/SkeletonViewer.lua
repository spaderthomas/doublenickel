---------------------
-- BOUNDING VOLUME --
---------------------
BoundingVolume = tdengine.class.define('BoundingVolume')

function BoundingVolume:init(params)
  self.radius = params.radius or 50
  self.a = tdengine.vec2(params.a)
  self.b = tdengine.vec2(params.b)
end

tdengine.enum.define(
  'ParticleAttachment',
  {
    Bone = 0,
    Joint = 1
  }
)


------------------------
-- SKELETAL ANIMATION --
------------------------
SkeletalAnimation = tdengine.class.define('SkeletalAnimation')

function SkeletalAnimation:init(params)
  self.scale = params.scale or 1
  self.speed = params.speed or 1
  self.skeleton = params.skeleton

  self.joint_state = {}
  self.animation = {}

  if params.file_name then
    self:load_from_file(params.file_name)
  end

  self.accumulated = 0
  self.done = false
end

function SkeletalAnimation:load_from_file(file_name)
  local file_path = tdengine.ffi.resolve_format_path('skeletal_animation', file_name):to_interned()
  local file_data = tdengine.module.read(file_path)
  self:load_from_data(file_data)
end

function SkeletalAnimation:load_from_data(animation)
  self.animation = animation
  self.name = animation.name

  -- Load joint samples, scaling them and converting to vec2
  for joint_name, joint_samples in pairs(self.animation.joints) do
    for index, joint_sample in pairs(joint_samples) do
      self.animation.joints[joint_name][index] =
          tdengine.vec2(joint_sample):scale(self.scale)
    end
  end

  -- Initialize our runtime joint state
  for joint_name, joint_samples in pairs(self.animation.joints) do
    self.joint_state[joint_name] = {
      position = joint_samples[1]:copy()
    }
  end
end

function SkeletalAnimation:update()
  self.accumulated = self.accumulated + tdengine.dt * self.speed
  self.done = self.done or self.accumulated >= self.animation.num_samples
  self.accumulated = tdengine.math.fmod(self.accumulated, self.animation.num_samples)

  self.ka = math.floor(self.accumulated) + 1
  self.kb = tdengine.math.mod1(self.ka + 1, self.animation.num_samples)

  for joint_name, joint_samples in pairs(self.animation.joints) do
    local sa = joint_samples[self.ka]
    local sb = joint_samples[self.kb]
    local mix = self.accumulated - math.floor(self.accumulated)

    self.joint_state[joint_name].position = tdengine.interpolation.Lerp2(sa, sb, mix)
  end
end

-----------------------------------
-- SKELETAL ANIMATION CONTROLLER --
-----------------------------------
SkeletalAnimationController = tdengine.class.define('SkeletalAnimationController')

SkeletalAnimationController.states = tdengine.enum.define(
  'SkeletalAnimationState',
  {
    Play = 0,
    Blend = 1,
    PlaySequence = 2,
    BlendSequence = 3,
  }
)

function SkeletalAnimationController:find_particle_system(joint_a, joint_b)
  for index, system in pairs(self.particle_systems) do
    local match = system.joint_a == joint_a and system.joint_b == joint_b
    match = match or system.joint_b == joint_a and system.joint_a == joint_b
    if match then
      return system
    end
  end
end

function SkeletalAnimationController:init(params)
  self.state = self.states.Play
  self.skeleton = params.skeleton
  self.speed = params.speed or 1
  self.scale = params.scale or 1

  local rig_path = tdengine.ffi.resolve_format_path('particle_rig', 'sk_mixamo'):to_interned()
  self.particle_systems = tdengine.module.read(rig_path)
  for index, system in pairs(self.particle_systems) do
    system.bounding_volume = BoundingVolume:new(system.bounding_volume)
    system.bounding_volume.radius = system.bounding_volume.radius / 4
    system.attachment = tdengine.enum.load(system.attachment)
    system.handle = tdengine.ffi.lf_create(system.num_particles / 8)

    tdengine.ffi.lf_set_velocity(system.handle, 0, 0)
    tdengine.ffi.lf_init(system.handle)
  end
end

function SkeletalAnimationController:play(file_name)
  self.animation = self:build_animation(file_name)
  self.joint_state = self.animation.joint_state

  for index, system in pairs(self.particle_systems) do
    tdengine.ffi.lf_set_velocity(system.handle, 0, 0)
  end
end

function SkeletalAnimationController:build_animation(file_name)
  return SkeletalAnimation:new({
    skeleton = self.skeleton,
    speed = self.speed,
    scale = self.scale,
    file_name = file_name
  })
end

function SkeletalAnimationController:blend_to(file_name)
  self.blend_data = {
    animation = self:build_animation(file_name),
    interpolation = tdengine.interpolation.EaseInOut:new({ time = .4, exponent = 4 }),
  }

  self.state = self.states.Blend
end

function SkeletalAnimationController:play_sequence(file_names)
  self.sequence_data = {
    sequence = deep_copy_any(file_names),
    current_index = 1,
    blend_to = nil,
    interpolation = tdengine.interpolation.EaseInOut:new({ time = 2, exponent = 4 }),
  }
  self.animation = self:build_animation(file_names[1])


  self.state = self.states.PlaySequence
end

function SkeletalAnimationController:update()
  if self.state == self.states.Play then
    self.animation:update()
    self.joint_state = self.animation.joint_state
  elseif self.state == self.states.PlaySequence then
    self.animation:update()
    self.joint_state = self.animation.joint_state

    if self.animation.done then
      self.sequence_data.interpolation:reset()
      self.sequence_data.current_index = tdengine.math.mod1(self.sequence_data.current_index + 1,
        #self.sequence_data.sequence)
      self.sequence_data.blend = self:build_animation(self.sequence_data.sequence[self.sequence_data.current_index])
      self.state = self.states.BlendSequence
    end
  elseif self.state == self.states.BlendSequence then
    if self.sequence_data.interpolation:update() then
      self.animation = self.sequence_data.blend
      self.sequence_data.interpolation:reset()
      self.state = self.states.PlaySequence
    end

    self.animation:update()
    self.sequence_data.blend:update()
    self:blend_animations(self.animation, self.sequence_data.blend, self.sequence_data.interpolation:get_value())
  elseif self.state == self.states.Blend then
    if self.blend_data.interpolation:update() then
      self.animation = self.blend_data.animation
      self.state = self.states.Play
      return
    end

    self.animation:update()
    self.blend_data.animation:update()
    self:blend_animations(self.animation, self.blend_data.animation, self.blend_data.interpolation:get_value())
  end

  self:update_particle_systems()
end

function SkeletalAnimationController:blend_animations(a, b, t)
  self.joint_state = {}
  for joint_name, sample_a in pairs(a.joint_state) do
    local sample_b = b.joint_state[joint_name]
    self.joint_state[joint_name] = {
      position = tdengine.interpolation.Lerp2(sample_a.position, sample_b.position, t)
    }
  end
end

function SkeletalAnimationController:update_particle_systems()
  for index, particle_system in pairs(self.particle_systems) do
    self:update_particle_system(particle_system)
  end
end

function SkeletalAnimationController:update_particle_system(system)
  if not system.handle then return end

  local last_position = system.bounding_volume.b:copy()

  if system.attachment == tdengine.enums.ParticleAttachment.Joint then
    local sample = self.joint_state[system.joint].position

    system.bounding_volume.a:assign(sample)
    system.bounding_volume.b:assign(sample)
  elseif system.attachment == tdengine.enums.ParticleAttachment.Bone then
    local sample_a = self.joint_state[system.joint_a].position
    local sample_b = self.joint_state[system.joint_b].position
    local offset = sample_a:subtract(sample_b):normalize():scale(system.bounding_volume.radius * .75)

    system.bounding_volume.a:assign(sample_a:subtract(offset))
    system.bounding_volume.b:assign(sample_b:add(offset))
  end

  tdengine.ffi.lf_set_volume(
    system.handle,
    system.bounding_volume.a.x, system.bounding_volume.a.y,
    system.bounding_volume.b.x, system.bounding_volume.b.y,
    system.bounding_volume.radius)

  local delta = system.bounding_volume.b:subtract(last_position)
  tdengine.ffi.lf_set_velocity(system.handle, delta.x, delta.y)

  tdengine.ffi.lf_update(system.handle)
end

---------------------
-- SKELETON VIEWER --
---------------------
SkeletonViewer = tdengine.entity.define('SkeletonViewer')

SkeletonViewer.states = tdengine.enum.define(
  'SkeletonViewerState',
  {
    Idle = 0,
    DragWorld = 1
  }
)

function SkeletonViewer:init()
  self.state = self.states.Idle
  self.layers = {
    bounding_volume = 99,
    bone = 100,
    rotation_marker = 101,
    joint = 102,
    label = 103,
  }

  self.__editor_controls = {
    draw_labels = false,
    draw_joints = true,
    draw_bounding_volumes = false,
    animation_speed = 30,
    scale = .0625
  }

  self.style = {
    joint_size = math.max(8 * self.__editor_controls.scale, 2),
    bone_size = 3,
    label_padding = 8,
  }

  self.colors = {
    joint = tdengine.colors.cadet_gray:copy(),
    bone = tdengine.colors.cadet_gray:copy(),
    label = tdengine.colors.white:copy(),
  }

  self.hotkeys = {
    blend = glfw.keys.TAB,
    sequence = glfw.keys.ONE,
    inspect = glfw.keys.TWO,
  }

  self:load_from_file('sk_mixamo')

  self:load_animation('ska_idle')

  for joint_name, joint in pairs(self.skeleton.joints) do
    joint.position = tdengine.vec2(joint.position)
  end
end

function SkeletonViewer:load_from_file(file_name)
  local file_path = tdengine.ffi.resolve_format_path('skeleton', file_name):to_interned()
  local skeleton = tdengine.module.read(file_path)
  self:load_from_data(skeleton)
end

function SkeletonViewer:load_from_data(skeleton)
  self.skeleton = deep_copy_any(skeleton)
end

function SkeletonViewer:load_animation(file_name)
  tdengine.ffi.lf_destroy_all()

  self.animation = SkeletalAnimationController:new({
    skeleton = self.skeleton,
    scale = self.__editor_controls.scale,
    speed = self.__editor_controls.animation_speed,
  })
  self.animation:play(file_name)
end

function SkeletonViewer:update()
  self:update_state()
  self:update_hotkeys()
  self:update_playground()

  self.animation:update()

  --self:draw_skeleton()
  self:draw_animation()
  self:draw_particle_systems()
end

function SkeletonViewer:update_state()
  if self.state == self.states.Idle then
    local game_view = tdengine.find_entity_editor('GameViewManager')
    if tdengine.input.pressed(glfw.keys.MOUSE_BUTTON_1) and game_view.hover then
      self.state = tdengine.enums.SkeletonViewerState.DragWorld
    end
  elseif self.state == self.states.DragWorld then
    if not tdengine.input.down(glfw.keys.MOUSE_BUTTON_1) then
      self.state = tdengine.enums.SkeletonViewerState.Idle
      return
    end

    local camera = tdengine.entity.find('Camera')
    camera:move(tdengine.input.mouse_delta():scale(-1))
  end
end

function SkeletonViewer:update_hotkeys()
  if tdengine.input.pressed(self.hotkeys.blend) then
    if self.animation.animation.name == 'ska_mma_idle' then
      self.animation:blend_to('ska_idle')
    else
      self.animation:blend_to('ska_mma_idle')
    end
  end

  if tdengine.input.pressed(self.hotkeys.inspect) then
    local system = self.animation:find_particle_system('mixamorig:Spine1', 'mixamorig:Spine2')
    if system then
      tdengine.ffi.lf_inspect(system.handle)
    end
  end
  if tdengine.input.pressed(self.hotkeys.sequence) then
    local sequence = {
      'ska_sneak_walk',
      'ska_sneak_walk',
      'ska_cast_spell',
    }
    self.animation:play_sequence(sequence)
  end
end

function SkeletonViewer:update_playground()
end

function SkeletonViewer:draw_animation()
  tdengine.ffi.set_world_space(true)
  if self.__editor_controls.draw_joints then
    for joint_name, joint_sample in pairs(self.animation.joint_state) do
      self:draw_joint(joint_sample.position.x, joint_sample.position.y)

      local joint = self.skeleton.joints[joint_name]
      if joint.children then
        for _, child_id in pairs(joint.children) do
          local child_sample = self.animation.joint_state[child_id]
          self:draw_bone(joint_sample.position.x, joint_sample.position.y, child_sample.position.x, child_sample.position.y)
        end
      end
    end
  end

  if self.__editor_controls.draw_labels then
    tdengine.ffi.set_layer(self.layers.label)
    for joint_name, joint_sample in pairs(self.animation.joint_state) do
      local label = tdengine.ffi.prepare_text_ex(joint_name, joint_sample.position.x, joint_sample.position.y,
        'merriweather-16', 0, self.colors.label:to_vec4(), true)
      label.position.y = label.position.y + (label.height / 2)
      label.position.x = label.position.x + self.style.joint_size + self.style.label_padding
      tdengine.ffi.draw_prepared_text(label)
    end
  end
end

function SkeletonViewer:draw_skeleton()
  tdengine.ffi.set_world_space(true)
  if self.__editor_controls.draw_joints then
    for joint_name, joint in pairs(self.skeleton.joints) do
      self:draw_joint(joint.position.x, joint.position.y)
    end
  end
end

function SkeletonViewer:draw_bone(px, py, cx, cy)
  tdengine.ffi.set_layer(self.layers.bone)
  tdengine.ffi.draw_line(
    px, py, cx, cy,
    self.style.bone_size,
    self.colors.bone:alpha(.5):to_vec4()
  )
end

function SkeletonViewer:draw_joint(px, py)
  tdengine.ffi.set_layer(self.layers.joint)
  tdengine.ffi.draw_circle_sdf(px, py, self.style.joint_size, self.colors.joint:to_vec4(), 1)
end

function SkeletonViewer:draw_particle_systems()
  tdengine.ffi.draw_circle_sdf(0, 0,
  20, tdengine.colors.white:to_vec4(), 2)

  if not self.__editor_controls.draw_bounding_volumes then return end

  -- tdengine.set_blend_enabled(true)
  -- tdengine.set_blend_mode(tdengine.enums.BlendMode.ONE, tdengine.enums.BlendMode.ONE_MINUS_SRC_ALPHA)
  tdengine.ffi.set_world_space(true)
  tdengine.ffi.set_layer(self.layers.bounding_volume)

  local color = tdengine.colors.animal_well.magenta:copy()
  local colors = {
    tdengine.colors.animal_well.celestial_blue,
    tdengine.colors.animal_well.magenta,
    tdengine.colors.animal_well.dark_cyan,
    tdengine.colors.animal_well.mantis,
    tdengine.colors.animal_well.mauve,
    tdengine.colors.indian_red,
    tdengine.colors.spring_green,
    tdengine.colors.cadet_gray,
  }

  for index, particle_system in pairs(self.animation.particle_systems) do
    if particle_system.num_particles == 0 then goto continue end

  tdengine.ffi.set_layer(self.layers.bounding_volume + index)

    local color = colors[tdengine.math.mod1(index, #colors)]

    tdengine.ffi.draw_circle_sdf(particle_system.bounding_volume.a.x, particle_system.bounding_volume.a.y,
      particle_system.bounding_volume.radius, color:to_vec4(), 2)
    tdengine.ffi.draw_circle_sdf(particle_system.bounding_volume.b.x, particle_system.bounding_volume.b.y,
      particle_system.bounding_volume.radius, color:to_vec4(), 2)

    local distance = particle_system.bounding_volume.b:subtract(particle_system.bounding_volume.a)
    for i = 0, distance:length(), 2 do
      local p = tdengine.interpolation.Lerp2(particle_system.bounding_volume.a, particle_system.bounding_volume.b, i / distance:length())
      tdengine.ffi.draw_circle_sdf(p.x, p.y,
      particle_system.bounding_volume.radius, color:to_vec4(), 2)

    end
    -- tdengine.ffi.draw_line(
    --   particle_system.bounding_volume.a.x, particle_system.bounding_volume.a.y,
    --   particle_system.bounding_volume.b.x, particle_system.bounding_volume.b.y,
    --   particle_system.bounding_volume.radius * 2,
    --   color:to_vec4())

    ::continue::
  end

  -- tdengine.set_blend_mode(tdengine.enums.BlendMode.SRC_ALPHA, tdengine.enums.BlendMode.ONE_MINUS_SRC_ALPHA)
end
return {
  ["8e702ae9-a635-406a-b63d-fa0df387c3ee"] = {
    components = {
      Collider = {
        attach_offset = {
          x = 0,
          y = 0
        },
        attached = "",
        impl = {
          dimension = {
            x = 198.69158935546875,
            y = 199.72305297851563
          },
          position = {
            x = -699.81280338764191,
            y = 200.44334322214127
          }
        },
        kind = {
          __enum = "ColliderKind",
          value = "Static"
        },
        name = "Collider",
        shape = {
          __enum = "ColliderShape",
          value = "Box"
        },
        uuid = "b7a5c66f-45f1-4f15-b4dd-bde05b9cc591",
        world_space = true
      }
    },
    name = "SampleEntity",
    tag = "LeftSample",
    uuid = "8e702ae9-a635-406a-b63d-fa0df387c3ee"
  },
  ["31de5da6-0d1f-4803-b2cd-27c2406fdf3e"] = {
    color = {
      a = 0.25,
      b = 0,
      g = 0,
      r = 1
    },
    components = {
      Collider = {
        attach_offset = {
          x = 0,
          y = 0
        },
        attached = "",
        impl = {
          dimension = {
            x = 100,
            y = 100
          },
          position = {
            x = -49.844236493110657,
            y = 349.03040504455566
          }
        },
        kind = {
          __enum = "ColliderKind",
          value = "Bypass"
        },
        name = "Collider",
        shape = {
          __enum = "ColliderShape",
          value = "Box"
        },
        uuid = "bce7ef42-c8ac-449e-a403-054681f1734a",
        world_space = true
      }
    },
    gravity_enabled = false,
    gravity_intensity = 1,
    gravity_source = {
      x = 0,
      y = 0
    },
    jitter_base_velocity = false,
    jitter_max_velocity = false,
    jitter_opacity = false,
    jitter_size = false,
    layer = 31,
    lifetime = 4,
    master_opacity = 1,
    name = "ParticleSystem",
    opacity_interpolate_active = false,
    opacity_interpolate_target = 0,
    opacity_interpolate_time = 0,
    opacity_jitter = 0,
    particle_data = {
      size = {
        x = 10,
        y = 10
      }
    },
    particle_kind = {
      __enum = "ParticleKind",
      value = "Quad"
    },
    position_mode = 0,
    size_jitter = 0,
    spawn_rate = 20,
    start_disabled = false,
    uuid = "31de5da6-0d1f-4803-b2cd-27c2406fdf3e",
    velocity_base = {
      x = 0,
      y = 0
    },
    velocity_jitter = {
      x = 0,
      y = 0
    },
    velocity_max = {
      x = 4,
      y = 4
    },
    warmup = 0
  },
  ["fae44240-9b23-4568-a7c0-58f4e8b2a659"] = {
    components = {
      Collider = {
        attach_offset = {
          x = 0,
          y = 0
        },
        attached = "",
        impl = {
          position = {
            x = 600.12466394901276,
            y = 100.72017019987106
          },
          radius = 100
        },
        kind = {
          __enum = "ColliderKind",
          value = "Static"
        },
        name = "Collider",
        shape = {
          __enum = "ColliderShape",
          value = "Circle"
        },
        uuid = "0932fb73-70fc-4f30-b37a-70c670badf01",
        world_space = true
      }
    },
    name = "SampleEntity",
    tag = "RightSample",
    uuid = "fae44240-9b23-4568-a7c0-58f4e8b2a659"
  }
}
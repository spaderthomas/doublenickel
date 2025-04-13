return {
  write_paths = {
  },
  install_paths = {
    asset = {
      path = 'asset',
      children = {
        fonts = {
          path = 'fonts',
          children = {
            font = '%s'
          }
        },
        shaders = {
          path = 'shaders',
          children = {
            shader_includes = 'include',
            shader = '%s',
          }
        },
        audio = {
          path = 'audio'
        },
        images = {
          path = 'images',
          children = {
            image = '%s'
          }
        },

      }
    }
  },
  app_paths = {
      skeletal_animations = {
      named_parent = 'data',
      path = 'skeletal_animations',
      children = {
        skeletal_animation = '%s.lua',
      }
    },
    skeletons = {
      path = 'skeletons',
      children = {
        skeleton = '%s.lua'
      }
    },
    particle_rigs = {
      path = 'particle_rigs',
      children = {
        particle_rig = '%s.lua'
      }
    }

  }
}

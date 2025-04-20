return {
  write_paths = {
  },
  install_paths = {
    data = {
      path = 'data',
      children = {
        scenes = {
          path = 'scenes',
          children = {
            scene = '%s'
          }
        }
      }
    },
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
        layouts = {
          path = 'layouts',
          children = {
            layout = '%s.ini'
          }
        },
      }
    }
  },
  app_paths = {}
}

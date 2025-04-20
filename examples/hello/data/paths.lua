return {
  write_paths = {},
  install_paths = {
    asset = {
      path = 'asset',
      children = {
        layouts = {
          path = 'layouts',
          children = {
            layout = '%s.ini'
          }
        }
      }
    },
    data = {
      path = 'data',
      children = {
        scenes = {
          path = 'scenes',
          children = {
            scene = '%s.lua'
          }
        }
      }
    }
  },
  app_paths = {}
}

return {
  write_paths = {},
  app_paths = {},
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
        layouts = {
          path = 'layouts',
          children = {
            layout = '%s.ini'
          }
        },
      }
    }
  },
}

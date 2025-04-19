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
    }
  },
  app_paths = {}
}

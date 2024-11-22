return {
	write_paths = {
		main = {
			path = 'boonbane',
			children = {
				screenshots = {
					path = 'screenshots',
					children = {
						screenshot = '%s'
					}
				},
				saves = {
					path = 'saves',
					children = {
						save = '%s.lua'
					}
				},
			}
		}
	},
	install_paths = {
	}
}

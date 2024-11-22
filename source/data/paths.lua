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

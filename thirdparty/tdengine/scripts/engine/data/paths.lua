return {
	write_paths = {
		app_write = 'boonbane',
	},
	install_paths = {
		scripts = {
			path = 'scripts',
			children = {
				engine_scripts = {
					path = 'engine',
					children = {
						engine_components = 'components',
						engine_core = {
							path = 'core',
							children = {
								engine_script = '%s'
							}
						},
						engine_data = {
							path = 'data',
							children = {
								editor_scene = 'editor.lua',
								cimgui_header = 'cimgui.lua',
							}
						},
						engine_editor = 'editor',
						engine_entities = 'entities',
						engine_libs = 'third_party',
					}
				},
				user_scripts = {
					path = 'user',
					children = {
						components = 'components',
						core = 'core',
						data = {
							path = 'data',
							children = {
								layouts = 'layouts',
								layout = 'layouts/%s.ini',
								dialogues = {
									path = 'dialogue',
									children = {
										dialogue_folder = '%s',
										dialogue_source = '%s/dialogue.lua',
										dialogue_metadata = '%s/editor.lua',
									}
								},
								scenes = 'scenes',
								scene = 'scenes/%s.lua',
								states = 'states',
								state = 'states/%s.lua',
								path_info = 'paths.lua',
								action_info = 'actions.lua',
								animation_info = 'animations.lua',
								background_info = 'backgrounds.lua',
								character_info = 'characters.lua',
								font_info = 'fonts.lua',
								ffi_info = 'ffi.lua',
								texture_info = 'textures.lua',
								gpu_info = 'gpu.lua',

								skeletal_animations = {
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
						},
						dialogue = 'dialogue',
						editor = 'editor',
						entities = 'entities',
						subsystems = 'subsystems',
					}
				},
			},
		},
		asset = {
			path = 'asset',
			children = {
				steam = 'steam',
				fonts = 'fonts',
				font = 'fonts/%s.ttf',
				images = {
					path = 'images',
					children = {
						image = '%s',
						atlases = 'atlas',
						atlas = 'atlas/%s'
					},
				},
				shaders = {
					path = 'shaders',
					children = {
						vertex_shader = '%s',
						fragment_shader = '%s',
						compute_shader = '%s',
					}
				},
				audio = 'audio',
			}
		},
	}
}

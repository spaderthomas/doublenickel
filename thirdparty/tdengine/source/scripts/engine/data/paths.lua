return {
	write_paths = {
		app_write = 'boonbane',
	},
	engine_paths = {
		dn_assets = {
			path = 'asset',
			children = {
				dn_audio = 'audio',
				dn_fonts = {
					path = 'fonts',
					children = {
						dn_font = '%s'
					}
				},
				dn_images = {
					path = 'images',
					children = {
						dn_image = '%s',
						dn_atlases = 'atlas',
						dn_atlas = 'atlas/%s'
					},
				},
				dn_shaders = {
					path = 'shaders',
					children = {
						dn_shader_includes = 'include',
						dn_shader = '%s',
					}
				},
			}
		},
		engine_scripts = {
			path = 'source/scripts/engine',
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
	},
	install_paths = {
	},
	app_paths = {
		app_entry_point = 'App.lua',
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
			}
		},
		dialogue = 'dialogue',
		editor = 'editor',
		entities = 'entities',
		subsystems = 'subsystems',
	},
}

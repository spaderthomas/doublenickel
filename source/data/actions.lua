return {
	actions = {},
	action_sets = {
		GameControls = {
			default = true,
			actions = {
				'Action_Game',
			}
		},
		MenuControls = {
			actions = {
				'Action_Menu',
			}
		},
	},
	keyboard_controls = {
		Action_Game = {
			key = glfw.keys.TAB,
			event = 'press'
		},
		Action_Menu = {
			key = glfw.keys.TAB,
			event = 'press'
		},
	},
	descriptions = {
		Action_Game = "An action in the game action set; all the names here need to match what's in your Steam Input configuration",
		Action_Menu = "An action in the menu action set; all the names here need to match what's in your Steam Input configuration",
	},
	order = {
		Action_Game = 1,

		Action_Menu = 1,
	}
}

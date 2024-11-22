void init_actions() {
	if (!engine.steam) return;
	
	steam_input.steam = SteamInput();
	
	bool explicitly_run_frame = false; // i.e. use SteamAPI_RunCallbacks
	if (!steam_input.steam->Init(explicitly_run_frame)) {
		tdns_log.write("%s: could not initialize steam input", __func__);
		return;
	}

	steam_input.actions = standard_allocator.alloc_array<Action>(SteamInputManager::MAX_ACTIONS);
	steam_input.action_sets = standard_allocator.alloc_array<ActionSet>(SteamInputManager::MAX_ACTION_SETS);

	steam_input.poll_for_inputs();
	steam_input.poll_for_controllers();
	steam_input.register_actions();
}

void update_actions() {
	steam_input.poll_for_controllers();
	steam_input.poll_for_inputs();
	
	// Reset all actions for this frame
	arr_for(steam_input.actions, action) {
		action->old_state = action->state;
		action->state = false;
	}

	steam_input.got_controller_input = false;

	// If we were asked to activate an action set last frame, do that now. Action set updates are deferred to
	// a known point in the frame so that all entities get consistent inputs on the frame the set is changed.
	if (steam_input.queued_action_set) {
		steam_input.steam->ActivateActionSet(steam_input.controller, steam_input.queued_action_set->handle);
		steam_input.current_action_set = steam_input.queued_action_set;
		steam_input.queued_action_set = nullptr;
	}

	if (steam_input.update_cooldown_hack()) return;

	// Poll for action inputs
	auto& input = get_input_manager();
	arr_for(steam_input.actions, action) {
		if (!steam_input.is_action_active(action)) continue;

		// If Steam is loaded and there is a controller, query SteamInput
		if (steam_input.controller) {
			auto action_data = steam_input.steam->GetDigitalActionData(steam_input.controller, action->handle);
			if (action_data.bState) {
				steam_input.got_controller_input = true;
				action->state = true;
			}
		}

		// Check keyboard controls
		if (action->key_event == KeyEvent::Press) {
			action->state |= was_key_pressed(action->key);
		}
		else if (action->key_event == KeyEvent::Down) {
			action->state |= is_key_down(action->key);
		}
	}

	// This is a little out of place because I probably need to merge action.cpp and input.cpp. But, the idea
	// is that we've received all input for the frame, so we know whether to consider ourselves as in mouse
	// + keyboard mode or controller mode
	auto& input_manager = get_input_manager();
	if (steam_input.got_controller_input) {
		steam_input.last_input_device = InputDevice::Controller;
	}
	else if (input_manager.got_keyboard_input || input_manager.got_mouse_input) {
		steam_input.last_input_device = InputDevice::MouseAndKeyboard;
	}
}


//
// STEAM INPUT MANAGER
//
void SteamInputManager::register_actions() {
	steam->RunFrame();

	auto l = get_lua().state;
	lua_getglobal(l, "tdengine");
	lua_pushstring(l, "action");
	lua_gettable(l, -2);
	lua_pushstring(l, "init");
	lua_gettable(l, -2);

	lua_pcall(l, 0, 0, 0);
}

void SteamInputManager::poll_for_inputs() {
	//if (steam) steam->RunFrame();
}

void SteamInputManager::poll_for_controllers() {
	if (!engine.steam) return;

	auto controllers = bump_allocator.alloc_array<InputHandle_t>(STEAM_INPUT_MAX_COUNT);
	controllers.size = SteamInput()->GetConnectedControllers(controllers.data);

	auto last_controller = controller;
	controller = controllers.size ? *controllers[0] : 0;

	bool different = last_controller != controller;
	bool added_controller = different && controller && !last_controller;
	bool replaced_controller = different && controller;
	bool removed_controller = different && !controller;
	
	if (added_controller) {
		auto controller_name = get_controller_name(controller);
		tdns_log.write("%s: added controller; num_controllers = %d, active_controller = %s", __func__, controllers.size, controller_name);

		// SteamInput, from my testing, doesn't return valid handles until a controller is connected. Therefore, when
		// we see that a controller connects, we need to re-register everything. This should be idempotent when there
		// are valid handles, so we can call it over and over.
		register_actions();

	}
	else if (replaced_controller) {
		auto controller_name = get_controller_name(controller);
		auto last_controller_name = get_controller_name(last_controller);
		tdns_log.write("%s: replaced controller; num_controllers = %d, active_controller = %s, last_controller = %s", 
			__func__, 
			controllers.size, controller_name,
			last_controller_name);

		// Whatever action set was active on the other controller should be active on the new one
		if (current_action_set) {
			steam->ActivateActionSet(controller, current_action_set->handle);
		}
	}
	else if (removed_controller) {
		tdns_log.write("%s: no controller detected", __func__);
	}

	if (added_controller || replaced_controller) {
		load_controller_glyphs();
	};
}

void SteamInputManager::load_controller_glyphs() {
	auto origins = bump_allocator.alloc_array<EInputActionOrigin>(STEAM_INPUT_MAX_COUNT);
		
	arr_for(actions, action) {
		arr_clear(&origins);
		origins.size = steam->GetDigitalActionOrigins(controller, action->set->handle, action->handle, origins.data);
		
		if (origins.size) {
			auto origin = *origins[0]; //i.e, k_EInputActionOrigin_PS4_X
			auto glyph_path = SteamInput()->GetGlyphPNGForActionOrigin(origin, k_ESteamInputGlyphSize_Small, 0);

			int height, width, channels;
			unsigned char* glyph_data = (unsigned char*)stbi_load(glyph_path, &height, &width, &channels, 0);
			defer { stbi_image_free(glyph_data); };
			if (channels != 4) {
				tdns_log.write("could not load glyph %s for action %s; num_channels = %d", glyph_path, action->name, channels);
				continue;
			}

			auto sprite = find_sprite_no_default(action->name);
			if (sprite) create_sprite_ex(sprite, action->name, glyph_data, width, height, channels);
			else        create_sprite(action->name, glyph_data, width, height, channels);

			tdns_log.write("loaded controller glyph %s for action %s", glyph_path, action->name);
		}
	}
}

bool SteamInputManager::is_action_active(Action* action) {
	if (!current_action_set) return false;
	return action->set	== current_action_set;
}


ActionSet* SteamInputManager::find_action_set(const char* name) {
	arr_for(action_sets, action_set) {
		if (!strncmp(name, action_set->name, ActionSet::NAME_MAX)) {
			return action_set;
		}
	}
	
	return nullptr;
}

ActionSet* SteamInputManager::find_action_set(Steam::ActionSet handle) {
	arr_for(action_sets, action_set) {
		if (action_set->handle == handle) {
			return action_set;
		}
	}
	
	return nullptr;
}

ActionSet* SteamInputManager::find_current_action_set() {
	return current_action_set;
}

Action* SteamInputManager::find_action(const char* name) {
	arr_for(actions, action) {
		if (!strncmp(name, action->name, Action::NAME_MAX)) {
			return action;
		}
	}
	
	return nullptr;
}

tstring SteamInputManager::get_controller_name(Steam::Controller controller) {
	auto controller_type = steam->GetInputTypeForHandle(controller);
	const char* name = "";

	if (!controller) {
		name = "No Controller";
	}
	else if (controller_type == k_ESteamInputType_Unknown) {
		name = "Unknown Controller"; 
	}
	else if (controller_type == k_ESteamInputType_SteamController) {
		name = "Steam Controller";
	}
	else if (controller_type == k_ESteamInputType_XBox360Controller) {
		name = "XBox 360 Controller";
	}
	else if (controller_type == k_ESteamInputType_XBoxOneController) {
		name = "XBox One Controller";
	}
	else if (controller_type == k_ESteamInputType_GenericGamepad) {
		name = "Generic Controller";
	}
	else if (controller_type == k_ESteamInputType_PS4Controller) {
		name = "PS4 Controller";
	}
	else if (controller_type == k_ESteamInputType_PS3Controller) {
		name = "PS3 Controller";
	}
	else if (controller_type == k_ESteamInputType_SwitchProController) {
		name = "Switch Pro Controller";
	}
	else {
		name = "A Seriously Unknown Controller";
	}

	return copy_string(name, &bump_allocator);
}

bool SteamInputManager::update_cooldown_hack() {
	action_set_cooldown = std::max(action_set_cooldown - 1, 0);
	return action_set_cooldown > 0;
}

//
// EXPORT API
//
void activate_action_set(const char* name) {
	if (!engine.steam) return;
	steam_input.queued_action_set = steam_input.find_action_set(name);
	steam_input.action_set_cooldown = SteamInputManager::action_set_cooldown_max;
}

int32 get_action_set_cooldown() {
	return steam_input.action_set_cooldown;
}

void register_action_set(const char* name) {
	if (!engine.steam) return;

	auto action_set = steam_input.find_action_set(name);
	if (!action_set) action_set = arr_push(&steam_input.action_sets);

	std::strncpy(action_set->name, name, ActionSet::NAME_MAX);
	action_set->handle = steam_input.steam->GetActionSetHandle(name);
}

void register_action(const char* name, u32 key, u32 key_event, const char* action_set) {
	if (!engine.steam) return;

	auto action = steam_input.find_action(name);
	if (!action) action = arr_push(&steam_input.actions);

	action->key = key;
	action->key_event = static_cast<KeyEvent>(key_event);
	action->handle = steam_input.steam->GetDigitalActionHandle(name);
	action->set = steam_input.find_action_set(action_set);
	std::strncpy(action->name, name, Action::NAME_MAX);		
}

bool is_digital_active(const char* name) {
	if (!engine.steam) return false;
	
	auto action = steam_input.find_action(name);
	if (!action) return false;

	return action->state;
}

bool was_digital_active(const char* name) {
	if (!engine.steam) return false;

	auto action = steam_input.find_action(name);
	if (!action) return false;
	
	return action->old_state;
}

bool was_digital_pressed(const char* name) {
	if (!engine.steam) return false;

	auto action = steam_input.find_action(name);
	if (!action) return false;

	return action->state && !action->old_state;
}

int32 get_input_device() {
	return static_cast<int32>(steam_input.last_input_device);
}

const char* get_active_action_set() {
	auto action_set = steam_input.find_current_action_set();
	if (action_set) {
		return action_set->name;
	}

	return "ACTION_SET_NONE";
}

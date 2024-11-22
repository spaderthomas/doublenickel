InputManager::InputManager() {
	fill_shift_map();
}
	
void InputManager::fill_shift_map() {
	fox_for(i, 128) {
		shift_map[i] = 0;
	}
		
	shift_map[' ']  =  ' ';
	shift_map['\''] =  '"';
	shift_map[',']  =  '<';
	shift_map['-']  =  '_';
	shift_map['.']  =  '>';
	shift_map['/']  =  '?';

	shift_map['0']  =  ')';
	shift_map['1']  =  '!';
	shift_map['2']  =  '@';
	shift_map['3']  =  '#';
	shift_map['4']  =  '$';
	shift_map['5']  =  '%';
	shift_map['6']  =  '^';
	shift_map['7']  =  '&';
	shift_map['8']  =  '*';
	shift_map['9']  =  '(';

	shift_map[';']  =  ':';
	shift_map['=']  =  '+';
	shift_map['[']  =  '{';
	shift_map['\\'] =  '|';
	shift_map[']']  =  '}';
	shift_map['`']  =  '~';
		
	shift_map['a']  =  'A';
	shift_map['b']  =  'B';
	shift_map['c']  =  'C';
	shift_map['d']  =  'D';
	shift_map['e']  =  'E';
	shift_map['f']  =  'F';
	shift_map['g']  =  'G';
	shift_map['h']  =  'H';
	shift_map['i']  =  'I';
	shift_map['j']  =  'J';
	shift_map['k']  =  'K';
	shift_map['l']  =  'L';
	shift_map['m']  =  'M';
	shift_map['n']  =  'N';
	shift_map['o']  =  'O';
	shift_map['p']  =  'P';
	shift_map['q']  =  'Q';
	shift_map['r']  =  'R';
	shift_map['s']  =  'S';
	shift_map['t']  =  'T';
	shift_map['u']  =  'U';
	shift_map['v']  =  'V';
	shift_map['w']  =  'W';
	shift_map['x']  =  'X';
	shift_map['y']  =  'Y';
	shift_map['z']  =  'Z';
}

InputManager& get_input_manager() {
	static InputManager manager;
	return manager;
}

void update_input() {
	auto& input = get_input_manager();

	// Reset between frames
	fox_for(key, GLFW_KEY_LAST) {
		input.was_down[key] = input.is_down[key];
	}

	input.scroll.x = 0;
	input.scroll.y = 0;
	input.mouse_delta.x = 0;
	input.mouse_delta.y = 0;
	input.got_keyboard_input = false;
	input.got_mouse_input = false;

	// Fill the input manager's buffers with what GLFW tells us
	glfwPollEvents();

	// Determine whether ImGui is requesting key inputs
	ImGuiIO& imgui = ImGui::GetIO();
	input.is_editor_requesting_input = false;
	input.is_editor_requesting_input |= imgui.WantCaptureKeyboard;
	input.is_editor_requesting_input |= imgui.WantCaptureMouse;
	input.is_editor_requesting_input &= !input.game_focus;
}

////////////////////
// GLFW Callbacks //
////////////////////
void GLFW_Cursor_Pos_Callback(GLFWwindow* glfw, double x, double y) {
	auto& input_manager = get_input_manager();
	input_manager.got_mouse_input = true;
	
	if (x < 0) x = 0;
	if (y < 0) y = 0;

	auto last_mouse = input_manager.mouse;

	input_manager.mouse.x = x / window.content_area.x;
	input_manager.mouse.y = 1 - (y / window.content_area.y);

	// I'm not totally sure why this is the case, but these events need to be kind of debounced. That is,
	// one call to glfwPollEvents() might call this callback more than one times (in practice, two, but
	// who knows if that's a hard limit.
	//
	// In other words, we need to *accumulate* delta, and then at the beginning of the next frame the
	// input manager will reset the delta before it polls events
	input_manager.mouse_delta.x += input_manager.mouse.x - last_mouse.x;
	input_manager.mouse_delta.y += input_manager.mouse.y - last_mouse.y;
}

void GLFW_Mouse_Button_Callback(GLFWwindow* window, int button, int action, int mods) {
	auto& input_manager = get_input_manager();
	input_manager.got_mouse_input = true;

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_LEFT] = true;
		}
		if (action == GLFW_RELEASE) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_LEFT] = false;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_RIGHT] = true;
		}
		if (action == GLFW_RELEASE) {
			input_manager.is_down[GLFW_MOUSE_BUTTON_RIGHT] = false;
		}
	}
}

void GLFW_Scroll_Callback(GLFWwindow* window, double dx, double dy) {
	auto& input_manager = get_input_manager();
	input_manager.got_mouse_input = true;

	input_manager.scroll.x = dx;
	input_manager.scroll.y = dy;
}

void GLFW_Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto& manager = get_input_manager();
	manager.got_keyboard_input = true;
	
	if (action == GLFW_PRESS) {
			manager.is_down[key] = true;
	}
	if (action == GLFW_RELEASE) {
			manager.is_down[key] = false;
	}
}

void GLFW_Error_Callback(int err, const char* msg) {
	tdns_log.write("GLFW error: code = %d, message = %s", err, msg);
}

void GLFW_Window_Size_Callback(GLFWwindow* window_handle, int width, int height) {
	tdns_log.write("%s: width = %d, height = %d", __func__, width, height);
	
	window.content_area.x = width;
	window.content_area.y = height;

	auto swapchain = gpu_acquire_swapchain();
	swapchain->size = window.content_area;

	// In the editor, the game are is specified by some editor code that figures out the best size
	// for it. In the packaged build, it's just the entire screen, so we sync it up with the window.
#if !defined(FM_EDITOR)
	tdns_log.write("%s: width = %d, height = %d", __func__, width, height);

	Coord::game_area_size.x = width;
	Coord::game_area_size.y = height;
#endif
	
	glViewport(0, 0, width, height);
}


/////////////
// LUA API //
/////////////
bool is_editor_requesting_input() {
	auto& input = get_input_manager();
	return input.is_editor_requesting_input;
}

bool was_key_pressed(int key) {
	auto& input = get_input_manager();
	return input.is_down[key] && !input.was_down[key];
}

bool was_key_released(int key) {
	auto& input = get_input_manager();
	return !input.is_down[key] && input.was_down[key];
}

bool is_key_down(int key) {
	auto& input = get_input_manager();
	return input.is_down[key];
}

bool is_mod_down(int mod) {
	auto& input = get_input_manager();
	
	bool down = false;
	if (mod == GLFW_KEY_CONTROL) {
		down |= input.is_down[GLFW_KEY_RIGHT_CONTROL];
		down |= input.is_down[GLFW_KEY_LEFT_CONTROL];
	}
	if (mod == GLFW_KEY_SUPER) {
		down |= input.is_down[GLFW_KEY_LEFT_SUPER];
		down |= input.is_down[GLFW_KEY_RIGHT_SUPER];
	}
	if (mod == GLFW_KEY_SHIFT) {
		down |= input.is_down[GLFW_KEY_LEFT_SHIFT];
		down |= input.is_down[GLFW_KEY_RIGHT_SHIFT];
	}
	if (mod == GLFW_KEY_ALT) {
		down |= input.is_down[GLFW_KEY_LEFT_ALT];
		down |= input.is_down[GLFW_KEY_RIGHT_ALT];
	}

	return down;
}

bool was_chord_pressed(int mod, int key) {
	return is_mod_down(mod) && was_key_pressed(key);
}

Vector2 get_scroll() {
	auto& input = get_input_manager();
	return input.scroll;
}

Vector2 get_mouse_delta() {
	return get_mouse_delta_converted(static_cast<uint32>(Coord::T::Game));
}

Vector2 get_mouse_delta_converted(uint32 coordinate) {
	auto& input = get_input_manager();
	return Coord::convert_mag(input.mouse_delta, Coord::T::Screen, static_cast<Coord::T>(coordinate));
}

u32 shift_key(u32 key) {
	auto& input = get_input_manager();

	bool upper = key >= GLFW_KEY_A && key <= GLFW_KEY_Z;
	bool shift = is_mod_down(GLFW_KEY_SHIFT);

	if (shift && upper) {
    return key;
	}
	else if (shift && !upper) {
		return input.shift_map[key];
	}
	else if (!shift && upper) {
		return key + 32;
	}
	else if (!shift && !upper) {
		return key;
	}

	return key;
}

void set_game_focus(bool focus) {
	auto& input = get_input_manager();
	input.game_focus = focus;
}

#ifndef DN_INPUT_H
#define DN_INPUT_H

typedef enum {
	DN_INPUT_DEVICE_MOUSE_AND_KEYBOARD = 0,
	DN_INPUT_DEVICE_CONTROLLER = 1,
} dn_input_device_t;

typedef struct {
	Vector2 framebuffer_position;
	Vector2 framebuffer_size;
	Vector2 camera;
	Vector2 mouse;
	Vector2 mouse_delta;
	Vector2 scroll;
	bool got_keyboard_input;
	bool got_mouse_input;

	bool is_down[GLFW_KEY_LAST];
	bool was_down[GLFW_KEY_LAST];
	char shift_map[128];
} dn_input_t;
dn_input_t dn_input;



DN_API bool              dn_input_pressed(int key);
DN_API bool              dn_input_released(int key);
DN_API bool              dn_input_down(int key);
DN_API bool              dn_input_mod_down(int mod);
DN_API bool              dn_input_chord_pressed(int mod, int key);
DN_API Vector2           dn_input_scroll();
DN_API Vector2           dn_input_mouse(dn_coord_t unit);
DN_API Vector2           dn_input_mouse_delta(dn_coord_t unit);
DN_API u32               dn_input_shift_key(u32 key);
DN_API dn_input_device_t dn_input_get_device();
DN_IMP void              dn_input_init();
DN_IMP void              dn_input_update();       
DN_IMP void              dn_input_callback_cursor(GLFWwindow* window, double xpos, double ypos);
DN_IMP void              dn_input_callback_click(GLFWwindow* window, int button, int action, int mods);
DN_IMP void              dn_input_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods);
DN_IMP void              dn_input_callback_scroll(GLFWwindow* window, double xoffset, double yoffset);
DN_IMP void              dn_input_callback_error(int err, const char* msg);
DN_IMP void              dn_input_callback_window_size(GLFWwindow* window, int width, int height);
#endif



#ifdef DN_INPUT_IMPLEMENTATION

void dn_input_init() {
	dn_os_zero_memory(dn_input.shift_map, sizeof(dn_input.shift_map));
		
	dn_input.shift_map[' ']  =  ' ';
	dn_input.shift_map['\''] =  '"';
	dn_input.shift_map[',']  =  '<';
	dn_input.shift_map['-']  =  '_';
	dn_input.shift_map['.']  =  '>';
	dn_input.shift_map['/']  =  '?';

	dn_input.shift_map['0']  =  ')';
	dn_input.shift_map['1']  =  '!';
	dn_input.shift_map['2']  =  '@';
	dn_input.shift_map['3']  =  '#';
	dn_input.shift_map['4']  =  '$';
	dn_input.shift_map['5']  =  '%';
	dn_input.shift_map['6']  =  '^';
	dn_input.shift_map['7']  =  '&';
	dn_input.shift_map['8']  =  '*';
	dn_input.shift_map['9']  =  '(';

	dn_input.shift_map[';']  =  ':';
	dn_input.shift_map['=']  =  '+';
	dn_input.shift_map['[']  =  '{';
	dn_input.shift_map['\\'] =  '|';
	dn_input.shift_map[']']  =  '}';
	dn_input.shift_map['`']  =  '~';
		
	dn_input.shift_map['a']  =  'A';
	dn_input.shift_map['b']  =  'B';
	dn_input.shift_map['c']  =  'C';
	dn_input.shift_map['d']  =  'D';
	dn_input.shift_map['e']  =  'E';
	dn_input.shift_map['f']  =  'F';
	dn_input.shift_map['g']  =  'G';
	dn_input.shift_map['h']  =  'H';
	dn_input.shift_map['i']  =  'I';
	dn_input.shift_map['j']  =  'J';
	dn_input.shift_map['k']  =  'K';
	dn_input.shift_map['l']  =  'L';
	dn_input.shift_map['m']  =  'M';
	dn_input.shift_map['n']  =  'N';
	dn_input.shift_map['o']  =  'O';
	dn_input.shift_map['p']  =  'P';
	dn_input.shift_map['q']  =  'Q';
	dn_input.shift_map['r']  =  'R';
	dn_input.shift_map['s']  =  'S';
	dn_input.shift_map['t']  =  'T';
	dn_input.shift_map['u']  =  'U';
	dn_input.shift_map['v']  =  'V';
	dn_input.shift_map['w']  =  'W';
	dn_input.shift_map['x']  =  'X';
	dn_input.shift_map['y']  =  'Y';
	dn_input.shift_map['z']  =  'Z';
}

void dn_input_update() {
	auto& input = dn_input;

	// Reset between frames
	dn_for(key, GLFW_KEY_LAST) {
		input.was_down[key] = input.is_down[key];
	}

	input.scroll.x = 0;
	input.scroll.y = 0;
	input.mouse_delta.x = 0;
	input.mouse_delta.y = 0;
	input.got_keyboard_input = false;
	input.got_mouse_input = false;

	glfwPollEvents();
}

////////////////////
// GLFW Callbacks //
////////////////////
void dn_input_callback_cursor(GLFWwindow* glfw, double x, double y) {
	auto& input_manager = dn_input;
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

void dn_input_callback_click(GLFWwindow* window, int button, int action, int mods) {
	auto& input_manager = dn_input;
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

void dn_input_callback_scroll(GLFWwindow* window, double dx, double dy) {
	auto& input_manager = dn_input;
	input_manager.got_mouse_input = true;

	input_manager.scroll.x = dx;
	input_manager.scroll.y = dy;
}

void dn_input_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto& manager = dn_input;
	manager.got_keyboard_input = true;
	
	if (action == GLFW_PRESS) {
			manager.is_down[key] = true;
	}
	if (action == GLFW_RELEASE) {
			manager.is_down[key] = false;
	}
}

void dn_input_callback_error(int err, const char* msg) {
	dn_log("GLFW error: code = %d, message = %s", err, msg);
}

void dn_input_callback_window_size(GLFWwindow* window_handle, int width, int height) {
	dn_log("%s: width = %d, height = %d", __func__, width, height);
	
	window.content_area.x = width;
	window.content_area.y = height;
	glViewport(0, 0, width, height);

	if (!dn_gpu.targets.size) {
		return;
	}
	auto swapchain = dn_gpu_acquire_swapchain();
	swapchain->size = window.content_area;
	
}


/////////////
// LUA API //
/////////////


bool dn_input_pressed(int key) {
	auto& input = dn_input;
	return input.is_down[key] && !input.was_down[key];
}

bool dn_input_released(int key) {
	auto& input = dn_input;
	return !input.is_down[key] && input.was_down[key];
}

bool dn_input_down(int key) {
	auto& input = dn_input;
	return input.is_down[key];
}

bool dn_input_mod_down(int mod) {
	auto& input = dn_input;
	
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

bool dn_input_chord_pressed(int mod, int key) {
	return dn_input_mod_down(mod) && dn_input_pressed(key);
}

Vector2 dn_input_mouse(dn_coord_t unit) {
	auto& input = dn_input;
	switch (unit) {
		case DN_COORD_UNIT_SCREEN: return input.mouse; break;
		case DN_COORD_UNIT_WINDOW: return dn_coord_screen_to_window(input.mouse.x, input.mouse.y); break;
		case DN_COORD_UNIT_GAME:   return dn_coord_screen_to_game(input.mouse.x, input.mouse.y); break;
		case DN_COORD_UNIT_WORLD:  return dn_coord_screen_to_world(input.mouse.x, input.mouse.y); break;
	}

	DN_ASSERT(false);
	return {0};
}

Vector2 dn_input_mouse_delta(dn_coord_t unit) {
	auto& input = dn_input;
	switch (unit) {
		case DN_COORD_UNIT_SCREEN: return input.mouse_delta; break;
		case DN_COORD_UNIT_WINDOW: return dn_coord_screen_to_window_mag(input.mouse_delta.x, input.mouse_delta.y); break;
		case DN_COORD_UNIT_GAME:   return dn_coord_screen_to_game_mag(input.mouse_delta.x, input.mouse_delta.y); break;
		case DN_COORD_UNIT_WORLD:  return dn_coord_screen_to_world_mag(input.mouse_delta.x, input.mouse_delta.y); break;
	}

	DN_ASSERT(false);
	return {0};
}

Vector2 dn_input_scroll() {
	auto& input = dn_input;
	return input.scroll;
}

u32 dn_input_shift_key(u32 key) {
	auto& input = dn_input;

	bool upper = key >= GLFW_KEY_A && key <= GLFW_KEY_Z;
	bool shift = dn_input_mod_down(GLFW_KEY_SHIFT);

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

dn_input_device_t dn_input_get_device() {
	return steam_input.last_input_device;
}
#endif
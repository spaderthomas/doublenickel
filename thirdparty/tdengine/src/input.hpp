#define INPUT_MASK_EDITOR   0x1
#define INPUT_MASK_GAME     0x2
#define INPUT_MASK_PASSWORD 0x4
#define INPUT_MASK_ALL      0xFF

struct InputManager {
	Vector2 framebuffer_position;
	Vector2 framebuffer_size;
	Vector2 camera;
	Vector2 mouse;
	Vector2 mouse_delta;
	Vector2 scroll;
	bool is_editor_requesting_input;
	bool got_keyboard_input;
	bool got_mouse_input;
	bool game_focus;


	uint32 mask;
	uint32 last_mask;

	bool is_down[GLFW_KEY_LAST];
	bool was_down[GLFW_KEY_LAST];
	char shift_map[128];

	InputManager();

	void start_imgui();
	void stop_imgui();
	void begin_frame();
	void end_frame();	
	void fill_shift_map();
	void check_focus();
};

InputManager& get_input_manager();
void update_input();

// GLFW Callbacks
void GLFW_Cursor_Pos_Callback(GLFWwindow* window, double xpos, double ypos);
void GLFW_Mouse_Button_Callback(GLFWwindow* window, int button, int action, int mods);
void GLFW_Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFW_Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
void GLFW_Error_Callback(int err, const char* msg);
void GLFW_Window_Size_Callback(GLFWwindow* window, int width, int height);

FM_LUA_EXPORT bool is_editor_requesting_input();
FM_LUA_EXPORT bool was_key_pressed(int key);
FM_LUA_EXPORT bool was_key_released(int key);
FM_LUA_EXPORT bool is_key_down(int key);
FM_LUA_EXPORT bool is_mod_down(int mod);
FM_LUA_EXPORT bool was_chord_pressed(int mod, int key);
FM_LUA_EXPORT Vector2 get_scroll();
FM_LUA_EXPORT Vector2 get_mouse_delta();
FM_LUA_EXPORT Vector2 get_mouse_delta_converted(uint32 coordinate);
FM_LUA_EXPORT void hide_cursor();
FM_LUA_EXPORT void show_cursor();
FM_LUA_EXPORT u32 shift_key(u32 key);
FM_LUA_EXPORT void set_game_focus(bool focus);

enum class WindowFlags {
	None = 0,
	Windowed = 1 << 0,
	Border = 1 << 1,
	Vsync = 1 << 2
};
DEFINE_ENUM_FLAG_OPERATORS(WindowFlags)

enum class DisplayMode : u32 {
	// 16:9
	p480,
	p720,
	p1080,
	p1440,
	p2160,

	// 16:10
	p1280_800,

	// Fullscreen
	FullScreen
};

struct WindowInfo {
	GLFWwindow* handle;
	WindowFlags flags;
	DisplayMode display_mode;
	Vector2I windowed_position;
	Vector2 native_resolution;
	Vector2 requested_area;
	Vector2 content_area;
};
WindowInfo window;

void init_glfw();
void shutdown_glfw();
void set_native_resolution(float width, float height);
float get_display_scale();

FM_LUA_EXPORT Vector2 get_content_area();
FM_LUA_EXPORT Vector2 get_game_area_size();
FM_LUA_EXPORT Vector2 get_native_resolution();

FM_LUA_EXPORT void set_game_area_size(float x, float y);
FM_LUA_EXPORT void set_game_area_position(float x, float y);

FM_LUA_EXPORT void create_window(const char* title, u32 x, u32 y, WindowFlags flags);
FM_LUA_EXPORT void set_window_icon(const char* path);
FM_LUA_EXPORT void set_display_mode(DisplayMode mode);
FM_LUA_EXPORT DisplayMode get_display_mode();
FM_LUA_EXPORT void hide_cursor();
FM_LUA_EXPORT void show_cursor();

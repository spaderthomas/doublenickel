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

DN_API Vector2 get_content_area();
DN_API Vector2 get_game_area_size();
DN_API Vector2 get_native_resolution();

DN_API void set_game_area_size(float x, float y);
DN_API void set_game_area_position(float x, float y);

DN_API void create_window(const char* title, u32 x, u32 y, WindowFlags flags);
DN_API void set_window_icon(const char* path);
DN_API void set_display_mode(DisplayMode mode);
DN_API DisplayMode get_display_mode();
DN_API void hide_cursor();
DN_API void show_cursor();

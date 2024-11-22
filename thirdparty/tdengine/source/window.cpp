void shutdown_glfw() {
	glfwDestroyWindow(window.handle);
	glfwTerminate();
}

void create_window(const char* title, u32 x, u32 y, WindowFlags flags) {
	set_native_resolution(x, y);
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	auto monitor = glfwGetPrimaryMonitor();

	int wax, way, wsx, wsy;
	glfwGetMonitorWorkarea(monitor, &wax, &way, &wsx, &wsy);


	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Translate the window flags into GLFW stuff that'll set up the window correctly	
	if (enum_any(flags & WindowFlags::Windowed)) {
		monitor = nullptr;
	}
	
	if (!enum_any(flags & WindowFlags::Border)) {
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	}

	tdns_log.write("creating window: native_resolution = [%.0f, %.0f], content_area = [%.0f, %.0f], windowed = %d, border = %d, vsync = %d, refresh_rate = %d, monitor = %d",
				   window.native_resolution.x, window.native_resolution.y,
				   window.content_area.x, window.content_area.y,
				   flags & WindowFlags::Windowed,
				   flags & WindowFlags::Border,
				   flags & WindowFlags::Vsync,
				   mode->refreshRate,
				   monitor != nullptr);

	// Init the window, give it a GL context, and load OpenGL. Don't bother passing in the real size here, because we're going to set it later.
	window.handle = glfwCreateWindow(1, 1, title, monitor, NULL);
	glfwMakeContextCurrent(window.handle);

	// Initialize OpenGL
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	auto version = glGetString(GL_VERSION);
    if (version) {
		tdns_log.write("OpenGL version: %s", version);
    } else {
		tdns_log.write("Failed to get OpenGL version");
    }

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(on_opengl_message, 0);

	// This has to be done after context creation
	if (enum_any(flags & WindowFlags::Vsync)) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}

	glfwSetCursorPosCallback(window.handle, GLFW_Cursor_Pos_Callback);
	glfwSetMouseButtonCallback(window.handle, GLFW_Mouse_Button_Callback);
	glfwSetKeyCallback(window.handle, GLFW_Key_Callback);
	glfwSetScrollCallback(window.handle, GLFW_Scroll_Callback);
	glfwSetWindowSizeCallback(window.handle, GLFW_Window_Size_Callback);

	init_noise();
	init_imgui();
	init_render();
	init_texture_atlas(); // Invert control
	init_backgrounds(); // Invert control
	init_screenshots(); // Use the asset loader
	init_particles();
	init_fluid();

#ifdef FM_EDITOR
	// Set a best guess for our default output resolution based on the monitor
	if (mode->width == 3840) {
		set_display_mode(DisplayMode::p2160);
	}
	else if (mode->width == 2560) {
		set_display_mode(DisplayMode::p1440);
	}
	else if (mode->width == 1920) {
		set_display_mode(DisplayMode::p1080);
	}
	else {
		set_display_mode(DisplayMode::p1080);
	}
#else
	if (SteamUtils()->IsSteamRunningOnSteamDeck()) {
		set_display_mode(DisplayMode::p1280_800);
	} else {
		set_display_mode(DisplayMode::p1080);
	}
#endif
}

void set_window_icon(const char* path) {
	int width, height, channels;
	unsigned char* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels) {
		std::cerr << "Failed to load icon image!" << std::endl;
		return;
	}

	GLFWimage icon;
	icon.width = width;
	icon.height = height;
	icon.pixels = pixels;

	glfwSetWindowIcon(window.handle, 1, &icon);

	stbi_image_free(pixels);
}

void set_native_resolution(float width, float height) {
	window.native_resolution.x = width;
	window.native_resolution.y = height;
}

void set_display_mode(DisplayMode mode) {
	tdns_log.write("%s: mode = %d", __func__, static_cast<int>(mode));
	
	if (window.display_mode == DisplayMode::FullScreen && mode != DisplayMode::FullScreen) {
		// Toggle back to windowed
		glfwSetWindowMonitor(window.handle, NULL, window.windowed_position.x, window.windowed_position.y, window.requested_area.x, window.requested_area.y, GLFW_DONT_CARE);
	}

	window.display_mode = mode;

	if (window.display_mode == DisplayMode::FullScreen) {
		// Toggle to full screen
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
        glfwGetWindowPos(window.handle, &window.windowed_position.x, &window.windowed_position.y);
        glfwSetWindowMonitor(window.handle, monitor, 0, 0, video_mode->width, video_mode->height, video_mode->refreshRate);
		return;
	}

	if (window.display_mode == DisplayMode::p480) {
		window.requested_area.x = 854;
		window.requested_area.y = 480;
	}
	else if (window.display_mode == DisplayMode::p720) {
		window.requested_area.x = 1280;
		window.requested_area.y = 720;
	}
	else if (window.display_mode == DisplayMode::p1080) {
		window.requested_area.x = 1920;
		window.requested_area.y = 1080;
	}
	else if (window.display_mode == DisplayMode::p1440) {
		window.requested_area.x = 2560;
		window.requested_area.y = 1440;
	}
	else if (window.display_mode == DisplayMode::p2160) {
		window.requested_area.x = 3840;
		window.requested_area.y = 2160;
	}
	else if (window.display_mode == DisplayMode::p1280_800) {
		window.requested_area.x = 1280;
		window.requested_area.y = 800;
	}
	
	glfwSetWindowSize(window.handle, window.requested_area.x, window.requested_area.y);
}

FM_LUA_EXPORT void set_window_size(int x, int y) {
	glfwSetWindowSize(window.handle, x, y);
}

DisplayMode get_display_mode() {
	return window.display_mode;
}

float get_display_scale() {
	return window.content_area.x / window.native_resolution.x;
}

void hide_cursor() {
	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void show_cursor() {
	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

Vector2 get_native_resolution() {
	return window.native_resolution;
}

Vector2 get_content_area() {
	return window.content_area;
}

Vector2 get_game_area_size() {
	return Coord::game_area_size;
}

void set_game_area_size(float32 x, float32 y) {
	Coord::game_area_size.x = x;
	Coord::game_area_size.y = y;
}

void set_game_area_position(float32 x, float32 y) {
	Coord::game_area_position.x = x;
	Coord::game_area_position.y = y;
}

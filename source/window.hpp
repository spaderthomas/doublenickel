#ifndef DN_WINDOW_H
#define DN_WINDOW_H
typedef enum {
  DN_WINDOW_FLAG_NONE = 0,
  DN_WINDOW_FLAG_WINDOWED = 1,
  DN_WINDOW_FLAG_BORDER = 2,
  DN_WINDOW_FLAG_VSYNC = 4
} dn_window_flags_t;

typedef enum {
  DN_DISPLAY_MODE_AUTO,

  // 16:9
  DN_DISPLAY_MODE_180,
  DN_DISPLAY_MODE_480,
  DN_DISPLAY_MODE_720,
  DN_DISPLAY_MODE_1080,
  DN_DISPLAY_MODE_1440,
  DN_DISPLAY_MODE_2160,

  // 16:10
  DN_DISPLAY_MODE_1280_800,

  // Fullscreen
  DN_DISPLAY_MODE_FULLSCREEN
} dn_display_mode_t;

typedef struct {
  const char* title;
  const char* icon;
  dn_display_mode_t display_mode;
  Vector2 native_resolution;
  dn_window_flags_t flags;
} dn_window_config_t;

typedef struct {
  GLFWwindow* handle;
  dn_window_flags_t flags;
  dn_display_mode_t display_mode;
  Vector2I windowed_position;
  Vector2 native_resolution;
  Vector2 requested_area;
  Vector2 content_area;
} dn_window_t;
dn_window_t window;

DN_IMP void               dn_window_shutdown();
DN_API dn_window_config_t dn_window_config_default();
DN_API void               dn_window_init(dn_window_config_t config);
DN_API void               dn_window_set_native_resolution(float width, float height);
DN_API Vector2            dn_window_get_content_area();
DN_API Vector2            dn_window_get_native_resolution();
DN_API void               dn_window_set_icon(const char* path);
DN_API void               dn_window_set_display_mode(dn_display_mode_t mode);
DN_API dn_display_mode_t  dn_window_get_display_mode();
DN_API void               dn_window_set_cursor_visible(bool visible);
#endif

#ifdef DN_WINDOW_IMPLEMENTATION
void dn_window_shutdown() {
  glfwDestroyWindow(window.handle);
  glfwTerminate();
}

dn_window_config_t dn_window_config_default() {
  return {
    .title = "doublenickel",
    .icon = dn_paths_resolve_format("dn_image", "logo/icon.png"),
    .display_mode = DN_DISPLAY_MODE_AUTO,
    .native_resolution = {
      .x = 1920, 
      .y = 1080 
    },
    .flags = (dn_window_flags_t)(DN_WINDOW_FLAG_BORDER | DN_WINDOW_FLAG_WINDOWED | DN_WINDOW_FLAG_VSYNC)
  };
}

void dn_window_init(dn_window_config_t config) {
  dn_window_set_native_resolution(config.native_resolution.x, config.native_resolution.y);
  
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
  if (config.flags & DN_WINDOW_FLAG_WINDOWED) {
    monitor = nullptr;
  }
  
  if (!(config.flags & DN_WINDOW_FLAG_BORDER)) {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  }

  dn_log("creating window: native_resolution = [%.0f, %.0f], content_area = [%.0f, %.0f], windowed = %d, border = %d, vsync = %d, refresh_rate = %d, monitor = %d",
           window.native_resolution.x, window.native_resolution.y,
           window.content_area.x, window.content_area.y,
           config.flags & DN_WINDOW_FLAG_WINDOWED,
           config.flags & DN_WINDOW_FLAG_BORDER,
           config.flags & DN_WINDOW_FLAG_VSYNC,
           mode->refreshRate,
           monitor != nullptr);

  // Init the window, give it a GL context, and load OpenGL. Don't bother passing in the real size here, because we're going to set it later.
  window.handle = glfwCreateWindow(1, 1, config.title, monitor, NULL);
  glfwMakeContextCurrent(window.handle);

  // Initialize OpenGL
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  auto version = glGetString(GL_VERSION);
    if (version) {
    dn_log("OpenGL version: %s", version);
    } else {
    dn_log("Failed to get OpenGL version");
    }

  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(gl_error_callback, 0);

  // This has to be done after context creation
  if (config.flags & DN_WINDOW_FLAG_VSYNC) {
    glfwSwapInterval(1);
  }
  else {
    glfwSwapInterval(0);
  }

  glfwSetCursorPosCallback(window.handle, dn_input_callback_cursor);
  glfwSetMouseButtonCallback(window.handle, dn_input_callback_click);
  glfwSetKeyCallback(window.handle, dn_input_callback_key);
  glfwSetScrollCallback(window.handle, dn_input_callback_scroll);
  glfwSetWindowSizeCallback(window.handle, dn_input_callback_window_size);

  if (config.display_mode == DN_DISPLAY_MODE_AUTO) {
    // Set a best guess for our default output resolution based on the monitor
    if (mode->width == 3840) {
      dn_window_set_display_mode(DN_DISPLAY_MODE_2160);
    }
    else if (mode->width == 2560) {
      dn_window_set_display_mode(DN_DISPLAY_MODE_1440);
    }
    else if (mode->width == 1920) {
      dn_window_set_display_mode(DN_DISPLAY_MODE_1080);
    }
    else {
      dn_window_set_display_mode(DN_DISPLAY_MODE_1080);
    }
  }
  else {
    dn_window_set_display_mode(config.display_mode);
  }                                     

  dn_window_set_icon(config.icon);
}

void dn_window_set_icon(const char* path) {
  dn_log("%s: %s", __func__, path);
  
  int width, height, channels;
  unsigned char* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
  if (!pixels) {
    dn_log("%s: Failed to load icon image", __func__);
    return;
  }

  GLFWimage icon;
  icon.width = width;
  icon.height = height;
  icon.pixels = pixels;

  glfwSetWindowIcon(window.handle, 1, &icon);

  stbi_image_free(pixels);
}

void dn_window_set_native_resolution(float width, float height) {
  window.native_resolution.x = width;
  window.native_resolution.y = height;
}

void dn_window_set_display_mode(dn_display_mode_t mode) {
  dn_log("%s: mode = %d", __func__, static_cast<int>(mode));
  
  if (window.display_mode == DN_DISPLAY_MODE_FULLSCREEN && mode != DN_DISPLAY_MODE_FULLSCREEN) {
    // Toggle back to windowed
    glfwSetWindowMonitor(window.handle, NULL, window.windowed_position.x, window.windowed_position.y, window.requested_area.x, window.requested_area.y, GLFW_DONT_CARE);
  }

  window.display_mode = mode;

  switch (window.display_mode) {
    case DN_DISPLAY_MODE_FULLSCREEN: {
      GLFWmonitor* monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
      glfwGetWindowPos(window.handle, &window.windowed_position.x, &window.windowed_position.y);
      glfwSetWindowMonitor(window.handle, monitor, 0, 0, video_mode->width, video_mode->height, video_mode->refreshRate);

      return;
    } break;
    case DN_DISPLAY_MODE_180: window.requested_area = { .x = 320, .y = 180 }; break;
    case DN_DISPLAY_MODE_480: window.requested_area = { .x = 854, .y = 480 }; break;
    case DN_DISPLAY_MODE_720: window.requested_area = { .x = 1280, .y = 720 }; break;
    case DN_DISPLAY_MODE_1080: window.requested_area = { .x = 1920, .y = 1080 }; break;
    case DN_DISPLAY_MODE_1440: window.requested_area = { .x = 2560, .y = 1440 }; break;
    case DN_DISPLAY_MODE_2160: window.requested_area = { .x = 3840, .y = 2160 }; break;
    case DN_DISPLAY_MODE_1280_800: window.requested_area = { .x = 1280, .y = 800 }; break;
  }

  glfwSetWindowSize(window.handle, window.requested_area.x, window.requested_area.y);
}

DN_API void set_window_size(int x, int y) {
  glfwSetWindowSize(window.handle, x, y);
}

dn_display_mode_t dn_window_get_display_mode() {
  return window.display_mode;
}

void dn_window_set_cursor_visible(bool visible) {
  glfwSetInputMode(window.handle, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

Vector2 dn_window_get_native_resolution() {
  return window.native_resolution;
}

Vector2 dn_window_get_content_area() {
  return window.content_area;
}
#endif
#ifndef DN_IMGUI_H
#define DN_IMGUI_H

typedef struct {
  Vector4 light;
  Vector4 medium_light;
  Vector4 low_light;
  Vector4 neutral;
  Vector4 medium_dark;
} dn_imgui_colors_t;

typedef struct {
  char* layout_to_load;
  bool open_file_browser;
  bool close_file_browser;  
  dn_imgui_colors_t colors;
} dn_imgui_t;
dn_imgui_t dn_imgui;
ImGui::FileBrowser dn_imgui_file_browser;


DN_IMP void    dn_imgui_init();
DN_IMP void    dn_imgui_shutdown();
DN_IMP void    dn_imgui_update();
DN_API void    dn_imgui_push_font(const char* font_name, u32 size);
DN_API void    dn_imgui_image(const char* image, float sx, float sy);
DN_API void    dn_imgui_file_browser_open();
DN_API void    dn_imgui_file_browser_close();
DN_API void    dn_imgui_file_browser_set_work_dir(const char* directory);
DN_API bool    dn_imgui_file_browser_is_file_selected();
DN_API dn_tstring_t dn_imgui_file_browser_get_selected_file();
DN_API void    dn_imgui_load_layout(const char* file_name);
DN_API void    dn_imgui_save_layout(const char* file_name);
DN_API void    dn_imgui_load_colors(dn_imgui_colors_t colors);

#endif



#ifdef DN_IMGUI_IMPLEMENTATION
void dn_imgui_init() {
  dn_log("%s", __func__);
  dn_imgui = dn_zero_initialize();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  auto& imgui = ImGui::GetIO();
  imgui.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  //imgui.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  imgui.ConfigWindowsMoveFromTitleBarOnly = true;
  ImGui::StyleColorsDark();

  imgui.IniFilename = nullptr;

  auto& style = ImGui::GetStyle();
  style.FrameRounding = 6;
  style.WindowRounding = 6;
  style.WindowBorderSize = 0;

  dn_imgui_load_colors({
    .light        = { .r = 0.47f, .g = 0.47f, .b = 0.47f, .a = 255.0f },
    .medium_light = { .r = 0.35f, .g = 0.35f, .b = 0.35f, .a = 255.0f },
    .low_light    = { .r = 0.31f, .g = 0.31f, .b = 0.31f, .a = 255.0f },
    .neutral      = { .r = 0.22f, .g = 0.22f, .b = 0.22f, .a = 255.0f },
    .medium_dark  = { .r = 0.16f, .g = 0.16f, .b = 0.16f, .a = 255.0f },
  });
  
  // Engine will pick this up on the first tick (before ImGui renders, so no flickering)
  dn_imgui_load_layout("default");

  dn_log("%s: Initializing ImGui backend", __func__);
  ImGui_ImplGlfw_InitForOpenGL(window.handle, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  dn_imgui_file_browser = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);
}

void dn_imgui_update() {
  if (dn_imgui.layout_to_load) {
    ImGui::LoadIniSettingsFromDisk(dn_imgui.layout_to_load);

    dn_allocators.standard.free(dn_imgui.layout_to_load); 
    dn_imgui.layout_to_load = nullptr;
  }
  
  ImGui_ImplGlfw_NewFrame();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui::NewFrame();

  if (dn_imgui.open_file_browser) {
    dn_imgui_file_browser.Open();
    dn_imgui.open_file_browser = false;
  }

  if (dn_imgui.close_file_browser) {
    dn_imgui_file_browser.Close();
    dn_imgui.close_file_browser = false;
  }
  
  dn_imgui_file_browser.Display();
}

void dn_imgui_shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
}

void dn_imgui_load_colors(dn_imgui_colors_t colors) {
  ImVec4* imgui_colors = ImGui::GetStyle().Colors;
  
  #define DN_VEC4_TO_IMVEC4(dn_vec) ImVec4((dn_vec.r), (dn_vec.g), (dn_vec.b), (dn_vec.a))

  imgui_colors[ImGuiCol_Button]             = DN_VEC4_TO_IMVEC4(colors.medium_light);
  imgui_colors[ImGuiCol_ButtonHovered]      = DN_VEC4_TO_IMVEC4(colors.light);
  imgui_colors[ImGuiCol_ButtonActive]       = DN_VEC4_TO_IMVEC4(colors.medium_light);
  imgui_colors[ImGuiCol_FrameBg]            = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_FrameBgHovered]     = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_FrameBgActive]      = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_Header]             = DN_VEC4_TO_IMVEC4(colors.neutral);
  imgui_colors[ImGuiCol_HeaderHovered]      = DN_VEC4_TO_IMVEC4(colors.low_light);
  imgui_colors[ImGuiCol_HeaderActive]       = DN_VEC4_TO_IMVEC4(colors.neutral);
  imgui_colors[ImGuiCol_Separator]          = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_Tab]                = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_TabHovered]         = DN_VEC4_TO_IMVEC4(colors.neutral);
  imgui_colors[ImGuiCol_TabActive]          = DN_VEC4_TO_IMVEC4(colors.neutral);
  imgui_colors[ImGuiCol_TabUnfocused]       = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_TabUnfocusedActive] = DN_VEC4_TO_IMVEC4(colors.neutral);
  imgui_colors[ImGuiCol_TitleBg]            = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_TitleBgActive]      = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_TitleBgCollapsed]   = DN_VEC4_TO_IMVEC4(colors.medium_dark);
  imgui_colors[ImGuiCol_WindowBg]           = DN_VEC4_TO_IMVEC4(colors.neutral);

  dn_imgui.colors = colors;
}

void dn_imgui_load_layout(const char* file_name) {
  dn_log("%s: %s", __func__, file_name);
  dn_imgui.layout_to_load = dn_paths_resolve_format_ex("layout", file_name, &dn_allocators.standard);
}

void dn_imgui_save_layout(const char* file_name) {
  auto file_path = dn_paths_resolve_format("layout", file_name);  
  ImGui::SaveIniSettingsToDisk(file_path);
}


void dn_imgui_push_font(const char* font_name, u32 size) {
  auto font = dn_font_find(font_name, size);
  if (font && font->imfont) {
    ImGui::PushFont(font->imfont);
  }
}

void dn_imgui_image(const char* image, float sx, float sy) {
  auto sprite = find_sprite(image);
  if (!sprite) sprite = find_sprite("debug.png");

  auto uv = sprite->uv;
  float xmin = sprite->uv[0].x;
  float xmax = sprite->uv[2].x;
  float ymin = sprite->uv[0].y;
  float ymax = sprite->uv[1].y;

  auto texture = find_texture(sprite->texture);
  ImGui::Image((ImTextureID)texture->handle, ImVec2(sx, sy), ImVec2(xmin, ymin), ImVec2(xmax, ymax));
}

void dn_imgui_file_browser_open() {
  dn_imgui.open_file_browser = true;
}

void dn_imgui_file_browser_close() {
  dn_imgui.close_file_browser = true;
}

void dn_imgui_file_browser_set_work_dir(const char* directory) {
  dn_imgui_file_browser.SetPwd(directory);
}

bool dn_imgui_file_browser_is_file_selected() {
  return dn_imgui_file_browser.HasSelected();
}

dn_tstring_t dn_imgui_file_browser_get_selected_file() {
  std::string selected = dn_imgui_file_browser.GetSelected().string();
  dn_imgui_file_browser.ClearSelected();

  return dn_string_copy(selected, &dn_allocators.bump);
}
#endif
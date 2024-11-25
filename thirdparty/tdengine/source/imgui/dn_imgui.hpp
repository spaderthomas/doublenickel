#ifndef DN_IMGUI_H
#define DN_IMGUI_H

typedef struct {
	ImVec4 light;
	ImVec4 medium_light;
	ImVec4 low_light;
	ImVec4 neutral;
	ImVec4 medium_dark;
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
DN_API tstring dn_imgui_file_browser_get_selected_file();
DN_API void    dn_imgui_load_layout(const char* file_name);
DN_API void    dn_imgui_save_layout(const char* file_name);
DN_API void    dn_imgui_load_colors(dn_imgui_colors_t colors);

#endif



#ifdef DN_IMGUI_IMPLEMENTATION
void dn_imgui_init() {
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
		.light = ImVec4(0.47f, 0.47f, 0.47f, 255.0f),
		.medium_light = ImVec4(0.35f, 0.35f, 0.35f, 255.0f),
		.low_light = ImVec4(0.31f, 0.31f, 0.31f, 255.0f),
		.neutral = ImVec4(0.22f, 0.22f, 0.22f, 255.0f),
		.medium_dark = ImVec4(0.16f, 0.16f, 0.16f, 255.0f),
	});
	
	// Engine will pick this up on the first tick (before ImGui renders, so no flickering)
	dn_imgui_load_layout("default");

	ImGui_ImplGlfw_InitForOpenGL(window.handle, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	dn_imgui_file_browser = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);
}

void dn_imgui_update() {
	if (dn_imgui.layout_to_load) {
		ImGui::LoadIniSettingsFromDisk(dn_imgui.layout_to_load);

		standard_allocator.free(dn_imgui.layout_to_load); 
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
	
	imgui_colors[ImGuiCol_Button]             = colors.medium_light;
	imgui_colors[ImGuiCol_ButtonHovered]      = colors.light;
	imgui_colors[ImGuiCol_ButtonActive]       = colors.medium_light;
	imgui_colors[ImGuiCol_FrameBg]            = colors.medium_dark;
	imgui_colors[ImGuiCol_FrameBgHovered]     = colors.medium_dark;
	imgui_colors[ImGuiCol_FrameBgActive]      = colors.medium_dark;
	imgui_colors[ImGuiCol_Header]             = colors.neutral;
	imgui_colors[ImGuiCol_HeaderHovered]      = colors.low_light;
	imgui_colors[ImGuiCol_HeaderActive]       = colors.neutral;
	imgui_colors[ImGuiCol_Separator]          = colors.medium_dark;
	imgui_colors[ImGuiCol_Tab]                = colors.medium_dark;
	imgui_colors[ImGuiCol_TabHovered]         = colors.neutral;
	imgui_colors[ImGuiCol_TabActive]          = colors.neutral;
	imgui_colors[ImGuiCol_TabUnfocused]       = colors.medium_dark;
	imgui_colors[ImGuiCol_TabUnfocusedActive] = colors.neutral;
	imgui_colors[ImGuiCol_TitleBg]            = colors.medium_dark;
	imgui_colors[ImGuiCol_TitleBgActive]      = colors.medium_dark;
	imgui_colors[ImGuiCol_TitleBgCollapsed]   = colors.medium_dark;
	imgui_colors[ImGuiCol_WindowBg]           = colors.neutral;

	dn_imgui.colors = colors;
}

void dn_imgui_load_layout(const char* file_name) {
	dn_imgui.layout_to_load = dn_paths_resolve_format_ex("layout", file_name, &standard_allocator);
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

tstring dn_imgui_file_browser_get_selected_file() {
	std::string selected = dn_imgui_file_browser.GetSelected().string();
	dn_imgui_file_browser.ClearSelected();

	return copy_string(selected, &bump_allocator);
}
#endif
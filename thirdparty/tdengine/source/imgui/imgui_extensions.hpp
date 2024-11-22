void init_imgui();
void update_imgui();
void shutdown_imgui();

// Set this string from a script, and we will pick up a new layout next tick
string layout_to_load;
FM_LUA_EXPORT void use_editor_layout(const char* file_name);
FM_LUA_EXPORT void save_editor_layout(const char* file_name);
FM_LUA_EXPORT void render_imgui();

	ImGui::FileBrowser im_file_browser;
	bool open_file_browser;
	bool close_file_browser;

namespace ImGuiColors {
	ImVec4 Light = ImVec4(0.47f, 0.47f, 0.47f, 255.0f);
	ImVec4 MediumLight = ImVec4(0.35f, 0.35f, 0.35f, 255.0f);
	ImVec4 LowLight = ImVec4(0.31f, 0.31f, 0.31f, 255.0f);
	ImVec4 Neutral = ImVec4(0.22f, 0.22f, 0.22f, 255.0f);
	ImVec4 MediumDark = ImVec4(0.16f, 0.16f, 0.16f, 255.0f);

	void Load() {
		ImVec4* colors = ImGui::GetStyle().Colors;
		
		colors[ImGuiCol_Button]             = MediumLight;
		colors[ImGuiCol_ButtonHovered]      = Light;
		colors[ImGuiCol_ButtonActive]       = MediumLight;
		colors[ImGuiCol_FrameBg]            = MediumDark;
		colors[ImGuiCol_FrameBgHovered]     = MediumDark;
		colors[ImGuiCol_FrameBgActive]      = MediumDark;
		colors[ImGuiCol_Header]             = Neutral;
		colors[ImGuiCol_HeaderHovered]      = LowLight;
		colors[ImGuiCol_HeaderActive]       = Neutral;
		colors[ImGuiCol_Separator]          = MediumDark;
		colors[ImGuiCol_Tab]                = MediumDark;
		colors[ImGuiCol_TabHovered]         = Neutral;
		colors[ImGuiCol_TabActive]          = Neutral;
		colors[ImGuiCol_TabUnfocused]       = MediumDark;
		colors[ImGuiCol_TabUnfocusedActive] = Neutral;
		colors[ImGuiCol_TitleBg]            = MediumDark;
		colors[ImGuiCol_TitleBgActive]      = MediumDark;
		colors[ImGuiCol_TitleBgCollapsed]   = MediumDark;
		colors[ImGuiCol_WindowBg]           = Neutral;
	}
};

FM_LUA_EXPORT void IGE_PushGameFont(const char* font_name);
FM_LUA_EXPORT void IGE_GameImage(const char* image, float sx, float sy);
FM_LUA_EXPORT void IGE_OpenFileBrowser();
FM_LUA_EXPORT void IGE_CloseFileBrowser();
FM_LUA_EXPORT void IGE_SetFileBrowserWorkDir(const char* directory);
FM_LUA_EXPORT bool IGE_IsAnyFileSelected();
FM_LUA_EXPORT tstring IGE_GetSelectedFile();
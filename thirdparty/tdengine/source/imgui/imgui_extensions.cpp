void init_imgui() {
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

	ImGuiColors::Load();
	
	// Engine will pick this up on the first tick (before ImGui renders, so no flickering)
	use_editor_layout("minimal");

	ImGui_ImplGlfw_InitForOpenGL(window.handle, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	im_file_browser = ImGui::FileBrowser(ImGuiFileBrowserFlags_SelectDirectory);
}

void UpdateFileBrowser() {
	if (open_file_browser) {
		im_file_browser.Open();
		open_file_browser = false;
	}

	if (close_file_browser) {
		im_file_browser.Close();
		close_file_browser = false;
	}
	
	im_file_browser.Display();
}


void update_imgui() {
	if (layout_to_load) {
		ImGui::LoadIniSettingsFromDisk(layout_to_load);

		standard_allocator.free(layout_to_load); 
		layout_to_load = nullptr;
	}
	
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	UpdateFileBrowser();
}

void render_imgui() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdown_imgui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void use_editor_layout(const char* file_name) {
	layout_to_load = resolve_format_path_ex("layout", file_name, &standard_allocator);
}

void save_editor_layout(const char* file_name) {
	auto file_path = resolve_format_path("layout", file_name);	
	ImGui::SaveIniSettingsToDisk(file_path);
}


void IGE_PushGameFont(const char* font_name) {
	auto font = font_find(font_name);
	if (font && font->imfont) {
		ImGui::PushFont(font->imfont);
	}
}

void IGE_GameImage(const char* image, float sx, float sy) {
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

void IGE_OpenFileBrowser() {
	open_file_browser = true;
}

void IGE_CloseFileBrowser() {
	close_file_browser = true;
}

void IGE_SetFileBrowserWorkDir(const char* directory) {
	im_file_browser.SetPwd(directory);
}

bool IGE_IsAnyFileSelected() {
	return im_file_browser.HasSelected();
}

tstring IGE_GetSelectedFile() {
	std::string selected = im_file_browser.GetSelected().string();
	im_file_browser.ClearSelected();

	return copy_string(selected, &bump_allocator);
}
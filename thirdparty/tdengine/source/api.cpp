int API::screen_dimensions(lua_State* lua) {
	lua_newtable(lua);

	lua_pushstring(lua, "x");
	lua_pushnumber(lua, window.content_area.x);
	lua_settable(lua, -3);

	lua_pushstring(lua, "y");
	lua_pushnumber(lua, window.content_area.y);
	lua_settable(lua, -3);
	
	return 1;
}

int API::sprite_size(lua_State* lua) {
	fm_assert(lua_gettop(lua) == 1);

	int32 sx = 0;
	int32 sy = 0;
	
	const char* name = lua_tostring(lua, -1);
	auto sprite = find_sprite(name);
	if (!sprite) goto done;

	sx = sprite->size.x;
	sy = sprite->size.y;

	done:
	lua_pushnumber(lua, sx);
	lua_pushnumber(lua, sy);
	return 2;
}

int API::log(lua_State* lua) {
	fm_assert(lua_gettop(lua) == 1);

	const char* fmt = lua_tostring(lua, 1);
	tdns_log.write(Log_Flags::Default, fmt);

	return 0;
}


int API::scandir_impl(lua_State* lua) {
	fm_assert(lua_gettop(lua) == 1);

	const char* dir = lua_tostring(lua, 1);

	lua_newtable(lua);
	int32 i = 1;

#ifdef _WIN32
    WIN32_FIND_DATA find_data;
	
	auto check_dir_entry = [&](){
		if (!strcmp(find_data.cFileName, ".")) return;
		if (!strcmp(find_data.cFileName, "..")) return;
		lua_pushinteger(lua, i++);
		lua_pushstring(lua, find_data.cFileName);
		lua_settable(lua, -3);
	};

	
	// Find the first one
    auto handle = FindFirstFile(dir, &find_data);
	if (handle == INVALID_HANDLE_VALUE) return 1;

	check_dir_entry();

    // Look for more
	while (FindNextFile(handle, &find_data)) {
		check_dir_entry();
	}

    FindClose(handle);
#endif
	
	return 1;
}

int API::get_character_size(lua_State* l) {
	fm_assert(lua_gettop(l) >= 1);

	// Parse arguments
	char c = lua_tointeger(l, 1);

	// Find the font, or just use the editor by default
	FontInfo* font = nullptr;
	if (lua_gettop(l) == 2) font = font_find(lua_tostring(l, 2));
	if (!font) font = font_find("inconsolata-32");

	// If the font is an ImGui font, ask ImGui. Otherwise, calculate it ourself.
	float32 x;
	float32 y;
	if (font->imfont) {
		auto size = font->imfont->CalcTextSizeA(
                 (float32)font->size,
				 FLT_MAX, FLT_MAX,
				 (const char*)&c, (const char*)&c + 1,
				 NULL);
		x = size.x / window.content_area.x;
		y = size.y / window.content_area.y;
	} 
	else {
		auto glyph = font->glyphs[c];
		x = glyph->advance.x / get_display_scale();
		y = font->max_advance.y / get_display_scale();
	}

	lua_pushnumber(l, x);
	lua_pushnumber(l, y);
	return 2;
}

int API::get_window_scale(lua_State* l) {
	fm_assert(lua_gettop(l) == 0);

	lua_pushnumber(l, get_display_scale());
	return 1;
}

int API::get_game_texture(lua_State* l) {
	//lua_pushnumber(l, render.native_render_target->texture);
	lua_pushnumber(l, 0);
	//lua_pushnumber(l, scene_target->color_buffer);
	return 1;
}


void register_api() {
	tdns_log.write(Log_Flags::File, "registering lua API");

	auto l = get_lua().state;
	
	lua_getglobal(l, "tdengine");
	luaL_register(l, 0, api);
	lua_pop(l, 1);
}
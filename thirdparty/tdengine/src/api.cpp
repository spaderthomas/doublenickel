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

// [float, float] cursor(optional coord::t)
int API::cursor(lua_State* lua) {
	auto coordinate = Coord::T::Window;

	if (lua_gettop(lua) == 1) {
		coordinate = (Coord::T)lua_tonumber(lua, -1);
	}
	
	auto& manager = get_input_manager();
	auto output = Coord::convert(manager.mouse, Coord::T::Screen, coordinate);
	lua_pushnumber(lua, output.x);
	lua_pushnumber(lua, output.y);
	
	return 2;
} 

int API::cursor_delta(lua_State* lua) {
	auto coordinate = Coord::T::Window;

	if (lua_gettop(lua) == 1) {
		coordinate = (Coord::T)lua_tonumber(lua, -1);
	}

	auto& manager = get_input_manager();
	auto output = Coord::convert_mag(manager.mouse_delta, Coord::T::Screen, coordinate);
	lua_pushnumber(lua, output.x);
	lua_pushnumber(lua, output.y);
	
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

int API::get_mouse_scroll(lua_State* l) {
	NO_ARGS();
	
	auto input = get_input_manager();
	lua_pushnumber(l, input.scroll.x);
	lua_pushnumber(l, input.scroll.y);
	
	return 2;
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

PreparedText* prepare_text_api() {
	auto& state = get_lua();
	auto l = state.state;
	
	BEGIN_ARGS();
	ADD_STRING_ARG(TEXT);
	ADD_OPTIONAL_TABLE_ARG(POSITION);
	ADD_OPTIONAL_TABLE_ARG(OPTIONS);
	END_ARGS();

	const char* text = nullptr;
	Vector2 position;
	Vector4 color = colors::white;
	const char* font = "inconsolata-32";
	float32 wrap = 0;
	bool world_space = false;

	state.parse_string(IDX_TEXT, &text);
	if (!text) return nullptr;

	if (HAS_ARG(IDX_POSITION)) {
		state.parse_vec2(IDX_POSITION, &position);
	}
	
	if (HAS_ARG(IDX_OPTIONS)) {
		state.parse_bool("world", &world_space);
		state.parse_string("font", &font);
		state.parse_float("wrap", &wrap);
		state.parse_color("color", &color);
	}

	return prepare_text_ex(text, position.x, position.y, font, wrap, color, true);
}

int API::calc_text_size(lua_State* l) {
	lua_pushnil(l);
	lua_insert(l, 2);
	auto prepared_text = prepare_text_api();
	if (prepared_text) {
		lua_pushnumber(l, prepared_text->width);
		lua_pushnumber(l, prepared_text->height);
		return 2;
	}
	
	return 0;
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

// float convert(float, float, Coord, Coord)
int API::convert(lua_State* l) {
	fm_assert(lua_gettop(l) == 4);
	Vector2 input;
	Coord::T from;
	Coord::T to;
	Coord::Dim dim = Coord::Dim::Any;

	input.x = lua_tonumber(l, 1);
	input.y = lua_tonumber(l, 2);
	from  = (Coord::T)((int32)lua_tonumber(l, 3));
	to    = (Coord::T)((int32)lua_tonumber(l, 4));

	Vector2 output = Coord::convert(input, from, to);
	lua_pushnumber(l, output.x);
	lua_pushnumber(l, output.y);
		
	return 2;
}



bool does_path_exist(const char* path) {
	return std::filesystem::exists(path);
}

bool is_regular_file(const char* path) {
	return std::filesystem::is_regular_file(path);	
}

bool is_directory(const char* path) {
	return std::filesystem::is_directory(path);		
}

void remove_directory(const char* path) {
	std::filesystem::remove_all(path);
}

void create_directory(const char* path) {
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directories(path);
	}
}

void create_named_directory(const_string name) {
	auto path = resolve_named_path(name);
	create_directory(path);
}

DateTime get_date_time() {
    DateTime date_time;

    // Get the current time point
    auto now = std::chrono::system_clock::now();

    // Convert it to time_t to extract calendar time
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert to tm structure for local time
    std::tm local_time = *std::localtime(&now_time_t);

    // Extract milliseconds from the current time point
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    // Fill in the DateTime struct
    date_time.year = local_time.tm_year + 1900;  // tm_year is years since 1900
    date_time.month = local_time.tm_mon + 1;     // tm_mon is 0-based (January is 0)
    date_time.day = local_time.tm_mday;
    date_time.hour = local_time.tm_hour;
    date_time.minute = local_time.tm_min;
    date_time.second = local_time.tm_sec;
    date_time.millisecond = static_cast<int>(milliseconds.count());

    return date_time;
}

void register_api() {
	tdns_log.write(Log_Flags::File, "registering lua API");

	auto l = get_lua().state;
	
	lua_getglobal(l, "tdengine");
	luaL_register(l, 0, api);
	lua_pop(l, 1);
}

void register_enums() {
	tdns_log.write(Log_Flags::File, "registering lua enums");
	
	auto l = get_lua().state;
	
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);

	// Coordinate
	lua_newtable(l);
	lua_pushstring(l, "world");
	lua_pushnumber(l, (int)Coord::T::World);
	lua_settable(l, -3);
	lua_pushstring(l, "screen");
	lua_pushnumber(l, (int)Coord::T::Screen);
	lua_settable(l, -3);
	lua_pushstring(l, "window");
	lua_pushnumber(l, (int)Coord::T::Window);
	lua_settable(l, -3);
	lua_pushstring(l, "game");
	lua_pushnumber(l, (int) Coord::T::Game);
	lua_settable(l, -3);
	lua_pushstring(l, "coordinate");
	lua_insert(l, -2);
	lua_settable(l, -3);

	// FillMode
	lua_newtable(l);
	lua_pushstring(l, "fill");
	lua_pushnumber(l, (int)FillMode::Fill);
	lua_settable(l, -3);
	lua_pushstring(l, "outline");
	lua_pushnumber(l, (int)FillMode::Outline);
	lua_settable(l, -3);
	lua_pushstring(l, "fill_mode");
	lua_insert(l, -2);
	lua_settable(l, -3);

}

void register_constants() {
	tdns_log.write(Log_Flags::File, "registering lua constants");
	
	auto l = get_lua().state;
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "constants");
	lua_gettable(l, -2);
	DEFER_POP(l);
	
	lua_pushstring(l, "tile_size");
	lua_pushnumber(l, Background::TILE_SIZE);
	lua_settable(l, -3);
}


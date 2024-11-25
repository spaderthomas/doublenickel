// I used to use the regular Lua API for loading in C functions, but now I prefer to use LuaJIT's FFI whenever possible. It has
// way, way nicer ergonomics. But, it's a pain to convert all these functions from the Lua API signature to a normal native one,
// so a lot of these are just holdovers.
//
// In general, for new stuff, I prefer to just put the stuff I'm exporting to Lua with the rest of the code. In other words, if
// there are new drawing functions, I'll put them in draw.hpp rather than having all Lua exports in one file (like this). I still
// stick new stuff in here if it doesn't fit anywhere else, but I always use the FFI instead of making new lua_State functions.
namespace API {
	// Input
	int cursor(lua_State* l);
	int cursor_delta(lua_State* l);
	int get_mouse_scroll(lua_State* l);

	// logging
	int log(lua_State* l);

	// internal
	int scandir_impl(lua_State* l);

	// game
	int convert(lua_State* l);

	// font
	int get_character_size(lua_State* l);
	int calc_text_size(lua_State* l);

	// window
	int screen_dimensions(lua_State* l);
	int get_window_scale(lua_State* l);
	int get_game_texture(lua_State* l);
	
	// sprite
	int sprite_size(lua_State* l);

}





void register_api();

const struct luaL_Reg api [] = {
	{ "get_mouse_scroll",         &API::get_mouse_scroll },
	{ "chsize",                   &API::get_character_size },
	{ "scandir_impl",             &API::scandir_impl },
	{ "log",                      &API::log },
	{ "screen_dimensions",        &API::screen_dimensions },
	{ "sprite_size",              &API::sprite_size },
	{ "get_window_scale",         &API::get_window_scale },
	{ "get_game_texture",         &API::get_game_texture },
	{  NULL,                      NULL }
};

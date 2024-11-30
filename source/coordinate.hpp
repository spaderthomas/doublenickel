#ifndef DN_COORDINATE_H
#define DN_COORDINATE_H

typedef enum {
	// Exactly where you are on the monitor. In other words, a fraction of the output resolution
	// in screen units of [0, 1]
	DN_COORD_UNIT_SCREEN = 0,

	// Where you are on the window of the screen displaying the game; the game is rendered
	// to a framebuffer, which could be displayed as some fraction of the screen in any position.
	// Window dn_coord_data take into account the position and size of that framebuffer. In the
	// case where the game is running full screen, this is equivalent to Screen
	//
	// In other words, a fraction of the framebuffer resolution in screen units of [0, 1]
	DN_COORD_UNIT_WINDOW = 1,

	// Same as Window, except it's in the range of [0, native_resolution]
	DN_COORD_UNIT_GAME = 2,

	// Same as Game, except takes the camera into account
	DN_COORD_UNIT_WORLD = 3,
} dn_coord_t;

typedef struct {
	dn_vector2_t camera;
	dn_vector2_t framebuffer_position;
	dn_vector2_t framebuffer_size;
} dn_coord_data_t;
dn_coord_data_t dn_coord_data;

DN_API dn_coord_data_t dn_coord_get();
DN_API void            dn_coord_set_camera(float x, float y);
DN_API void            dn_coord_set_framebuffer_position(float x, float y);
DN_API void            dn_coord_set_framebuffer_size(float x, float y);
DN_API dn_vector2_t         dn_coord_screen_to_window(float x, float y);
DN_API dn_vector2_t         dn_coord_screen_to_game(float x, float y);
DN_API dn_vector2_t         dn_coord_screen_to_world(float x, float y);
DN_API dn_vector2_t         dn_coord_window_to_screen(float x, float y);
DN_API dn_vector2_t         dn_coord_window_to_game(float x, float y);
DN_API dn_vector2_t         dn_coord_window_to_world(float x, float y);
DN_API dn_vector2_t         dn_coord_game_to_screen(float x, float y);
DN_API dn_vector2_t         dn_coord_game_to_window(float x, float y);
DN_API dn_vector2_t         dn_coord_game_to_world(float x, float y);
DN_API dn_vector2_t         dn_coord_world_to_screen(float x, float y);
DN_API dn_vector2_t         dn_coord_world_to_window(float x, float y);
DN_API dn_vector2_t         dn_coord_world_to_game(float x, float y);
DN_API dn_vector2_t         dn_coord_screen_to_window_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_screen_to_game_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_screen_to_world_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_window_to_screen_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_window_to_game_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_window_to_world_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_game_to_screen_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_game_to_window_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_game_to_world_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_world_to_screen_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_world_to_window_mag(float x, float y);
DN_API dn_vector2_t         dn_coord_world_to_game_mag(float x, float y);
               
#endif

#ifdef DN_COORDINATE_IMPLEMENTATION
dn_coord_data_t dn_coord_get() {
	return dn_coord_data;
}

void dn_coord_set_camera(float x, float y) {
	dn_coord_data.camera = { .x = x, .y = y };
}

void dn_coord_set_framebuffer_position(float x, float y) {
	dn_coord_data.framebuffer_position = { .x = x, .y = y };
}

void dn_coord_set_framebuffer_size(float x, float y) {
	dn_coord_data.framebuffer_size = { .x = x, .y = y };
}

dn_vector2_t dn_coord_screen_to_window(float x, float y) {
	auto framebuffer_bottom = window.content_area.y - (dn_coord_data.framebuffer_position.y + dn_coord_data.framebuffer_size.y);
	return { 
		.x = ((x * window.content_area.x) - dn_coord_data.framebuffer_position.x) / dn_coord_data.framebuffer_size.x,
		.y = ((y * window.content_area.y) - framebuffer_bottom) / dn_coord_data.framebuffer_size.y
	};
}

dn_vector2_t dn_coord_screen_to_game(float x, float y) {
	auto win = dn_coord_screen_to_window(x, y);
	return dn_coord_window_to_game(win.x, win.y);
}

dn_vector2_t dn_coord_screen_to_world(float x, float y) {
	auto win = dn_coord_screen_to_window(x, y);
	auto game = dn_coord_window_to_game(win.x, win.y);
	return dn_coord_game_to_world(game.x, game.y);
}

dn_vector2_t dn_coord_window_to_screen(float x, float y) {
	return {
		.x = ((x * dn_coord_data.framebuffer_size.x) + dn_coord_data.framebuffer_position.x) / window.content_area.x,
		.y = ((y * dn_coord_data.framebuffer_size.y) + dn_coord_data.framebuffer_position.y) / window.content_area.y,
	};
}

dn_vector2_t dn_coord_window_to_game(float x, float y) {
	return {
		.x = x * window.native_resolution.x,
		.y = y * window.native_resolution.y
	};
}

dn_vector2_t dn_coord_window_to_world(float x, float y) {
	auto game = dn_coord_window_to_game(x, y);
	return dn_coord_game_to_world(game.x, game.y);
}

dn_vector2_t dn_coord_game_to_screen(float x, float y) {
	auto win = dn_coord_game_to_window(x, y);
	return dn_coord_window_to_screen(win.x, win.y);
}

dn_vector2_t dn_coord_game_to_window(float x, float y) {
	return {
		.x = x / window.native_resolution.x,
		.y = y / window.native_resolution.y,
	};
}

dn_vector2_t dn_coord_game_to_world(float x, float y) {
	return {
		.x = x + dn_coord_data.camera.x,
		.y = y + dn_coord_data.camera.y,
	};
}

dn_vector2_t dn_coord_world_to_screen(float x, float y) {
	auto game = dn_coord_world_to_game(x, y);
	auto win = dn_coord_game_to_window(game.x, game.y);
	return dn_coord_window_to_screen(win.x, win.y);
}

dn_vector2_t dn_coord_world_to_window(float x, float y) {
	auto game = dn_coord_world_to_game(x, y);
	return dn_coord_game_to_window(game.x, game.y);
}

dn_vector2_t dn_coord_world_to_game(float x, float y) {
	return {
		.x = x - dn_coord_data.camera.x,
		.y = y - dn_coord_data.camera.y,
	};
}
     
dn_vector2_t dn_coord_screen_to_window_mag(float x, float y) {
	return {
		.x = x * window.content_area.x / dn_coord_data.framebuffer_size.x,
		.y = y * window.content_area.y / dn_coord_data.framebuffer_size.y,
	};
}

dn_vector2_t dn_coord_screen_to_game_mag(float x, float y) {
	auto win = dn_coord_screen_to_window_mag(x, y);
	return {
		.x = win.x * window.native_resolution.x,
		.y = win.y * window.native_resolution.y,
	};
}

dn_vector2_t dn_coord_screen_to_world_mag(float x, float y) {
	auto win = dn_coord_screen_to_window_mag(x, y);
	auto game = dn_coord_window_to_game_mag(win.x, win.y);
	return dn_coord_game_to_world_mag(game.x, game.y);
}

dn_vector2_t dn_coord_window_to_screen_mag(float x, float y) {
	return {
		.x = x * dn_coord_data.framebuffer_size.x / window.content_area.x,
		.y = y * dn_coord_data.framebuffer_size.y / window.content_area.y,
	};
}

dn_vector2_t dn_coord_window_to_game_mag(float x, float y) {
	return {
		.x = x * window.native_resolution.x,
		.y = y * window.native_resolution.y,
	};
}
dn_vector2_t dn_coord_window_to_world_mag(float x, float y) {
	return {
		.x = x * window.native_resolution.x,
		.y = y * window.native_resolution.y,
	};
}

dn_vector2_t dn_coord_game_to_screen_mag(float x, float y) {
	auto w = dn_coord_game_to_window_mag(x, y);
	return dn_coord_window_to_screen_mag(w.x, w.y);
}

dn_vector2_t dn_coord_game_to_window_mag(float x, float y) {
	return {
		.x = x / window.native_resolution.x,
		.y = y / window.native_resolution.y,
	};
}

dn_vector2_t dn_coord_game_to_world_mag(float x, float y) {
	return {
		.x = x,
		.y = y
	};
}

dn_vector2_t dn_coord_world_to_screen_mag(float x, float y) {
	return dn_coord_game_to_screen_mag(x, y);
}

dn_vector2_t dn_coord_world_to_window_mag(float x, float y) {
	return dn_coord_game_to_window_mag(x, y);

}
dn_vector2_t dn_coord_world_to_game_mag(float x, float y) {
	return {
		.x = x, 
		.y = y
	};
}

#endif
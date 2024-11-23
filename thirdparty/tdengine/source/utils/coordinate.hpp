#ifndef COORDINATE_H
#define COORDINATE_H
typedef enum {
	// Exactly where you are on the monitor. In other words, a fraction of the output resolution
	// in screen units of [0, 1]
	COORD_UNIT_SCREEN = 0,

	// Where you are on the window of the screen displaying the game; the game is rendered
	// to a framebuffer, which could be displayed as some fraction of the screen in any position.
	// Window coordinates take into account the position and size of that framebuffer. In the
	// case where the game is running full screen, this is equivalent to Screen
	//
	// In other words, a fraction of the framebuffer resolution in screen units of [0, 1]
	COORD_UNIT_WINDOW = 1,

	// Same as Window, except it's in the range of [0, native_resolution]
	COORD_UNIT_GAME = 2,

	// Same as Game, except takes the camera into account
	COORD_UNIT_WORLD = 3,
} CoordinateUnit;

typedef struct {
	Vector2 camera;
	Vector2 framebuffer_position;
	Vector2 framebuffer_size;
} Coordinates;
Coordinates coordinates;

FM_LUA_EXPORT Coordinates  coord_get();
FM_LUA_EXPORT void         coord_set_camera(float x, float y);
FM_LUA_EXPORT void         coord_set_framebuffer_position(float x, float y);
FM_LUA_EXPORT void         coord_set_framebuffer_size(float x, float y);

Vector2 coord_screen_to_window(float x, float y);
Vector2 coord_screen_to_game(float x, float y);
Vector2 coord_screen_to_world(float x, float y);
Vector2 coord_window_to_screen(float x, float y);
Vector2 coord_window_to_game(float x, float y);
Vector2 coord_window_to_world(float x, float y);
Vector2 coord_game_to_screen(float x, float y);
Vector2 coord_game_to_window(float x, float y);
Vector2 coord_game_to_world(float x, float y);
Vector2 coord_world_to_screen(float x, float y);
Vector2 coord_world_to_window(float x, float y);
Vector2 coord_world_to_game(float x, float y);
      
Vector2 coord_screen_to_window_mag(float x, float y);
Vector2 coord_screen_to_game_mag(float x, float y);
Vector2 coord_screen_to_world_mag(float x, float y);
Vector2 coord_window_to_screen_mag(float x, float y);
Vector2 coord_window_to_game_mag(float x, float y);
Vector2 coord_window_to_world_mag(float x, float y);
Vector2 coord_game_to_screen_mag(float x, float y);
Vector2 coord_game_to_window_mag(float x, float y);
Vector2 coord_game_to_world_mag(float x, float y);
Vector2 coord_world_to_screen_mag(float x, float y);
Vector2 coord_world_to_window_mag(float x, float y);
Vector2 coord_world_to_game_mag(float x, float y);

#endif

#ifdef COORDINATE_IMPLEMENTATION
Coordinates coord_get() {
	return coordinates;
}

void coord_set_camera(float x, float y) {
	coordinates.camera = { .x = x, .y = y };
}

void coord_set_framebuffer_position(float x, float y) {
	coordinates.framebuffer_position = { .x = x, .y = y };
}

void coord_set_framebuffer_size(float x, float y) {
	coordinates.framebuffer_size = { .x = x, .y = y };
}

Vector2 coord_screen_to_window(float x, float y) {
	auto framebuffer_bottom = window.content_area.y - (coordinates.framebuffer_position.y + coordinates.framebuffer_size.y);
	return { 
		.x = ((x * window.content_area.x) - coordinates.framebuffer_position.x) / coordinates.framebuffer_size.x,
		.y = ((y * window.content_area.y) - framebuffer_bottom) / coordinates.framebuffer_size.y
	};
}

Vector2 coord_screen_to_game(float x, float y) {
	auto win = coord_screen_to_window(x, y);
	return coord_window_to_game(win.x, win.y);
}

Vector2 coord_screen_to_world(float x, float y) {
	auto win = coord_screen_to_window(x, y);
	auto game = coord_window_to_game(win.x, win.y);
	return coord_game_to_world(game.x, game.y);
}

Vector2 coord_window_to_screen(float x, float y) {
	return {
		.x = ((x * coordinates.framebuffer_size.x) + coordinates.framebuffer_position.x) / window.content_area.x,
		.y = ((y * coordinates.framebuffer_size.y) + coordinates.framebuffer_position.y) / window.content_area.y,
	};
}

Vector2 coord_window_to_game(float x, float y) {
	return {
		.x = x * window.native_resolution.x,
		.y = y * window.native_resolution.y
	};
}

Vector2 coord_window_to_world(float x, float y) {
	auto game = coord_window_to_game(x, y);
	return coord_game_to_world(game.x, game.y);
}

Vector2 coord_game_to_screen(float x, float y) {
	auto win = coord_game_to_window(x, y);
	return coord_window_to_screen(win.x, win.y);
}

Vector2 coord_game_to_window(float x, float y) {
	return {
		.x = x / window.native_resolution.x,
		.y = y / window.native_resolution.y,
	};
}

Vector2 coord_game_to_world(float x, float y) {
	return {
		.x = x + coordinates.camera.x,
		.y = y + coordinates.camera.y,
	};
}

Vector2 coord_world_to_screen(float x, float y) {
	auto game = coord_world_to_game(x, y);
	auto win = coord_game_to_window(game.x, game.y);
	return coord_window_to_screen(win.x, win.y);
}

Vector2 coord_world_to_window(float x, float y) {
	auto game = coord_world_to_game(x, y);
	return coord_game_to_window(game.x, game.y);
}

Vector2 coord_world_to_game(float x, float y) {
	return {
		.x = x - coordinates.camera.x,
		.y = y - coordinates.camera.y,
	};
}
     
Vector2 coord_screen_to_window_mag(float x, float y) {
	return {
		.x = x * window.content_area.x / coordinates.framebuffer_size.x,
		.y = y * window.content_area.y / coordinates.framebuffer_size.y,
	};
}

Vector2 coord_screen_to_game_mag(float x, float y) {
	auto win = coord_screen_to_window_mag(x, y);
	return {
		.x = win.x * window.native_resolution.x,
		.y = win.y * window.native_resolution.y,
	};
}

Vector2 coord_screen_to_world_mag(float x, float y) {
	auto win = coord_screen_to_window(x, y);
	auto game = coord_window_to_game(win.x, win.y);
	return coord_game_to_screen(game.x, game.y);
}

Vector2 coord_window_to_screen_mag(float x, float y) {
	return {
		.x = x * coordinates.framebuffer_size.x / window.content_area.x,
		.y = y * coordinates.framebuffer_size.y / window.content_area.y,
	};
}

Vector2 coord_window_to_game_mag(float x, float y) {
	return {
		.x = x * window.native_resolution.x,
		.y = y * window.native_resolution.y,
	};
}
Vector2 coord_window_to_world_mag(float x, float y) {
	return {
		.x = x * window.native_resolution.x,
		.y = y * window.native_resolution.y,
	};
}

Vector2 coord_game_to_screen_mag(float x, float y) {
	auto w = coord_game_to_window_mag(x, y);
	return coord_window_to_screen_mag(w.x, w.y);
}

Vector2 coord_game_to_window_mag(float x, float y) {
	return {
		.x = x / window.native_resolution.x,
		.y = y / window.native_resolution.y,
	};
}

Vector2 coord_game_to_world_mag(float x, float y) {
	return {
		.x = x,
		.y = y
	};
}

Vector2 coord_world_to_screen_mag(float x, float y) {
	return coord_game_to_screen_mag(x, y);
}

Vector2 coord_world_to_window_mag(float x, float y) {
	return coord_game_to_window_mag(x, y);

}
Vector2 coord_world_to_game_mag(float x, float y) {
	return {
		.x = x, 
		.y = y
	};
}

#endif
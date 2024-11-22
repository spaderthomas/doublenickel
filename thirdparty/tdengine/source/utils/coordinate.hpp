namespace Coord {
	enum class T : u32 {
		// Exactly where you are on the monitor. In other words, a fraction of the output resolution
		// in screen units of [0, 1]
		Screen = 0,

		// Where you are on the window of the screen displaying the game; the game is rendered
		// to a framebuffer, which could be displayed as some fraction of the screen in any position.
		// Window coordinates take into account the position and size of that framebuffer. In the
		// case where the game is running full screen, this is equivalent to Screen
		//
		// In other words, a fraction of the framebuffer resolution in screen units of [0, 1]
		Window = 1,

		// Same as Window, except it's in the range of [0, native_resolution]
		Game = 2,

		// Same as Game, except takes the camera into account
		World = 3,
	};

	enum class Dim {
		X,
		Y,
		Any
	};

	Vector2 game_area_position;
	Vector2 game_area_size;

	Vector2 convert(Vector2 input, T from, T to);
	float32 convert_mag(float32 input, T from, T to, Dim dim = Dim::Any);
	Vector2 convert_mag(Vector2 input, T from, T to);
}

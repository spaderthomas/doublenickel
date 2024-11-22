namespace Coord {
	Vector2 convert(Vector2 input, T from, T to) {
		Vector2 output = input;

		// Screen 
		if (from == T::Screen) {
			if (to == T::Game) {
				output = convert(input,  T::Screen, T::Window);
				output = convert(output, T::Window, T::Game);
			}
			else if (to == T::World) {
				output = convert(input,  T::Screen, T::Window);
				output = convert(output, T::Window, T::Game);
				output = convert(output, T::Game, T::World);
			}
			else if (to == T::Window) {
				auto framebuffer_bottom = window.content_area.y - (Coord::game_area_position.y + Coord::game_area_size.y);
				output.x = ((input.x * window.content_area.x) - Coord::game_area_position.x) / Coord::game_area_size.x;
				output.y = ((input.y * window.content_area.y) - framebuffer_bottom) / Coord::game_area_size.y;
			}
		}

		// Window
		else if (from == T::Window) {
			if (to == T::Screen) {
				output.x = ((input.x * Coord::game_area_size.x) + Coord::game_area_position.x) / window.content_area.x;
				output.y = ((input.y * Coord::game_area_size.y) + Coord::game_area_position.y) / window.content_area.y;
			}
			else if (to == T::Game) {
				output.x =  input.x * window.native_resolution.x;
				output.y =  input.y * window.native_resolution.y;
			}
			else if (to == T::World) {
				output = convert(input,  T::Window, T::Game);
				output = convert(output, T::Game, T::World);
			}
		}

		// Game
		else if (from == T::Game) {
			if (to == T::World) {
				output.x = input.x + render.camera.x;
				output.y = input.y + render.camera.y;
			}
			else if (to == T::Screen) {
				output = convert(input,  T::Game,   T::Window);
				output = convert(output, T::Window, T::Screen);
			}
			else if (to == T::Window) {
				output.x = input.x / window.native_resolution.x;
				output.y = input.y / window.native_resolution.y;
			}
		}
		
		// World
		else if (from == T::World) {
			if (to == T::Screen) {
				output = convert(input,  T::World,  T::Game);
				output = convert(output, T::Game,   T::Window);
				output = convert(output, T::Window, T::Screen);
			}
			else if (to == T::Window) {
				output = convert(input,  T::World,  T::Game);
				output = convert(output, T::Game,   T::Window);
			}
			else if (to == T::Game) {
				output.x = input.x - render.camera.x;
				output.y = input.y - render.camera.y;
			}
		}

		return output;
	}
	
	float32 convert_mag(float32 input, T from, T to, Dim dim) {
		if (to   == T::World)  to   = T::Game;
		if (from == T::World)  from = T::Game;
		
		// Screen
		if (from == T::Screen) {
			if (to == T::Window) {
				if (dim == Dim::X) return input * (window.content_area.x / Coord::game_area_size.x);
				if (dim == Dim::Y) return input * (window.content_area.y / Coord::game_area_size.y);
			}
			else if (to == T::Game) {
				input = convert_mag(input, T::Screen, T::Window, dim);
				if (dim == Dim::X) return input * window.native_resolution.x;
				if (dim == Dim::Y) return input * window.native_resolution.y;
			}
			else if (to == T::Screen) {
				return input;
			}
			else {
				assert(false);
			}

		}

		else if (from == T::Window) {
			if (to == T::Screen) {
				if (dim == Dim::X) return input * (Coord::game_area_size.x / window.content_area.x);
				if (dim == Dim::Y) return input * (Coord::game_area_size.y / window.content_area.y);
			}
			else if (to == T::Game) {
				if (dim == Dim::X) return input * window.native_resolution.x;
				if (dim == Dim::Y) return input * window.native_resolution.y;
			}
			else if (to == T::Window) {
				return input;
			}
			else {
				assert(false);
			}
		}

		// Game
		else if (from == T::Game) {
			if (to == T::Screen) {
				input = convert_mag(input, T::Game, T::Window, dim);
				input = convert_mag(input, T::Window, T::Screen, dim);
				return input;
			}
			else if (to == T::Window) {
				if (dim == Dim::X) return input / window.native_resolution.x;
				if (dim == Dim::Y) return input / window.native_resolution.y;
			}
			else if (to == T::Game) {
				return input;
			}
			else {
				assert(false);
			}
		}

		// Naughty!
		else {
			assert(false);
		}

		return input;
	}

	Vector2 convert_mag(Vector2 input, T from, T to) {
		Vector2 output;
		output.x = convert_mag(input.x, from, to, Dim::X);
		output.y = convert_mag(input.y, from, to, Dim::Y);
		return output;
	}
}

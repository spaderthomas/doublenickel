void test_convert_mag() {
	Vector2 magnitude;
	Vector2 result;

	// The screen is twice as big as the texture we're rendering the game to. The framebuffer is half
	// the size of the game texture, and it's centered on the screen.
	window.native_resolution.x = 1000;
	window.native_resolution.y = 1000;
	window.content_area.x = 2000;
	window.content_area.y = 2000;
	Coord::game_area_position.x = 750;
	Coord::game_area_position.y = 750;
	Coord::game_area_size.x = 500;
	Coord::game_area_size.y = 500;

	
	// SCREEN -> *
	
	// If something's width is the whole screen, then its width is 4x the width of the framebuffer
	magnitude.x = 1.f;
	result = Coord::convert_mag(magnitude, Coord::T::Screen, Coord::T::Window);
	assert(result.x == 4.f);

	// In game units (where one unit == one pixel of native resolution, which does not change), that's
	// 4x the native width
	result = Coord::convert_mag(magnitude, Coord::T::Screen, Coord::T::Game);
	assert(result.x == window.native_resolution.x * 4);

	// World == Game for magnitudes
	result = Coord::convert_mag(magnitude, Coord::T::Screen, Coord::T::World);
	assert(result.x == window.native_resolution.x * 4);


	// WINDOW -> *
	
	// If something is the width of the framebuffer, it's 1/4 of the screen
	result = Coord::convert_mag(magnitude, Coord::T::Window, Coord::T::Screen);
	assert(result.x == .25);

	// If something is the width of the framebuffer, it's exactly NATIVE_RESOLUTION game units,
	// by definition
	result = Coord::convert_mag(magnitude, Coord::T::Window, Coord::T::Game);
	assert(result.x == window.native_resolution.x);

	// World == Game
	result = Coord::convert_mag(magnitude, Coord::T::Window, Coord::T::World);
	assert(result.x == window.native_resolution.x);

	
	// GAME -> *

	magnitude.x = window.native_resolution.x;
	
	result = Coord::convert_mag(magnitude, Coord::T::Game, Coord::T::Window);
	assert(result.x == 1.f);

	result = Coord::convert_mag(magnitude, Coord::T::Game, Coord::T::Screen);
	assert(result.x == .25f);

	result = Coord::convert_mag(magnitude, Coord::T::Game, Coord::T::World);
	assert(result.x == window.native_resolution.x);
}

void test_convert_point() {
	Vector2 point;
	Vector2 result;

	static constexpr int32 NATIVE = 1000;
	
	window.native_resolution.x = 1000;
	window.native_resolution.y = 1000;
	window.content_area.x = 2000;
	window.content_area.y = 2000;
	Coord::game_area_position.x = 500;
	Coord::game_area_position.y = 500;
	Coord::game_area_size.x = 500;
	Coord::game_area_size.y = 500;

	render.camera.x = NATIVE / 2;
	render.camera.y = NATIVE / 2;

	point = Vector2(1.f, 1.f);

	// SCREEN -> *
	result = Coord::convert(point, Coord::T::Screen, Coord::T::Window);
	assert(result.x == 3.f);

	result = Coord::convert(point, Coord::T::Screen, Coord::T::Game);
	assert(result.x == 3000.f);

	result = Coord::convert(point, Coord::T::Screen, Coord::T::World);
	assert(result.x == 3500.f);

	// WINDOW -> *
	result = Coord::convert(point, Coord::T::Window, Coord::T::Game);
	assert(result.x == window.native_resolution.x);

	result = Coord::convert(point, Coord::T::Window, Coord::T::World);
	assert(result.x == window.native_resolution.x + render.camera.x);

	// If a point is at the edge of the framebuffer, then it should be (distance from edge of screen
	// to framebuffer) + (width of framebuffer), in screen coordinates. The framebuffer is 1/4 from
	// the edge of the screen, and is 1/4 of the screen in size.
	result = Coord::convert(point, Coord::T::Window, Coord::T::Screen);
	assert(result.x == .5);

	// GAME -> *
	point = Vector2(1000, 1000);
	result = Coord::convert(point, Coord::T::Game, Coord::T::Screen);
	assert(result.x == .5f);
	
	result = Coord::convert(point, Coord::T::Game, Coord::T::Window);
	assert(result.x == 1.f);

	result = Coord::convert(point, Coord::T::Game, Coord::T::World);
	assert(result.x == 1500);

	// WORLD -> *
	point = Vector2(1500, 1500);
	result = Coord::convert(point, Coord::T::World, Coord::T::Screen);
	assert(result.x == .5f);
	
	result = Coord::convert(point, Coord::T::World, Coord::T::Window);
	assert(result.x == 1.f);

	result = Coord::convert(point, Coord::T::World, Coord::T::Game);
	assert(result.x == 1000);
}

void test_generational_arena() {
	GenerationalArena<u32> arena;
	arena.init(32);
	auto rza = arena.insert(69);
	auto gza = arena.insert(420);
	auto bill = arena.insert(7);

	
	assert(*arena[rza] == 69);
	assert(*arena[gza] == 420);
	assert(*arena[bill] == 7);

	arena.remove(bill);

	auto murray = arena.insert(9001);
	assert(!arena.contains(bill));
	assert(arena.contains(murray));

	assert(arena[bill] == nullptr);
	assert(*arena[murray] == 9001);

	u32 values [] = {
		69, 420, 9001
	};
	u32 index = 0;
	for (const auto& value : arena) {
		assert(value == values[index++]);
	}
}

void test_bump_allocator() {
	MemoryAllocator* allocator = &bump_allocator;

	auto memory_block = (u32*)ma_alloc(allocator, sizeof(u32) * 8);
	memory_block[0] = 69;
	memory_block[7] = 420;
	assert(bump_allocator.allocations[0] == sizeof(u32) * 8);

	memory_block = (u32*)ma_realloc(allocator, memory_block, sizeof(u32) * 16);
	assert(memory_block[0] == 69);
	assert(memory_block[7] == 420);
	assert(bump_allocator.allocations[sizeof(u32) * 8] == sizeof(u32) * 16);
}
void test_dyn_array() {
	auto array = dyn_array_alloc_t<u32>(ma_find("bump"));
	assert(dyn_array_head(array)->size == 0);

	dyn_array_push(array, 69);
	assert(array[0] == 69);
	assert(dyn_array_head(array)->size == 1);

	dyn_array_push(array, 70);
	assert(array[0] == 69);
	assert(array[1] == 70);
	assert(dyn_array_head(array)->size == 2);
	
	dyn_array_push(array, 71);
	assert(array[0] == 69);
	assert(array[1] == 70);
	assert(array[2] == 71);
	assert(dyn_array_head(array)->size == 3);
}

void run_tests() {
	test_bump_allocator();
	test_dyn_array();
	test_generational_arena();
	test_convert_mag();
	test_convert_point();
}

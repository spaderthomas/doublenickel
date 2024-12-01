#ifndef DN_TEST_H
#define DN_TEST_H
void dn_test_init();
#endif

#ifdef DN_TEST_IMPLEMENTATION
// void test_convert_mag() {
//  dn_vector2_t magnitude;
//  dn_vector2_t result;

//  // The screen is twice as big as the texture we're rendering the game to. The framebuffer is half
//  // the size of the game texture, and it's centered on the screen.
//  window.native_resolution.x = 1000;
//  window.native_resolution.y = 1000;
//  window.content_area.x = 2000;
//  window.content_area.y = 2000;
//  Coord::game_area_position.x = 750;
//  Coord::game_area_position.y = 750;
//  Coord::game_area_size.x = 500;
//  Coord::game_area_size.y = 500;

  
//  // SCREEN -> *
  
//  // If something's width is the whole screen, then its width is 4x the width of the framebuffer
//  magnitude.x = 1.f;
//  result = Coord::convert_mag(magnitude, Coord::T::Screen, Coord::T::Window);
//  assert(result.x == 4.f);

//  // In game units (where one unit == one pixel of native resolution, which does not change), that's
//  // 4x the native width
//  result = Coord::convert_mag(magnitude, Coord::T::Screen, Coord::T::Game);
//  assert(result.x == window.native_resolution.x * 4);

//  // World == Game for magnitudes
//  result = Coord::convert_mag(magnitude, Coord::T::Screen, Coord::T::World);
//  assert(result.x == window.native_resolution.x * 4);


//  // WINDOW -> *
  
//  // If something is the width of the framebuffer, it's 1/4 of the screen
//  result = Coord::convert_mag(magnitude, Coord::T::Window, Coord::T::Screen);
//  assert(result.x == .25);

//  // If something is the width of the framebuffer, it's exactly NATIVE_RESOLUTION game units,
//  // by definition
//  result = Coord::convert_mag(magnitude, Coord::T::Window, Coord::T::Game);
//  assert(result.x == window.native_resolution.x);

//  // World == Game
//  result = Coord::convert_mag(magnitude, Coord::T::Window, Coord::T::World);
//  assert(result.x == window.native_resolution.x);

  
//  // GAME -> *

//  magnitude.x = window.native_resolution.x;
  
//  result = Coord::convert_mag(magnitude, Coord::T::Game, Coord::T::Window);
//  assert(result.x == 1.f);

//  result = Coord::convert_mag(magnitude, Coord::T::Game, Coord::T::Screen);
//  assert(result.x == .25f);

//  result = Coord::convert_mag(magnitude, Coord::T::Game, Coord::T::World);
//  assert(result.x == window.native_resolution.x);
// }

// void test_convert_point() {
//  dn_vector2_t point;
//  dn_vector2_t result;

//  static constexpr i32 NATIVE = 1000;
  
//  window.native_resolution.x = 1000;
//  window.native_resolution.y = 1000;
//  window.content_area.x = 2000;
//  window.content_area.y = 2000;
//  Coord::game_area_position.x = 500;
//  Coord::game_area_position.y = 500;
//  Coord::game_area_size.x = 500;
//  Coord::game_area_size.y = 500;

//  render.camera.x = NATIVE / 2;
//  render.camera.y = NATIVE / 2;

//  point = dn_vector2_t(1.f, 1.f);

//  // SCREEN -> *
//  result = Coord::convert(point, Coord::T::Screen, Coord::T::Window);
//  assert(result.x == 3.f);

//  result = Coord::convert(point, Coord::T::Screen, Coord::T::Game);
//  assert(result.x == 3000.f);

//  result = Coord::convert(point, Coord::T::Screen, Coord::T::World);
//  assert(result.x == 3500.f);

//  // WINDOW -> *
//  result = Coord::convert(point, Coord::T::Window, Coord::T::Game);
//  assert(result.x == window.native_resolution.x);

//  result = Coord::convert(point, Coord::T::Window, Coord::T::World);
//  assert(result.x == window.native_resolution.x + render.camera.x);

//  // If a point is at the edge of the framebuffer, then it should be (distance from edge of screen
//  // to framebuffer) + (width of framebuffer), in screen coordinates. The framebuffer is 1/4 from
//  // the edge of the screen, and is 1/4 of the screen in size.
//  result = Coord::convert(point, Coord::T::Window, Coord::T::Screen);
//  assert(result.x == .5);

//  // GAME -> *
//  point = dn_vector2_t(1000, 1000);
//  result = Coord::convert(point, Coord::T::Game, Coord::T::Screen);
//  assert(result.x == .5f);
  
//  result = Coord::convert(point, Coord::T::Game, Coord::T::Window);
//  assert(result.x == 1.f);

//  result = Coord::convert(point, Coord::T::Game, Coord::T::World);
//  assert(result.x == 1500);

//  // WORLD -> *
//  point = dn_vector2_t(1500, 1500);
//  result = Coord::convert(point, Coord::T::World, Coord::T::Screen);
//  assert(result.x == .5f);
  
//  result = Coord::convert(point, Coord::T::World, Coord::T::Window);
//  assert(result.x == 1.f);

//  result = Coord::convert(point, Coord::T::World, Coord::T::Game);
//  assert(result.x == 1000);
// }

void dn_gen_arena_test() {
  dn_gen_arena_t<u32> arena;
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

void dn_dynamic_array_test() {
  dn_dynamic_array<u32> array;
  dn::dynamic_array::init(&array, &dn_allocators.bump);
  DN_ASSERT(array.size == 0);

  dn::dynamic_array::push(&array, 32u);
  dn::dynamic_array::push(&array, 420u);
  DN_ASSERT(*dn::dynamic_array::at(&array, 0) == 32u);
  DN_ASSERT(*dn::dynamic_array::at(&array, 1) == 420u);
  DN_ASSERT(array.size == 2);
  DN_ASSERT(array.capacity >= 2);

  dn::dynamic_array::push(&array, 69u);
  dn::dynamic_array::push(&array, 69420u);
  dn::dynamic_array::push(&array, 42069u);
  dn::dynamic_array::push(&array, 690u);
  DN_ASSERT(array.size == 6);
  DN_ASSERT(array.capacity >= 6);
  DN_ASSERT(*dn::dynamic_array::at(&array, 2) == 69u);
  DN_ASSERT(*dn::dynamic_array::at(&array, 3) == 69420u);
  DN_ASSERT(*dn::dynamic_array::at(&array, 4) == 42069u);
  DN_ASSERT(*dn::dynamic_array::at(&array, 5) == 690u);
}

void dn_bump_allocator_test() {
  dn_bump_allocator_clear(&dn_allocators.bump);
  dn_allocator_t* allocator = &dn_allocators.bump;


  auto memory_block = (u32*)dn_allocator_alloc(allocator, sizeof(u32) * 8);
  memory_block[0] = 69;
  memory_block[7] = 420;
  assert(dn_allocators.bump.allocations[0] == sizeof(u32) * 8);

  memory_block = (u32*)dn_allocator_realloc(allocator, memory_block, sizeof(u32) * 16);
  assert(memory_block[0] == 69);
  assert(memory_block[7] == 420);
  assert(dn_allocators.bump.allocations[sizeof(u32) * 8] == sizeof(u32) * 16);
}

void dn_path_test() {
  DN_ASSERT(dn_path_is_extension("file.png", dn_string_literal(".png")));
  DN_ASSERT(dn_string_equal(
    dn_string_literal("file.png"), 
    dn_path_extract_file_name("C:/Users/jg/file.png")
  ));
}

void dn_string_test() {
  typedef struct {
    dn_string_t jerry;
    dn_string_t garcia;
    const char* jerry_cstr;
    std::string jerry_std;
  } dn_strings_t;

  dn_strings_t data = {
    .jerry  = dn_string_literal("jerry garcia"),
    .garcia = dn_string_literal("jerry garcia"),
    .jerry_cstr = "jerry garcia",
    .jerry_std = std::string("jerry garcia"),
  };

  DN_ASSERT(data.jerry.len == strlen(data.jerry_cstr));

  // u32                dn_cstr_len(const char* str);
  DN_ASSERT(dn_cstr_len(data.jerry_cstr) == 12);

  // bool               dn_cstr_equal(const char* a, const char* b);
  DN_ASSERT(dn_cstr_equal(data.jerry_cstr, data.jerry_cstr));

  // DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
  DN_ASSERT(dn_string_equal(data.jerry, data.garcia));

  // DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);
  DN_ASSERT(dn_string_equal_cstr(data.jerry, data.jerry_cstr));

  // DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
  dn_string_t jerry_from_dn = dn_string_copy(data.jerry,  &dn_allocators.bump);
  DN_ASSERT(dn_string_equal(data.jerry, jerry_from_dn));

  // DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
  dn_string_t jerry_from_cstr = dn_string_copy_cstr(data.jerry_cstr,  &dn_allocators.bump);
  DN_ASSERT(dn_string_equal(data.jerry, jerry_from_cstr));

  // DN_API dn_string_t dn_string_copy_std(const std::string& str, dn_allocator_t* allocator);
  dn_string_t jerry_from_std = dn_string_copy_std(data.jerry_std,  &dn_allocators.bump);
  DN_ASSERT(dn_string_equal(data.jerry, jerry_from_std));

  // DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
  char* jerry_cstr = dn_string_to_cstr_ex(data.jerry, &dn_allocators.bump);
  DN_ASSERT(dn_cstr_len(jerry_cstr) == dn_cstr_len(data.jerry_cstr));
  DN_ASSERT(dn_cstr_equal(jerry_cstr, data.jerry_cstr));

  // DN_IMP char*       dn_cstr_copy(const char* str, u32 length, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy(const std::string& str, dn_allocator_t* allocator = nullptr);
  // DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator = nullptr);
  // DN_API void        dn_cstr_copy(const char* str, char* buffer, u32 buffer_length);
  // DN_API void        dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);
}

void dn_string_builder_test() {
  // DN_API void        dn_string_builder_grow(dn_string_builder_t* builder);
  dn_test_scope(dn_string_builder_grow, {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump };
    dn_string_builder_grow(&builder, 32);
    DN_ASSERT(builder.buffer.capacity >= 32);
    dn_string_builder_grow(&builder, 8);
    DN_ASSERT(builder.buffer.capacity >= 32);
    dn_string_builder_grow(&builder, 64);
    DN_ASSERT(builder.buffer.capacity >= 64);
  });


  // DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
  // DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
  // DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
  dn_test_scope(dn_string_builder_append, {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump };
    dn_string_builder_append(&builder, dn_string_literal("jerry"));
    dn_string_builder_append_cstr(&builder, "/");
    dn_string_builder_append(&builder, dn_string_literal("garcia"));
    DN_ASSERT(dn_string_equal(dn_string_builder_write(&builder), dn_string_literal("jerry/garcia")));
  });

  // DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
  dn_test_scope(dn_string_builder_write_cstr, {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump };
    dn_string_builder_append(&builder, dn_string_literal("jerry"));
    DN_ASSERT(dn_cstr_equal(dn_string_builder_write_cstr(&builder), "jerry"));
  });

  // DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
  dn_test_scope(dn_string_builder_write_fmt, {
    dn_string_builder_t builder = { .buffer = dn_zero_initialize(), .allocator = &dn_allocators.bump };
    dn_string_builder_append_fmt(&builder, dn_string_literal("%d:%.2f:%s"), 69, 420.69, "blazeit");
    DN_ASSERT(dn_string_equal(dn_string_builder_write(&builder), dn_string_literal("69:420.69:blazeit")));
  });
}


void dn_os_scan_directory_test() {
  dn_os_create_directory(dn_string_literal("tmp"));
  dn_os_create_file(dn_string_literal("tmp/jerry"));
  dn_os_create_file(dn_string_literal("tmp/pigpen"));
  dn_os_create_file(dn_string_literal("tmp/bobby"));
  dn_os_create_file(dn_string_literal("tmp/phil"));
  dn_os_create_file(dn_string_literal("tmp/bill"));
  dn_os_create_file(dn_string_literal("tmp/keith"));
  auto entries = dn_os_scan_directory(dn_string_literal("tmp"));
  DN_ASSERT(entries.count == 6);
}

void dn_test_init() {
  dn_string_test();
  dn_string_builder_test();
  dn_dynamic_array_test();
  dn_gen_arena_test();
  dn_bump_allocator_test();
  dn_os_scan_directory_test();
  // test_convert_point();
  dn_path_test();
}
#endif

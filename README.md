`doublenickel` is a header only framework for building games and tech demos on Windows, written in a minimal C-style C++ and scripted with LuaJIT.

## overview
This is a library-fied version of the code used to ship [Deep Copy](https://store.steampowered.com/app/2639990/Deep_Copy/). It's a simple, header only framework that lives somewhere between platform abstraction libraries (like `sokol` or `LOVE2D`) and true blue game engines. It provides a ton of things out of the box that I find essential for making games and tech demos:
- A fully hotloaded LuaJIT environment
- ImGui integration, both in C++ and Lua, which allows every piece of the engine to be inspectable and editable
- ImGui layouts for making custom views
- Easy entry points to run code at predefined points in the frame (e.g. an entity that will `update()`)
- A GPU API on top of OpenGL inspired by D3D11 and WebGPU
- A few data structures (mostly flavors of arrays)
- Automatic asset loading *and* hotloading; just point the framework at the right directory, and it'll hotload your shaders, audio files, images -- anything.
- Precompiled binaries for the few libraries that are needed

This is by no means a comprehensive list, just the things I find most important. 

## how do i use this?
Well, you probably shouldn't -- yet. I'm still working on porting over features from Deep Copy, and things are likely to break. That being said, the framework is very much usable, and I use it every day.

If you're a source code person, check out the [example repo]() to get a taste for what applications built with `doublenickel` look like.

## why?
Writing your own game engine is pure folly, but writing your own game is very reasonable and productive. Commercial engines are fine, but I enjoy writing all the software for my games, and I wanted to keep the real and hard fought benefits I gained from making my first commercial game:
- Hotloading your software is the most important thing in your development environment. It should work every time, and you shouldn't have to sacrifice debuggability or put restrictions on what can be hotloaded.
- Hotloading assets is nearly as important.
- LuaJIT has the best C FFI, and can be used for 99% of code in a game. It's incredible to use, and I want to use it every day.
- Big picture architectures for games of the scale I make are a waste of time. Everything doesn't need to be tightly packed SOAs of `Component`s that get processed in `System`s. Sometimes, those things are appropriate, but I want something that lets me easily make those things on top of the core engine rather than be a part of it.
- Having a good UI for tweaking your data is important. 

There are excellent platform abstraction APIs that smooth over a lot of noisy details, but they don't provide the higher level niceties that are the most important part. And there are game engines, which provide all of those things and several orders of magnitude more, but commercial game engines have a ton of problems, and I don't like to use them. `doublenickel` is meant to fit in a perfect sweet spot in between -- I have consistent, fast APIs for all of the platform stuff, but it's simple enough as to be completely extensible or modifiable when a project grows.

## broken
A ton of stuff is unported, untested, or broken from its incarnation in Deep Copy.
- Images (currently disabled)
  - There are some minor synchronization issues with temporary storage
  - The flow of control needs to be inverted; instead of the framework querying a Lua table for texture atlases, a piece of Lua code needs to call into a framework function.
  - Texture atlases have to be packed semi-manually i.e. you must specify a list of directories to be packed into an atlas, but if the collective images are too large to fit in our atlas, it breaks. It's very annoying and error prone.
  - The API is not namespaced
  - This needs to use the new asset importer system (which is also untested -- this is the dog food)
  - Ditto for backgrounds (i.e. images too large for a single texture which must be tiled)
- Rendering
  - The old engine could sort draw calls according to layer to draw them with the Painter's Algorithm. I think we'll need to enable depth testing, and keep a separate command buffer for transparent objects.
  - There are a lot of places in the code using the immediate mode drawing API (e.g. `draw_quad()`). There should be a default SDF renderer in the framework which provides these.
  - Quite a few things in the GPU API aren't tested enough:
    - Scissor regions
    - Renderer state (world space, camera)
  - There is no more zoom
- Input
  - The action set system (a layer of indirection on top of inputs, where you query for actions rather than raw inputs) is totally mapped to Steam Input. After using Steam Input, I would prefer to not use Steam Input. I'm not sure the right way to do this, because I like Steam's builtin controller remapper.
  - Rip out GLFW and put in SDL. I don't have anything against GLFW, but SDL is more or less a first party Valve library.
- Fluid
  - Reimplement the fluid simulations with the new GPU API
- Text
  - I'd like to use SDF fonts for text, but the current text renderer is actually pretty good.
  - Text is limited to 1024 characters per chunk. I do not know why.
- Particles
  - I have a CPU particle system which can do maybe ten thousand particles per frame at 60FPS. I'd like to just nuke this and make an instanced GPU particle system.
- Internals
  - I still use C strings in most places in the engine, instead of `dn_string_t`
  - I'd like the preprocessor to be (a) totally separated from `stb_include` and (b) a *little* more featureful (perhaps processing `#define`)
- Lua
  - I had a simple `vec2` class in Lua which I used before I leaned on the FFI so much. I'd like to nuke it and replace it with `dn_vector2_t`
  - There are several subsystems you might not want to use at all (e.g. dialogue, or animations). I'd like it to be ergonomic to disable those.
  - Do not hardcode paths to subsystem data (e.g. animations, characters, backgrounds) in the framework
  - A simple API to tell the framework to automatically load some header into the FFI
  - `doublenickel.color` is from the far, far before times
  - The dialogue editor is still drawn with ImGui and is extremely slow
  - The collider editor does not work

- `doublenickel.ffi.set_camera()`
- `doublenickel.ffi.set_layer()`
- `doublenickel.ffi.set_world_space()`
- `doublenickel.ffi.end_world_space()`
- `doublenickel.ffi.set_window_size()`
- `doublenickel.ffi.set_zoom()`
- `doublenickel.ffi.draw_image_ex()`
- `doublenickel.ffi.draw_quad_l()`
- `doublenickel.ffi.draw_quad()`
- `doublenickel.ffi.draw_quad_sdf()`
- `doublenickel.ffi.draw_line()`
- `doublenickel.ffi.draw_image_l()`
- `doublenickel.ffi.draw_prepared_text()`
- `doublenickel.ffi.draw_text_ex()`
- `doublenickel.ffi.begin_scissor()`
- `doublenickel.ffi.end_scissor()`
- `doublenickel.ffi.register_action_set()`
- `doublenickel.ffi.activate_action_set()`
- `doublenickel.ffi.get_active_action_set()`
- `doublenickel.ffi.get_action_set_cooldown()`
- `doublenickel.ffi.was_digital_pressed()`


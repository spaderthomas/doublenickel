doublenickel.lifecycle.callbacks = doublenickel.enum.define(
  'LifecycleCallback',
  {
    on_begin_frame = 0,
    on_end_frame = 1,
    on_scene_rendered = 2,
    on_render_scene = 3,
    on_editor_play = 4,
    on_editor_stop = 5,
    on_main_menu = 6,
    on_init_game = 7,
    on_start_game = 8,
    on_swapchain_ready = 9,
    on_engine_viewer = 10,
  }
)

doublenickel.lifecycle.update_callbacks = doublenickel.enum.define(
  'UpdateCallback',
  {
    update = 0,
    draw = 1,
    late_init = 2,
    play = 3,
    stop = 4,
    init = 5,
    deinit = 6,
  }
)


function doublenickel.lifecycle.run_callback(callback_id)
  local function run_callback(objects)
    for _, object in pairs(objects) do
      local fn = object[callback_id:to_string()]
      fn(object)
    end
  end

  run_callback(doublenickel.editor.entities)
  run_callback(doublenickel.subsystem.subsystems)
end
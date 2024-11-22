function tdengine.update_game(dt)
  local before = collectgarbage('count')
  tdengine.ffi.tm_begin('update')
  
  tdengine.dt = dt
  tdengine.elapsed_time = tdengine.elapsed_time + dt
  tdengine.frame = tdengine.frame + 1

  tdengine.input.update()

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_begin_frame)

  tdengine.gpus.update()
  tdengine.editor.update()
  tdengine.scene.update()
  tdengine.entity.update()
  tdengine.subsystem.update()
  tdengine.gui.update()
  tdengine.audio.update()

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_end_frame)
  
  tdengine.ffi.tm_end('update')

  tdengine.gpus.render()
  local after = collectgarbage('count')
  local memory_delta = after - before

  tdengine.ffi.tm_begin('gc')

  local max_iter = 1000
  local iter = 0

  local memory_before_step = collectgarbage('count')
  -- collectgarbage('stop')
  -- collectgarbage('setpause', 100)
  -- collectgarbage('step', 100)
  local memory_after_step = collectgarbage('count')
  local memory_collected = math.abs(memory_after_step - memory_before_step)

  -- collectgarbage('stop')

  tdengine.ffi.tm_end('gc')
  -- print(collectgarbage('count'))
  -- print(string.format('(%.3f) (%.3f), before: %.3f, after: %.3f', memory_delta, memory_collected, before, after))


end

ffi.cdef([[
  typedef struct {
    Vector2 v2;
  } L1A;
  
  typedef struct {
    L1A l1;
    float x;
  } L2A;
  
  typedef struct {
    L2A l2;
    float x;
  } L3A;
]])
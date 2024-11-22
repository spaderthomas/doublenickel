function tdengine.update_game(dt)
  tdengine.ffi.tm_begin('update')
  
  tdengine.dt = dt
  tdengine.elapsed_time = tdengine.elapsed_time + dt
  tdengine.frame = tdengine.frame + 1

  tdengine.input.update()

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_begin_frame)

  tdengine.editor.update()
  tdengine.scene.update()
  tdengine.entity.update()
  tdengine.subsystem.update()
  tdengine.gui.update()
  tdengine.audio.update()

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_end_frame)
  
  tdengine.ffi.tm_end('update')

  tdengine.gpus.render()
end
function doublenickel.update_game(dt)
  dn.time_metric_begin('update')
  
  doublenickel.dt = dt
  doublenickel.elapsed_time = doublenickel.elapsed_time + dt
  doublenickel.frame = doublenickel.frame + 1

  doublenickel.input.update()

  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_begin_frame)

  doublenickel.editor.update()
  doublenickel.scene.update()
  doublenickel.entity.update()
  doublenickel.subsystem.update()
  doublenickel.gui.update()
  doublenickel.audio.update()

  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_end_frame)
  
  dn.time_metric_end('update')

  doublenickel.gpu.render()
end
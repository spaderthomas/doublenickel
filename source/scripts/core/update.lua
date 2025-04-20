function dn.update_game(dt)
  dn.time_metric.begin('update')
  
  dn.dt = dt
  dn.elapsed_time = dn.elapsed_time + dt
  dn.frame = dn.frame + 1

  dn.input.update()

  dn.lifecycle.run_callback(dn.lifecycle.callbacks.on_begin_frame)

  dn.editor.update()
  -- dn.scene.update()
  dn.entity.update()
  -- dn.subsystem.update()
  -- dn.gui.update()
  -- dn.audio.update()

  dn.lifecycle.run_callback(dn.lifecycle.callbacks.on_end_frame)
  
  dn.time_metric.stop('update')

  dn.gpu.render()
end
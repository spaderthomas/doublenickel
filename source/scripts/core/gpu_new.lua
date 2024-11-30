-------------
-- STRUCTS --
-------------




----------------
-- GPU MODULE --
----------------
local self = doublenickel.gpu
function doublenickel.gpu.init()
  self.render_targets = {}
  self.buffers = {}
  self.shaders = {}
  self.resolutions = {}
  self.assets = {}
end

function doublenickel.gpu.render()
  doublenickel.ffi.dn_time_metric_begin('render')

  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_render_scene)
  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_scene_rendered)

  local swapchain = doublenickel.ffi.dn_gpu_acquire_swapchain()
  doublenickel.ffi.dn_gpu_render_target_bind(swapchain)
  doublenickel.ffi.dn_gpu_render_target_clear(swapchain)
  doublenickel.app:on_swapchain_ready()
  doublenickel.ffi.dn_gpu_swap_buffers()

  doublenickel.ffi.dn_time_metric_end('render')
end


function doublenickel.gpu.build(gpu_info)
  self.add_resolutions(gpu_info.resolutions)
  self.add_render_targets(gpu_info.render_targets)
  self.add_buffers(gpu_info.buffers)
  self.add_shaders(gpu_info.shaders)
end


function doublenickel.gpu.find(id)
  return self.assets[id:to_qualified_string()]
end
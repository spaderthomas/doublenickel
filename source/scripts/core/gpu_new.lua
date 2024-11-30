-------------
-- STRUCTS --
-------------




----------------
-- GPU MODULE --
----------------
local self = tdengine.gpu
function tdengine.gpu.init()
  self.render_targets = {}
  self.buffers = {}
  self.shaders = {}
  self.resolutions = {}
  self.assets = {}
end

function tdengine.gpu.render()
  tdengine.ffi.dn_time_metric_begin('render')

  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_render_scene)
  tdengine.lifecycle.run_callback(tdengine.lifecycle.callbacks.on_scene_rendered)

  local swapchain = tdengine.ffi.dn_gpu_acquire_swapchain()
  tdengine.ffi.dn_gpu_render_target_bind(swapchain)
  tdengine.ffi.dn_gpu_render_target_clear(swapchain)
  tdengine.app:on_swapchain_ready()
  tdengine.ffi.dn_gpu_swap_buffers()

  tdengine.ffi.dn_time_metric_end('render')
end


function tdengine.gpu.build(gpu_info)
  self.add_resolutions(gpu_info.resolutions)
  self.add_render_targets(gpu_info.render_targets)
  self.add_buffers(gpu_info.buffers)
  self.add_shaders(gpu_info.shaders)
end


function tdengine.gpu.find(id)
  return self.assets[id:to_qualified_string()]
end
local self = dn.gpu
function dn.gpu.init()
  self.render_targets = {}
  self.buffers = {}
  self.shaders = {}
  self.resolutions = {}
  self.assets = {}
end

function dn.gpu.render()
  dn.time_metric.begin('render')

  dn.lifecycle.run_callback(dn.lifecycle.callbacks.on_render_scene)
  dn.lifecycle.run_callback(dn.lifecycle.callbacks.on_scene_rendered)

  local swapchain = dn.ffi.gpu_acquire_swapchain()
  dn.ffi.gpu_render_target_bind(swapchain)
  dn.ffi.gpu_render_target_clear(swapchain)
  app:on_swapchain_ready()
  dn.ffi.gpu_swap_buffers()

  dn.time_metric.stop('render')
end


function dn.gpu.build(gpu_info)
  self.add_resolutions(gpu_info.resolutions)
  self.add_render_targets(gpu_info.render_targets)
  self.add_buffers(gpu_info.buffers)
  self.add_shaders(gpu_info.shaders)
end


function dn.gpu.find(id)
  return self.assets[id:to_qualified_string()]
end
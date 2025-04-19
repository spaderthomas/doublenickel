local self = doublenickel.gpu
function doublenickel.gpu.init()
  self.render_targets = {}
  self.buffers = {}
  self.shaders = {}
  self.resolutions = {}
  self.assets = {}
end

function doublenickel.gpu.render()
  doublenickel.time_metric.begin('render')

  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_render_scene)
  doublenickel.lifecycle.run_callback(doublenickel.lifecycle.callbacks.on_scene_rendered)

  local swapchain = dn.gpu_acquire_swapchain()
  dn.gpu_render_target_bind(swapchain)
  dn.gpu_render_target_clear(swapchain)
  doublenickel.app:on_swapchain_ready()
  dn.gpu_swap_buffers()

  doublenickel.time_metric.stop('render')
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
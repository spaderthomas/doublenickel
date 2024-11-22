PostProcess = tdengine.subsystem.define('PostProcess')

tdengine.enum.define(
  'BloomMode',
  {
    Filter = 0,
    Blur = 1,
    Combine = 2,
    Map = 3,
    MapBlur = 4,
  }
)






function PostProcess:on_start_game()


  -- self.bloom_filter = SimplePostProcess:new()
  -- self.bloom_filter:set_render_pass('bloom_blur')
  -- self.bloom_filter:set_shader('bloom')
  -- self.bloom_filter:add_uniform('unfiltered_frame', 'post_process', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.bloom_filter:add_uniform('mode', tdengine.enums.BloomMode.Filter, tdengine.enums.UniformKind.Enum)

  -- self.chromatic_aberration = SimplePostProcess:new()
  -- self.chromatic_aberration:set_render_pass('post_process')
  -- self.chromatic_aberration:set_shader('chromatic_aberration')
  -- self.chromatic_aberration:add_uniform('unprocessed_frame', 'post_process', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.chromatic_aberration:add_uniform('blur_map', 'bloom_blur', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.chromatic_aberration:add_uniform('red_adjust', 0.9, tdengine.enums.UniformKind.F32)
  -- self.chromatic_aberration:add_uniform('blue_adjust', 0.9, tdengine.enums.UniformKind.F32)
  -- self.chromatic_aberration:add_uniform('green_adjust', 0.9, tdengine.enums.UniformKind.F32)
  -- self.chromatic_aberration:add_uniform('pixel_step', 2, tdengine.enums.UniformKind.I32)
  -- self.chromatic_aberration:add_uniform('edge_threshold', 0.05, tdengine.enums.UniformKind.F32)

  -- self.scanlines = SimplePostProcess:new()
  -- self.scanlines:set_render_pass('post_process')
  -- self.scanlines:set_shader('scanline')
  -- self.scanlines:add_uniform('unprocessed_frame', 'post_process', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.scanlines:add_uniform('bloom_map', 'bloom_blur', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.scanlines:add_uniform('red_adjust', 2.4, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('green_adjust', 1.7, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('blue_adjust', 1.3, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('bright_adjust', 0.1, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('oscillation_speed', 2, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('oscillation_intensity', 0.75, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('scanline_darkness', 1.0, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('scanline_min', 0.0, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('scanline_max', 0.75, tdengine.enums.UniformKind.F32)
  -- self.scanlines:add_uniform('scanline_height_px', 7, tdengine.enums.UniformKind.I32)

  -- self.bloom_filter = SimplePostProcess:new()
  -- self.bloom_filter:set_render_pass('bloom_blur')
  -- self.bloom_filter:set_shader('bloom')
  -- self.bloom_filter:add_uniform('unfiltered_frame', 'post_process', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.bloom_filter:add_uniform('mode', tdengine.enums.BloomMode.Filter, tdengine.enums.UniformKind.Enum)

  -- self.bloom_blur = SimplePostProcess:new()
  -- self.bloom_blur:set_render_pass('bloom_blur')
  -- self.bloom_blur:set_shader('bloom')
  -- self.bloom_blur:add_uniform('bloomed_frame', 'bloom_blur', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.bloom_blur:add_uniform('mode', tdengine.enums.BloomMode.Blur, tdengine.enums.UniformKind.Enum)

  -- self.bloom_combine = SimplePostProcess:new()
  -- self.bloom_combine:set_render_pass('post_process')
  -- self.bloom_combine:set_shader('bloom')
  -- self.bloom_combine:add_uniform('unfiltered_frame', 'post_process', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.bloom_combine:add_uniform('bloomed_frame', 'bloom_blur', tdengine.enums.UniformKind.RenderPassTexture)
  -- self.bloom_combine:add_uniform('mode', tdengine.enums.BloomMode.Combine, tdengine.enums.UniformKind.Enum)


  -- self.copy_output = SimplePostProcess:new()
  -- self.copy_output:set_render_pass('output')
  -- self.copy_output:set_shader('blit')
  -- self.copy_output:add_uniform('source', 'post_process', tdengine.enums.UniformKind.RenderPassTexture)

  -- self.upscale_scene = SimplePostProcess.blit('post_process', 'scene')
  -- self.upscale_color = SimplePostProcess.blit('upscale_color', 'color')
  -- self.upscale_normals = SimplePostProcess.blit('upscale_normals', 'normals')
  -- self.upscale_lit_scene = SimplePostProcess.blit('upscale_lit_scene', 'lit_scene')
  
end

function PostProcess:on_scene_rendered()
  tdengine.ffi.gpu_render_target_blit(
    tdengine.gpus.find(RenderTarget.Color),
    tdengine.gpus.find(RenderTarget.UpscaledColor)
)
end

function PostProcess:upscale()
    -- self.upscale_color:render()
  -- self.upscale_normals:render()
  -- self.upscale_lit_scene:render()
  -- self.upscale_scene:render()

  -- -- self:post_process()
  -- self.copy_output:render()
end

function PostProcess:post_process()
  -- self.chromatic_aberration:render()

  -- tdengine.ffi.gpu_render_target_clear(tdengine.gpu.find_read_target('bloom_blur'))
  -- self.bloom_filter:render()
  -- for bloom_index = 1, 2 do
  --   self.bloom_blur:render()
  -- end
  -- self.bloom_combine:render()


  -- self.scanlines:render()
end


















return {
	resolutions = {
		{
			id = Resolution.Native,
			size = {
				x = 320,
				y = 180
			}
		},
		{
			id = Resolution.Upscaled,
			size = {
				x = 1024,
				y = 576
			}
		},
	},
	buffers = {
		{
			id = Buffer.Lights,
			descriptor = {
				usage = GpuBufferUsage.Static,
				kind = GpuBufferKind.Storage,
				size = 420
			},
		}
	},
	render_targets = {
		{
			id = RenderTarget.Color,
			descriptor = {
				resolution = Resolution.Native,
			}
		},
		{
			id = RenderTarget.Normals,
			descriptor = {
				resolution = Resolution.Native,
			}
		},
		{
			id = RenderTarget.LightMap,
			descriptor = {
				resolution = Resolution.Native,
			}
		},
		{
			id = RenderTarget.LitScene,
			descriptor = {
				resolution = Resolution.Native,
			}
		},
		{
			id = RenderTarget.UpscaledColor,
			descriptor = {
				resolution = Resolution.Upscaled,
			}
		},
		{
			id = RenderTarget.UpscaledNormals,
			descriptor = {
				resolution = Resolution.Upscaled,
			}
		},
		{
			id = RenderTarget.UpscaledLitScene,
			descriptor = {
				resolution = Resolution.Upscaled,
			}
		},
		{
			id = RenderTarget.Editor,
			descriptor = {
				resolution = Resolution.Native,
			}
		},

	},
	command_buffers = {
		{
			id = CommandBuffer.Color,
			descriptor = {
				max_vertices = 1024,
				max_draw_calls = 64,
				vertex_attributes = {
					{
						count = 3,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 4,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 2,
						kind = tdengine.enums.VertexAttributeKind.Float
					}
				}
			}
		},
		{
			id = CommandBuffer.Normals,
			descriptor = {
				max_vertices = 1024,
				max_draw_calls = 64,
				vertex_attributes = {
					{
						count = 3,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 4,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 2,
						kind = tdengine.enums.VertexAttributeKind.Float
					}
				}
			}
		},
		{
			id = CommandBuffer.Upscale,
			descriptor = {
				max_vertices = 1024,
				max_draw_calls = 64,
				vertex_attributes = {
					{
						count = 3,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 4,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 2,
						kind = tdengine.enums.VertexAttributeKind.Float
					}
				}
			}
		},
		{
			id = CommandBuffer.LightMap,
			descriptor = {
				max_vertices = 1024,
				max_draw_calls = 64,
				vertex_attributes = {
					{
						count = 3,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 4,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 2,
						kind = tdengine.enums.VertexAttributeKind.Float
					}
				}
			}
		},
		{
			id = CommandBuffer.Editor,
			descriptor = {
				max_vertices = 64 * 1024,
				max_draw_calls = 256,
				vertex_attributes = {
					{
						count = 3,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 4,
						kind = tdengine.enums.VertexAttributeKind.Float
					},
					{
						count = 2,
						kind = tdengine.enums.VertexAttributeKind.Float
					}
				}
			}
		},
		{
			id = CommandBuffer.Shape,
			descriptor = {
				max_vertices = 1024,
				max_draw_calls = 64,
				vertex_attributes = {}
			}
		},
	},
	graphics_pipelines = {
		{
			id = GraphicsPipeline.Color,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.Color,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.Color
			}
		},
		{
			id = GraphicsPipeline.Normals,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.Normals,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.Normals
			}
		},
		{
			id = GraphicsPipeline.VisualizeLightMap,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.LightMap,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.LightMap
			}
		},
		{
			id = GraphicsPipeline.LightScene,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.LitScene,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.LightMap
			}
		},
		{
			id = GraphicsPipeline.UpscaleColor,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.UpscaledColor,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.Upscale
			}
		},
		{
			id = GraphicsPipeline.UpscaleNormals,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.UpscaledNormals,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.Upscale
			}
		},
		{
			id = GraphicsPipeline.UpscaleLitScene,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.UpscaledLitScene,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.Upscale
			}
		},
		{
			id = GraphicsPipeline.Editor,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.Editor,
					load_op = tdengine.enums.GpuLoadOp.Clear
				},
				command_buffer = CommandBuffer.Editor
			}
		},
		{
			id = GraphicsPipeline.Shape,
			descriptor = {
				color_attachment = {
					read = nil,
					write = RenderTarget.Color,
					load_op = tdengine.enums.GpuLoadOp.None
				},
				command_buffer = CommandBuffer.Shape
			}
		},
	},
	draw_configurations = {
		{
			id = DrawConfiguration.LightScene,
			shader = Shader.ApplyLighting,
			uniforms = {
				{
					name = 'light_map',
					kind = tdengine.enums.UniformKind.RenderTarget,
					-- actually, color attachment means "pull the read texture from some render pass' color attachment"
					value = RenderTarget.LightMap
				},
				{
					name = 'color_buffer',
					kind = tdengine.enums.UniformKind.RenderTarget,
					value = RenderTarget.Color
				},
				{
					name = 'normal_buffer',
					kind = tdengine.enums.UniformKind.RenderTarget,
					value = RenderTarget.Normals
				},
				{
					name = 'editor',
					kind = tdengine.enums.UniformKind.RenderTarget,
					value = RenderTarget.LightMap -- @fix
				},
				{
					name = 'num_lights',
					kind = tdengine.enums.UniformKind.I32,
					value = 1
				}
			},
			ssbos = {
				{
					id = Buffer.Lights,
					index = 0
				}
			}
		},
		{
			id = DrawConfiguration.VisualizeLightMap,
			pipeline = GraphicsPipeline.VisualizeLightMap,
			shader = Shader.LightMap,
			uniforms = {
				{
					name = 'num_lights',
					kind = tdengine.enums.UniformKind.I32,
					value = 1
				}
			},
			ssbos = {
				{
					id = Buffer.Lights,
					index = 0
				}
			}
		}

	},
	shaders = {
		{
			id = Shader.ApplyLighting,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'apply_lighting',
				vertex_shader = 'apply_lighting.vertex',
				fragment_shader = 'apply_lighting.fragment'
			}
		},
		{
			id = Shader.Shape,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'shape',
				vertex_shader = 'shape.vertex',
				fragment_shader = 'shape.fragment'
			}
		},
		{
			id = Shader.SdfNormal,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'sdf_normal',
				vertex_shader = 'sdf_normal.vertex',
				fragment_shader = 'sdf_normal.fragment'
			}
		},
		{
			id = Shader.LightMap,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'light_map',
				vertex_shader = 'light_map.vertex',
				fragment_shader = 'light_map.fragment'
			}
		},
		{
			id = Shader.Solid,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'solid',
				vertex_shader = 'solid.vertex',
				fragment_shader = 'solid.fragment'
			}
		},
		{
			id = Shader.Sprite,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'sprite',
				vertex_shader = 'sprite.vertex',
				fragment_shader = 'sprite.fragment'
			}
		},
		{
			id = Shader.Text,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'text',
				vertex_shader = 'text.vertex',
				fragment_shader = 'text.fragment'
			}
		},
		{
			id = Shader.PostProcess,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'post_process',
				vertex_shader = 'post_process.vertex',
				fragment_shader = 'post_process.fragment'
			}
		},
		{
			id = Shader.Blit,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'blit',
				vertex_shader = 'blit.vertex',
				fragment_shader = 'blit.fragment'
			}
		},
		{
			id = Shader.Particle,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'particle',
				vertex_shader = 'particle.vertex',
				fragment_shader = 'particle.fragment'
			}
		},
		{
			id = Shader.Fluid,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'fluid',
				vertex_shader = 'fluid.vertex',
				fragment_shader = 'fluid.fragment'
			}
		},
		{
			id = Shader.FluidEulerian,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'fluid_eulerian',
				vertex_shader = 'fluid_eulerian.vertex',
				fragment_shader = 'fluid_eulerian.fragment'
			}
		},
		{
			id = Shader.Scanline,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'scanline',
				vertex_shader = 'scanline.vertex',
				fragment_shader = 'scanline.fragment'
			}
		},
		{
			id = Shader.Bloom,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'bloom',
				vertex_shader = 'bloom.vertex',
				fragment_shader = 'bloom.fragment'
			}
		},
		{
			id = Shader.ChromaticAberration,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'chromatic_aberration',
				vertex_shader = 'chromatic_aberration.vertex',
				fragment_shader = 'chromatic_aberration.fragment'
			}
		},
		{
			id = Shader.FluidInit,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Compute,
				name = 'fluid_init',
				compute_shader = 'fluid_init.compute',
			}
		},
		{
			id = Shader.FluidUpdate,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Compute,
				name = 'fluid_update',
				compute_shader = 'fluid_update.compute',
			}
		},
		{
			id = Shader.FluidEulerianInit,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Compute,
				name = 'fluid_eulerian_init',
				compute_shader = 'fluid_eulerian_init.compute',
			}
		},
		{
			id = Shader.FluidEulerianUpdate,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Compute,
				name = 'fluid_eulerian_update',
				compute_shader = 'fluid_eulerian_update.compute',
			}
		},
	}
}

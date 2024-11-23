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
	},
	render_targets = {
		{
			id = RenderTarget.Native,
			descriptor = {
				resolution = Resolution.Native,
			}
		},
		{
			id = RenderTarget.Upscaled,
			descriptor = {
				resolution = Resolution.Upscaled,
			}
		}
	},
	shaders = {
		{
			id = Shader.Shape,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'shape',
				vertex_shader = 'sdf/shape.vertex',
				fragment_shader = 'sdf/shape.fragment'
			}
		},
		{
			id = Shader.Solid,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'solid',
				vertex_shader = 'solid/solid.vertex',
				fragment_shader = 'solid/solid.fragment'
			}
		},
		{
			id = Shader.Sprite,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'sprite',
				vertex_shader = 'sprite/sprite.vertex',
				fragment_shader = 'sprite/sprite.fragment'
			}
		},
		{
			id = Shader.Text,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'text',
				vertex_shader = 'text/text.vertex',
				fragment_shader = 'text/text.fragment'
			}
		},
		{
			id = Shader.Blit,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'blit',
				vertex_shader = 'blit/blit.vertex',
				fragment_shader = 'blit/blit.fragment'
			}
		},
	}
}

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
			id = Shader.Blit,
			descriptor = {
				kind = tdengine.enums.GpuShaderKind.Graphics,
				name = 'blit',
				vertex_shader = 'blit.vertex',
				fragment_shader = 'blit.fragment'
			}
		},
	}
}

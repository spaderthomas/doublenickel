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
	shaders = {}
}

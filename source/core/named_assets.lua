Resolution = tdengine.enum.define(
	'Resolution', 
	{
		Native = 0,
		Upscaled = 1
	}
)

Shader = tdengine.enum.define(
  'Shader',
  {
    ApplyLighting = 0,
		Shape = 1,
		Sdf = 2,
		SdfNormal = 3,
		LightMap = 4,
		Solid = 5,
		Sprite = 6,
		Text = 7,
		PostProcess = 8,
		Blit = 9,
		Particle = 10,
		Fluid = 11,
		FluidEulerian = 12,
		Scanline = 13,
		Bloom = 14,
		ChromaticAberration = 15,
		FluidInit = 16,
		FluidUpdate = 17,
		FluidEulerianInit = 18,
		FluidEulerianUpdate = 19,
	}
)

GraphicsPipeline = tdengine.enum.define(
  'GraphicsPipeline',
  {
    ChromaticAberration = 0,
    BloomBlur = 1,
    Color = 2,
    Shapes = 3,
		VisualizeLightMap = 4,
		LightScene = 5,
		UpscaleColor = 6,
		UpscaleNormals = 7,
		UpscaleLitScene = 8,
    Normals = 9,
    Editor = 10,
    Shape = 11,
  }
)

DrawConfiguration = tdengine.enum.define(
  'DrawConfiguration',
  {
		LightScene = 0,
		VisualizeLightMap = 1,
  }
)


RenderTarget = tdengine.enum.define(
  'RenderTarget',
  {
    LitScene = 0,
    Color = 1,
    Normals = 2,
    LightMap = 3,
    Editor = 4,
		UpscaledColor = 6,
		UpscaledNormals = 7,
		UpscaledLitScene = 8,
	}
)

CommandBuffer = tdengine.enum.define(
	'CommandBuffer',
	{
		Color = 0,
		Normals = 1,
		Upscale = 2,
		LightMap = 3,
    Editor = 4,
    Shape = 5,
	}
)

Buffer = tdengine.enum.define(
	'Buffer',
	{
		Lights = 0,
	}
)

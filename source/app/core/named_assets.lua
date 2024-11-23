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
		Shape = 1,
		Sdf = 2,
		SdfNormal = 3,
		Solid = 5,
		Sprite = 6,
		Text = 7,
		Blit = 9,
	}
)


RenderTarget = tdengine.enum.define(
  'RenderTarget',
  {
    Native = 0,
    Upscaled = 1,
	}
)

Buffer = tdengine.enum.define(
	'Buffer',
	{
		Lights = 0,
	}
)

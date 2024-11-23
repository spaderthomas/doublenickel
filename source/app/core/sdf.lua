Sdf = tdengine.enum.define(
	'Sdf',
	{
		Circle = tdengine.ffi.SDF_SHAPE_CIRCLE,
		Ring = tdengine.ffi.SDF_SHAPE_RING,
		Box = tdengine.ffi.SDF_SHAPE_BOX,
		OrientedBox = tdengine.ffi.SDF_SHAPE_ORIENTED_BOX,
		Combine = tdengine.ffi.SDF_SHAPE_COMBINE
	}
)

SdfCombineOp = tdengine.enum.define(
	'SdfCombineOp',
	{
		Union = tdengine.ffi.SDF_COMBINE_OP_UNION,
		Intersection = tdengine.ffi.SDF_COMBINE_OP_INTERSECTION,
		Subtraction = tdengine.ffi.SDF_COMBINE_OP_SUBTRACTION,
	}
)

SdfSmoothingKernel = tdengine.enum.define(
	'SdfSmoothingKernel',
	{
		None = tdengine.ffi.SDF_SMOOTH_KERNEL_NONE,
		PolynomialQuadratic = tdengine.ffi.SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC,
	}
)


SdfInstance = tdengine.class.metatype('SdfInstance')
function SdfInstance:init(params)
	self.buffer_index = params.buffer_index or 0
	self.kind = tdengine.enum.is_enum(params.kind) and params.kind:to_number() or params.kind
end

SdfCombineHeader = tdengine.class.metatype('SdfCombineHeader')
function SdfCombineHeader:init(params)
	self.num_sdfs = params.num_sdfs
end

SdfCombineEntry = tdengine.class.metatype('SdfCombineEntry')
function SdfCombineEntry:init(params)
	self.kind = params.kind
	self.buffer_index = params.buffer_index
	self.combine_op = params.combine_op:to_number()
	self.kernel = params.kernel:to_number()
end

SdfHeader = tdengine.class.metatype('SdfHeader')
function SdfHeader:init(params)
	self.shape = params.shape:to_number()
	self.color = tdengine.color(params.color):to_vec3()
  self.position = Vector2:new(params.position.x or 0, params.position.y or 0)
  self.rotation = params.rotation or 0
	self.edge_thickness = params.edge_thickness or 1
end

function SdfHeader:init_raw(params)
	local header = ffi.new('SdfHeader')
	header.color = params[1]
  header.position = params[2]
  header.rotation = params[3]
	header.edge_thickness = params[4]
	header.shape = params[5]
	return header
end

SdfCircle = tdengine.class.metatype('SdfCircle')
function SdfCircle:init(params)
	params.shape = Sdf.Circle
	self.header = SdfHeader:new(params)
  self.radius = params.radius or 10
end

SdfRing = tdengine.class.metatype('SdfRing')
function SdfRing:init(params)
	params.shape = Sdf.Ring
	self.header = SdfHeader:new(params)
  self.inner_radius = params.inner_radius or 10
  self.outer_radius = params.outer_radius or 20
end

SdfOrientedBox = tdengine.class.metatype('SdfOrientedBox')
function SdfOrientedBox:init(params)
	params.shape = Sdf.OrientedBox
	self.header = SdfHeader:new(params)
  self.size = Vector2:new(params.size.x or 20, params.size.y or 2)
end
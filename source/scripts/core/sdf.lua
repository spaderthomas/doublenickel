
SdfRenderer = doublenickel.class.metatype('dn_sdf_renderer_t', 'dn_sdf')
function SdfRenderer:init(params)
	self = doublenickel.ffi.sdf_renderer_create(params.buffer_size)
end

function SdfRenderer:oriented_box(params)
	dn.sdf_oriented_box_ex(
		self,
		params.position.x, params.position.y,
		params.color.r, params.color.g, params.color.b, 
		params.rotation,
		params.edge_thickness,
		params.length,
		params.thickness
	)
end

function SdfRenderer:foo(params)
end

function SdfRenderer:circle(params)
	dn.sdf_circle_ex(
		self,
		params.position.x, params.position.y,
		params.color.r, params.color.g, params.color.b, 
		params.rotation,
		params.edge_thickness,
		params.radius
	)
end

SdfInstance = doublenickel.class.metatype('dn_sdf_instance_t')
function SdfInstance:init(params)
	self.buffer_index = params.buffer_index or 0
	self.kind = doublenickel.enum.is_enum(params.kind) and params.kind:to_number() or params.kind
end

SdfCombineHeader = doublenickel.class.metatype('dn_sdf_combine_header_t')
function SdfCombineHeader:init(params)
	self.num_sdfs = params.num_sdfs
end

SdfCombineEntry = doublenickel.class.metatype('dn_sdf_combine_entry_t')
function SdfCombineEntry:init(params)
	self.kind = params.kind
	self.buffer_index = params.buffer_index
	self.combine_op = params.combine_op:to_number()
	self.kernel = params.kernel:to_number()
end

SdfHeader = doublenickel.class.metatype('dn_sdf_header_t')
function SdfHeader:init(params)
	self.shape = params.shape:to_number()
	self.color = doublenickel.color(params.color):to_vec3()
  self.position = Vector2:new(params.position.x or 0, params.position.y or 0)
  self.rotation = params.rotation or 0
	self.edge_thickness = params.edge_thickness or 1
end

function SdfHeader:init_raw(params)
	local header = ffi.new('dn_sdf_header_t')
	header.color = params[1]
  header.position = params[2]
  header.rotation = params[3]
	header.edge_thickness = params[4]
	header.shape = params[5]
	return header
end

SdfCircle = doublenickel.class.metatype('dn_sdf_circle_t')
function SdfCircle:init(params)
	params.shape = Sdf.Circle
	self.header = SdfHeader:new(params)
  self.radius = params.radius or 10
end

SdfRing = doublenickel.class.metatype('dn_sdf_ring_t')
function SdfRing:init(params)
	params.shape = Sdf.Ring
	self.header = SdfHeader:new(params)
  self.inner_radius = params.inner_radius or 10
  self.outer_radius = params.outer_radius or 20
end

SdfOrientedBox = doublenickel.class.metatype('dn_sdf_oriented_box_t')
function SdfOrientedBox:init(params)
	params.shape = Sdf.OrientedBox
	self.header = SdfHeader:new(params)
  self.size = Vector2:new(params.size.x or 20, params.size.y or 2)
end


function tdengine.ffi.namespaced_metatype(namespace, struct_name)
	ffi.metatype(struct_name, {
		__index = function(struct_instance, fn_name)
			return ffi.C[namespace .. '_' .. fn_name]
		end
	})
end

function tdengine.ffi.namespace(prefix)
	local namespace = {}
	setmetatable(namespace, {
		__index = function(__namespace, fn_name)
			return ffi.C[prefix .. '_' .. fn_name]
		end
	})
	return namespace
end


function tdengine.ffi.init()
	setmetatable(
		tdengine.ffi,
		{
			__index = function(self, key)
				local wrapper = rawget(self, key)
				if wrapper then return wrapper end

				local found, value = pcall(function() return ffi.C[key] end)
				if found then return value end
			end
		}
	)

	local string_metatable = {
		__index = {
			to_interned = function(self)
				if tdengine.ffi.is_nil(self.data) then dbg() end
				return ffi.string(self.data)
			end,
		}
	}
	ffi.metatype('tstring', string_metatable)
	ffi.metatype('string', string_metatable)

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


	CoordinateSystem = tdengine.enum.define(
		'CoordinateSystem',
		{
			World = tdengine.ffi.DN_COORD_UNIT_WORLD,
			Screen = tdengine.ffi.DN_COORD_UNIT_SCREEN,
			Window = tdengine.ffi.DN_COORD_UNIT_WINDOW,
			Game = tdengine.ffi.DN_COORD_UNIT_GAME,
		}
	)

	InputDevice = tdengine.enum.define(
		'InputDevice',
		{
			MouseAndKeyboard = 0,
			Controller = 1
		}
	)

	ParticleKind = tdengine.enum.define(
		'ParticleKind',
		{
			Quad = tdengine.ffi.ParticleKind_Quad,
			Circle = tdengine.ffi.ParticleKind_Circle,
			Image = tdengine.ffi.ParticleKind_Image,
			Invalid = tdengine.ffi.ParticleKind_Invalid,
		}
	)

	WindowFlags = tdengine.enum.define(
		'WindowFlags',
		{
			None = tdengine.ffi.DN_WINDOW_FLAG_NONE,
			Windowed = tdengine.ffi.DN_WINDOW_FLAG_WINDOWED,
			Border = tdengine.ffi.DN_WINDOW_FLAG_BORDER,
			Vsync = tdengine.ffi.DN_WINDOW_FLAG_VSYNC,
		}
	)

	GpuResourceId = tdengine.enum.define(
		'GpuResourceId',
		{
			Framebuffer = tdengine.ffi.GPU_RESOURCE_FRAMEBUFFER,
			Shader = tdengine.ffi.GPU_RESOURCE_SHADER,
			Program = tdengine.ffi.GPU_RESOURCE_PROGRAM,
		}
	)

	GpuShaderKind = tdengine.enum.define(
		'GpuShaderKind',
		{
			Graphics = tdengine.ffi.GPU_SHADER_GRAPHICS,
			Compute = tdengine.ffi.GPU_SHADER_COMPUTE,
		}
	)

	GpuDrawMode = tdengine.enum.define(
		'GpuDrawMode',
		{
			Arrays = tdengine.ffi.GPU_DRAW_MODE_ARRAYS,
			Instance = tdengine.ffi.GPU_DRAW_MODE_INSTANCE,
		}
	)

	GpuDrawPrimitive = tdengine.enum.define(
		'GpuDrawPrimitive',
		{
			Triangles = tdengine.ffi.GPU_PRIMITIVE_TRIANGLES,
		}
	)

	GpuVertexAttributeKind = tdengine.enum.define(
		'GpuVertexAttributeKind',
		{
			Float = tdengine.ffi.GPU_VERTEX_ATTRIBUTE_FLOAT,
			U32 = tdengine.ffi.GPU_VERTEX_ATTRIBUTE_U32,
		}
	)

	GpuBlendFunction = tdengine.enum.define(
		'GpuBlendFunction',
		{
			Add = tdengine.ffi.GPU_BLEND_FUNC_ADD,
			Subtract = tdengine.ffi.GPU_BLEND_FUNC_SUBTRACT,
			ReverseSubtract = tdengine.ffi.GPU_BLEND_FUNC_REVERSE_SUBTRACT,
			Min = tdengine.ffi.GPU_BLEND_FUNC_MIN,
			Max = tdengine.ffi.GPU_BLEND_FUNC_MAX,

		}
	)

	GpuUniformKind = tdengine.enum.define(
		'GpuUniformKind',
		{
			None =    tdengine.ffi.GPU_UNIFORM_NONE,
			Matrix4 = tdengine.ffi.GPU_UNIFORM_MATRIX4,
			Matrix3 = tdengine.ffi.GPU_UNIFORM_MATRIX3,
			Matrix2 = tdengine.ffi.GPU_UNIFORM_MATRIX2,
			Vector4 = tdengine.ffi.GPU_UNIFORM_VECTOR4,
			Vector3 = tdengine.ffi.GPU_UNIFORM_VECTOR3,
			Vector2 = tdengine.ffi.GPU_UNIFORM_VECTOR2,
			F32 =     tdengine.ffi.GPU_UNIFORM_F32,
			I32 =     tdengine.ffi.GPU_UNIFORM_I32,
			Texture = tdengine.ffi.GPU_UNIFORM_TEXTURE,
			Enum =    tdengine.ffi.GPU_UNIFORM_ENUM,
		}
	)


	GpuBufferUsage = tdengine.enum.define(
		'GpuBufferUsage',
		{
			Static =  tdengine.ffi.GPU_BUFFER_USAGE_STATIC,
			Dynamic = tdengine.ffi.GPU_BUFFER_USAGE_DYNAMIC,
			Stream =  tdengine.ffi.GPU_BUFFER_USAGE_STREAM,
		}
	)

	GpuBufferKind = tdengine.enum.define(
		'GpuBufferKind',
		{
			Storage = tdengine.ffi.GPU_BUFFER_KIND_STORAGE,
			Array = tdengine.ffi.GPU_BUFFER_KIND_ARRAY,
		}
	)

	UniformKind = tdengine.enum.define(
		'UniformKind',
		{
			Matrix4        = tdengine.ffi.UniformKind_Matrix4,
			Matrix3        = tdengine.ffi.UniformKind_Matrix3,
			Vector4        = tdengine.ffi.UniformKind_Vector4,
			Vector3        = tdengine.ffi.UniformKind_Vector3,
			Vector2        = tdengine.ffi.UniformKind_Vector2,
			F32            = tdengine.ffi.UniformKind_F32,
			I32            = tdengine.ffi.UniformKind_I32,
			Texture        = tdengine.ffi.UniformKind_Texture,
			PipelineOutput = tdengine.ffi.UniformKind_PipelineOutput,
			RenderTarget   = tdengine.ffi.UniformKind_RenderTarget,
			Enum = 201,
		}
	)

	GpuMemoryBarrier = tdengine.enum.define(
		'GpuMemoryBarrier',
		{
			ShaderStorage = tdengine.ffi.GPU_MEMORY_BARRIER_STORAGE,
			BufferUpdate = tdengine.ffi.GPU_MEMORY_BARRIER_BUFFER_UPDATE,
		}
	)

	DisplayMode = tdengine.enum.define(
		'DisplayMode',
		{
			p480 = ffi.C.DN_DISPLAY_MODE_480,
			p720 = ffi.C.DN_DISPLAY_MODE_720,
			p1080 = ffi.C.DN_DISPLAY_MODE_1080,
			p1440 = ffi.C.DN_DISPLAY_MODE_1440,
			p2160 = ffi.C.DN_DISPLAY_MODE_2160,
			p1280_800 = ffi.C.DN_DISPLAY_MODE_1280_800,
			Fullscreen = ffi.C.DN_DISPLAY_MODE_FULLSCREEN,
		}
	)

	GpuLoadOp = tdengine.enum.define(
		'GpuLoadOp',
		{
			None = tdengine.ffi.GPU_LOAD_OP_NONE,
			Clear = tdengine.ffi.GPU_LOAD_OP_CLEAR,
		}
	)

	imgui.internal.init_c_api()
	imgui.internal.init_lua_api()
	imgui.internal.init_lua_api_overwrites()

	local header = tdengine.module.read_from_named_path('ffi_info')
	ffi.cdef(header)
end


----------------
-- REFLECTION --
----------------
tdengine.enum.define(
	'ctype',
	{
		void = 0,
		int = 1,
		float = 2,
		enum = 3,
		constant = 4,
		ptr = 5,
		ref = 6,
		array = 7,
		struct = 8,
		union = 9,
		func = 10,
		field = 11,
		bitfield = 12
	}
)

function tdengine.ffi.is_nil(cdata)
	return cdata == nil
end

function tdengine.ffi.field_ptr(cdata, member)
	local inner_type = tdengine.ffi.inner_type(member)
	local type_name = tdengine.ffi.type_name(inner_type)
	local byte_ptr = ffi.cast('u8*', cdata)
	return ffi.cast(type_name .. '*', byte_ptr + member.offset)
end

function tdengine.ffi.type_name(inner_type)
	local ctype = tdengine.enums.ctype

	local type_name = inner_type.what
	if ctype.float:match(inner_type.what) then
		if inner_type.size == 4 then
			type_name = 'float'
		elseif inner_type.size == 8 then
			type_name = 'double'
		end
	elseif ctype.int:match(inner_type.what) then
		if inner_type.bool then
			type_name = 'bool'
		elseif inner_type.unsigned then
			if inner_type.size == 1 then
				type_name = 'uint8_t'
			elseif inner_type.size == 2 then
				type_name = 'uint16_t'
			elseif inner_type.size == 4 then
				type_name = 'uint32_t'
			elseif inner_type.size == 8 then
				type_name = 'uint64_t'
			end
		else
			if inner_type.size == 1 then
				type_name = 'int8_t'
			elseif inner_type.size == 2 then
				type_name = 'int16_t'
			elseif inner_type.size == 4 then
				type_name = 'int32_t'
			elseif inner_type.size == 8 then
				type_name = 'int64_t'
			end
		end
	end

	return type_name
end

function tdengine.ffi.imgui_datatype(inner_type)
	local ctype = tdengine.enums.ctype

	local type_name = inner_type.what
	if ctype.float:match(inner_type.what) then
		if inner_type.size == 4 then
			type_name = ffi.C.ImGuiDataType_Float
		elseif inner_type.size == 8 then
			type_name = ffi.C.ImGuiDataType_Double
		end
	elseif ctype.int:match(inner_type.what) then
		if inner_type.unsigned then
			if inner_type.size == 1 then
				type_name = ffi.C.ImGuiDataType_U8
			elseif inner_type.size == 2 then
				type_name = ffi.C.ImGuiDataType_U16
			elseif inner_type.size == 4 then
				type_name = ffi.C.ImGuiDataType_U32
			elseif inner_type.size == 8 then
				type_name = ffi.C.ImGuiDataType_U64
			end
		else
			if inner_type.size == 1 then
				type_name = ffi.C.ImGuiDataType_S8
			elseif inner_type.size == 2 then
				type_name = ffi.C.ImGuiDataType_S16
			elseif inner_type.size == 4 then
				type_name = ffi.C.ImGuiDataType_S32
			elseif inner_type.size == 8 then
				type_name = ffi.C.ImGuiDataType_S64
			end
		end
	end

	return type_name
end

function tdengine.ffi.imgui_datatypeof(cdata)
	return tdengine.ffi.imgui_datatype(tdengine.ffi.inner_typeof(cdata))
end

function tdengine.ffi.pretty_type(type_info)
	local ctype = tdengine.enums.ctype

	local inner_type = tdengine.ffi.inner_type(type_info)

	-- p(type_info)
	local type_name = inner_type.what
	if ctype.struct:match(inner_type.what) then
		type_name = inner_type.name or string.format('struct %d', inner_type.typeid)
	elseif ctype.enum:match(type_info.what) then
		type_name = inner_type.name or string.format('enum %d', inner_type.typeid)
	elseif ctype.float:match(inner_type.what) then
		if inner_type.size == 4 then
			type_name = 'f32'
		elseif inner_type.size == 8 then
			type_name = 'f64'
		end
	elseif ctype.int:match(inner_type.what) then
		if inner_type.bool then
			return 'bool'
		elseif inner_type.unsigned then
			if inner_type.size == 1 then
				type_name = 'u8'
			elseif inner_type.size == 2 then
				type_name = 'u16'
			elseif inner_type.size == 4 then
				type_name = 'u32'
			elseif inner_type.size == 8 then
				type_name = 'u64'
			else
				type_name = 'unknown unsigned'
			end
		else
			if inner_type.size == 1 then
				type_name = 's8'
			elseif inner_type.size == 2 then
				type_name = 's16'
			elseif inner_type.size == 4 then
				type_name = 's32'
			elseif inner_type.size == 8 then
				type_name = 's64'
			else
				type_name = 'unknown signed'
			end
		end
	else
		type_name = 'unknown'
	end

	return type_name

end

function tdengine.ffi.pretty_typeof(cdata)
	return tdengine.ffi.pretty_type(tdengine.ffi.typeof(cdata))
end

function tdengine.ffi.pretty_ptr(type_info)
	return string.format('%s*', tdengine.ffi.pretty_type(type_info))
end

function tdengine.ffi.pretty_ptrof(cdata)
	return string.format('%s*', tdengine.ffi.pretty_type(tdengine.ffi.typeof(cdata)))
end

function tdengine.ffi.address_of(cdata)
	-- Not for actual addressing; just for printing
	local s = tostring(cdata)
	local parts = s:split(':')
	return parts[2]:gsub(' ', '')
end

function tdengine.ffi.is_composite_type(type_info)
	return
		tdengine.enums.ctype.struct:match(type_info.what) or
		tdengine.enums.ctype.array:match(type_info.what)
end

function tdengine.ffi.sorted_members(type_info)
	local members = {}
	for member in type_info:members() do
		table.insert(members, member)
	end

	table.sort(members, function (a, b)
		local A_FIRST = true
		local B_FIRST = false

		local is_a_composite = tdengine.ffi.is_composite_type(a)
		local is_b_composite = tdengine.ffi.is_composite_type(b)

		if is_a_composite and not is_b_composite then
			return A_FIRST
		elseif not is_a_composite and is_b_composite then
			return B_FIRST
		else
			return a.name < b.name
		end
	end)

	return tdengine.iterator.values(members)
end

function tdengine.ffi.typeof(cdata)
	return reflect.typeof(cdata)
end

function tdengine.ffi.inner_typeof(cdata)
	return tdengine.ffi.inner_type(reflect.typeof(cdata))
end

function tdengine.ffi.inner_type(type_info)
	local ctype = tdengine.enums.ctype

	if ctype.ref:match(type_info.what) then
		type_info = type_info.element_type
	elseif ctype.ptr:match(type_info.what) then
		type_info = type_info.element_type
	elseif ctype.field:match(type_info.what) then
		type_info = tdengine.ffi.inner_type(type_info.type)
	elseif ctype.enum:match(type_info.what) then
		type_info = type_info.type
	end

	return type_info
end

function tdengine.ffi.is_opaque(cdata)
	return tdengine.ffi.inner_typeof(cdata).size == 'none'
end

function tdengine.ffi.ptr_type(ctype)
	return string.format('%s *', ctype)
end



---------------
-- METATYPES -- 
---------------

----------------------
-- MEMORY ALLOCATOR --
----------------------
dn_allocator_t = tdengine.class.metatype('dn_allocator_t')

function dn_allocator_t:find(name)
	return tdengine.ffi.dn_allocator_find(name)
end

function dn_allocator_t:add(name)
	return tdengine.ffi.dn_allocator_add(self, name)
end

function dn_allocator_t:alloc(size)
	return tdengine.ffi.dn_allocator_alloc(self, size)
end

function dn_allocator_t:free(pointer)
	return tdengine.ffi.dn_allocator_free(self, pointer)
end

function dn_allocator_t:alloc_array(ctype, n)
	return ffi.cast(tdengine.ffi.ptr_type(ctype), tdengine.ffi.dn_allocator_alloc(self, ffi.sizeof(ctype) * n))
end


------------
-- MATRIX --
------------
Matrix3 = tdengine.class.metatype('Matrix3')
Matrix3:set_metamethod('__index', function(self, key)
	print('x')
	if type(key) == 'number' then
		return self.data[key]
	end
end)

function Matrix3:Identity()
	local matrix = Matrix3:new()
	matrix.data[0][0] = 1
	matrix.data[1][1] = 1
	matrix.data[2][2] = 1
	return matrix
end

function Matrix3:init(data)
	if data then
		for i = 0, 2 do
			for j = 0, 2 do
				self.data[i][j] = data[i + 1][j + 1]
			end
		end
	end	
end

function Matrix3:serialize()
	local serialized = {}

	for i = 0, 2 do
		serialized[i + 1] = {}
		for j = 0, 2 do
			serialized[i + 1][j + 1] = self.data[i][j]
		end
	end
	return serialized
end


Matrix4 = tdengine.class.metatype('Matrix4')

function Matrix4:init(data)
	if data then
		for i = 0, 3 do
			for j = 0, 3 do
				self.data[i][j] = data[i + 1][j + 1]
			end
		end
	end	
end

function Matrix4:serialize()
	local serialized = {}

	for i = 0, 3 do
		serialized[i + 1] = {}
		for j = 0, 3 do
			serialized[i + 1][j + 1] = self.data[i][j]
		end
	end
	return serialized
end

------------
-- VECTOR --
------------
Vector2 = tdengine.class.metatype('Vector2')
function Vector2:init(x, y)
	self.x = x or self.x
	self.y = y or self.y
end

function Vector2:scale(scalar)
	return Vector2:new(self.x * scalar, self.y * scalar)
end

Vector3 = tdengine.class.metatype('Vector3')
function  Vector3:init(x, y, z)
	self.x = x or self.x
	self.y = y or self.y
	self.z = z or self.z
end


Vector4 = tdengine.class.metatype('Vector4')
function Vector4:init(x, y, z, w)
	self.x = x or self.x
	self.y = y or self.y
	self.z = z or self.z
	self.w = w or self.w
end

Vertex = tdengine.class.metatype('Vertex')
function Vertex:Quad(top, bottom, left, right)
	local vertices = ffi.new('Vertex [6]')

	vertices[0].position = Vector3:new(left, top)
	vertices[1].position = Vector3:new(left, bottom)
	vertices[2].position = Vector3:new(right, bottom)
	vertices[3].position = Vector3:new(left, top)
	vertices[4].position = Vector3:new(right, bottom)
	vertices[5].position = Vector3:new(right, top)

	return vertices
end



-------------------
-- DYNAMIC ARRAY --
-------------------
DynamicArray = tdengine.class.define('DynamicArray')
function DynamicArray:init(ctype, allocator)
	self.data = ffi.new('void* [1]')
	self.value_type = ctype
	self.reference_type = string.format('%s [1]', ctype)
	self.pointer_type = string.format('%s*', ctype)
	self.element_size = ffi.sizeof(self.value_type)
	self.allocator = allocator or tdengine.ffi.dn_allocator_find('bump')

	self.data[0] = tdengine.ffi.dn_dynamic_array_create(self.element_size, self.allocator)
end

function DynamicArray:push(value)
	local marshalled_value = ffi.new(self.reference_type, value)
	tdengine.ffi.dn_dynamic_array_push_n(self.data, marshalled_value, 1)
end

function DynamicArray:at(index)
	local pointer = ffi.cast(self.pointer_type, self.data[0])
	return pointer[index]
end


CpuBuffer = tdengine.class.define('CpuBuffer')

function CpuBuffer:init(ctype, capacity)
  self.size = 0
  self.capacity = capacity
  self.ctype = ctype
  self.data = ffi.new(string.format('%s [%d]', ctype, capacity))
end

function CpuBuffer:push(element)
  -- tdengine.debug.assert(self.size < self.capacity)

	if element then self.data[self.size] = element end
  self.size = self.size + 1

	return slot
end

function CpuBuffer:fast_clear()
  self.size = 0
end

function CpuBuffer:fast_push(value)
	self.data[self.size] = value
	self.size = self.size + 1
end

function tdengine.class.define_fast(name)

end

FastCpuBuffer = {}
setmetatable(FastCpuBuffer, {
	__index = {
		push = function(self, value)
			self.data[self.size] = value
			self.size = self.size + 1
		end
	}
})

BackedGpuBuffer = tdengine.class.define('BackedGpuBuffer')
function BackedGpuBuffer:init(ctype, capacity, gpu_buffer)
  self.ctype = ctype
  self.cpu_buffer = CpuBuffer:new(ctype, capacity)
  self.gpu_buffer = GpuBuffer:new(ctype, capacity, gpu_buffer)
end

function BackedGpuBuffer:owned(ctype, capacity, gpu_buffer_descriptor)
	gpu_buffer_descriptor.size = ffi.sizeof(ctype) * capacity
	return BackedGpuBuffer:new(ctype, capacity, tdengine.ffi.gpu_buffer_create(gpu_buffer_descriptor))
end

function BackedGpuBuffer:fast_clear()
	return self.cpu_buffer:fast_clear()
end

function BackedGpuBuffer:push(data)
	return self.cpu_buffer:push(data)
end

function BackedGpuBuffer:size()
	return self.cpu_buffer.size
end

function BackedGpuBuffer:sync()
  tdengine.ffi.gpu_buffer_sync_subdata(
  	self.gpu_buffer:to_ctype(), self.cpu_buffer.data,
  	ffi.sizeof(self.ctype) * self.cpu_buffer.size,
  	0)
end



GpuBuffer = tdengine.class.define('GpuBuffer')

function GpuBuffer:init(ctype, capacity, gpu_buffer)
  self.ctype = ctype
  self.capacity = capacity
  self.buffer = gpu_buffer or tdengine.ffi.gpu_buffer_create(GpuBufferDescriptor:new({
		kind = GpuBufferKind.Storage,
		usage = GpuBufferUsage.Static,
		size = ffi.sizeof(ctype) * capacity
	}))
end

function GpuBuffer:to_ctype()
	return self.buffer
end

function GpuBuffer:zero()
  tdengine.ffi.gpu_buffer_zero(self.buffer, self.capacity * ffi.sizeof(self.ctype))
end

function GpuBuffer:bind_base(base)
  tdengine.ffi.gpu_buffer_bind_base(self.buffer, base)
end


WindowConfig = tdengine.class.metatype('dn_window_config_t')
function WindowConfig:init(params)
	local default = tdengine.ffi.dn_window_config_default();
	self.title = params.title or default.title
	self.icon = params.icon or default.icon
	self.native_resolution = params.native_resolution or default.native_resolution
	self.flags = params.flags or default.flags
end

AudioConfig = tdengine.class.metatype('dn_audio_config_t')
function AudioConfig:init(params)
	local allocator = tdengine.ffi.dn_allocator_find('bump')

	self.num_dirs = #params.dirs
	self.dirs = allocator:alloc_array('dn_path_t', #params.dirs)
	for i = 0, #params.dirs - 1 do
		self.dirs[i] = params.dirs[i + 1]
	end
end

FontDescriptor = tdengine.class.metatype('dn_font_descriptor_t')
function FontDescriptor:init(params)
  self.id = params.id
  self.file_name = params.file_name

  self.sizes = {0}
  for i in tdengine.iterator.keys(params.sizes) do
    self.sizes[i - 1] = params.sizes[i]
  end
  self.flags = 0
  if params.default       then self.flags = bit.bor(self.flags, ffi.C.DN_FONT_FLAG_DEFAULT) end
  if params.imgui         then self.flags = bit.bor(self.flags, ffi.C.DN_FONT_FLAG_IMGUI) end
end

FontConfig = tdengine.class.metatype('dn_font_config_t')
function FontConfig:init(params)
	local allocator = tdengine.ffi.dn_allocator_find('bump')

	self.num_fonts = #params.fonts
	self.fonts = allocator:alloc_array('dn_font_descriptor_t', #params.fonts)
	for i = 0, #params.fonts - 1 do
		self.fonts[i] = FontDescriptor:new(params.fonts[i + 1])
	end
end


AppConfig = tdengine.class.metatype('dn_app_config_t')
function AppConfig:init(params)
	self.window = params.window or tdengine.ffi.dn_window_config_default()
	self.audio = params.audio or tdengine.ffi.dn_audio_config_default()
	self.font = params.font or tdengine.ffi.dn_font_config_default()
end

------------------
-- FFI WRAPPERS --
------------------
function tdengine.ffi.dm_window_get_display_mode()
	return tdengine.enums.DisplayMode(ffi.C.dm_window_get_display_mode())
end

function tdengine.ffi.set_camera()
end


function tdengine.ffi.push_fullscreen_quad()
	local n = ffi.C.dn_window_get_native_resolution()
	local uvs = nil
	local opacity = 1.0
	ffi.C.push_quad(0, n.y, n.x, n.y, uvs, opacity);
end


InputDevice = tdengine.enum.define(
	'InputDevice',
	{
		MouseAndKeyboard = 0,
		Controller = 1
	}
)

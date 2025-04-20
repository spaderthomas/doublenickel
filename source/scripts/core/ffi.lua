function dn.ffi.namespaced_metatype(namespace, struct_name)
  ffi.metatype(struct_name, {
    __index = function(struct_instance, fn_name)
      return ffi.C[namespace .. '_' .. fn_name]
    end
  })
end

function dn.ffi.namespace(prefix)
  setmetatable(namespace, {
    __index = function(__namespace, fn_name)
      return ffi.C[prefix .. '_' .. fn_name]
    end
  })
  return namespace
end

function dn.ffi.init()
  setmetatable(
    dn.ffi,
    {
      __index = function(self, fn_name)
        return ffi.C['dn_' .. fn_name]
      end
    }
  )

  setmetatable(
    dn.unported,
    {
      __index = function(self, fn_name)
        dn.ffi.log('dn.unported.%s', fn_name)
        dbg()
        return function() end
      end
    }
  )

  CType = dn.enum.define(
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

  Sdf = dn.enum.define(
    'Sdf',
    {
      Circle = ffi.C.DN_SDF_SHAPE_CIRCLE,
      Ring = ffi.C.DN_SDF_SHAPE_RING,
      Box = ffi.C.DN_SDF_SHAPE_BOX,
      OrientedBox = ffi.C.DN_SDF_SHAPE_ORIENTED_BOX,
      Combine = ffi.C.DN_SDF_SHAPE_COMBINE
    }
  )

  SdfCombineOp = dn.enum.define(
    'SdfCombineOp',
    {
      Union = ffi.C.DN_SDF_COMBINE_OP_UNION,
      Intersection = ffi.C.DN_SDF_COMBINE_OP_INTERSECTION,
      Subtraction = ffi.C.DN_SDF_COMBINE_OP_SUBTRACTION,
    }
  )

  SdfSmoothingKernel = dn.enum.define(
    'SdfSmoothingKernel',
    {
      None = ffi.C.DN_SDF_SMOOTH_KERNEL_NONE,
      PolynomialQuadratic = ffi.C.DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC,
    }
  )


  CoordinateSystem = dn.enum.define(
    'CoordinateSystem',
    {
      World = ffi.C.DN_COORD_UNIT_WORLD,
      Screen = ffi.C.DN_COORD_UNIT_SCREEN,
      Window = ffi.C.DN_COORD_UNIT_WINDOW,
      Game = ffi.C.DN_COORD_UNIT_GAME,
    }
  )

  InputDevice = dn.enum.define(
    'InputDevice',
    {
      MouseAndKeyboard = 0,
      Controller = 1
    }
  )

  WindowFlags = dn.enum.define(
    'WindowFlags',
    {
      None = ffi.C.DN_WINDOW_FLAG_NONE,
      Windowed = ffi.C.DN_WINDOW_FLAG_WINDOWED,
      Border = ffi.C.DN_WINDOW_FLAG_BORDER,
      Vsync = ffi.C.DN_WINDOW_FLAG_VSYNC,
    }
  )

  GpuResourceId = dn.enum.define(
    'GpuResourceId',
    {
      Framebuffer = ffi.C.DN_GPU_RESOURCE_FRAMEBUFFER,
      Shader = ffi.C.DN_GPU_RESOURCE_SHADER,
      Program = ffi.C.DN_GPU_RESOURCE_PROGRAM,
    }
  )

  GpuShaderKind = dn.enum.define(
    'GpuShaderKind',
    {
      Graphics = ffi.C.DN_GPU_SHADER_GRAPHICS,
      Compute = ffi.C.DN_GPU_SHADER_COMPUTE,
    }
  )

  GpuDrawMode = dn.enum.define(
    'GpuDrawMode',
    {
      Arrays = ffi.C.DN_GPU_DRAW_MODE_ARRAYS,
      Instance = ffi.C.DN_GPU_DRAW_MODE_INSTANCE,
    }
  )

  GpuDrawPrimitive = dn.enum.define(
    'GpuDrawPrimitive',
    {
      Triangles = ffi.C.DN_GPU_PRIMITIVE_TRIANGLES,
    }
  )

  GpuVertexAttributeKind = dn.enum.define(
    'GpuVertexAttributeKind',
    {
      Float = ffi.C.DN_GPU_VERTEX_ATTRIBUTE_FLOAT,
      U32 = ffi.C.DN_GPU_VERTEX_ATTRIBUTE_U32,
    }
  )

  GpuBlendFunction = dn.enum.define(
    'GpuBlendFunction',
    {
      Add = ffi.C.DN_GPU_BLEND_FUNC_ADD,
      Subtract = ffi.C.DN_GPU_BLEND_FUNC_SUBTRACT,
      ReverseSubtract = ffi.C.DN_GPU_BLEND_FUNC_REVERSE_SUBTRACT,
      Min = ffi.C.DN_GPU_BLEND_FUNC_MIN,
      Max = ffi.C.DN_GPU_BLEND_FUNC_MAX,

    }
  )

  GpuUniformKind = dn.enum.define(
    'GpuUniformKind',
    {
      None =    ffi.C.DN_GPU_UNIFORM_NONE,
      Matrix4 = ffi.C.DN_GPU_UNIFORM_MATRIX4,
      Matrix3 = ffi.C.DN_GPU_UNIFORM_MATRIX3,
      Matrix2 = ffi.C.DN_GPU_UNIFORM_MATRIX2,
      Vector4 = ffi.C.DN_GPU_UNIFORM_VECTOR4,
      Vector3 = ffi.C.DN_GPU_UNIFORM_VECTOR3,
      Vector2 = ffi.C.DN_GPU_UNIFORM_VECTOR2,
      F32 =     ffi.C.DN_GPU_UNIFORM_F32,
      I32 =     ffi.C.DN_GPU_UNIFORM_I32,
      Texture = ffi.C.DN_GPU_UNIFORM_TEXTURE,
      Enum =    ffi.C.DN_GPU_UNIFORM_ENUM,
    }
  )


  GpuBufferUsage = dn.enum.define(
    'GpuBufferUsage',
    {
      Static =  ffi.C.DN_GPU_BUFFER_USAGE_STATIC,
      Dynamic = ffi.C.DN_GPU_BUFFER_USAGE_DYNAMIC,
      Stream =  ffi.C.DN_GPU_BUFFER_USAGE_STREAM,
    }
  )

  GpuBufferKind = dn.enum.define(
    'GpuBufferKind',
    {
      Storage = ffi.C.DN_GPU_BUFFER_KIND_STORAGE,
      Array = ffi.C.DN_GPU_BUFFER_KIND_ARRAY,
    }
  )

  UniformKind = dn.enum.define(
    'UniformKind',
    {
      Matrix4        = ffi.C.DN_GPU_UNIFORM_MATRIX4,
      Matrix3        = ffi.C.DN_GPU_UNIFORM_MATRIX3,
      Matrix2        = ffi.C.DN_GPU_UNIFORM_MATRIX2,
      Vector4        = ffi.C.DN_GPU_UNIFORM_VECTOR4,
      Vector3        = ffi.C.DN_GPU_UNIFORM_VECTOR3,
      Vector2        = ffi.C.DN_GPU_UNIFORM_VECTOR2,
      F32            = ffi.C.DN_GPU_UNIFORM_F32,
      I32            = ffi.C.DN_GPU_UNIFORM_I32,
      Texture        = ffi.C.DN_GPU_UNIFORM_TEXTURE,
      Enum = ffi.C.DN_GPU_UNIFORM_ENUM,
    }
  )

  GpuMemoryBarrier = dn.enum.define(
    'GpuMemoryBarrier',
    {
      ShaderStorage = ffi.C.DN_GPU_MEMORY_BARRIER_STORAGE,
      BufferUpdate = ffi.C.DN_GPU_MEMORY_BARRIER_BUFFER_UPDATE,
    }
  )

  DisplayMode = dn.enum.define(
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

  GpuLoadOp = dn.enum.define(
    'GpuLoadOp',
    {
      None = ffi.C.DN_GPU_LOAD_OP_NONE,
      Clear = ffi.C.DN_GPU_LOAD_OP_CLEAR,
    }
  )

  imgui.internal.init_c_api()
  imgui.internal.init_lua_api()
  imgui.internal.init_lua_api_overwrites()
end


-- ██████╗ ███████╗███████╗██╗     ███████╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗
-- ██╔══██╗██╔════╝██╔════╝██║     ██╔════╝██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║
-- ██████╔╝█████╗  █████╗  ██║     █████╗  ██║        ██║   ██║██║   ██║██╔██╗ ██║
-- ██╔══██╗██╔══╝  ██╔══╝  ██║     ██╔══╝  ██║        ██║   ██║██║   ██║██║╚██╗██║
-- ██║  ██║███████╗██║     ███████╗███████╗╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║
-- ╚═╝  ╚═╝╚══════╝╚═╝     ╚══════╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
function dn.reflection.is_nil(cdata)
  return cdata == nil
end

function dn.reflection.field_ptr(cdata, member)
  local inner_type = dn.reflection.inner_type(member)
  local type_name = dn.reflection.type_name(inner_type)
  local byte_ptr = ffi.cast('u8*', cdata)
  return ffi.cast(type_name .. '*', byte_ptr + member.offset)
end

function dn.reflection.type_name(inner_type)
  local ctype = dn.enums.ctype

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

function dn.reflection.imgui_datatype(inner_type)
  local ctype = dn.enums.ctype

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

function dn.reflection.imgui_datatypeof(cdata)
  return dn.reflection.imgui_datatype(dn.reflection.inner_typeof(cdata))
end

function dn.reflection.pretty_type(type_info)
  local ctype = dn.enums.ctype

  local inner_type = dn.reflection.inner_type(type_info)

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

function dn.reflection.pretty_typeof(cdata)
  return dn.reflection.pretty_type(dn.reflection.typeof(cdata))
end

function dn.reflection.pretty_ptr(type_info)
  return string.format('%s*', dn.reflection.pretty_type(type_info))
end

function dn.reflection.pretty_ptrof(cdata)
  return string.format('%s*', dn.reflection.pretty_type(dn.reflection.typeof(cdata)))
end

function dn.reflection.address_of(cdata)
  -- Not for actual addressing; just for printing
  local s = tostring(cdata)
  local parts = s:split(':')
  return parts[2]:gsub(' ', '')
end

function dn.reflection.is_composite_type(type_info)
  return
    dn.enums.ctype.struct:match(type_info.what) or
    dn.enums.ctype.array:match(type_info.what)
end

function dn.reflection.sorted_members(type_info)
  local members = {}
  for member in type_info:members() do
    table.insert(members, member)
  end

  table.sort(members, function (a, b)
    local A_FIRST = true
    local B_FIRST = false

    local is_a_composite = dn.reflection.is_composite_type(a)
    local is_b_composite = dn.reflection.is_composite_type(b)

    if is_a_composite and not is_b_composite then
      return A_FIRST
    elseif not is_a_composite and is_b_composite then
      return B_FIRST
    else
      return a.name < b.name
    end
  end)

  return dn.iterator.values(members)
end

function dn.reflection.typeof(cdata)
  return reflect.typeof(cdata)
end

function dn.reflection.inner_typeof(cdata)
  return dn.reflection.inner_type(reflect.typeof(cdata))
end

function dn.reflection.inner_type(type_info)
  local ctype = dn.enums.ctype

  if ctype.ref:match(type_info.what) then
    type_info = type_info.element_type
  elseif ctype.ptr:match(type_info.what) then
    type_info = type_info.element_type
  elseif ctype.field:match(type_info.what) then
    type_info = dn.reflection.inner_type(type_info.type)
  elseif ctype.enum:match(type_info.what) then
    type_info = type_info.type
  end

  return type_info
end

function dn.reflection.is_opaque(cdata)
  return dn.reflection.inner_typeof(cdata).size == 'none'
end

function dn.reflection.ptr_type(ctype)
  return string.format('%s *', ctype)
end



-- ███╗   ███╗███████╗████████╗ █████╗ ████████╗██╗   ██╗██████╗ ███████╗███████╗
-- ████╗ ████║██╔════╝╚══██╔══╝██╔══██╗╚══██╔══╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔════╝
-- ██╔████╔██║█████╗     ██║   ███████║   ██║    ╚████╔╝ ██████╔╝█████╗  ███████╗
-- ██║╚██╔╝██║██╔══╝     ██║   ██╔══██║   ██║     ╚██╔╝  ██╔═══╝ ██╔══╝  ╚════██║
-- ██║ ╚═╝ ██║███████╗   ██║   ██║  ██║   ██║      ██║   ██║     ███████╗███████║
-- ╚═╝     ╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚═╝     ╚══════╝╚══════╝

------------
-- STRING --
------------
String = dn.class.metatype('dn_string_t')
function String:init(path)
  if not path then dbg() end
  self.data = ffi.cast('u8*', path)
  self.len = #path
end

function String:to_interned()
  return ffi.string(self.data, self.len)
end


----------------------
-- MEMORY ALLOCATOR --
----------------------
dn_allocator_t = dn.class.metatype('dn_allocator_t')

function dn_allocator_t:find(name)
  return dn.ffi.allocator_find(name)
end

function dn_allocator_t:add(name)
  return dn.ffi.allocator_add(self, name)
end

function dn_allocator_t:alloc(size)
  return dn.ffi.allocator_alloc(self, size)
end

function dn_allocator_t:free(pointer)
  return dn.ffi.allocator_free(self, pointer)
end

function dn_allocator_t:alloc_array(ctype, n)
  return ffi.cast(
    dn.reflection.ptr_type(ctype), 
    dn.ffi.allocator_alloc(self, ffi.sizeof(ctype) * n)
  )
end


------------
-- MATRIX --
------------
Matrix3 = dn.class.metatype('dn_matrix3_t')
Matrix3:set_metamethod('__index', function(self, key)
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

Matrix4 = dn.class.metatype('dn_matrix4_t')

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
Vector2 = dn.class.metatype('dn_vector2_t')
function Vector2:init(x, y)
  self.x = x or self.x
  self.y = y or self.y
end

function Vector2:scale(scalar)
  return Vector2:new(self.x * scalar, self.y * scalar)
end

function Vector2:copy()
  return Vector2:new(self.x, self.y)
end

Vector3 = dn.class.metatype('dn_vector3_t')
function  Vector3:init(x, y, z)
  self.x = x or self.x
  self.y = y or self.y
  self.z = z or self.z
end


Vector4 = dn.class.metatype('dn_vector4_t')
function Vector4:init(x, y, z, w)
  self.x = x or self.x
  self.y = y or self.y
  self.z = z or self.z
  self.w = w or self.w
end


---------------
-- CPU BUFFER --
---------------
CpuBuffer = dn.class.define('CpuBuffer')

function CpuBuffer:init(ctype, capacity)
  self.size = 0
  self.capacity = capacity
  self.ctype = ctype
  self.data = ffi.new(string.format('%s [%d]', ctype, capacity))
end

function CpuBuffer:push(element)
  -- dn.debug.assert(self.size < self.capacity)

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

function dn.class.define_fast(name)

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


-----------------------
-- BACKED GPU BUFFER --
-----------------------
BackedGpuBuffer = dn.class.define('BackedGpuBuffer')
function BackedGpuBuffer:init(ctype, capacity, gpu_buffer)
  self.ctype = ctype
  self.cpu_buffer = CpuBuffer:new(ctype, capacity)
  self.gpu_buffer = GpuBuffer:new(ctype, capacity, gpu_buffer)
end

function BackedGpuBuffer:owned(ctype, capacity, gpu_buffer_descriptor)
  gpu_buffer_descriptor.size = ffi.sizeof(ctype) * capacity
  return BackedGpuBuffer:new(ctype, capacity, dn.ffi.gpu_buffer_create(gpu_buffer_descriptor))
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
  dn.ffi.gpu_buffer_sync_subdata(
    self.gpu_buffer:to_ctype(), self.cpu_buffer.data,
    ffi.sizeof(self.ctype) * self.cpu_buffer.size,
    0)
end


----------------
-- GPU BUFFER --
----------------
GpuBuffer = dn.class.define('GpuBuffer')

function GpuBuffer:init(ctype, capacity, gpu_buffer)
  self.ctype = ctype
  self.capacity = capacity
  self.buffer = gpu_buffer or dn.ffi.gpu_buffer_create(GpuBufferDescriptor:new({
    kind = GpuBufferKind.Storage,
    usage = GpuBufferUsage.Static,
    size = ffi.sizeof(ctype) * capacity
  }))
end

function GpuBuffer:to_ctype()
  return self.buffer
end

function GpuBuffer:zero()
  dn.ffi.gpu_buffer_zero(self.buffer, self.capacity * ffi.sizeof(self.ctype))
end

function GpuBuffer:bind_base(base)
  dn.ffi.gpu_buffer_bind_base(self.buffer, base)
end


---------------------
-- GPU DESCRIPTORS --
---------------------
GpuShaderDescriptor = dn.class.metatype('dn_gpu_shader_descriptor_t')
function GpuShaderDescriptor:init(params)
  if dn.enum.is_enum(params.name) then
    self.name = params.name:to_qualified_string()
  else
    self.name = params.name
  end

  if GpuShaderKind.Graphics:match(params.kind) then
    self.kind = GpuShaderKind.Graphics:to_number()
    self.vertex_shader = params.vertex_shader
    self.fragment_shader = params.fragment_shader
  elseif GpuShaderKind.Compute:match(params.kind) then
    self.kind = GpuShaderKind.Compute:to_number()
    self.compute_shader = params.compute_shader
  end
end

GpuRenderTargetDescriptor = dn.class.metatype('dn_gpu_render_target_descriptor_t')
function GpuRenderTargetDescriptor:init(params)
  if dn.enum.is_enum(params.name) then
    self.name = params.name:to_qualified_string()
  else
    self.name = params.name
  end

  self.size = params.size
end

GpuBufferDescriptor = dn.class.metatype('dn_gpu_buffer_descriptor_t')
function GpuBufferDescriptor:init(params)
  self.kind = dn.enum.load(params.kind):to_number()
  self.usage = dn.enum.load(params.usage):to_number()
  self.capacity = params.size
  self.element_size = 1
end

GpuUniformDescriptor = dn.class.metatype('dn_gpu_uniform_descriptor_t')
function GpuUniformDescriptor:init(params)
  self.name = params.name
  self.kind = dn.enum.load(params.kind):to_number()
end

GpuCommandBufferDescriptor = dn.class.metatype('dn_gpu_command_buffer_descriptor_t')
function GpuCommandBufferDescriptor:init(params)
  self.max_commands = params.max_commands
end

GpuVertexAttribute = dn.class.metatype('dn_gpu_vertex_attribute_t')
function GpuVertexAttribute:init(params)
  self.kind = dn.enum.load(params.kind):to_number()
  self.count = params.count
  self.divisor = params.divisor or 0
end

GpuBufferLayout = dn.class.metatype('dn_gpu_buffer_layout_t')
function GpuBufferLayout:init(params)
  local allocator = dn.ffi.allocator_find('standard')

  self.num_vertex_attributes = #params.vertex_attributes
  self.vertex_attributes = allocator:alloc_array('dn_gpu_vertex_attribute_t', self.num_vertex_attributes)
  for i = 1, self.num_vertex_attributes, 1 do
    self.vertex_attributes[i - 1] = GpuVertexAttribute:new(params.vertex_attributes[i])
  end
end

GpuRasterState = dn.class.metatype('dn_gpu_raster_state_t')
function GpuRasterState:init(params)
  self.shader = dn.gpu.find(params.shader)
  self.primitive = dn.enum.load(params.primitive):to_number()
end

GpuPipelineDescriptor = dn.class.metatype('dn_gpu_pipeline_descriptor_t')
function GpuPipelineDescriptor:init(params)
  local allocator = dn.ffi.allocator_find('standard')

  self.raster = dn_gpu_raster_state_t:new(params.raster)

  self.num_buffer_layouts = #params.buffer_layouts
  self.buffer_layouts = allocator:alloc_array('dn_gpu_buffer_layout_t', self.num_buffer_layouts)
  for i = 1, self.num_buffer_layouts do
    self.buffer_layouts[i - 1] = GpuBufferLayout:new(params.buffer_layouts[i])
  end
end

GpuRenderPass = dn.class.metatype('dn_gpu_render_pass_t')
function GpuRenderPass:init(params)
  if params.color then
    params.color.attachment = dn.enum.load(params.color.attachment)
    self.color.attachment = dn.asset.find(params.color.attachment)
    self.color.load = dn.enum.load(params.color.load):to_number()

    dn.debug.assert(not dn.reflection.is_nil(self.color.attachment))
  end
end

GpuUniformBinding = dn.class.metatype('dn_gpu_uniform_binding_t')
function GpuUniformBinding:init(params)
  self.uniform = params.uniform
  self.binding_index = params.binding_index or 0

  if     GpuUniformKind.Matrix4:match(self.uniform.kind) then self.data.mat4 = params.value
  elseif GpuUniformKind.Matrix3:match(self.uniform.kind) then self.data.mat3 = params.value
  elseif GpuUniformKind.Matrix2:match(self.uniform.kind) then self.data.mat2 = params.value
  elseif GpuUniformKind.Vector4:match(self.uniform.kind) then self.data.vec4 = params.value
  elseif GpuUniformKind.Vector3:match(self.uniform.kind) then self.data.vec3 = params.value
  elseif GpuUniformKind.Vector2:match(self.uniform.kind) then self.data.vec2 = params.value
  elseif GpuUniformKind.F32:match(self.uniform.kind)     then self.data.f32 = params.value
  elseif GpuUniformKind.I32:match(self.uniform.kind)     then self.data.i32 = params.value
  elseif GpuUniformKind.Enum:match(self.uniform.kind)    then self.data.i32 = dn.enum.load(params.value):to_number()
  elseif GpuUniformKind.Texture:match(self.uniform.kind) then self.data.texture = params.value
  else   dn.debug.assert(false) end
end

GpuVertexBufferBinding = dn.class.metatype('dn_gpu_vertex_buffer_binding_t')
function GpuVertexBufferBinding:init(buffer)
  if type(buffer) == 'cdata' then
    self.buffer = buffer
  else
    self.buffer = dn.gpu.find(buffer)
  end
end

GpuStorageBufferBinding = dn.class.metatype('dn_gpu_storage_buffer_binding_t')
function GpuStorageBufferBinding:init(params)
  if type(params.buffer) == 'cdata' then
    self.buffer = params.buffer
  else
    self.buffer = dn.gpu.find(params.buffer)
  end

  self.base = params.base
end

GpuBufferBinding = dn.class.metatype('dn_gpu_buffer_binding_t')
function GpuBufferBinding:init(params)
  local allocator = dn.ffi.allocator_find('standard')

  if params.vertex then
    self.vertex.count = #params.vertex
    self.vertex.bindings = allocator:alloc_array('dn_gpu_vertex_buffer_binding_t', self.vertex.count)
    for i = 1, self.vertex.count do
      self.vertex.bindings[i - 1] = GpuVertexBufferBinding:new(params.vertex[i])
    end
  end

  if params.uniforms then
    self.uniforms.count = #params.uniforms
    self.uniforms.bindings = allocator:alloc_array('dn_gpu_uniform_binding_t', self.uniforms.count)
    for i = 1, self.uniforms.count do
      self.uniforms.bindings[i - 1] = GpuUniformBinding:new(params.uniforms[i])
    end
  end

  if params.storage then
    self.storage.count = #params.storage
    self.storage.bindings = allocator:alloc_array('dn_gpu_storage_binding_t', self.storage.count)
    for i = 1, self.storage.count do
      self.storage.bindings[i - 1] = GpuStorageBufferBinding:new(params.storage[i])
    end
  end
end


-- ███████╗███████╗██╗    ██╗    ██╗██████╗  █████╗ ██████╗ ██████╗ ███████╗██████╗ ███████╗
-- ██╔════╝██╔════╝██║    ██║    ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝
-- █████╗  █████╗  ██║    ██║ █╗ ██║██████╔╝███████║██████╔╝██████╔╝█████╗  ██████╔╝███████╗
-- ██╔══╝  ██╔══╝  ██║    ██║███╗██║██╔══██╗██╔══██║██╔═══╝ ██╔═══╝ ██╔══╝  ██╔══██╗╚════██║
-- ██║     ██║     ██║    ╚███╔███╔╝██║  ██║██║  ██║██║     ██║     ███████╗██║  ██║███████║
-- ╚═╝     ╚═╝     ╚═╝     ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝
function dn.ffi.log(fmt, ...)
  return ffi.C.dn_log_str(String:new(string.format(fmt, ...)))
end

local DN_COLOR_GREEN = string.char(27) .. "[34m"
local DN_COLOR_GRAY = string.char(27) .. "[90m"
local DN_COLOR_RED = string.char(27) .. "[91m"
local DN_COLOR_BLUE = string.char(27) .. "[94m"
local DN_COLOR_YELLOW = string.char(27) .. "[33m"
local DN_COLOR_RESET = string.char(27) .. "[0m"

function dn.ffi.warn(tag, fmt, ...)
  dn.ffi.log_colored(DN_COLOR_YELLOW, tag, fmt, ...)
end

function dn.ffi.info(tag, fmt, ...)
  dn.ffi.log_colored(DN_COLOR_GREEN, tag, fmt, ...)
end

function dn.ffi.trace(tag, fmt, ...)
  if not trace_enabled then return end
  dn.ffi.log_colored(DN_COLOR_GRAY, tag, fmt, ...)
end

function dn.ffi.log_colored(color, tag, fmt, ...)
  if fmt then
    local message = string.format(fmt, ...)
    message = string.format('[%s%s%s] %s', color, tag, DN_COLOR_RESET, message)
    dn.ffi.log(message)
  else
    local message = string.format('[%s%s%s]', color, tag, DN_COLOR_RESET)
    dn.ffi.log(message)
  end
end

function dn.ffi.time_metrics_add(name)
    return ffi.C.dn_time_metrics_add(String:new(name))
end

function dn.ffi.paths_resolve(name)
  return ffi.C.dn_paths_resolve(String:new(name)):to_interned()
end

function dn.ffi.paths_resolve_format(name, file_name)
  return ffi.C.dn_paths_resolve_format(String:new(name), String:new(file_name)):to_interned()
end

function dn.ffi.paths_add_subpath(name, parent_name, relative_path)
  return ffi.C.dn_paths_add_subpath(String:new(name), String:new(parent_name), String:new(relative_path))
end

function dn.ffi.paths_add_ex(name, absolute_path)
  return ffi.C.dn_paths_add_ex(String:new(name), String:new(absolute_path))
end

function dn.ffi.paths_is_registered(name)
  return ffi.C.dn_paths_is_registered(String:new(name))
end

function dn.ffi.os_scan_directory(path)
  return ffi.C.dn_os_scan_directory(String:new(path))
end

function dn.ffi.os_scan_directory_recursive(path)
  return ffi.C.dn_os_scan_directory_recursive(String:new(path))
end

function dn.ffi.os_does_path_exist(path)
  return ffi.C.dn_os_does_path_exist(String:new(path))
end

function dn.ffi.os_is_regular_file(path)
  return ffi.C.dn_os_is_regular_file(String:new(path))
end

function dn.ffi.os_is_directory(path)
  return ffi.C.dn_os_is_directory(String:new(path))
end

function dn.ffi.os_create_directory(path)
  return ffi.C.dn_os_create_directory(String:new(path))
end

function dn.ffi.os_remove_directory(path)
  return ffi.C.dn_os_remove_directory(String:new(path))
end

function dn.ffi.os_create_file(path)
  return ffi.C.dn_os_create_file(String:new(path))
end

function dn.ffi.os_remove_file(path)
  return ffi.C.dn_os_remove_file(String:new(path))
end

function dn.ffi.os_file_mod_time(path)
  return ffi.C.dn_os_file_mod_time(String:new(path))
end

function dn.ffi.window_get_display_mode()
  return dn.enums.DisplayMode(ffi.C.dn_window_get_display_mode())
end

function dn.ffi.asset_registry_find(name)
  return ffi.C.dn_asset_registry_find(String:new(name))
end

function dn.ffi.set_camera()
end

function dn.ffi.push_fullscreen_quad()
  local n = ffi.C.dn_window_get_native_resolution()
  local uvs = nil
  local opacity = 1.0
  ffi.C.push_quad(0, n.y, n.x, n.y, uvs, opacity);
end

function dn.ffi.gpu_begin_render_pass(command_buffer, render_pass)
  dn.ffi.trace('dn.ffi.gpu.begin_render_pass')
  ffi.C.dn_gpu_begin_render_pass(command_buffer, render_pass)
end
function doublenickel.ffi.namespaced_metatype(namespace, struct_name)
  ffi.metatype(struct_name, {
    __index = function(struct_instance, fn_name)
      return ffi.C[namespace .. '_' .. fn_name]
    end
  })
end

function doublenickel.ffi.namespace(prefix)
  local namespace = {}
  setmetatable(namespace, {
    __index = function(__namespace, fn_name)
      if fn_name == 'os_does_path_exist' then
        for key in doublenickel.iterator.keys(dn) do
          print(key)
        end
      end
      print(rawget(dn, fn_name))
      print(fn_name)
      return ffi.C[prefix .. '_' .. fn_name]
    end
  })
  return namespace
end


function doublenickel.ffi.init()
  setmetatable(
    doublenickel.ffi,
    {
      __index = function(self, key)
        local wrapper = rawget(self, key)
        if wrapper then return wrapper end

        local found, value = pcall(function() return ffi.C[key] end)
        if found then return value end
      end
    }
  )

  dn = doublenickel.ffi.namespace('dn')

  local string_metatable = {
    __index = {
      to_interned = function(self)
        if doublenickel.ffi.is_nil(self.data) then dbg() end
        return ffi.string(self.data)
      end,
    }
  }
  ffi.metatype('dn_tstring_t', string_metatable)
  ffi.metatype('string', string_metatable)

  Sdf = doublenickel.enum.define(
    'Sdf',
    {
      Circle = doublenickel.ffi.DN_SDF_SHAPE_CIRCLE,
      Ring = doublenickel.ffi.DN_SDF_SHAPE_RING,
      Box = doublenickel.ffi.DN_SDF_SHAPE_BOX,
      OrientedBox = doublenickel.ffi.DN_SDF_SHAPE_ORIENTED_BOX,
      Combine = doublenickel.ffi.DN_SDF_SHAPE_COMBINE
    }
  )

  SdfCombineOp = doublenickel.enum.define(
    'SdfCombineOp',
    {
      Union = doublenickel.ffi.DN_SDF_COMBINE_OP_UNION,
      Intersection = doublenickel.ffi.DN_SDF_COMBINE_OP_INTERSECTION,
      Subtraction = doublenickel.ffi.DN_SDF_COMBINE_OP_SUBTRACTION,
    }
  )

  SdfSmoothingKernel = doublenickel.enum.define(
    'SdfSmoothingKernel',
    {
      None = doublenickel.ffi.DN_SDF_SMOOTH_KERNEL_NONE,
      PolynomialQuadratic = doublenickel.ffi.DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC,
    }
  )


  CoordinateSystem = doublenickel.enum.define(
    'CoordinateSystem',
    {
      World = doublenickel.ffi.DN_COORD_UNIT_WORLD,
      Screen = doublenickel.ffi.DN_COORD_UNIT_SCREEN,
      Window = doublenickel.ffi.DN_COORD_UNIT_WINDOW,
      Game = doublenickel.ffi.DN_COORD_UNIT_GAME,
    }
  )

  InputDevice = doublenickel.enum.define(
    'InputDevice',
    {
      MouseAndKeyboard = 0,
      Controller = 1
    }
  )

  ParticleKind = doublenickel.enum.define(
    'ParticleKind',
    {
      Quad = doublenickel.ffi.ParticleKind_Quad,
      Circle = doublenickel.ffi.ParticleKind_Circle,
      Image = doublenickel.ffi.ParticleKind_Image,
      Invalid = doublenickel.ffi.ParticleKind_Invalid,
    }
  )

  WindowFlags = doublenickel.enum.define(
    'WindowFlags',
    {
      None = doublenickel.ffi.DN_WINDOW_FLAG_NONE,
      Windowed = doublenickel.ffi.DN_WINDOW_FLAG_WINDOWED,
      Border = doublenickel.ffi.DN_WINDOW_FLAG_BORDER,
      Vsync = doublenickel.ffi.DN_WINDOW_FLAG_VSYNC,
    }
  )

  GpuResourceId = doublenickel.enum.define(
    'GpuResourceId',
    {
      Framebuffer = doublenickel.ffi.DN_GPU_RESOURCE_FRAMEBUFFER,
      Shader = doublenickel.ffi.DN_GPU_RESOURCE_SHADER,
      Program = doublenickel.ffi.DN_GPU_RESOURCE_PROGRAM,
    }
  )

  GpuShaderKind = doublenickel.enum.define(
    'GpuShaderKind',
    {
      Graphics = doublenickel.ffi.DN_GPU_SHADER_GRAPHICS,
      Compute = doublenickel.ffi.DN_GPU_SHADER_COMPUTE,
    }
  )

  GpuDrawMode = doublenickel.enum.define(
    'GpuDrawMode',
    {
      Arrays = doublenickel.ffi.DN_GPU_DRAW_MODE_ARRAYS,
      Instance = doublenickel.ffi.DN_GPU_DRAW_MODE_INSTANCE,
    }
  )

  GpuDrawPrimitive = doublenickel.enum.define(
    'GpuDrawPrimitive',
    {
      Triangles = doublenickel.ffi.DN_GPU_PRIMITIVE_TRIANGLES,
    }
  )

  GpuVertexAttributeKind = doublenickel.enum.define(
    'GpuVertexAttributeKind',
    {
      Float = doublenickel.ffi.DN_GPU_VERTEX_ATTRIBUTE_FLOAT,
      U32 = doublenickel.ffi.DN_GPU_VERTEX_ATTRIBUTE_U32,
    }
  )

  GpuBlendFunction = doublenickel.enum.define(
    'GpuBlendFunction',
    {
      Add = doublenickel.ffi.DN_GPU_BLEND_FUNC_ADD,
      Subtract = doublenickel.ffi.DN_GPU_BLEND_FUNC_SUBTRACT,
      ReverseSubtract = doublenickel.ffi.DN_GPU_BLEND_FUNC_REVERSE_SUBTRACT,
      Min = doublenickel.ffi.DN_GPU_BLEND_FUNC_MIN,
      Max = doublenickel.ffi.DN_GPU_BLEND_FUNC_MAX,

    }
  )

  GpuUniformKind = doublenickel.enum.define(
    'GpuUniformKind',
    {
      None =    doublenickel.ffi.DN_GPU_UNIFORM_NONE,
      Matrix4 = doublenickel.ffi.DN_GPU_UNIFORM_MATRIX4,
      Matrix3 = doublenickel.ffi.DN_GPU_UNIFORM_MATRIX3,
      Matrix2 = doublenickel.ffi.DN_GPU_UNIFORM_MATRIX2,
      Vector4 = doublenickel.ffi.DN_GPU_UNIFORM_VECTOR4,
      Vector3 = doublenickel.ffi.DN_GPU_UNIFORM_VECTOR3,
      Vector2 = doublenickel.ffi.DN_GPU_UNIFORM_VECTOR2,
      F32 =     doublenickel.ffi.DN_GPU_UNIFORM_F32,
      I32 =     doublenickel.ffi.DN_GPU_UNIFORM_I32,
      Texture = doublenickel.ffi.DN_GPU_UNIFORM_TEXTURE,
      Enum =    doublenickel.ffi.DN_GPU_UNIFORM_ENUM,
    }
  )


  GpuBufferUsage = doublenickel.enum.define(
    'GpuBufferUsage',
    {
      Static =  doublenickel.ffi.DN_GPU_BUFFER_USAGE_STATIC,
      Dynamic = doublenickel.ffi.DN_GPU_BUFFER_USAGE_DYNAMIC,
      Stream =  doublenickel.ffi.DN_GPU_BUFFER_USAGE_STREAM,
    }
  )

  GpuBufferKind = doublenickel.enum.define(
    'GpuBufferKind',
    {
      Storage = doublenickel.ffi.DN_GPU_BUFFER_KIND_STORAGE,
      Array = doublenickel.ffi.DN_GPU_BUFFER_KIND_ARRAY,
    }
  )

  UniformKind = doublenickel.enum.define(
    'UniformKind',
    {
      Matrix4        = doublenickel.ffi.DN_GPU_UNIFORM_MATRIX4,
      Matrix3        = doublenickel.ffi.DN_GPU_UNIFORM_MATRIX3,
      Matrix2        = doublenickel.ffi.DN_GPU_UNIFORM_MATRIX2,
      Vector4        = doublenickel.ffi.DN_GPU_UNIFORM_VECTOR4,
      Vector3        = doublenickel.ffi.DN_GPU_UNIFORM_VECTOR3,
      Vector2        = doublenickel.ffi.DN_GPU_UNIFORM_VECTOR2,
      F32            = doublenickel.ffi.DN_GPU_UNIFORM_F32,
      I32            = doublenickel.ffi.DN_GPU_UNIFORM_I32,
      Texture        = doublenickel.ffi.DN_GPU_UNIFORM_TEXTURE,
      Enum = doublenickel.ffi.DN_GPU_UNIFORM_ENUM,
    }
  )

  GpuMemoryBarrier = doublenickel.enum.define(
    'GpuMemoryBarrier',
    {
      ShaderStorage = doublenickel.ffi.DN_GPU_MEMORY_BARRIER_STORAGE,
      BufferUpdate = doublenickel.ffi.DN_GPU_MEMORY_BARRIER_BUFFER_UPDATE,
    }
  )

  DisplayMode = doublenickel.enum.define(
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

  GpuLoadOp = doublenickel.enum.define(
    'GpuLoadOp',
    {
      None = doublenickel.ffi.DN_GPU_LOAD_OP_NONE,
      Clear = doublenickel.ffi.DN_GPU_LOAD_OP_CLEAR,
    }
  )

  imgui.internal.init_c_api()
  imgui.internal.init_lua_api()
  imgui.internal.init_lua_api_overwrites()
end


----------------
-- REFLECTION --
----------------
doublenickel.enum.define(
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

function doublenickel.ffi.is_nil(cdata)
  return cdata == nil
end

function doublenickel.ffi.field_ptr(cdata, member)
  local inner_type = doublenickel.ffi.inner_type(member)
  local type_name = doublenickel.ffi.type_name(inner_type)
  local byte_ptr = ffi.cast('u8*', cdata)
  return ffi.cast(type_name .. '*', byte_ptr + member.offset)
end

function doublenickel.ffi.type_name(inner_type)
  local ctype = doublenickel.enums.ctype

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

function doublenickel.ffi.imgui_datatype(inner_type)
  local ctype = doublenickel.enums.ctype

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

function doublenickel.ffi.imgui_datatypeof(cdata)
  return doublenickel.ffi.imgui_datatype(doublenickel.ffi.inner_typeof(cdata))
end

function doublenickel.ffi.pretty_type(type_info)
  local ctype = doublenickel.enums.ctype

  local inner_type = doublenickel.ffi.inner_type(type_info)

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

function doublenickel.ffi.pretty_typeof(cdata)
  return doublenickel.ffi.pretty_type(doublenickel.ffi.typeof(cdata))
end

function doublenickel.ffi.pretty_ptr(type_info)
  return string.format('%s*', doublenickel.ffi.pretty_type(type_info))
end

function doublenickel.ffi.pretty_ptrof(cdata)
  return string.format('%s*', doublenickel.ffi.pretty_type(doublenickel.ffi.typeof(cdata)))
end

function doublenickel.ffi.address_of(cdata)
  -- Not for actual addressing; just for printing
  local s = tostring(cdata)
  local parts = s:split(':')
  return parts[2]:gsub(' ', '')
end

function doublenickel.ffi.is_composite_type(type_info)
  return
    doublenickel.enums.ctype.struct:match(type_info.what) or
    doublenickel.enums.ctype.array:match(type_info.what)
end

function doublenickel.ffi.sorted_members(type_info)
  local members = {}
  for member in type_info:members() do
    table.insert(members, member)
  end

  table.sort(members, function (a, b)
    local A_FIRST = true
    local B_FIRST = false

    local is_a_composite = doublenickel.ffi.is_composite_type(a)
    local is_b_composite = doublenickel.ffi.is_composite_type(b)

    if is_a_composite and not is_b_composite then
      return A_FIRST
    elseif not is_a_composite and is_b_composite then
      return B_FIRST
    else
      return a.name < b.name
    end
  end)

  return doublenickel.iterator.values(members)
end

function doublenickel.ffi.typeof(cdata)
  return reflect.typeof(cdata)
end

function doublenickel.ffi.inner_typeof(cdata)
  return doublenickel.ffi.inner_type(reflect.typeof(cdata))
end

function doublenickel.ffi.inner_type(type_info)
  local ctype = doublenickel.enums.ctype

  if ctype.ref:match(type_info.what) then
    type_info = type_info.element_type
  elseif ctype.ptr:match(type_info.what) then
    type_info = type_info.element_type
  elseif ctype.field:match(type_info.what) then
    type_info = doublenickel.ffi.inner_type(type_info.type)
  elseif ctype.enum:match(type_info.what) then
    type_info = type_info.type
  end

  return type_info
end

function doublenickel.ffi.is_opaque(cdata)
  return doublenickel.ffi.inner_typeof(cdata).size == 'none'
end

function doublenickel.ffi.ptr_type(ctype)
  return string.format('%s *', ctype)
end



---------------
-- METATYPES -- 
---------------

------------
-- STRING --
------------

dn.String = doublenickel.class.metatype('dn_string_t')
function dn.String:init(path)
  self.data = ffi.cast('u8*', path)
  self.len = #path
end

----------------------
-- MEMORY ALLOCATOR --
----------------------
dn_allocator_t = doublenickel.class.metatype('dn_allocator_t')

function dn_allocator_t:find(name)
  return dn.allocator_find(name)
end

function dn_allocator_t:add(name)
  return dn.allocator_add(self, name)
end

function dn_allocator_t:alloc(size)
  return dn.allocator_alloc(self, size)
end

function dn_allocator_t:free(pointer)
  return dn.allocator_free(self, pointer)
end

function dn_allocator_t:alloc_array(ctype, n)
  return ffi.cast(
    doublenickel.ffi.ptr_type(ctype), 
    dn.allocator_alloc(self, ffi.sizeof(ctype) * n)
  )
end


------------
-- MATRIX --
------------
Matrix3 = doublenickel.class.metatype('dn_matrix3_t')
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


Matrix4 = doublenickel.class.metatype('dn_matrix4_t')

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
Vector2 = doublenickel.class.metatype('dn_vector2_t')
function Vector2:init(x, y)
  self.x = x or self.x
  self.y = y or self.y
end

function Vector2:scale(scalar)
  return Vector2:new(self.x * scalar, self.y * scalar)
end

Vector3 = doublenickel.class.metatype('dn_vector3_t')
function  Vector3:init(x, y, z)
  self.x = x or self.x
  self.y = y or self.y
  self.z = z or self.z
end


Vector4 = doublenickel.class.metatype('dn_vector4_t')
function Vector4:init(x, y, z, w)
  self.x = x or self.x
  self.y = y or self.y
  self.z = z or self.z
  self.w = w or self.w
end

Vertex = doublenickel.class.metatype('Vertex')
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



CpuBuffer = doublenickel.class.define('CpuBuffer')

function CpuBuffer:init(ctype, capacity)
  self.size = 0
  self.capacity = capacity
  self.ctype = ctype
  self.data = ffi.new(string.format('%s [%d]', ctype, capacity))
end

function CpuBuffer:push(element)
  -- doublenickel.debug.assert(self.size < self.capacity)

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

function doublenickel.class.define_fast(name)

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

BackedGpuBuffer = doublenickel.class.define('BackedGpuBuffer')
function BackedGpuBuffer:init(ctype, capacity, gpu_buffer)
  self.ctype = ctype
  self.cpu_buffer = CpuBuffer:new(ctype, capacity)
  self.gpu_buffer = GpuBuffer:new(ctype, capacity, gpu_buffer)
end

function BackedGpuBuffer:owned(ctype, capacity, gpu_buffer_descriptor)
  gpu_buffer_descriptor.size = ffi.sizeof(ctype) * capacity
  return BackedGpuBuffer:new(ctype, capacity, doublenickel.ffi.gpu_buffer_create(gpu_buffer_descriptor))
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
  doublenickel.ffi.gpu_buffer_sync_subdata(
    self.gpu_buffer:to_ctype(), self.cpu_buffer.data,
    ffi.sizeof(self.ctype) * self.cpu_buffer.size,
    0)
end



GpuBuffer = doublenickel.class.define('GpuBuffer')

function GpuBuffer:init(ctype, capacity, gpu_buffer)
  self.ctype = ctype
  self.capacity = capacity
  self.buffer = gpu_buffer or doublenickel.ffi.gpu_buffer_create(GpuBufferDescriptor:new({
    kind = GpuBufferKind.Storage,
    usage = GpuBufferUsage.Static,
    size = ffi.sizeof(ctype) * capacity
  }))
end

function GpuBuffer:to_ctype()
  return self.buffer
end

function GpuBuffer:zero()
  doublenickel.ffi.gpu_buffer_zero(self.buffer, self.capacity * ffi.sizeof(self.ctype))
end

function GpuBuffer:bind_base(base)
  doublenickel.ffi.gpu_buffer_bind_base(self.buffer, base)
end

---------------------
-- GPU DESCRIPTORS --
---------------------
GpuShaderDescriptor = doublenickel.class.metatype('dn_gpu_shader_descriptor_t')
function GpuShaderDescriptor:init(params)
  if doublenickel.enum.is_enum(params.name) then
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

GpuRenderTargetDescriptor = doublenickel.class.metatype('dn_gpu_render_target_descriptor_t')
function GpuRenderTargetDescriptor:init(params)
  if doublenickel.enum.is_enum(params.name) then
    self.name = params.name:to_qualified_string()
  else
    self.name = params.name
  end

  self.size = params.size
end

GpuBufferDescriptor = doublenickel.class.metatype('dn_gpu_buffer_descriptor_t')
function GpuBufferDescriptor:init(params)
  self.kind = doublenickel.enum.load(params.kind):to_number()
  self.usage = doublenickel.enum.load(params.usage):to_number()
  self.capacity = params.size
  self.element_size = 1
end

GpuUniformDescriptor = doublenickel.class.metatype('dn_gpu_uniform_descriptor_t')
function GpuUniformDescriptor:init(params)
  self.name = params.name
  self.kind = doublenickel.enum.load(params.kind):to_number()
end

GpuCommandBufferDescriptor = doublenickel.class.metatype('dn_gpu_command_buffer_descriptor_t')
function GpuCommandBufferDescriptor:init(params)
  self.max_commands = params.max_commands
end

GpuVertexAttribute = doublenickel.class.metatype('dn_gpu_vertex_attribute_t')
function GpuVertexAttribute:init(params)
  self.kind = doublenickel.enum.load(params.kind):to_number()
  self.count = params.count
  self.divisor = params.divisor or 0
end

GpuBufferLayout = doublenickel.class.metatype('dn_gpu_buffer_layout_t')
function GpuBufferLayout:init(params)
  local allocator = dn.allocator_find('standard')

  self.num_vertex_attributes = #params.vertex_attributes
  self.vertex_attributes = allocator:alloc_array('dn_gpu_vertex_attribute_t', self.num_vertex_attributes)
  for i = 1, self.num_vertex_attributes, 1 do
    self.vertex_attributes[i - 1] = GpuVertexAttribute:new(params.vertex_attributes[i])
  end
end

GpuRasterState = doublenickel.class.metatype('dn_gpu_raster_state_t')
function GpuRasterState:init(params)
  self.shader = doublenickel.gpu.find(params.shader)
  self.primitive = doublenickel.enum.load(params.primitive):to_number()
end

GpuPipelineDescriptor = doublenickel.class.metatype('dn_gpu_pipeline_descriptor_t')
function GpuPipelineDescriptor:init(params)
  local allocator = dn.allocator_find('standard')

  self.raster = dn_gpu_raster_state_t:new(params.raster)

  self.num_buffer_layouts = #params.buffer_layouts
  self.buffer_layouts = allocator:alloc_array('dn_gpu_buffer_layout_t', self.num_buffer_layouts)
  for i = 1, self.num_buffer_layouts do
    self.buffer_layouts[i - 1] = GpuBufferLayout:new(params.buffer_layouts[i])
  end
end

GpuRenderPass = doublenickel.class.metatype('dn_gpu_render_pass_t')
function GpuRenderPass:init(params)
  if params.color then
    params.color.attachment = doublenickel.enum.load(params.color.attachment)
    self.color.attachment = doublenickel.asset.find(params.color.attachment)
    self.color.load = doublenickel.enum.load(params.color.load):to_number()

    doublenickel.debug.assert(not doublenickel.ffi.is_nil(self.color.attachment))
  end

end

GpuUniformBinding = doublenickel.class.metatype('dn_gpu_uniform_binding_t')
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
  elseif GpuUniformKind.Enum:match(self.uniform.kind)    then self.data.i32 = doublenickel.enum.load(params.value):to_number()
  elseif GpuUniformKind.Texture:match(self.uniform.kind) then self.data.texture = params.value
  else   doublenickel.debug.assert(false) end
end

GpuVertexBufferBinding = doublenickel.class.metatype('dn_gpu_vertex_buffer_binding_t')
function GpuVertexBufferBinding:init(buffer)
  if type(buffer) == 'cdata' then
    self.buffer = buffer
  else
    self.buffer = doublenickel.gpu.find(buffer)
  end
end

GpuStorageBufferBinding = doublenickel.class.metatype('dn_gpu_storage_buffer_binding_t')
function GpuStorageBufferBinding:init(params)
  if type(params.buffer) == 'cdata' then
    self.buffer = params.buffer
  else
    self.buffer = doublenickel.gpu.find(params.buffer)
  end

  self.base = params.base
end

GpuBufferBinding = doublenickel.class.metatype('dn_gpu_buffer_binding_t')
function GpuBufferBinding:init(params)
  local allocator = dn.allocator_find('standard')

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


---------
-- APP --
---------
WindowConfig = doublenickel.class.metatype('dn_window_config_t')
function WindowConfig:init(params)
  local default = dn.window_config_default();
  self.title = params.title or default.title
  self.icon = params.icon or default.icon
  self.native_resolution = params.native_resolution or default.native_resolution
  self.flags = params.flags or default.flags
  self.display_mode = params.display_mode or default.display_mode
end

AudioConfig = doublenickel.class.metatype('dn_audio_config_t')
function AudioConfig:init(params)
  local allocator = dn.allocator_find('bump')

  self.num_dirs = #params.dirs
  self.dirs = allocator:alloc_array('dn_path_t', #params.dirs)
  for i = 0, #params.dirs - 1 do
    self.dirs[i] = params.dirs[i + 1]
  end
end

FontDescriptor = doublenickel.class.metatype('dn_font_descriptor_t')
function FontDescriptor:init(params)
  if doublenickel.enum.is_enum(params.id) then
    self.id = params.id:to_qualified_string()
  else
    self.id = params.id
  end

  self.file_path = params.file_path

  self.sizes = {0}
  for i in doublenickel.iterator.keys(params.sizes) do
    self.sizes[i - 1] = params.sizes[i]
  end
  self.flags = 0
  if params.default       then self.flags = bit.bor(self.flags, ffi.C.DN_FONT_FLAG_DEFAULT) end
  if params.imgui         then self.flags = bit.bor(self.flags, ffi.C.DN_FONT_FLAG_IMGUI) end
end

FontConfig = doublenickel.class.metatype('dn_font_config_t')
function FontConfig:init(params)
  local allocator = dn.allocator_find('bump')

  self.num_fonts = #params.fonts
  self.fonts = allocator:alloc_array('dn_font_descriptor_t', #params.fonts)
  for i = 0, #params.fonts - 1 do
    self.fonts[i] = FontDescriptor:new(params.fonts[i + 1])
  end
end

GpuConfig = doublenickel.class.metatype('dn_gpu_config_t')
function GpuConfig:init(params)
  local allocator = dn.allocator_find('bump')

  self.shader_path = params.shader_path or nil

  self.num_search_paths = params.search_paths and #params.search_paths or 0
  self.search_paths = allocator:alloc_array('const char*', self.num_search_paths)
  for i = 0, self.num_search_paths - 1 do
    self.search_paths[i] = params.search_paths[i + 1]
  end

  self.num_shaders = params.shaders and #params.shaders or 0
  self.shaders = allocator:alloc_array('dn_gpu_shader_descriptor_t', self.num_shaders)
  for i = 0, self.num_shaders - 1 do
    self.shaders[i] = GpuShaderDescriptor:new(params.shaders[i + 1])
  end

  self.num_render_targets = params.render_targets and #params.render_targets or 0
  self.render_targets = allocator:alloc_array('dn_gpu_render_target_descriptor_t', self.num_render_targets)
  for i = 0, self.num_render_targets - 1 do
    self.render_targets[i] = GpuRenderTargetDescriptor:new(params.render_targets[i + 1])
  end
end

SteamConfig = doublenickel.class.metatype('dn_steam_config_t')
function SteamConfig:init(params)
  self.app_id = params.app_id
end

AppConfig = doublenickel.class.metatype('dn_app_config_t')
function AppConfig:init(params)
  self.window = params.window or dn.window_config_default()
  self.audio = params.audio or dn.audio_config_default()
  self.font = params.font or dn.font_config_default()
  self.gpu = params.gpu or ffi.new('dn_gpu_config_t')
  self.asset = params.asset or ffi.new('dn_asset_config_t')
  self.steam = params.steam or ffi.new('dn_steam_config_t')
  self.target_fps = params.target_fps or 60
end


------------------
-- FFI WRAPPERS --
------------------
function dn.window_get_display_mode()
  return doublenickel.enums.DisplayMode(ffi.C.dn_window_get_display_mode())
end

function dn.os_scan_directory(path)
  return ffi.C.dn_os_scan_directory(dn.String:new(path))
end

function dn.os_scan_directory_recursive(path)
  return ffi.C.dn_os_scan_directory_recursive(dn.String:new(path))
end

function dn.os_does_path_exist(path)
  dbg()
  return ffi.C.dn_os_does_path_exist(dn.String:new(path))
end

function dn.os_is_regular_file(path)
  return ffi.C.dn_os_is_regular_file(dn.String:new(path))
end

function dn.os_is_directory(path)
  return ffi.C.dn_os_is_directory(dn.String:new(path))
end

function dn.os_create_directory(path)
  return ffi.C.dn_os_create_directory(dn.String:new(path))
end

function dn.os_remove_directory(path)
  return ffi.C.dn_os_remove_directory(dn.String:new(path))
end

function dn.os_create_file(path)
  return ffi.C.dn_os_create_file(dn.String:new(path))
end

function dn.os_remove_file(path)
  return ffi.C.dn_os_remove_file(dn.String:new(path))
end

function dn.os_file_mod_time(path)
  return ffi.C.dn_os_file_mod_time(dn.String:new(path))
end


function doublenickel.ffi.set_camera()
end


function doublenickel.ffi.push_fullscreen_quad()
  local n = ffi.C.dn_window_get_native_resolution()
  local uvs = nil
  local opacity = 1.0
  ffi.C.push_quad(0, n.y, n.x, n.y, uvs, opacity);
end
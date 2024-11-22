CtypeTest = tdengine.class.define('CtypeTest')

CtypeTest.Cases = tdengine.enum.define(
  'CtypeTestCases', 
  {
    TypeInitializers = 0,
    TableInitializers = 1,
    ZeroInitialize = 2,
    AssignFields = 3,
    TableInitializerNoConstructor = 4,
    CInitializer = 5,
    AllocAndAssign = 6,
    EngineType = 7,
    EngineTypeRaw = 8,
    BumpAllocAndAssign = 9,
  }
)

function CtypeTest:init()
  self.num_iterations = 10000
  self.metrics = {}

  self.renderer = ffi.new('SdfRenderer [1]')
  self.renderer = ffi.C.sdf_renderer_create(10000)


  for case in self.Cases:iterate() do
    tdengine.time_metric.add(case:to_string())
  end

end

function CtypeTest:run_case(case)
  local V2Raw = ffi.typeof('Vector2')
  local V3Raw = ffi.typeof('Vector3')
  local SdfHeaderRaw = ffi.typeof('SdfHeader')

  tdengine.ffi.tm_begin(case:to_string())
  if self.Cases.TypeInitializers:match(case) then
    for i = 1, self.num_iterations do
      local header = SdfHeaderRaw(
        V3Raw(0, 1, 1),
        V2Raw(0, 1),
        0.0,
        1.0,
        ffi.C.SDF_SHAPE_CIRCLE
      )
    end

  elseif self.Cases.TableInitializers:match(case) then
    for i = 1, self.num_iterations do
      local header = SdfHeaderRaw(
        { 1.0, 0.0, 0.4 },
        { x, y },
        0,
        1.5,
        ffi.C.SDF_SHAPE_CIRCLE
      )
    end

  elseif self.Cases.ZeroInitialize:match(case) then
    for i = 1, self.num_iterations do
      local header = SdfHeaderRaw()
    end

  elseif self.Cases.AssignFields:match(case) then
    for i = 1, self.num_iterations do
      local header = SdfHeaderRaw()
      header.color.x = 1.0
      header.color.y = 1.0
      header.color.z = 1.0
      header.position.x = 1.0
      header.position.y = 1.0
      header.rotation = 1.0
      header.edge_thickness = 1.0
      header.shape = ffi.C.SDF_SHAPE_CIRCLE
    end

  elseif self.Cases.AllocAndAssign:match(case) then
    local header = SdfHeaderRaw()
    for i = 1, self.num_iterations do
      header.color.x = 1.0
      header.color.y = 1.0
      header.color.z = 1.0
      header.position.x = 1.0
      header.position.y = 1.0
      header.rotation = 1.0
      header.edge_thickness = 1.0
      header.shape = ffi.C.SDF_SHAPE_CIRCLE
    end

  elseif self.Cases.TableInitializerNoConstructor:match(case) then
    for i = 1, self.num_iterations do
      local header = {
        { 1.0, 0.0, 0.4 },
        { x, y },
        0,
        1.5,
        ffi.C.SDF_SHAPE_CIRCLE
      }
    end

  elseif self.Cases.CInitializer:match(case) then
    for i = 1, self.num_iterations do
      ffi.C.sdf_circle(self.renderer, 10)
    end

  elseif self.Cases.EngineType:match(case) then
    for i = 1, self.num_iterations do
      local header = SdfHeader:new({
        color = tdengine.colors.prussian_blue,
        position = Vector2:new(0, 1),
        rotation = 0.0,
        edge_thickness = 1.0,
        shape = Sdf.Circle
      })
    end 
  
  elseif self.Cases.EngineTypeRaw:match(case) then
    for i = 1, self.num_iterations do
      local header = SdfHeader:init_raw({
        { 1.0, 0.0, 0.4 },
        { x, y },
        0,
        1.5,
        ffi.C.SDF_SHAPE_CIRCLE
      })
    end

  elseif self.Cases.BumpAllocAndAssign:match(case) then
    local allocator = tdengine.ffi.ma_find('bump')
    local size = ffi.sizeof('SdfHeader')
    local header = ffi.cast('SdfHeader*', ffi.C.ma_alloc(allocator, size))

    for i = 1, self.num_iterations do
      header.color.x = 1.0
      header.color.y = 1.0
      header.color.z = 1.0
      header.position.x = 1.0
      header.position.y = 1.0
      header.rotation = 1.0
      header.edge_thickness = 1.0
      header.shape = ffi.C.SDF_SHAPE_CIRCLE
    end

  end
  tdengine.ffi.tm_end(case:to_string())

  local time = truncate(tdengine.ffi.tm_last(case:to_string()) * 1000, 4)
  table.insert(self.metrics, {
    case = case:to_string(),
    time = time,
    time_per_iter = time / self.num_iterations
  })
end

function CtypeTest:run()
  for case in self.Cases:iterate() do
    self:run_case(case)
  end

  table.sort(self.metrics, function(a, b) return a.time < b.time end)
  p(self.metrics)
end

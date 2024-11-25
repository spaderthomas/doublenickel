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

function tdengine.fonts.build(data)
  tdengine.fonts.data = data

  local allocator = tdengine.ffi.dn_allocator_find('bump')
  local descriptors = allocator:alloc_array('dn_font_descriptor_t', #data)

  for i = 1, #data do
    descriptors[i - 1] = FontDescriptor:new(data[i])
  end

  tdengine.ffi.dn_font_bake_n(descriptors, #data)
end

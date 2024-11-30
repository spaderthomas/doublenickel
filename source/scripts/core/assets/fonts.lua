
function doublenickel.fonts.build(data)
  doublenickel.fonts.data = data

  local allocator = doublenickel.ffi.dn_allocator_find('bump')
  local descriptors = allocator:alloc_array('dn_font_descriptor_t', #data)

  for i = 1, #data do
    descriptors[i - 1] = FontDescriptor:new(data[i])
  end

  doublenickel.ffi.dn_font_bake_n(descriptors, #data)
end

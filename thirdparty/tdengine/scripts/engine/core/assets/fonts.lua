function tdengine.fonts.load()
  tdengine.fonts.data = tdengine.module.read_from_named_path('font_info')
end

function tdengine.fonts.init()
  for font_id, font_data in pairs(tdengine.fonts.data) do
    local file_path = tdengine.ffi.resolve_format_path('font', font_data.font):to_interned()
    tdengine.ffi.create_font(font_id, file_path, font_data.size)
  end

  tdengine.ffi.add_imgui_font('editor-16')
  for font_id, font_data in pairs(tdengine.fonts.data) do
    if font_data.imgui then
      tdengine.ffi.add_imgui_font(font_id)
    end
  end
end

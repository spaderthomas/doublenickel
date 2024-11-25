FontDescriptor = tdengine.class.metatype('dn_font_descriptor_t')
function FontDescriptor:init(params)
  self.id = params.id
  self.file_path = params.file_path
  self.size = params.size
  self.imgui = params.imgui
  self.is_default = params.is_default
end

function tdengine.fonts.load()
  tdengine.fonts.data = tdengine.module.read_from_named_path('font_info')
end

function tdengine.fonts.init()
  for font_id, font_data in pairs(tdengine.fonts.data) do
    local file_path = tdengine.ffi.dn_paths_resolve_format('font', font_data.font):to_interned()
    tdengine.ffi.dn_font_create(font_id, file_path, font_data.size)
  end

  tdengine.ffi.dn_font_add_to_imgui('editor-16')
  for font_id, font_data in pairs(tdengine.fonts.data) do
    if font_data.imgui then
      tdengine.ffi.dn_font_add_to_imgui(font_id)
    end
  end
end

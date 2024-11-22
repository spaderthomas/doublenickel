struct GlyphInfo {
	Vector2* verts;
	Vector2* uv;
	Vector2 size;
	Vector2 bearing;
	Vector2 advance;
	float descender;
};

struct FontInfo {
	size_t hash;
	char path [256];
	
	uint32 size;
	uint32 texture;
	ImFont* imfont;
	Vector2 resolution;
	 
	ArrayView<GlyphInfo> glyphs;
	Vector2 max_advance;
	Vector2 max_glyph;
	float32 line_spacing;
};


FM_LUA_EXPORT void create_font(const char* id, const char* file_path, u32 size);
FM_LUA_EXPORT void add_imgui_font(const char* id);

FontInfo* font_find(size_t hash);
FontInfo* font_find(const char* name);

void init_fonts() {
	arr_clear(&font_infos);
	arr_clear(&glyph_infos);
	arr_clear(&text_uv_data);
	arr_clear(&text_vx_data);

	ImGui::GetIO().Fonts->Clear();
	ImGui_ImplOpenGL3_DestroyDeviceObjects();
	//load_all_fonts();
	ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void add_imgui_font(const char* id) {
	auto font = font_find(id);
	if (!font) {
		tdns_log.write("Tried to add font to ImGui, but couldn't find font; font_id = %s", id);
		return;
	}
	
	font->imfont = ImGui::GetIO().Fonts->AddFontFromFileTTF(font->path, static_cast<float>(font->size));
}

void create_font(const char* id, const char* file_path, u32 size) {
	FT_Library fm_freetype;

	tdns_log.write(Log_Flags::File, "%s: %s, %s, %d", __func__, id, file_path, size);
	
	if (FT_Init_FreeType(&fm_freetype)) {
		tdns_log.write("%s: failed to initialize FreeType", __func__);
		exit(0);
	}
	
	FT_Face face = nullptr;
	if (FT_New_Face(fm_freetype, file_path, 0, &face)) {
		tdns_log.write("%s: failed to load font, font = %s", __func__, file_path);
		return;
	}

	FontInfo* font = arr_push(&font_infos);
	font->hash = hash_label(id);
	strncpy(font->path, file_path, 256);
	font->size = size;
	
	static const i32 num_glyphs = 128;
	u32 base_glyph = font_infos.size * num_glyphs;
	arr_reserve(&glyph_infos, num_glyphs);
	font->glyphs = arr_view(&glyph_infos, base_glyph, num_glyphs);

	/* 
	   Jesus Christ, fonts are really hard. FreeType generally returns all of its metrics in "font units". 
	   To understand what a font unit (which is conveniently + aptly abbreviated to FU) is, you need to
	   first understand what EM is. EM is common; you can use it in CSS, for example. It's a unit that's 
	   relative to the size of the font. 

	   If a font is defined to be 16 pixels tall, then 1 EM equals 16 pixels. 1.5 EM would be 24 pixels, and
	   so on. It's a scale factor, rather than a unit per se.

	   FreeType's FUs are just EM, scaled. The reason for the indirection, instead of just using EM (and this
	   is purely my best guess), is that different fonts specify different sizes for what 1 EM means. The values
	   are usually 2048 for TTF and 1000 for other fonts.

	   The way to convert a size you get from the face into a real, true-blue fraction of the screen you're
	   looking at goes like this:
	     1. Retrieve the size from FreeType in FU
		 2. Determine how many EM the size is by dividing by units_per_EM
		 3. Multiply this by the base pixel size you gave to FreeType
		 4. Divide this by the current screen resolution

	   I tried a couple things to get the global face sizes correct, including:
	
	   (1) float max_height_fu = face->bbox.yMax - face->bbox.yMin;
	   This isn't correct. yMax is like the tallest ascender, and yMin is the lowest descender. So
	   while moving down this much per-line will DEFINITELY make sure nothing touches, in practice
	   it ends up leaving too much space between each line, because you're kerning for the
	   worst case scenario.
	*/
	float base_font_size = 16;
	float font_scale = (float)size / base_font_size;
	float pixel_size = base_font_size * font_scale;
	FT_Set_Pixel_Sizes(face, pixel_size, pixel_size);
		
	float max_height_fu = face->max_advance_height;
	float max_height_em = max_height_fu / face->units_per_EM;
	float max_height_px = max_height_em * pixel_size;
	font->max_advance.y = max_height_px;
	font->line_spacing  = (float)face->height;
	
	// Allocate an image buffer; just give a good guess to how big it needs to be
	i32 font_height_px = face->size->metrics.height >> 6;
	i32 glyphs_per_row = static_cast<i32>(ceil(sqrt(128)));
	
	i32 tex_height = font_height_px * glyphs_per_row;
	i32 tex_width = tex_height;
	auto buffer = bump_allocator.alloc<char>(tex_width * tex_height);

	// Read each character's bitmap into the image buffer
	Vector2 point = { 0 };
	for (GLubyte c = 0; c < num_glyphs; c++) {
		i32 failure = FT_Load_Char(face, c, FT_LOAD_RENDER);
		if (failure) {
			tdns_log.write("%s: failed to load character, char = %c", __func__, c);
			return;
		}

		// Copy this bitmap into the atlas buffer, but only for glyphs which we expect to render. For some fonts,
		// I noticed that escape codes (including \n) actually render a filled-in square with an X in the middle,
		// like you see for missing characters in your system font. I don't want this, so I don't render them to
		// the rasterized font.
		FT_Bitmap* bitmap = &face->glyph->bitmap;

		if (c > ' ') {
			if (point.x + bitmap->width > tex_width) {
				point.x = 0;
				point.y += font_height_px + 1;
			}

			for (i32 row = 0; row < bitmap->rows; row++) {
				for (i32 col = 0; col < bitmap->width; col++) {
					i32 x = (i32)(point.x + col);
					i32 y = (i32)(point.y + row);
					i32 ia = y * tex_width + x;
					i32 ib = row * bitmap->pitch + col;
					buffer[ia] = bitmap->buffer[ib];
				}
			}
		}

		// Load the glyph's info in GL units. We're rendering for a specific display mode, so
		// we use the current mode's resolution as opposed to the native resolution
		//
		// https://freetype.org/freetype2/docs/glyphs/glyphs-3.html
		GlyphInfo glyph;
		glyph.size.x = face->glyph->bitmap.width;
		glyph.size.y = face->glyph->bitmap.rows;
		glyph.bearing.x = face->glyph->bitmap_left;
		glyph.bearing.y = face->glyph->bitmap_top;
		glyph.advance.x = face->glyph->advance.x / 64.f;
		glyph.advance.y = face->glyph->advance.y / 64.f;
		glyph.descender = glyph.size.y - glyph.bearing.y;

		if (c == '\n') {
			glyph.advance.x = 0;
		}

		// I use these for calculating bounding boxes, which I define to not include anything below the baseline
		font->max_glyph.x = std::max(font->max_glyph.x, glyph.size.x - glyph.descender);
		font->max_glyph.y = std::max(font->max_glyph.y, glyph.size.y - glyph.descender);

		// Build the VXs and UVs
		float left   = glyph.bearing.x;
		float top    = glyph.size.y - glyph.descender;
		float right  = left + glyph.size.x;
		float bottom = top - glyph.size.y;
		Vector2 vertices[6] = {
				{ left,  top },
				{ left,  bottom },
				{ right, bottom },
			
				{ left,  top },
				{ right, bottom },
				{ right, top },
		};
		glyph.verts = arr_push(&text_vx_data, vertices, 6);

		float uv_left = point.x / tex_width;
		float uv_right = (point.x + face->glyph->bitmap.width) / tex_width;
		float uv_top = 1 - (point.y / tex_height); // Y-axis coordinates are flipped, because we flip the texture
		float uv_bottom = 1 - ((point.y + face->glyph->bitmap.rows) / tex_height);
		Vector2 uv[6] = {
				{ uv_left,  uv_top },
				{ uv_left,  uv_bottom },
				{ uv_right, uv_bottom },
			
				{ uv_left,  uv_top },
				{ uv_right, uv_bottom },
				{ uv_right, uv_top },
		};
		glyph.uv = arr_push(&text_uv_data, uv, 6);

		*font->glyphs[c] = glyph;
		
		// Advance the point horizontally for the next character
		point.x += bitmap->width + 1;
	}


	auto tmp = bump_allocator.alloc<char>(tex_width);
	for (i32 i = 0; i != tex_height / 2; i++) {
		char* top = buffer + (i * tex_width); // first element of top row
		char* btm = buffer + ((tex_height - i - 1) * tex_width); // first element of bottom row
		memcpy(tmp, top, tex_width);
		memcpy(top, btm, tex_width);
		memcpy(btm, tmp, tex_width);
	}

	glDeleteTextures(1, &font->texture);
	glGenTextures(1, &font->texture);
	glBindTexture(GL_TEXTURE_2D, font->texture);
	glTexImage2D(GL_TEXTURE_2D,0, GL_RED, tex_width, tex_height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	// https://discord.com/channels/239737791225790464/600063880533770251/1160586297526730935
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	FT_Done_Face(face);
	FT_Done_FreeType(fm_freetype);
}

FontInfo* font_find(size_t hash) {
	arr_for(font_infos, font) {
		if (font->hash == hash) return font;
	}
	return nullptr;
}
FontInfo* font_find(const char* id) {
	if (!id) return nullptr;

	auto hash = hash_label(id);
	arr_for(font_infos, font) {
		if (font->hash == hash) return font;
	}
	return nullptr;
}

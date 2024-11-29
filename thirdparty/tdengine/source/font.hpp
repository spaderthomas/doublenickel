#ifndef DN_FONT_H
#define DN_FONT_H

typedef enum {
  DN_FONT_FLAG_NONE = 0,
  DN_FONT_FLAG_IMGUI = 1,
  DN_FONT_FLAG_DEFAULT = 2,
  DN_FONT_FLAG_BOLD = 4,
  DN_FONT_FLAG_EXTRABOLD = 8,
  DN_FONT_FLAG_ITALIC = 16,
} dn_font_flags_t;

typedef struct {
  Vector2* verts;
  Vector2* uv;
  Vector2 size;
  Vector2 bearing;
  Vector2 advance;
  float descender;
} dn_baked_glyph_t;

typedef struct {
  dn_asset_name_t name;
  dn_hash_t hash;
  dn_path_t path;
  dn_font_flags_t flags;
  
  u32 size;
  u32 texture;
  ImFont* imfont;
  Vector2 resolution;
   
  dn_baked_glyph_t* glyphs;
  Vector2 max_advance;
  Vector2 max_glyph;
  float line_spacing;

} dn_baked_font_t;

typedef struct {
  const char* id;
  const char* file_path;
  u32 sizes [16];
  dn_font_flags_t flags;
} dn_font_descriptor_t;

typedef struct {
  dn_font_descriptor_t* fonts;
  u32 num_fonts;
} dn_font_config_t;

typedef struct {
  dn_fixed_array<dn_baked_font_t, 64> baked_fonts;
  dn_fixed_array<dn_baked_glyph_t, 8192> baked_glyphs;
  dn_fixed_array<Vector2, 65536> vertex_data;
  dn_fixed_array<Vector2, 65536> uv_data;

  dn_baked_font_t* default_font;
} dn_fonts_t;
dn_fonts_t dn_fonts;

DN_IMP dn_hash_t        dn_font_hash(const char* id, u32 size);

DN_API void             dn_font_init(dn_font_config_t config);
DN_API void             dn_font_bake_n(dn_font_descriptor_t* descriptors, u32 num_descriptors);
DN_API void             dn_font_bake(dn_font_descriptor_t descriptor);
DN_API dn_font_config_t dn_font_config_default();
DN_API dn_baked_font_t* dn_font_default();
DN_API dn_baked_font_t* dn_font_find(const char* id, u32 size);

#endif



#ifdef DN_FONT_IMPLEMENTATION
dn_font_config_t dn_font_config_default() {
  return {
    .fonts = NULL,
    .num_fonts = 0
  };
}

void dn_font_init(dn_font_config_t config) {
  dn_log("%s", __func__);
  
  dn_fonts = {0};

  dn::fixed_array::init(&dn_fonts.baked_fonts, &dn_allocators.standard);
  dn::fixed_array::init(&dn_fonts.baked_glyphs, &dn_allocators.standard);
  dn::fixed_array::init(&dn_fonts.vertex_data, &dn_allocators.standard);
  dn::fixed_array::init(&dn_fonts.uv_data, &dn_allocators.standard);

  dn_font_descriptor_t engine_fonts [] = {
    {
      .id = "merriweather",
      .file_path = dn_paths_resolve_format("dn_font", "Merriweather-Regular.ttf"),
      .sizes = { 16, 24, 28, 32 },
      .flags = DN_FONT_FLAG_NONE
    },
    {
      .id = "inconsolata",
      .file_path = dn_paths_resolve_format("dn_font", "Inconsolata-Regular.ttf"),
      .sizes = { 16, 12, 20, 24, 28, 32, 36, 48, 64 },
      .flags = DN_FONT_FLAG_IMGUI
    },
    {
      .id = "inconsolata-extrabold",
      .file_path = dn_paths_resolve_format("dn_font", "Inconsolata-ExtraBold.ttf"),
      .sizes = { 12, 16, 20, 24, 28, 32, 36, 48, 64 },
      .flags = DN_FONT_FLAG_IMGUI
    }
  };
  dn_font_bake_n(engine_fonts, DN_ARR_LEN(engine_fonts));
  dn_font_bake_n(config.fonts, config.num_fonts);
}

dn_baked_font_t* dn_font_default() {
  return dn_fonts.default_font;
}

void dn_font_bake_n(dn_font_descriptor_t* fonts, u32 num_fonts) {
  for (u32 i = 0; i < num_fonts; i++) {
    dn_font_bake(fonts[i]);
  }
}

void dn_font_bake(dn_font_descriptor_t desc) {
  dn_log("%s: (%s) %s", __func__, desc.id, desc.file_path);

  for (u32 i = 0; i < 16; i++) {
    auto size = desc.sizes[i];
    if (!size) break;

    FT_Library ft;

    dn_log_flags(DN_LOG_FLAG_FILE, "%s: %s, %s, %d", __func__, desc.id, desc.file_path, size);
    
    if (FT_Init_FreeType(&ft)) {
      dn_log("%s: Failed to initialize FreeType", __func__);
      exit(0);
    }
    
    FT_Face face = nullptr;
    if (FT_New_Face(ft, desc.file_path, 0, &face)) {
      dn_log("%s: Failed to load font; file_path = %s", __func__, desc.file_path);
      return;
    }

    dn_baked_font_t* font = dn::fixed_array::reserve(&dn_fonts.baked_fonts, 1);
    font->hash = dn_font_hash(desc.id, size);
    dn_string_copy(desc.id, font->name, DN_ASSET_NAME_LEN);
    dn_string_copy(desc.file_path, font->path, DN_MAX_PATH_LEN);
    font->size = size;
    font->flags = desc.flags;
    
    static const i32 num_glyphs = 128;
    font->glyphs = dn::fixed_array::reserve(&dn_fonts.baked_glyphs, num_glyphs);

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
    auto buffer = dn::allocator::alloc<char>(&dn_allocators.bump, tex_width * tex_height);

    // Read each character's bitmap into the image buffer
    Vector2 point = { 0 };
    for (GLubyte c = 0; c < num_glyphs; c++) {
      i32 failure = FT_Load_Char(face, c, FT_LOAD_RENDER);
      if (failure) {
        dn_log("%s: failed to load character, char = %c", __func__, c);
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
      dn_baked_glyph_t glyph;
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

      font->glyphs[c] = glyph;
      
      // Advance the point horizontally for the next character
      point.x += bitmap->width + 1;
    }


    auto tmp = dn::allocator::alloc<char>(&dn_allocators.bump, tex_width);
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
    FT_Done_FreeType(ft);

    if (desc.flags & DN_FONT_FLAG_IMGUI) {
      font->imfont = ImGui::GetIO().Fonts->AddFontFromFileTTF(font->path, static_cast<float>(font->size));
    }

    if (desc.flags & DN_FONT_FLAG_DEFAULT) {
      if (dn_fonts.default_font) {
        dn_log(
          "Tried to register default font, but a default was already registered; old = %s, new = %s",
          dn_fonts.default_font->path,
          font->path
        );
      }

      dn_fonts.default_font = font;
    }
  }
}

dn_hash_t dn_font_hash(const char* id, u32 size) {
  return dn_combine_hashes(dn_hash_bytes(id, strlen(id)), size);
}

dn_baked_font_t* dn_font_find(const char* id, u32 size) {
  if (!id) return dn_font_default();

  auto hash = dn_font_hash(id, size);
  for (u32 i = 0; i < dn_fonts.baked_fonts.size; i++) {
    auto font = dn::fixed_array::at(&dn_fonts.baked_fonts, i);
    if (font->hash == hash) return font;
  }

  return dn_font_default();
}

#endif

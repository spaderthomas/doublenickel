///////////////////
// dn_prepared_text_t //
///////////////////
void dn_prepared_text_t::init() {
	this->color = { 1.f };
	this->font = dn_font_default();
	this->wrap = 0;
}

void dn_prepared_text_t::set_font(dn_baked_font_t* font) {
	this->font = font;
}

void dn_prepared_text_t::set_text(const char* text) {
	// @spader: I learned that when the source buffer is larger than n, strncpy will not
	// null terminate the destination buffer. It is basically unfathomable to me that this
	// would ever be what you want, but apparently strncpy was written fifty years ago
	// to copy strings into 14 byte Unix directory.
	snprintf(this->text, MAX_TEXT_LEN, "%s", text);
		//strncpy(this->text, text, MAX_TEXT_LEN - 1);
}

void dn_prepared_text_t::set_wrap(f32 wrap) {
	this->wrap = wrap;
}

void dn_prepared_text_t::set_position(f32 x, f32 y) {
	this->position.x = x;
	this->position.y = y;
}

void dn_prepared_text_t::set_offset(f32 offset) {
	this->offset = offset;
}

void dn_prepared_text_t::set_color(dn_vector4_t color) {
	this->color = color;
}

void dn_prepared_text_t::set_precision(bool precision) {
	this->precise = precision;
}

bool dn_prepared_text_t::is_empty() {
	return !this->text[0];
}

i32 dn_prepared_text_t::count_lines() {
	return count_breaks() - 1;
}

i32 dn_prepared_text_t::count_breaks() {
	for (i32 i = 1; i < MAX_LINE_BREAKS; i++) {
		if (breaks[i]) continue;
		return i;
	}

	return MAX_LINE_BREAKS;
}

i32 dn_prepared_text_t::get_break(i32 index) {
	return breaks[index];
}

void dn_prepared_text_t::add_break(i32 index) {
	auto i = count_breaks();
	breaks[i] = index;
}

dn_array_view_t<char> dn_prepared_text_t::get_line(i32 index) {
	auto begin = get_break(index);
	auto end   = get_break(index + 1);
	auto count = end - begin;
	
	return dn_array_view(
		text + begin,
		count
	);
}

dn_prepared_text_t* dn_prepare_text(const char* text, f32 px, f32 py, dn_baked_font_t* font) {
	return dn_prepare_text_ex(text, px, py, font, 0, dn_colors::white, true);
}

dn_prepared_text_t* dn_prepare_text_wrap(const char* text, f32 px, f32 py, dn_baked_font_t* font, f32 wrap) {
	return dn_prepare_text_ex(text, px, py, font, wrap, dn_colors::white, true);
}

dn_prepared_text_t* dn_prepare_text_ex(const char* text, f32 px, f32 py, dn_baked_font_t* font, f32 wrap, dn_vector4_t color, bool precise) {
	if (!text) return nullptr;
	
	auto prepared_text = dn::allocator::alloc<dn_prepared_text_t>(&dn_allocators.bump, 1);
	prepared_text->init();
	prepared_text->set_text(text);
	prepared_text->set_position(px, py);
	prepared_text->set_color(color);
	prepared_text->set_font(font);
	prepared_text->set_wrap(wrap);
	prepared_text->set_precision(precise);
	
	// Calculate line breaks
	LineBreakContext context;
	context.set_info(prepared_text);
	context.calculate();

	// Calculate width. Just find the longest line.
	f32 this_line_width = 0;
	for (int i = 0; i < prepared_text->count_breaks(); i++) {
		auto line = prepared_text->get_line(i);
		dn_array_for(line, c) {
			if (*c == 0) break;
			dn_baked_glyph_t& glyph = prepared_text->font->glyphs[*c];
			this_line_width += glyph.advance.x;
		}

		prepared_text->width = dn_max(prepared_text->width, this_line_width);
		this_line_width = 0;
	}

	// Calculate the metrics we need to properly align text specified by a top-left position
	auto first_line = prepared_text->get_line(0);
	f32 first_line_height = 0;
	f32 first_line_descender = 0;
	dn_array_for(first_line, c) {
		if (*c == 0) break;
		dn_baked_glyph_t& glyph = prepared_text->font->glyphs[*c];
		first_line_height = std::max(first_line_height, glyph.size.y - glyph.descender);
		first_line_descender = std::max(first_line_descender, glyph.descender);
	}

	if (prepared_text->is_empty()) {
		prepared_text->height = 0;
		prepared_text->baseline_offset = 0;
		prepared_text->height_imprecise = 0;
		prepared_text->baseline_offset_imprecise = 0;
	}
	else {
		auto line = prepared_text->get_line(0);
		prepared_text->baseline_offset = first_line_height;
		prepared_text->baseline_offset_imprecise = prepared_text->font->max_glyph.y;

		if (prepared_text->count_lines() == 1) {
			prepared_text->height = first_line_height;
			prepared_text->height_imprecise = prepared_text->font->max_glyph.y;
		}
		else {
			// Technically wrong, but precise height doesn't matter for multiline text.
			prepared_text->height = prepared_text->font->max_advance.y * prepared_text->count_lines();
			prepared_text->height_imprecise = prepared_text->font->max_advance.y * prepared_text->count_lines();
		}
	}

	return prepared_text;
}

////////////////////////
// LINE BREAK CONTEXT //
////////////////////////
void LineBreakContext::set_info(dn_prepared_text_t* info) {
	this->info = info;
}

void LineBreakContext::calculate() {
	this->point = 0;
	this->point_max = this->info->wrap;

	// By definition, we always have a line break at index 0, and at the length of the string
	this->info->add_break(0);
	
	dn_defer {
		auto len = strlen(this->info->text);
		this->info->add_break(len);
	};

	// Text that never wraps never changes lines, because we don't support newline.
	if (this->info->wrap == 0) return;

	f32 word_size = 0;
	i32 word_begin  = 0;
	dn_baked_glyph_t* glyph  = nullptr;
	auto text = dn_array_view(info->text, MAX_TEXT_LEN);

	auto break_on_previous_word = [&]() {
		// word_begin points, inclusively, to the first character of the next word.
		// Since line breaks are inclusive, we've got to back to the space.
		this->info->add_break(word_begin);
		this->point = word_size + glyph->advance.x;
	};

	auto begin_new_word = [&](char* c) {
		word_begin = dn_array_indexof(&text, c) + 1;
		word_size = 0;
	};


	dn_array_for(text, c) {
		if (*c == 0) break;
		glyph = &this->info->font->glyphs[*c];

		if (*c == '\n') {
			// If the current word would not have fit on this line anyway, so move it to the next.
			if (this->point + word_size >= this->info->wrap) {
				break_on_previous_word();
			}

			// Add a line break at the index of the newline, begin a new word at the next character,
			// and reset the point (since we're starting at the very beginning of the next line)
			this->info->add_break(dn_array_indexof(&text, c));
			
			this->point = 0;
			begin_new_word(c);
		}
		else if (*c == ' ') {
			if (this->point + word_size >= this->info->wrap) {
				break_on_previous_word();
			}
			else {
				this->point += word_size + glyph->advance.x;
			}
		
			begin_new_word(c);
		} else {
			word_size += glyph->advance.x;
		}
	}

	if (this->point + word_size > this->point_max) {
		this->info->add_break(word_begin);
	}
}

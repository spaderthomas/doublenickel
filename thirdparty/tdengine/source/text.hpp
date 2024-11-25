#define MAX_TEXT_LEN 1024
#define MAX_LINE_BREAKS 32

struct dn_prepared_text {
	// @dn: This should be temporary storage 99% of the time
	char text [MAX_TEXT_LEN] = { 0 };
	Vector2 position;
	Vector2 padding;
	Vector4 color;
	FontInfo* font;
	float wrap;
	float offset;
	bool world_space = false;

	// [ b[i], b[i+1] )
	i32 breaks [MAX_LINE_BREAKS] = { 0 };

	// There are two modes: Precise and imprecise. Precise aims to put an exact bounding box around the text, from
	// the baseline to the top (i.e. excluding glyph pieces under the baseline). Imprecise aims to give a worst-case
	// bounding box for a chunk of text in a font.
	//
	// For most text, imprecise is OK. But, when we're centering text exactly, we need to know the exact size of
	// *these* glyphs, e.g. in a button. The drawback to precise text is that the baseline offset differs depending
	// on what glyphs it contains. For example, if the string is a short character like "s", then we'll adjust the
	// baseline less than for a tall character like "T". 
	float width = 0;
	float height = 0;
	float baseline_offset = 0;
	float baseline_offset_imprecise = 0;
	float height_imprecise = 0;
	bool precise = false;

	void init();
	void set_font(const char* name);
	void set_text(const char* text);
	void set_wrap(float wrap);
	void set_position(float x, float y);
	void set_offset(float offset);
	void set_color(Vector4 color);
	void set_precision(bool precision);
	bool is_empty();
	i32 count_breaks();
	i32 count_lines();
	i32 get_break(i32 index);
	void add_break(i32 index);
	ArrayView<char> get_line(i32 index);
};

FM_LUA_EXPORT dn_prepared_text* dn_prepare_text(const char* text, float px, float py, const char* font);
FM_LUA_EXPORT dn_prepared_text* dn_prepare_text_wrap(const char* text, float px, float py, const char* font, float wrap);
FM_LUA_EXPORT dn_prepared_text* dn_prepare_text_ex(const char* text, float px, float py, const char* font, float wrap, Vector4 color, bool precise);

struct LineBreakContext {
	// Input
	dn_prepared_text* info;

	// Calculated
	float point = 0;
	float point_max;

	void set_info(dn_prepared_text* info);
	void calculate();
};

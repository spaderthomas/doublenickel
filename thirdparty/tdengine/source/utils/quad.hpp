enum class PrimitiveType {
	None,
	Rect,
	Circle,
	Line,
};

struct Rect {
	Vector2 position;
	Vector2 dimension;
};

struct Circle {
	Vector2 position;
	float32 radius;
	uint32 segments; // @hack: This should be with the draw code, not the representation of a circle
};

struct Line {
	Vector2 begin;
	Vector2 end;
};

struct Primitive {
	Primitive() { memset(this, 0, sizeof(Primitive)); }
		
	PrimitiveType type;
	union {
		Rect rect;
		Circle circle;
		Line line;
	};
};

#define TD_MAKE_QUAD(top, bottom, left, right) \
    {                                     \
        { left, top },                    \
        { left, bottom },                 \
        { right, bottom },                \
                                          \
        { left, top },                    \
        { right, bottom },                \
        { right, top }                    \
    }                                     

#define dn_color_255_3(r, g, b) { (r) / 255.f, (g) / 255.f, (b) / 255.f, 1.0}
#define dn_color_255_4(r, g, b, a) { (r) / 255.f, (g) / 255.f, (b) / 255.f, (a) / 255.f}

namespace dn_colors {
	dn_vector4_t clear            = dn_color_255_4(0,   0,   0,   0);
	dn_vector4_t white            = dn_color_255_3(255, 255, 255);
	dn_vector4_t red              = dn_color_255_3(255, 0,   0);
	dn_vector4_t green            = dn_color_255_3(0,   255, 0);
	dn_vector4_t blue             = dn_color_255_3(0,   0,   255);

	dn_vector4_t gunmetal         = dn_color_255_3(43,  61,  65);
	dn_vector4_t paynes_gray      = dn_color_255_3(76,  95,  107);
	dn_vector4_t cadet_gray       = dn_color_255_3(131, 160, 160);
	dn_vector4_t charcoal         = dn_color_255_3(64,  67,  78);
	dn_vector4_t cool_gray        = dn_color_255_3(140, 148, 173);

	dn_vector4_t celadon          = dn_color_255_3(183, 227, 204);
	dn_vector4_t spring_green     = dn_color_255_3(89,  255, 160);
	dn_vector4_t mindaro          = dn_color_255_3(188, 231, 132);
	dn_vector4_t light_green      = dn_color_255_3(161, 239, 139);
	dn_vector4_t zomp             = dn_color_255_3(99,  160, 136);

	dn_vector4_t indian_red       = dn_color_255_3(180, 101, 111);
	dn_vector4_t tyrian_purple    = dn_color_255_3(95,  26,  55);
	dn_vector4_t cardinal         = dn_color_255_3(194, 37,  50);

	dn_vector4_t prussian_blue    = dn_color_255_3(16,  43,  63);
	dn_vector4_t midnight_green   = dn_color_255_3(25,  83,  95);

	dn_vector4_t orange           = dn_color_255_3(249, 166, 32);
	dn_vector4_t sunglow          = dn_color_255_3(255, 209, 102);
	dn_vector4_t selective_yellow = dn_color_255_3(250, 188, 42);

	dn_vector4_t cream            = dn_color_255_3(245, 255, 198);
	dn_vector4_t misty_rose       = dn_color_255_3(255, 227, 227);

	dn_vector4_t taupe            = dn_color_255_3(68,  53,  39);
	dn_vector4_t dark_green       = dn_color_255_3(4,   27,  21);
	dn_vector4_t rich_black       = dn_color_255_3(4,   10,  15);

};

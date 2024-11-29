#define dn_color_255_3(r, g, b) { (r) / 255.f, (g) / 255.f, (b) / 255.f, 1.0}
#define dn_color_255_4(r, g, b, a) { (r) / 255.f, (g) / 255.f, (b) / 255.f, (a) / 255.f}

namespace dn_colors {
	Vector4 clear            = dn_color_255_4(0,   0,   0,   0);
	Vector4 white            = dn_color_255_3(255, 255, 255);
	Vector4 red              = dn_color_255_3(255, 0,   0);
	Vector4 green            = dn_color_255_3(0,   255, 0);
	Vector4 blue             = dn_color_255_3(0,   0,   255);

	Vector4 gunmetal         = dn_color_255_3(43,  61,  65);
	Vector4 paynes_gray      = dn_color_255_3(76,  95,  107);
	Vector4 cadet_gray       = dn_color_255_3(131, 160, 160);
	Vector4 charcoal         = dn_color_255_3(64,  67,  78);
	Vector4 cool_gray        = dn_color_255_3(140, 148, 173);

	Vector4 celadon          = dn_color_255_3(183, 227, 204);
	Vector4 spring_green     = dn_color_255_3(89,  255, 160);
	Vector4 mindaro          = dn_color_255_3(188, 231, 132);
	Vector4 light_green      = dn_color_255_3(161, 239, 139);
	Vector4 zomp             = dn_color_255_3(99,  160, 136);

	Vector4 indian_red       = dn_color_255_3(180, 101, 111);
	Vector4 tyrian_purple    = dn_color_255_3(95,  26,  55);
	Vector4 cardinal         = dn_color_255_3(194, 37,  50);

	Vector4 prussian_blue    = dn_color_255_3(16,  43,  63);
	Vector4 midnight_green   = dn_color_255_3(25,  83,  95);

	Vector4 orange           = dn_color_255_3(249, 166, 32);
	Vector4 sunglow          = dn_color_255_3(255, 209, 102);
	Vector4 selective_yellow = dn_color_255_3(250, 188, 42);

	Vector4 cream            = dn_color_255_3(245, 255, 198);
	Vector4 misty_rose       = dn_color_255_3(255, 227, 227);

	Vector4 taupe            = dn_color_255_3(68,  53,  39);
	Vector4 dark_green       = dn_color_255_3(4,   27,  21);
	Vector4 rich_black       = dn_color_255_3(4,   10,  15);

};

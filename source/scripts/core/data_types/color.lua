doublenickel.enum.define(
	'ReadableTextColor', {
		Light = 0,
		Dark = 1
	}
)

function doublenickel.color(r, g, b, a)
	if not r then
		return doublenickel.colors.white:copy()
	elseif ffi.istype('float [4]', r) then
		local float_array = r
		return doublenickel.color(float_array[0], float_array[1], float_array[2], float_array[3])
	elseif type(r) == 'table' then
		local source = r
		if source.r and source.g and source.b and source.a then
			return doublenickel.color(source.r, source.g, source.b, source.a)
		else
			return doublenickel.colors.white:copy()
		end
	end

	local color = {
		r = r,
		g = g,
		b = b,
		a = a
	}

	-- Kind of hacky, because this was never a class so I'm not using the class tools I already have.
	-- But I don't care too much, and this works fine. The gist is that if you say something like:
	--
	--  local color = doublenickel.colors.black
	--  color.r = 1.0
	--
	-- Then black is globally modified. It's unintuitive that you have to call :copy(), but it's better
	-- than nothing and avoids too much API churn
	local mt = {
		__index = {
			copy = function(self)
				return doublenickel.color(self)
			end,
			to_ctype = function(self)
				return self:to_vec4()
			end,
			to_vec4 = function(self)
				return ffi.new('Vector4', self.r, self.g, self.b, self.a)
			end,
			to_vec3 = function(self)
				return Vector3:new(self.r, self.g, self.b)
			end,
			to_imvec4 = function(self)
				return ffi.new('ImVec4', self.r, self.g, self.b, self.a)
			end,
			to_floats = function(self)
				return ffi.new('float [4]', self.r, self.g, self.b, self.a)
			end,
			to_255 = function(self)
				return doublenickel.color255(self)
			end,
			to_u32 = function(self)
				return doublenickel.color32(math.floor(self.r * 255), math.floor(self.g * 255), math.floor(self.b * 255),
					math.floor(self.a * 255))
			end,
			premultiply = function(self)
				return doublenickel.color(self.r * self.a, self.g * self.a, self.b * self.a, 1.0)
			end,
			alpha = function(self, alpha)
				return doublenickel.color(self.r, self.g, self.b, alpha)
			end,
			readable_color = function(self)
				local sum = self.r + self.g + self.b
				return sum > .75 and doublenickel.enums.ReadableTextColor.Dark or doublenickel.enums.ReadableTextColor.Light
			end,
		}
	}
	setmetatable(color, mt)
	return color
end

function doublenickel.color255(r, g, b, a)
	if type(r) == 'table' then
		local source = r
		r = source.r
		g = source.g
		b = source.b
		a = source.a
	end

	return doublenickel.color(r / 255.0, g / 255.0, b / 255.0, a / 255.0)
end

function doublenickel.color32(r, g, b, a)
	if type(r) == 'table' then
		local color = r
		r = color.r
		g = color.g
		b = color.b
		a = color.a
	end
	a = math.min(a, 255) * math.pow(2, 24)
	b = math.min(b, 255) * math.pow(2, 16)
	g = math.min(g, 255) * math.pow(2, 8)
	r = math.min(r, 255)
	return r + g + b + a
end

function doublenickel.color_to_vec4(color)
	return ffi.new('Vector4', color.r, color.g, color.b, color.a)
end

function doublenickel.coherent_random_color()
	local random = math.random
	local index = random(1, #doublenickel.color_pallet)
	return doublenickel.color_pallet[index]
end

function doublenickel.random_color()
	local random = math.random
	return doublenickel.color(
		random(0, 256),
		random(0, 256),
		random(0, 256),
		255)
end

function doublenickel.is_color_like(t)
	if type(t) ~= 'table' then return false end

	local is_color_like = true
	is_color_like = is_color_like and type(t.r) == 'number'
	is_color_like = is_color_like and type(t.g) == 'number'
	is_color_like = is_color_like and type(t.b) == 'number'
	return is_color_like
end

doublenickel.colors                    = {}
doublenickel.colors.almost_black       = doublenickel.color(0.00, 0.00, 0.10, 1.00)
doublenickel.colors.baby_blue          = doublenickel.color(0.49, 0.55, 0.77, 1.00)
doublenickel.colors.blue               = doublenickel.color(0.00, 0.00, 1.00, 1.00)
doublenickel.colors.blue_light_trans   = doublenickel.color(0.00, 0.00, 1.00, 0.20)
doublenickel.colors.black              = doublenickel.color(0.00, 0.00, 0.00, 1.00)
doublenickel.colors.clear              = doublenickel.color(0.00, 0.00, 0.00, 0.00)
doublenickel.colors.gray_dark          = doublenickel.color(0.10, 0.10, 0.10, 1.00)
doublenickel.colors.gray_medium        = doublenickel.color(0.50, 0.50, 0.50, 1.00)
doublenickel.colors.gray_light         = doublenickel.color(0.75, 0.75, 0.75, 1.00)
doublenickel.colors.gray_light_trans   = doublenickel.color(0.75, 0.75, 0.75, 0.20)
doublenickel.colors.green              = doublenickel.color(0.00, 1.00, 0.00, 1.00)
doublenickel.colors.green_dark         = doublenickel.color(0.04, 0.40, 0.08, 1.00)
doublenickel.colors.green_medium_trans = doublenickel.color(0.00, 1.00, 0.00, 0.50)
doublenickel.colors.green_light_trans  = doublenickel.color(0.00, 1.00, 0.00, 0.25)
doublenickel.colors.grid_bg            = doublenickel.color(0.25, 0.25, 0.30, 0.80)
doublenickel.colors.hovered_button     = doublenickel.color(0.57, 0.57, 0.57, 1.00)
doublenickel.colors.idk                = doublenickel.color(0.00, 0.75, 1.00, 1.00)
doublenickel.colors.idle_button        = doublenickel.color(0.75, 0.75, 0.75, 1.00)
doublenickel.colors.maroon             = doublenickel.color(0.50, 0.08, 0.16, 1.00)
doublenickel.colors.maroon_dark        = doublenickel.color(0.40, 0.04, 0.08, 1.00)
doublenickel.colors.muted_purple       = doublenickel.color(0.35, 0.25, 0.34, 1.00)
doublenickel.colors.pale_red           = doublenickel.color(1.00, 0.10, 0.00, 0.10)
doublenickel.colors.pale_red2          = doublenickel.color(1.00, 0.10, 0.00, 0.50)
doublenickel.colors.pale_green         = doublenickel.color(0.10, 1.00, 0.00, 0.10)
doublenickel.colors.red                = doublenickel.color(1.00, 0.00, 0.00, 1.00)
doublenickel.colors.red_medium_trans   = doublenickel.color(1.00, 0.00, 0.00, 0.50)
doublenickel.colors.red_light_trans    = doublenickel.color(1.00, 0.00, 0.00, 0.25)
doublenickel.colors.red_xlight_trans   = doublenickel.color(1.00, 0.00, 0.00, 0.12)
doublenickel.colors.text               = doublenickel.color(1.00, 1.00, 1.00, 1.00)
doublenickel.colors.text_hl            = doublenickel.color(0.89, 0.91, 0.56, 1.00)
doublenickel.colors.ui_background      = doublenickel.color(0.00, 0.00, 0.00, 1.00)
doublenickel.colors.violet             = doublenickel.color(0.48, 0.43, 0.66, 1.00)
doublenickel.colors.white              = doublenickel.color(1.00, 1.00, 1.00, 1.00)
doublenickel.colors.white_trans        = doublenickel.color(1.00, 1.00, 1.00, 0.20)

-- A real pallette
doublenickel.colors.gunmetal           = doublenickel.color255(43, 61, 65, 255)
doublenickel.colors.paynes_gray        = doublenickel.color255(76, 95, 107, 255)
doublenickel.colors.cadet_gray         = doublenickel.color255(131, 160, 160, 255)
doublenickel.colors.charcoal           = doublenickel.color255(64, 67, 78, 255)
doublenickel.colors.cool_gray          = doublenickel.color255(140, 148, 173, 255)

doublenickel.colors.celadon            = doublenickel.color255(183, 227, 204, 255)
doublenickel.colors.spring_green       = doublenickel.color255(89, 255, 160, 255)
doublenickel.colors.mindaro            = doublenickel.color255(188, 231, 132, 255)
doublenickel.colors.light_green        = doublenickel.color255(161, 239, 139, 255)
doublenickel.colors.zomp               = doublenickel.color255(99, 160, 136, 255)

doublenickel.colors.indian_red         = doublenickel.color255(180, 101, 111, 255)
doublenickel.colors.tyrian_purple      = doublenickel.color255(95, 26, 55, 255)
doublenickel.colors.cardinal           = doublenickel.color255(194, 37, 50, 255)

doublenickel.colors.prussian_blue      = doublenickel.color255(16, 43, 63, 255)
doublenickel.colors.midnight_green     = doublenickel.color255(25, 83, 95, 255)

doublenickel.colors.orange             = doublenickel.color255(249, 166, 32, 255)
doublenickel.colors.sunglow            = doublenickel.color255(255, 209, 102, 255)
doublenickel.colors.selective_yellow   = doublenickel.color255(250, 188, 42, 255)

doublenickel.colors.cream              = doublenickel.color255(245, 255, 198, 255)
doublenickel.colors.misty_rose         = doublenickel.color255(255, 227, 227, 255)

doublenickel.colors.taupe              = doublenickel.color255(68, 53, 39, 255)
doublenickel.colors.dark_green         = doublenickel.color255(4, 27, 21, 255)
doublenickel.colors.rich_black         = doublenickel.color255(4, 10, 15, 255)

doublenickel.colors.pallette = {
	gray = {
		'charcoal',
		'cool_gray',
		'gunmetal',
		'paynes_gray',
		'cadet_gray',
	},

	green = {
		'celadon',
		'mindaro',
		'light_green',
		'spring_green',
		'zomp',
		'midnight_green',
	},
	red = {
		'tyrian_purple',
		'cardinal',
		'indian_red',
	},
	blue = {
		'prussian_blue',
	},
	orange = {
		'orange',
		'selective_yellow',
		'sunglow',
	},
	off_white = {
		'misty_rose',
		'cream',
	},
	dark = {
		'rich_black',
		'dark_green',
		'taupe',
	}
}


doublenickel.colors32                 = {}
doublenickel.colors32.button_red      = doublenickel.color32(150, 0, 0, 255)
doublenickel.colors32.button_red_dark = doublenickel.color32(75, 0, 0, 255)
doublenickel.colors32.button_gray     = doublenickel.color32(100, 100, 100, 255)
doublenickel.colors32.button_green    = doublenickel.color32(0, 150, 25, 255)
doublenickel.colors32.gunmetal        = doublenickel.color32(43, 61, 65, 255)
doublenickel.colors32.paynes_gray     = doublenickel.color32(76, 95, 107, 255)
doublenickel.colors32.cadet_gray      = doublenickel.color32(131, 160, 160, 255)

doublenickel.color_pallet             = {
	doublenickel.color(0.35, 0.25, 0.34, 1.00), -- English Violet
	doublenickel.color(0.48, 0.43, 0.66, 1.00), -- Dark Blue Gray
}

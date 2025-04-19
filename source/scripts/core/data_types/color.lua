dn.enum.define(
	'ReadableTextColor', {
		Light = 0,
		Dark = 1
	}
)

function dn.color(r, g, b, a)
	if not r then
		return dn.colors.white:copy()
	elseif ffi.istype('float [4]', r) then
		local float_array = r
		return dn.color(float_array[0], float_array[1], float_array[2], float_array[3])
	elseif type(r) == 'table' then
		local source = r
		if source.r and source.g and source.b and source.a then
			return dn.color(source.r, source.g, source.b, source.a)
		else
			return dn.colors.white:copy()
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
	--  local color = dn.colors.black
	--  color.r = 1.0
	--
	-- Then black is globally modified. It's unintuitive that you have to call :copy(), but it's better
	-- than nothing and avoids too much API churn
	local mt = {
		__index = {
			copy = function(self)
				return dn.color(self)
			end,
			to_ctype = function(self)
				return self:to_vec4()
			end,
			to_vec4 = function(self)
				return ffi.new('dn_vector4_t', self.r, self.g, self.b, self.a)
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
				return dn.color255(self)
			end,
			to_u32 = function(self)
				return dn.color32(math.floor(self.r * 255), math.floor(self.g * 255), math.floor(self.b * 255),
					math.floor(self.a * 255))
			end,
			premultiply = function(self)
				return dn.color(self.r * self.a, self.g * self.a, self.b * self.a, 1.0)
			end,
			alpha = function(self, alpha)
				return dn.color(self.r, self.g, self.b, alpha)
			end,
			readable_color = function(self)
				local sum = self.r + self.g + self.b
				return sum > .75 and dn.enums.ReadableTextColor.Dark or dn.enums.ReadableTextColor.Light
			end,
		}
	}
	setmetatable(color, mt)
	return color
end

function dn.color255(r, g, b, a)
	if type(r) == 'table' then
		local source = r
		r = source.r
		g = source.g
		b = source.b
		a = source.a
	end

	return dn.color(r / 255.0, g / 255.0, b / 255.0, a / 255.0)
end

function dn.color32(r, g, b, a)
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

function dn.color_to_vec4(color)
	return ffi.new('dn_vector4_t', color.r, color.g, color.b, color.a)
end

function dn.coherent_random_color()
	local random = math.random
	local index = random(1, #dn.color_pallet)
	return dn.color_pallet[index]
end

function dn.random_color()
	local random = math.random
	return dn.color(
		random(0, 256),
		random(0, 256),
		random(0, 256),
		255)
end

function dn.is_color_like(t)
	if type(t) ~= 'table' then return false end

	local is_color_like = true
	is_color_like = is_color_like and type(t.r) == 'number'
	is_color_like = is_color_like and type(t.g) == 'number'
	is_color_like = is_color_like and type(t.b) == 'number'
	return is_color_like
end

dn.colors                    = {}
dn.colors.almost_black       = dn.color(0.00, 0.00, 0.10, 1.00)
dn.colors.baby_blue          = dn.color(0.49, 0.55, 0.77, 1.00)
dn.colors.blue               = dn.color(0.00, 0.00, 1.00, 1.00)
dn.colors.blue_light_trans   = dn.color(0.00, 0.00, 1.00, 0.20)
dn.colors.black              = dn.color(0.00, 0.00, 0.00, 1.00)
dn.colors.clear              = dn.color(0.00, 0.00, 0.00, 0.00)
dn.colors.gray_dark          = dn.color(0.10, 0.10, 0.10, 1.00)
dn.colors.gray_medium        = dn.color(0.50, 0.50, 0.50, 1.00)
dn.colors.gray_light         = dn.color(0.75, 0.75, 0.75, 1.00)
dn.colors.gray_light_trans   = dn.color(0.75, 0.75, 0.75, 0.20)
dn.colors.green              = dn.color(0.00, 1.00, 0.00, 1.00)
dn.colors.green_dark         = dn.color(0.04, 0.40, 0.08, 1.00)
dn.colors.green_medium_trans = dn.color(0.00, 1.00, 0.00, 0.50)
dn.colors.green_light_trans  = dn.color(0.00, 1.00, 0.00, 0.25)
dn.colors.grid_bg            = dn.color(0.25, 0.25, 0.30, 0.80)
dn.colors.hovered_button     = dn.color(0.57, 0.57, 0.57, 1.00)
dn.colors.idk                = dn.color(0.00, 0.75, 1.00, 1.00)
dn.colors.idle_button        = dn.color(0.75, 0.75, 0.75, 1.00)
dn.colors.maroon             = dn.color(0.50, 0.08, 0.16, 1.00)
dn.colors.maroon_dark        = dn.color(0.40, 0.04, 0.08, 1.00)
dn.colors.muted_purple       = dn.color(0.35, 0.25, 0.34, 1.00)
dn.colors.pale_red           = dn.color(1.00, 0.10, 0.00, 0.10)
dn.colors.pale_red2          = dn.color(1.00, 0.10, 0.00, 0.50)
dn.colors.pale_green         = dn.color(0.10, 1.00, 0.00, 0.10)
dn.colors.red                = dn.color(1.00, 0.00, 0.00, 1.00)
dn.colors.red_medium_trans   = dn.color(1.00, 0.00, 0.00, 0.50)
dn.colors.red_light_trans    = dn.color(1.00, 0.00, 0.00, 0.25)
dn.colors.red_xlight_trans   = dn.color(1.00, 0.00, 0.00, 0.12)
dn.colors.text               = dn.color(1.00, 1.00, 1.00, 1.00)
dn.colors.text_hl            = dn.color(0.89, 0.91, 0.56, 1.00)
dn.colors.ui_background      = dn.color(0.00, 0.00, 0.00, 1.00)
dn.colors.violet             = dn.color(0.48, 0.43, 0.66, 1.00)
dn.colors.white              = dn.color(1.00, 1.00, 1.00, 1.00)
dn.colors.white_trans        = dn.color(1.00, 1.00, 1.00, 0.20)

-- A real pallette
dn.colors.gunmetal           = dn.color255(43, 61, 65, 255)
dn.colors.paynes_gray        = dn.color255(76, 95, 107, 255)
dn.colors.cadet_gray         = dn.color255(131, 160, 160, 255)
dn.colors.charcoal           = dn.color255(64, 67, 78, 255)
dn.colors.cool_gray          = dn.color255(140, 148, 173, 255)

dn.colors.celadon            = dn.color255(183, 227, 204, 255)
dn.colors.spring_green       = dn.color255(89, 255, 160, 255)
dn.colors.mindaro            = dn.color255(188, 231, 132, 255)
dn.colors.light_green        = dn.color255(161, 239, 139, 255)
dn.colors.zomp               = dn.color255(99, 160, 136, 255)

dn.colors.indian_red         = dn.color255(180, 101, 111, 255)
dn.colors.tyrian_purple      = dn.color255(95, 26, 55, 255)
dn.colors.cardinal           = dn.color255(194, 37, 50, 255)

dn.colors.prussian_blue      = dn.color255(16, 43, 63, 255)
dn.colors.midnight_green     = dn.color255(25, 83, 95, 255)

dn.colors.orange             = dn.color255(249, 166, 32, 255)
dn.colors.sunglow            = dn.color255(255, 209, 102, 255)
dn.colors.selective_yellow   = dn.color255(250, 188, 42, 255)

dn.colors.cream              = dn.color255(245, 255, 198, 255)
dn.colors.misty_rose         = dn.color255(255, 227, 227, 255)

dn.colors.taupe              = dn.color255(68, 53, 39, 255)
dn.colors.dark_green         = dn.color255(4, 27, 21, 255)
dn.colors.rich_black         = dn.color255(4, 10, 15, 255)

dn.colors.pallette = {
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


dn.colors32                 = {}
dn.colors32.button_red      = dn.color32(150, 0, 0, 255)
dn.colors32.button_red_dark = dn.color32(75, 0, 0, 255)
dn.colors32.button_gray     = dn.color32(100, 100, 100, 255)
dn.colors32.button_green    = dn.color32(0, 150, 25, 255)
dn.colors32.gunmetal        = dn.color32(43, 61, 65, 255)
dn.colors32.paynes_gray     = dn.color32(76, 95, 107, 255)
dn.colors32.cadet_gray      = dn.color32(131, 160, 160, 255)

dn.color_pallet             = {
	dn.color(0.35, 0.25, 0.34, 1.00), -- English Violet
	dn.color(0.48, 0.43, 0.66, 1.00), -- Dark Blue Gray
}

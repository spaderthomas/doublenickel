tdengine.enum.define(
	'ReadableTextColor', {
		Light = 0,
		Dark = 1
	}
)

function tdengine.color(r, g, b, a)
	if not r then
		return tdengine.colors.white:copy()
	elseif ffi.istype('float [4]', r) then
		local float_array = r
		return tdengine.color(float_array[0], float_array[1], float_array[2], float_array[3])
	elseif type(r) == 'table' then
		local source = r
		if source.r and source.g and source.b and source.a then
			return tdengine.color(source.r, source.g, source.b, source.a)
		else
			return tdengine.colors.white:copy()
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
	--  local color = tdengine.colors.black
	--  color.r = 1.0
	--
	-- Then black is globally modified. It's unintuitive that you have to call :copy(), but it's better
	-- than nothing and avoids too much API churn
	local mt = {
		__index = {
			copy = function(self)
				return tdengine.color(self)
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
				return tdengine.color255(self)
			end,
			to_u32 = function(self)
				return tdengine.color32(math.floor(self.r * 255), math.floor(self.g * 255), math.floor(self.b * 255),
					math.floor(self.a * 255))
			end,
			premultiply = function(self)
				return tdengine.color(self.r * self.a, self.g * self.a, self.b * self.a, 1.0)
			end,
			alpha = function(self, alpha)
				return tdengine.color(self.r, self.g, self.b, alpha)
			end,
			readable_color = function(self)
				local sum = self.r + self.g + self.b
				return sum > .75 and tdengine.enums.ReadableTextColor.Dark or tdengine.enums.ReadableTextColor.Light
			end,
		}
	}
	setmetatable(color, mt)
	return color
end

function tdengine.color255(r, g, b, a)
	if type(r) == 'table' then
		local source = r
		r = source.r
		g = source.g
		b = source.b
		a = source.a
	end

	return tdengine.color(r / 255.0, g / 255.0, b / 255.0, a / 255.0)
end

function tdengine.color32(r, g, b, a)
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

function tdengine.color_to_vec4(color)
	return ffi.new('Vector4', color.r, color.g, color.b, color.a)
end

function tdengine.coherent_random_color()
	local random = math.random
	local index = random(1, #tdengine.color_pallet)
	return tdengine.color_pallet[index]
end

function tdengine.random_color()
	local random = math.random
	return tdengine.color(
		random(0, 256),
		random(0, 256),
		random(0, 256),
		255)
end

function tdengine.is_color_like(t)
	if type(t) ~= 'table' then return false end

	local is_color_like = true
	is_color_like = is_color_like and type(t.r) == 'number'
	is_color_like = is_color_like and type(t.g) == 'number'
	is_color_like = is_color_like and type(t.b) == 'number'
	return is_color_like
end

tdengine.colors                    = {}
tdengine.colors.almost_black       = tdengine.color(0.00, 0.00, 0.10, 1.00)
tdengine.colors.baby_blue          = tdengine.color(0.49, 0.55, 0.77, 1.00)
tdengine.colors.blue               = tdengine.color(0.00, 0.00, 1.00, 1.00)
tdengine.colors.blue_light_trans   = tdengine.color(0.00, 0.00, 1.00, 0.20)
tdengine.colors.black              = tdengine.color(0.00, 0.00, 0.00, 1.00)
tdengine.colors.clear              = tdengine.color(0.00, 0.00, 0.00, 0.00)
tdengine.colors.gray_dark          = tdengine.color(0.10, 0.10, 0.10, 1.00)
tdengine.colors.gray_medium        = tdengine.color(0.50, 0.50, 0.50, 1.00)
tdengine.colors.gray_light         = tdengine.color(0.75, 0.75, 0.75, 1.00)
tdengine.colors.gray_light_trans   = tdengine.color(0.75, 0.75, 0.75, 0.20)
tdengine.colors.green              = tdengine.color(0.00, 1.00, 0.00, 1.00)
tdengine.colors.green_dark         = tdengine.color(0.04, 0.40, 0.08, 1.00)
tdengine.colors.green_medium_trans = tdengine.color(0.00, 1.00, 0.00, 0.50)
tdengine.colors.green_light_trans  = tdengine.color(0.00, 1.00, 0.00, 0.25)
tdengine.colors.grid_bg            = tdengine.color(0.25, 0.25, 0.30, 0.80)
tdengine.colors.hovered_button     = tdengine.color(0.57, 0.57, 0.57, 1.00)
tdengine.colors.idk                = tdengine.color(0.00, 0.75, 1.00, 1.00)
tdengine.colors.idle_button        = tdengine.color(0.75, 0.75, 0.75, 1.00)
tdengine.colors.maroon             = tdengine.color(0.50, 0.08, 0.16, 1.00)
tdengine.colors.maroon_dark        = tdengine.color(0.40, 0.04, 0.08, 1.00)
tdengine.colors.muted_purple       = tdengine.color(0.35, 0.25, 0.34, 1.00)
tdengine.colors.pale_red           = tdengine.color(1.00, 0.10, 0.00, 0.10)
tdengine.colors.pale_red2          = tdengine.color(1.00, 0.10, 0.00, 0.50)
tdengine.colors.pale_green         = tdengine.color(0.10, 1.00, 0.00, 0.10)
tdengine.colors.red                = tdengine.color(1.00, 0.00, 0.00, 1.00)
tdengine.colors.red_medium_trans   = tdengine.color(1.00, 0.00, 0.00, 0.50)
tdengine.colors.red_light_trans    = tdengine.color(1.00, 0.00, 0.00, 0.25)
tdengine.colors.red_xlight_trans   = tdengine.color(1.00, 0.00, 0.00, 0.12)
tdengine.colors.text               = tdengine.color(1.00, 1.00, 1.00, 1.00)
tdengine.colors.text_hl            = tdengine.color(0.89, 0.91, 0.56, 1.00)
tdengine.colors.ui_background      = tdengine.color(0.00, 0.00, 0.00, 1.00)
tdengine.colors.violet             = tdengine.color(0.48, 0.43, 0.66, 1.00)
tdengine.colors.white              = tdengine.color(1.00, 1.00, 1.00, 1.00)
tdengine.colors.white_trans        = tdengine.color(1.00, 1.00, 1.00, 0.20)

-- A real pallette
tdengine.colors.gunmetal           = tdengine.color255(43, 61, 65, 255)
tdengine.colors.paynes_gray        = tdengine.color255(76, 95, 107, 255)
tdengine.colors.cadet_gray         = tdengine.color255(131, 160, 160, 255)
tdengine.colors.charcoal           = tdengine.color255(64, 67, 78, 255)
tdengine.colors.cool_gray          = tdengine.color255(140, 148, 173, 255)

tdengine.colors.celadon            = tdengine.color255(183, 227, 204, 255)
tdengine.colors.spring_green       = tdengine.color255(89, 255, 160, 255)
tdengine.colors.mindaro            = tdengine.color255(188, 231, 132, 255)
tdengine.colors.light_green        = tdengine.color255(161, 239, 139, 255)
tdengine.colors.zomp               = tdengine.color255(99, 160, 136, 255)

tdengine.colors.indian_red         = tdengine.color255(180, 101, 111, 255)
tdengine.colors.tyrian_purple      = tdengine.color255(95, 26, 55, 255)
tdengine.colors.cardinal           = tdengine.color255(194, 37, 50, 255)

tdengine.colors.prussian_blue      = tdengine.color255(16, 43, 63, 255)
tdengine.colors.midnight_green     = tdengine.color255(25, 83, 95, 255)

tdengine.colors.orange             = tdengine.color255(249, 166, 32, 255)
tdengine.colors.sunglow            = tdengine.color255(255, 209, 102, 255)
tdengine.colors.selective_yellow   = tdengine.color255(250, 188, 42, 255)

tdengine.colors.cream              = tdengine.color255(245, 255, 198, 255)
tdengine.colors.misty_rose         = tdengine.color255(255, 227, 227, 255)

tdengine.colors.taupe              = tdengine.color255(68, 53, 39, 255)
tdengine.colors.dark_green         = tdengine.color255(4, 27, 21, 255)
tdengine.colors.rich_black         = tdengine.color255(4, 10, 15, 255)

tdengine.colors.pallette = {
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


tdengine.colors32                 = {}
tdengine.colors32.button_red      = tdengine.color32(150, 0, 0, 255)
tdengine.colors32.button_red_dark = tdengine.color32(75, 0, 0, 255)
tdengine.colors32.button_gray     = tdengine.color32(100, 100, 100, 255)
tdengine.colors32.button_green    = tdengine.color32(0, 150, 25, 255)
tdengine.colors32.gunmetal        = tdengine.color32(43, 61, 65, 255)
tdengine.colors32.paynes_gray     = tdengine.color32(76, 95, 107, 255)
tdengine.colors32.cadet_gray      = tdengine.color32(131, 160, 160, 255)

tdengine.color_pallet             = {
	tdengine.color(0.35, 0.25, 0.34, 1.00), -- English Violet
	tdengine.color(0.48, 0.43, 0.66, 1.00), -- Dark Blue Gray
}

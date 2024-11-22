local function ease(a, b, t)
	return 0
end

local Interpolator = tdengine.class.define('Interpolator')
tdengine.interpolation.Interpolator = Interpolator
tdengine.add_class_metamethod(
	Interpolator,
	'__call',
	function(_, a, b, t)
		t = ease(t, exp)
		return tdengine.interpolation.Lerp(a, b, t)
	end)

function Interpolator:init(params)
end

function Interpolator:update(dt)
end

function Interpolator:reset()
end

function Interpolator:is_done()
end

function Interpolator:get_value()
end

function Interpolator:set_target(target)
end

function Interpolator:set_start(start)
end

local Interpolator2 = tdengine.class.define('Interpolator2')
tdengine.interpolation.Interpolator2 = Interpolator2
tdengine.add_class_metamethod(Interpolator2, '__call', function(_, a, b, t, exp)
	return tdengine.vec2(
		tdengine.interpolation.Interpolator(a.x, b.x, t, exp),
		tdengine.interpolation.Interpolator(a.y, b.y, t, exp)
	)
end)

function Interpolator2:init(params)
end

function Interpolator2:update(dt)
end

function Interpolator2:reset()
end

function Interpolator2:is_done()
end

function Interpolator2:get_value()
end

function Interpolator2:set_target(target)
end

function Interpolator2:set_start(start)
end

tdengine.enum.define(
	'InterpolationMode',
	{
		Constant = 0,
		Constant2 = 1,
		Lerp = 2,
		Lerp2 = 3,
		SmoothDamp = 4,
		SmoothDamp2 = 5,
		EaseIn = 6,
		EaseIn2 = 7,
		EaseOut = 8,
		EaseOut2 = 9,
		EaseInOut = 10,
		EaseInOut2 = 11,
		EaseInOutBounce = 12,
		EaseInOutBounce2 = 13,
		Exponential = 14,
		Exponential2 = 15,
		Parabolic = 16,
		Parabolic2 = 17,
	}
)

function tdengine.interpolation.interpolate(mode, a, b, t, ...)
	mode = mode or tdengine.enums.InterpolationMode.Lerp

	local class = tdengine.class.find(mode:to_string())
	if not class then
		log.error(string.format("Couldn't find interpolation class for mode %s", mode:to_string())); return
	end

	return class(a, b, t, ...)
end

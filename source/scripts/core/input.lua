




doublenickel.input.channels = {
	editor = 'editor',
	game = 'game',
	gui = 'gui',
	main_menu = 'main_menu',
	feedback = 'feedback',
	any = 'any'
}

doublenickel.input.device_kinds = {
	mkb = 0,
	controller = 1,
}

function doublenickel.input.pressed(key, channel)
	if not doublenickel.input.is_channel_active(channel) then
		return false
	end

	return dn.input_pressed(key)
end

function doublenickel.input.released(key, channel)
	if not doublenickel.input.is_channel_active(channel) then
		return false
	end

	return dn.input_released(key)
end

function doublenickel.input.down(key, channel)
	if not doublenickel.input.is_channel_active(channel) then
		return false
	end

	return dn.input_down(key)
end

function doublenickel.input.mod_down(key, channel)
	if not doublenickel.input.is_channel_active(channel) then
		return false
	end

	return dn.input_mod_down(key)
end

function doublenickel.input.chord_pressed(mod, key, channel)
	if not doublenickel.input.is_channel_active(channel) then
		return false
	end

	return dn.input_chord_pressed(mod, key)
end

function doublenickel.input.scroll()
	return dn.input_scroll()
end

function doublenickel.input.mouse(coordinate)
	coordinate = coordinate or CoordinateSystem.World
	return dn.input_mouse(coordinate:to_number())
end

function doublenickel.input.mouse_delta(coordinate)
	coordinate = coordinate or CoordinateSystem.World
	return dn.input_mouse_delta(coordinate:to_number())
end


--------------
-- CHANNELS --
--------------
function doublenickel.input.enable_channel(channel)
	doublenickel.input.data.channels[channel] = true
end

function doublenickel.input.disable_channel(channel)
	doublenickel.input.data.channels[channel] = false
end

function doublenickel.input.solo_channel(channel)
	for channel, _ in pairs(doublenickel.input.data.channels) do
		doublenickel.input.disable_channel(channel)
	end

	doublenickel.input.enable_channel(channel)
end

function doublenickel.input.is_channel_active(channel)
	return true
end

---------------
-- INTERNALS --
---------------
local self = doublenickel.input


doublenickel.enum.define(
	'InputContext',
	{
		Editor = 0,
		Game = 1,
	}
)


function doublenickel.input.init()
	self.internal = {}
	self.internal.context_stack = doublenickel.data_types.stack:new()
end

function doublenickel.input.update()
	if not doublenickel.tick then
		self.internal.context_stack:clear()

		local view = doublenickel.find_entity_editor('GameViewManager')
		if view.hover then
			self.push_context(doublenickel.enums.InputContext.Game)
		else
			self.push_context(doublenickel.enums.InputContext.Editor)
		end
	end
end


function doublenickel.input.push_context(context)
	self.internal.context_stack:push(context)
end

function doublenickel.input.pop_context()
	self.internal.context_stack:pop()
end

 
function doublenickel.input.active_context()
	return self.internal.context_stack:peek()
end

function doublenickel.input.is_context_active(context)
	return self.internal.context_stack:peek() == context
end

function doublenickel.input.get_input_device()
	return InputDevice:new(dn.input_get_device())
end

function doublenickel.input.is_controller_mode()
	return doublenickel.input.get_input_device() == InputDevice.Controller
end

function doublenickel.input.is_mkb_mode()
	return doublenickel.input.get_input_device() == InputDevice.MouseAndKeyboard
end

-- @refactor: This is action specific. Maybe it's OK here...?
function doublenickel.input.is_digital_active(name)
	return doublenickel.ffi.is_digital_active(name)
end

function doublenickel.input.was_digital_active(name)
	return doublenickel.ffi.was_digital_active(name)
end

function doublenickel.input.was_digital_pressed(name)
	return doublenickel.ffi.was_digital_pressed(name)
end

ContextualInput = doublenickel.class.define('ContextualInput')

function ContextualInput:init(context, coordinate)
	self.context = context or doublenickel.enums.InputContext.Game
	self.coordinate = coordinate or doublenickel.enums.CoordinateSystem.World
end

function ContextualInput:pressed(key)
	if doublenickel.input.is_context_active(self.context) then
		return dn.input_pressed(key)
	end

	return false
end

function ContextualInput:released(key)
	if doublenickel.input.is_context_active(self.context) then
		return dn.input_released(key)
	end

	return false
end

function ContextualInput:down(key)
	if doublenickel.input.is_context_active(self.context) then
		return dn.input_down(key)
	end

	return false
end

function ContextualInput:mod_down(key)
	if doublenickel.input.is_context_active(self.context) then
		return dn.input_mod_down(key)
	end

	return false
end

function ContextualInput:chord_pressed(mod, key)
	if doublenickel.input.is_context_active(self.context) then
		return dn.input_chord_pressed(mod, key)
	end

	return false
end

function ContextualInput:scroll()
	if doublenickel.input.is_context_active(self.context) then
		return doublenickel.vec2(dn.input_scroll())
	end

	return doublenickel.vec2()
end

function ContextualInput:mouse(coordinate)
	coordinate = coordinate or self.coordinate
	return doublenickel.vec2(dn.input_mouse(coordinate:to_number()))
end

function ContextualInput:mouse_delta(coordinate)
	coordinate = coordinate or self.coordinate
	return dn.input_mouse_delta(coordinate:to_number())
end

function ContextualInput:left_click()
	return self:pressed(glfw.keys.MOUSE_BUTTON_1)
end

function ContextualInput:right_click()
	return self:pressed(glfw.keys.MOUSE_BUTTON_2)
end
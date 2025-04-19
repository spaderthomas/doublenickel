




dn.input.channels = {
	editor = 'editor',
	game = 'game',
	gui = 'gui',
	main_menu = 'main_menu',
	feedback = 'feedback',
	any = 'any'
}

dn.input.device_kinds = {
	mkb = 0,
	controller = 1,
}

function dn.input.pressed(key, channel)
	if not dn.input.is_channel_active(channel) then
		return false
	end

	return dn.ffi.input_pressed(key)
end

function dn.input.released(key, channel)
	if not dn.input.is_channel_active(channel) then
		return false
	end

	return dn.ffi.input_released(key)
end

function dn.input.down(key, channel)
	if not dn.input.is_channel_active(channel) then
		return false
	end

	return dn.ffi.input_down(key)
end

function dn.input.mod_down(key, channel)
	if not dn.input.is_channel_active(channel) then
		return false
	end

	return dn.ffi.input_mod_down(key)
end

function dn.input.chord_pressed(mod, key, channel)
	if not dn.input.is_channel_active(channel) then
		return false
	end

	return dn.ffi.input_chord_pressed(mod, key)
end

function dn.input.scroll()
	return dn.ffi.input_scroll()
end

function dn.input.mouse(coordinate)
	coordinate = coordinate or CoordinateSystem.World
	return dn.ffi.input_mouse(coordinate:to_number())
end

function dn.input.mouse_delta(coordinate)
	coordinate = coordinate or CoordinateSystem.World
	return dn.ffi.input_mouse_delta(coordinate:to_number())
end


--------------
-- CHANNELS --
--------------
function dn.input.enable_channel(channel)
	dn.input.data.channels[channel] = true
end

function dn.input.disable_channel(channel)
	dn.input.data.channels[channel] = false
end

function dn.input.solo_channel(channel)
	for channel, _ in pairs(dn.input.data.channels) do
		dn.input.disable_channel(channel)
	end

	dn.input.enable_channel(channel)
end

function dn.input.is_channel_active(channel)
	return true
end

---------------
-- INTERNALS --
---------------
local self = dn.input


dn.enum.define(
	'InputContext',
	{
		Editor = 0,
		Game = 1,
	}
)


function dn.input.init()
	self.internal = {}
	self.internal.context_stack = dn.data_types.stack:new()
end

function dn.input.update()
	if not dn.tick then
		self.internal.context_stack:clear()

		local view = dn.find_entity_editor('GameViewManager')
		if view.hover then
			self.push_context(dn.enums.InputContext.Game)
		else
			self.push_context(dn.enums.InputContext.Editor)
		end
	end
end


function dn.input.push_context(context)
	self.internal.context_stack:push(context)
end

function dn.input.pop_context()
	self.internal.context_stack:pop()
end

 
function dn.input.active_context()
	return self.internal.context_stack:peek()
end

function dn.input.is_context_active(context)
	return self.internal.context_stack:peek() == context
end

function dn.input.get_input_device()
	return InputDevice:new(dn.ffi.input_get_device())
end

function dn.input.is_controller_mode()
	return dn.input.get_input_device() == InputDevice.Controller
end

function dn.input.is_mkb_mode()
	return dn.input.get_input_device() == InputDevice.MouseAndKeyboard
end

-- @refactor: This is action specific. Maybe it's OK here...?
function dn.input.is_digital_active(name)
	return dn.unported.is_digital_active(name)
end

function dn.input.was_digital_active(name)
	return dn.unported.was_digital_active(name)
end

function dn.input.was_digital_pressed(name)
	return dn.unported.was_digital_pressed(name)
end

ContextualInput = dn.class.define('ContextualInput')

function ContextualInput:init(context, coordinate)
	self.context = context or dn.enums.InputContext.Game
	self.coordinate = coordinate or dn.enums.CoordinateSystem.World
end

function ContextualInput:pressed(key)
	if dn.input.is_context_active(self.context) then
		return dn.ffi.input_pressed(key)
	end

	return false
end

function ContextualInput:released(key)
	if dn.input.is_context_active(self.context) then
		return dn.ffi.input_released(key)
	end

	return false
end

function ContextualInput:down(key)
	if dn.input.is_context_active(self.context) then
		return dn.ffi.input_down(key)
	end

	return false
end

function ContextualInput:mod_down(key)
	if dn.input.is_context_active(self.context) then
		return dn.ffi.input_mod_down(key)
	end

	return false
end

function ContextualInput:chord_pressed(mod, key)
	if dn.input.is_context_active(self.context) then
		return dn.ffi.input_chord_pressed(mod, key)
	end

	return false
end

function ContextualInput:scroll()
	if dn.input.is_context_active(self.context) then
		return dn.vec2(dn.ffi.input_scroll())
	end

	return dn.vec2()
end

function ContextualInput:mouse(coordinate)
	coordinate = coordinate or self.coordinate
	return dn.vec2(dn.ffi.input_mouse(coordinate:to_number()))
end

function ContextualInput:mouse_delta(coordinate)
	coordinate = coordinate or self.coordinate
	return dn.ffi.input_mouse_delta(coordinate:to_number())
end

function ContextualInput:left_click()
	return self:pressed(glfw.keys.MOUSE_BUTTON_1)
end

function ContextualInput:right_click()
	return self:pressed(glfw.keys.MOUSE_BUTTON_2)
end
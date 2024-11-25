




tdengine.input.channels = {
	editor = 'editor',
	game = 'game',
	gui = 'gui',
	main_menu = 'main_menu',
	feedback = 'feedback',
	any = 'any'
}

tdengine.input.device_kinds = {
	mkb = 0,
	controller = 1,
}

function tdengine.input.pressed(key, channel)
	if not tdengine.input.is_channel_active(channel) then
		return false
	end

	return tdengine.ffi.dn_input_pressed(key)
end

function tdengine.input.released(key, channel)
	if not tdengine.input.is_channel_active(channel) then
		return false
	end

	return tdengine.ffi.dn_input_released(key)
end

function tdengine.input.down(key, channel)
	if not tdengine.input.is_channel_active(channel) then
		return false
	end

	return tdengine.ffi.dn_input_down(key)
end

function tdengine.input.mod_down(key, channel)
	if not tdengine.input.is_channel_active(channel) then
		return false
	end

	return tdengine.ffi.dn_input_mod_down(key)
end

function tdengine.input.chord_pressed(mod, key, channel)
	if not tdengine.input.is_channel_active(channel) then
		return false
	end

	return tdengine.ffi.dn_input_chord_pressed(mod, key)
end

function tdengine.input.scroll()
	return tdengine.ffi.dn_input_scroll()
end

function tdengine.input.mouse(coordinate)
	coordinate = coordinate or CoordinateSystem.World
	return tdengine.ffi.dn_input_mouse(coordinate:to_number())
end

function tdengine.input.mouse_delta(coordinate)
	coordinate = coordinate or CoordinateSystem.World
	return tdengine.ffi.dn_input_mouse_delta(coordinate:to_number())
end


--------------
-- CHANNELS --
--------------
function tdengine.input.enable_channel(channel)
	tdengine.input.data.channels[channel] = true
end

function tdengine.input.disable_channel(channel)
	tdengine.input.data.channels[channel] = false
end

function tdengine.input.solo_channel(channel)
	for channel, _ in pairs(tdengine.input.data.channels) do
		tdengine.input.disable_channel(channel)
	end

	tdengine.input.enable_channel(channel)
end

function tdengine.input.is_channel_active(channel)
	return true
end

---------------
-- INTERNALS --
---------------
local self = tdengine.input


tdengine.enum.define(
	'InputContext',
	{
		Editor = 0,
		Game = 1,
	}
)


function tdengine.input.init()
	self.internal = {}
	self.internal.context_stack = tdengine.data_types.stack:new()
end

function tdengine.input.update()
	if not tdengine.tick then
		self.internal.context_stack:clear()

		local view = tdengine.find_entity_editor('GameViewManager')
		if view.hover then
			self.push_context(tdengine.enums.InputContext.Game)
		else
			self.push_context(tdengine.enums.InputContext.Editor)
		end
	end
end


function tdengine.input.push_context(context)
	self.internal.context_stack:push(context)
end

function tdengine.input.pop_context()
	self.internal.context_stack:pop()
end

 
function tdengine.input.active_context()
	return self.internal.context_stack:peek()
end

function tdengine.input.is_context_active(context)
	return self.internal.context_stack:peek() == context
end

function tdengine.input.get_input_device()
	return InputDevice:new(tdengine.ffi.get_input_device())
end

function tdengine.input.is_controller_mode()
	return tdengine.input.get_input_device() == InputDevice.Controller
end

function tdengine.input.is_mkb_mode()
	return tdengine.input.get_input_device() == InputDevice.MouseAndKeyboard
end

-- @refactor: This is action specific. Maybe it's OK here...?
function tdengine.input.is_digital_active(name)
	return tdengine.ffi.is_digital_active(name)
end

function tdengine.input.was_digital_active(name)
	return tdengine.ffi.was_digital_active(name)
end

function tdengine.input.was_digital_pressed(name)
	return tdengine.ffi.was_digital_pressed(name)
end

ContextualInput = tdengine.class.define('ContextualInput')

function ContextualInput:init(context, coordinate)
	self.context = context or tdengine.enums.InputContext.Game
	self.coordinate = coordinate or tdengine.enums.CoordinateSystem.World
end

function ContextualInput:pressed(key)
	if tdengine.input.is_context_active(self.context) then
		return tdengine.ffi.dn_input_pressed(key)
	end

	return false
end

function ContextualInput:released(key)
	if tdengine.input.is_context_active(self.context) then
		return tdengine.ffi.dn_input_released(key)
	end

	return false
end

function ContextualInput:down(key)
	if tdengine.input.is_context_active(self.context) then
		return tdengine.ffi.dn_input_down(key)
	end

	return false
end

function ContextualInput:mod_down(key)
	if tdengine.input.is_context_active(self.context) then
		return tdengine.ffi.dn_input_mod_down(key)
	end

	return false
end

function ContextualInput:chord_pressed(mod, key)
	if tdengine.input.is_context_active(self.context) then
		return tdengine.ffi.dn_input_chord_pressed(mod, key)
	end

	return false
end

function ContextualInput:scroll()
	if tdengine.input.is_context_active(self.context) then
		return tdengine.vec2(tdengine.ffi.dn_input_scroll())
	end

	return tdengine.vec2()
end

function ContextualInput:mouse(coordinate)
	coordinate = coordinate or self.coordinate
	return tdengine.vec2(tdengine.ffi.dn_input_mouse(coordinate:to_number()))
end

function ContextualInput:mouse_delta(coordinate)
	coordinate = coordinate or self.coordinate
	return tdengine.ffi.dn_input_mouse_delta(coordinate:to_number())
end

function ContextualInput:left_click()
	return self:pressed(glfw.keys.MOUSE_BUTTON_1)
end

function ContextualInput:right_click()
	return self:pressed(glfw.keys.MOUSE_BUTTON_2)
end
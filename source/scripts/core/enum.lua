function doublenickel.enum.init()
end

local enum_ignore = {
	__enum = true,
	__metatable = true,
}

doublenickel.internal.enum_value_metatable = {
	__eq = function(a, b)
		local type_of_a = type(a)
		local type_of_b = type(b)

		local tb = type(b) == 'table'
		if not tb then return false end

		local ea = a.__enum
		local eb = b.__enum
		if not eb then return false end
		if ea ~= eb then return false end

		return a:to_number() == b:to_number()
	end,
	__tostring = function(self)
		return self:to_string()
	end,
}

doublenickel.internal.enum_proxy_metatable = {
	__index = function(self, key)
		local value = rawget(doublenickel.enum_data[self.name], key)
		if not value then return nil end

		return table.deep_copy(value)
	end,
	__call = function(self, value)
		for as_enum, as_string, as_number in self:iterate() do
			if as_string == value or as_number == value then
				return self[as_string]
			end
		end
	end
}

function doublenickel.enum.define(enum_name, values)
	-- This is what we fill with KVPs for the actual enum values
	doublenickel.enum_data[enum_name] = {}

	-- This is just a proxy which forwards to the actual data table. We can declare static methods for the enum
	-- class here; everything else goes to __index
	doublenickel.enums[enum_name] = {
		name = enum_name,
		iterate = function(self)
			local function iterator()
				for str, enum in pairs(doublenickel.enum_data[enum_name]) do
					coroutine.yield(enum, enum:to_string(), enum:to_number())
				end
			end

			return coroutine.wrap(iterator)
		end,
		match = function(_, other)
			if not doublenickel.enum.is_enum(other) then 
				return false 
			end

			return doublenickel.enums[other.__enum].name == enum_name
		end,
		new = function(self, value)
			for enum in self:iterate() do
				if enum:to_number() == value then return enum end
			end
		end
	}
	setmetatable(doublenickel.enums[enum_name], doublenickel.internal.enum_proxy_metatable)

	-- These are the entries for each enum value
	for string_id, value in pairs(values) do
		doublenickel.enum_data[enum_name][string_id] = {
			to_string = function()
				return string_id
			end,
			to_qualified_string = function() 
				return string.format('%s::%s', enum_name, string_id) 
			end,
			to_number = function()
				return value
			end,
			match = function(self, check_value)
				return check_value == string_id or check_value == value or check_value == self
			end,
			__editor = {
				ignore = enum_ignore
			},
			__enum = enum_name
		}

		setmetatable(doublenickel.enum_data[enum_name][string_id], doublenickel.internal.enum_value_metatable)
	end

	return doublenickel.enums[enum_name]
end

function doublenickel.enum.define_from_ctype(ctype)
	local type_info = dn.reflection.typeof(ctype)

	local prefix = string.format('%s_', ctype)
	local values = {}
	for value in type_info:values() do
		local cleaned_name = value.name:gsub(prefix, '')
		values[cleaned_name] = value.value
	end

	return doublenickel.enum.define(ctype, values)
end

function doublenickel.enum.is_enum(v)
	if type(v) ~= 'table' then return false end
	if not v.__enum then return false end
	return true
end

function doublenickel.enum.load(serialized_enum)
	if not serialized_enum then return nil end

	local enum_class = doublenickel.enum_data[serialized_enum.__enum]
	if not enum_class then return nil end

	return enum_class[serialized_enum.value or serialized_enum:to_string()] -- @hack
end

function doublenickel.enum.bitwise_and(...)
	local args = table.pack(...)

	local result = 0
	for i = 1, args.n do
		local enum_value = args[i]
		result = bitwise(doublenickel.op_and, result, enum_value:to_number())
	end

	return result
end

function doublenickel.enum.bitwise_or(...)
	local args = table.pack(...)

	local result = 0
	for i = 1, args.n do
		local enum_value = args[i]
		result = bitwise(doublenickel.op_or, result, enum_value:to_number())
	end

	return result
end

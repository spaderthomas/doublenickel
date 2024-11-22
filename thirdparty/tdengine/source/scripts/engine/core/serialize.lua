function tdengine.serialize_field(value)
	if type(value) == 'table' and value.serialize then
		return value:serialize()
	elseif type(value) == 'table' and value.__enum then
		return {
			__enum = value.__enum,
			value = value:to_string()
		}
	elseif type(value) == 'table' then
		return tdengine.serialize_table(value)
	elseif type(value) == 'function' then
		return nil
	else
		return deep_copy_any(value)
	end
end

function tdengine.serialize_table(t)
	local data = {}
	for key, value in pairs(t) do
		data[key] = tdengine.serialize_field(value)
	end

	return data
end

function tdengine.serialize_fields(t, fields, data)
	if not t then return end
	if not fields then return end

	for _, field in pairs(fields) do
		local value = t[field]
		if type(value) == 'table' and value.serialize then
			data[field] = value:serialize()
		elseif type(value) == 'table' and value.__enum then
			data[field] = {
				__enum = value.__enum,
				value = value:to_string()
			}
		elseif type(value) == 'function' then
			goto continue
		elseif type(value) == 'cdata' then
			data[field] = {}
			tdengine.serialize_fields(value, value.editor_fields, data[field])
		else
			data[field] = deep_copy_any(value)
		end

		::continue::
	end
end

local serialize_metadata = function(self, data)
	data.uuid = self.uuid
	data.name = self.name
	data.tag = self.tag and #self.tag > 0 and self.tag or nil
end

local serialize_components = function(self, data)
	-- Only serialize the entity's declared components; that way, anything added at runtime, or saved
	-- in a previous version where the entity declared that components, will not persist.
	local entity_type = tdengine.types[self.name]
	if entity_type.components then
		data.components = {}

		for index, component_name in pairs(entity_type.components) do
			local component = self.components[component_name]
			if not component then goto continue end

			local serialized_component = {}
			serialize_metadata(component, serialized_component)
			tdengine.serialize_fields(component, component.editor_fields, serialized_component)
			if component.serialize then
				local extra = component:serialize()
				for k, v in pairs(extra) do
					serialized_component[k] = tdengine.serialize_field(v)
				end
			end

			data.components[component_name] = serialized_component

			::continue::
		end
	end
end

function tdengine.serialize_entity(entity)
	local data = {}
	serialize_metadata(entity, data)
	tdengine.serialize_fields(entity, entity.editor_fields, data)
	serialize_components(entity, data)

	return data
end


function tdengine.deserialize_field(value)
	if type(value) == 'table' and value.__enum then
		return tdengine.enum.load(value)
	else
		return deep_copy_any(value)
	end
end

function tdengine.deserialize_entity(entity, serialized_entity)
	for key, value in pairs(serialized_entity) do
		entity[key] = tdengine.deserialize_field(value)
	end
end
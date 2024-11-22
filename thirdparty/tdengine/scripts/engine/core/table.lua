function table.shallow_copy(t)
	local t2 = {}
	for k, v in pairs(t) do
		t2[k] = v
	end
	return t2
end

function table.merge(source, dest)
	for k in tdengine.iterator.keys(source) do
		if type(source[k]) == 'table' then
			-- If the child table doesn't exist, create it
			dest[k] = dest[k] or {}

			-- Then, if the field isn't a table, we know that it already existed in the destination table but wasn't
			-- a table. Respect that, and don't overwrite it.
			if type(dest[k]) == 'table' then
				table.merge(source[k], dest[k])
			end
		else
			dest[k] = source[k]
		end
	end
end

function table.deep_copy(t)
	if not t then return {} end

	local t2 = {}
	for k, v in pairs(t) do
		if type(v) == 'table' then
			t2[k] = table.deep_copy(v)
		else
			t2[k] = v
		end
	end

	local mt = getmetatable(t)
	if mt then setmetatable(t2, mt) end
	return t2
end

function table.flatten(t, parent)
	function child_key(parent, child)
		if #parent > 0 then
			return string.format('%s.%s', parent, child)
		else
			return child
		end
	end

	parent = parent or ''
	local flat = {}
	for name, value in pairs(t) do
		if type(value) == 'table' then
			local children = table.flatten(value, child_key(parent, name))
			for i, child in pairs(children) do
				table.insert(flat, child)
			end
		else
			table.insert(flat, child_key(parent, name))
		end
	end

	return flat
end

function table.replace(dest, source)
	for k, v in pairs(source) do
		dest[k] = v
	end
end

function table.clear(t)
	for k, v in pairs(t) do
		t[k] = nil
	end
end

function table.append(arr, append)
	for i, v in pairs(append) do
		table.insert(arr, v)
	end
end

function table.empty(t)
	return next(t) == nil
end

function table.serialize(t, pretty)
	if pretty then
		local options = {
			comment = false
		}
		return serpent.block(t, options)
	else
		return plaindump(t)
	end
end

function table.pack(...)
	return { n = select("#", ...), ... }
end

function table.address(t)
	return table_address(t)
end

function table.get_or_nil(t, ...)
	local args = table.pack(...)
	for i = 1, args.n do
		if not t then return end
		t = t[args[i]]
	end

	return t
end

function table.collect_keys(t)
	local keys = {}
	for key in tdengine.iterator.keys(t) do
		table.insert(keys, key)
	end
	return keys
end

function table.random_key(t)
	local keys = {}
  for key in tdengine.iterator.keys(t) do 
		table.insert(keys, key) 
	end

  return keys[math.random(#keys)]
end

function table.random_value(t)
	return t[table.random_key(t)]
end


function table_address(t)
	if not t then return '0x00000000' end

	local to_string = nil
	local mt = getmetatable(t)
	if mt and mt.__tostring then
		to_string = mt.__tostring
		mt.__tostring = nil
	end

	local address = split(tostring(t), ' ')[2]

	if to_string then
		mt.__tostring = to_string
	end

	return address
end

function deep_copy_any(v)
	if type(v) == 'table' then return table.deep_copy(v) else return v end
end

function index_string(t, ks)
	if not t then dbg() end
	if not ks then dbg() end

	local value = t
	local keys = split(ks, '.')
	for i, key in pairs(keys) do
		if not value then return nil end
		value = value[key]
	end

	return value
end

function table_eq_shallow(t1, t2)
	for k, t1v in pairs(t1) do
		t2v = t2[k]
		if t1v ~= t2v then return false end
	end
	return true
end

function hash_table_entry(t, k)
	return string.format('%s.%s', table_address(t), k)
end

function dumb_hash(v)
	if type(v) == 'string' then
		return v
	elseif type(v) == 'number' then
		return tostring(v)
	elseif type(v) == 'boolean' then
		return tostring(v)
	elseif type(v) == 'table' then
		return table_address(v)
	end
end

function is_string(v)
	return type(v) == 'string'
end

function is_bool(v)
	return type(v) == 'boolean'
end

function is_number(v)
	return type(v) == 'number'
end

-- Copy all keys in source and all subtables into dest. Do not copy values
-- that already exist in dest. Also, remove all keys in dest that are not in
-- source. In other words, source is the canonical form, make dest comply.
function make_keys_match(source, dest)
	if source == nil then
		tdengine.log('make_keys_match: nil source table')
		return
	end

	if dest == nil then
		tdengine.log('make_keys_match: nil dest table')
		return
	end

	return add_source_keys(source, dest) or remove_dest_keys(source, dest)
end

--------------
-- INTERNAL --
--------------
function add_source_keys(source, dest)
	-- Copy source keys into dest
	local changed = false

	for key, value in pairs(source) do
		local continue = false

		-- Dest table doesn't have this key at all. Whether it's a table,
		-- or a single state field, we want to copy it from source
		if dest[key] == nil then
			dest[key] = dest[key] or source[key]
			changed = true
			continue = true
		end

		-- Both tables could have a child table that have different keys,
		-- so we must recurse
		if type(value) == 'table' and not continue then
			changed = changed or add_source_keys(source[key], dest[key])
		end
	end

	return changed
end

function remove_dest_keys(source, dest)
	-- Remove dest keys that are not in source
	local changed = false

	for key, value in pairs(dest) do
		local continue = false

		if source[key] == nil then
			dest[key] = nil
			changed = true
			continue = true
		end

		if type(value) == 'table' and not continue then
			changed = changed or remove_dest_keys(source[key], dest[key])
		end
	end

	return changed
end

---------------
-- ITERATORS --
---------------
function rpairs_iterator(t, i)
	i = i - 1
	if i == 0 then return nil end
	return i, t[i]
end

function pairs_iterator(t, i)
	if i >= #t then return nil end

	i = i + 1
	return i, t[i]
end

function rpairs(t)
	return rpairs_iterator, t, #t + 1
end

--------------
-- PRINTING --
--------------
-- https://github.com/pkulchenko/serpent/issues/23
-- Lots of thanks to @pkulchenko, the author of Serpent, who also posted this one-off quick
-- serializer for data that doesn't have self-referencing tables. It is *much* faster than
-- serpent.block()
function plaindump(val, opts, done)
	local keyignore = opts and opts.keyignore or {}
	local final = done == nil
	opts, done = opts or {}, done or {}
	local t = type(val)
	if t == "table" then
		done[#done + 1] = '{'
		done[#done + 1] = ''
		for key, value in pairs(val) do
			if not keyignore[key] then
				done[#done + 1] = '['
				plaindump(key, opts, done)
				done[#done + 1] = ']='
				plaindump(value, opts, done)
				done[#done + 1] = ","
			end
		end
		done[#done] = '}'
	elseif t == "string" then
		done[#done + 1] = ("%q"):format(val):gsub("\010", "n"):gsub("\026", "\\026")
	elseif t == "number" then
		done[#done + 1] = ("%.17g"):format(val)
	else
		done[#done + 1] = tostring(val)
	end
	return final and table.concat(done, '')
end

-- https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
-- Thanks to Alundaio on Stack Overflow for the code which I adapted to make this. It doesn't do self-referential tables
-- well, but it doesn't choke on them like plaindump() does
function print_table(node)
	local cache, stack, output = {}, {}, {}
	local depth = 1
	local output_str = "{\n"

	while true do
		-- The algorithm works like this: Calculate the number of keys in the table. Iterate over a table's keys, assigning
		-- each key a sequential index. When we encounter a subtable, cache the current index and begin printing the subtable.
		-- When the subtable is done, return to this table and begin again at the cached index.
		local size = 0
		for k, v in pairs(node) do
			size = size + 1
		end

		local cur_index = 1
		for k, v in pairs(node) do
			if (cache[node] == nil) or (cur_index >= cache[node]) then
				if (string.find(output_str, "}", output_str:len())) then
					output_str = output_str .. ",\n"
				elseif not (string.find(output_str, "\n", output_str:len())) then
					output_str = output_str .. "\n"
				end

				table.insert(output, output_str)
				output_str = ''

				local key = tostring(k)

				if type(v) == 'number' or type(v) == 'boolean' then
					output_str = string.format('%s %s %s = %s', output_str, string.rep(' ', depth), key, tostring(v))
				elseif type(v) == 'string' then
					output_str = string.format('%s %s %s = "%s"', output_str, string.rep(' ', depth), key, tostring(v))
				elseif (type(v) == 'table') then
					output_str = string.format('%s %s %s = {\n', output_str, string.rep(' ', depth), key)

					-- We'll immediately pop off this subtable and format it at the bottom of the function. We push the
					-- current node onto the stack as well, so that when the subtable is finished we return to the rest
					-- of this table
					table.insert(stack, node)
					table.insert(stack, v)

					-- Keep track of which index we were on for when we return to this table
					cache[node] = cur_index + 1
					break
				end

				if (cur_index == size) then
					output_str = string.format('%s \n %s }', output_str, string.rep(' ', depth - 1))
				else
					output_str = output_str .. ","
				end
			else
				if (cur_index == size) then
					output_str = string.format('%s \n %s }', output_str, string.rep(' ', depth - 1))
				end
			end

			cur_index = cur_index + 1
		end

		if (size == 0) then
			output_str = string.format('%s \n %s }', output_str, string.rep(' ', depth - 1))
		end

		if (#stack > 0) then
			node = stack[#stack]
			stack[#stack] = nil
			depth = cache[node] == nil and depth + 1 or depth - 1
		else
			break
		end
	end

	table.insert(output, output_str)
	output_str = table.concat(output)

	return output_str
end

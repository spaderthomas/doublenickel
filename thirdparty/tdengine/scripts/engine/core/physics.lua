----------------
-- PUBLIC API --
----------------
function tdengine.physics.move(eid, delta)
	local request = {
		eid = eid,
		delta = delta
	}
	table.insert(tdengine.physics.requests, request)
end

function tdengine.physics.is_point_inside(point, position, size)
	-- Not really a physics function, but I don't know where else to put it
	local right = point.x > position.x
	local left = point.x < position.x + size.x
	local below = point.y < position.y
	local above = point.y > position.y - size.y
	local inside = right and left and above and below
	return inside
end


----------------
-- ALGORITHMS --
----------------
function tdengine.physics.sat(ca, cb)
	local collision = true
	local penetration = tdengine.vec2()

	local axes = {}
	table.append(axes, ca:find_sat_axes(cb))
	table.append(axes, cb:find_sat_axes(ca))

	local min_overlap = tdengine.really_large_number
	for index, axis in pairs(axes) do
		local min_a, max_a = ca:project(axis)
		local min_b, max_b = cb:project(axis)

		-- Check for overlap. If any axis doesn't overlap, they're not colliding.
		local ab = max_a >= min_b and min_a <= max_b
		local ba = max_b >= min_a and min_b <= max_a
		if not (ab and ba) then
			collision = false
			break
		end

		-- Check for depth of overlap along this axis
		local head = math.max(min_a, min_b)
		local tail = math.min(max_a, max_b)
		local overlap = tail - head

		if min_overlap >= overlap then
			local ca = (min_a + max_a) / 2
			local cb = (min_b + max_b) / 2
			local scale = 1
			if cb > ca then scale = -1 end
			penetration.x = overlap * axis.x * scale
			penetration.y = overlap * axis.y * scale
			min_overlap = overlap
		end
	end

	return collision, penetration
end


------------
-- UPDATE --
------------
function tdengine.physics.update(dt)
	local colliders = tdengine.find_all_components('Collider')

	-- Reset the hit flag for all colliders
	for _, collider in pairs(colliders) do
		collider.hit = false
	end

	for index, request in pairs(tdengine.physics.requests) do
		-- If something was deleted between the time of the request and the processing,
		-- just ignore this request
		local entity = tdengine.find_entity_by_id(request.eid)
		if not entity then goto skip_request end

		local collider = entity:find_component('Collider')
		if not collider then goto skip_request end

		-- Apply the delta
		collider:move(request.delta)

		-- Then check for collisions, and resolve if necessary
		for _, other in pairs(colliders) do
			local skip_collider = false
			skip_collider = tdengine.physics.debug or skip_collider
			skip_collider = collider.uuid == other.uuid or skip_collider
			skip_collider = other.kind == tdengine.enums.ColliderKind.Bypass or skip_collider
			if skip_collider then goto skip_collider end

			local skip_resolution = false
			skip_resolution = other.kind == tdengine.enums.ColliderKind.Trigger or skip_resolution

			local collision, penetration = tdengine.physics.sat(collider, other)
			if collision then
				if not skip_resolution then
					collider:move(penetration)
				end

				collider.hit = true
				other.hit = true

				if collider.on_collision then collider.on_collision(other) end
				if other.on_collision then other.on_collision(collider) end
			end

			::skip_collider::
		end

		::skip_request::
	end

	tdengine.physics.requests = {}
end

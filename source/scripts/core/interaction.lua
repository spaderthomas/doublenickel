-- Mark all interactions for checking. This is a pull model. This will check the player's interaction
-- geometry against all other interactions.
function doublenickel.interaction.check()
	doublenickel.interaction.check_flag = true
end

-- Mark a single interaction to run. This is a push model. If something gets clicked on, you can mark
-- it here and the interaction loop will pick it up.
function doublenickel.interaction.mark(interaction)
	interaction.need_run = true
end

function doublenickel.interaction.update()
	local player = doublenickel.find_entity('Player') -- @refactor
	if not player then return end -- e.g. in a debug scene

	local player_interaction = player:find_component('Interaction')
	local interactions = doublenickel.find_all_components('Interaction')

	-- When you press the button to check for interactions, loop over all interactions and see if the
	-- player's interaction geometry intersects any interactable
	if doublenickel.interaction.check_flag then
		doublenickel.interaction.check_flag = false

		-- No broad phases here!
		for index, interaction in pairs(interactions) do
			if interaction == player_interaction then goto continue end

			local cp = player:find_component('Collider')
			local ci = interaction.collider
			if doublenickel.physics.sat(cp, ci) then
				interaction:on_interaction()
				break
			end

			::continue::
		end
	end

	-- If any were marked to run manually, do those
	for index, interaction in pairs(interactions) do
		if interaction == player then goto continue end
		if interaction.need_run then
			interaction:on_interaction()
			interaction.need_run = false
		end

		::continue::
	end
end

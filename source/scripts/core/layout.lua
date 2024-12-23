function doublenickel.resolution_aware_layout(name)
	local display_mode = dn.window_get_display_mode()

	if mode == doublenickel.enums.DisplayMode.p1080 then
		local aware_name = string.format('%s-1080', name)
		dn.imgui_load_layout(aware_name)
	elseif mode == doublenickel.enums.DisplayMode.p1440 then
		dn.imgui_load_layout(name)
	else
		-- I don't bother making editor layouts for other resolutions. Theoretically, if someone
		-- had a 4K monitor then they'd want that, but the way that the editor scales (or rather
		-- does not scale) is kind of fucked anyway, so who cares?
		dn.imgui_load_layout(name)
	end
end

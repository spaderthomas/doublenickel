AnimationEditor = tdengine.editor.define('AnimationEditor')

AnimationEditor.popup_kind = {
	edit = 'edit##animation_editor'
}

AnimationEditor.ids = {
	name = '##animation_editor:name',
	speed = '##animation_editor:speed',
	import = '##import:animation_editor'
}

function AnimationEditor:init()
	local popups = {
		[self.popup_kind.edit] = {
			window = 'Edit Animation',
			callback = function() self:popup() end
		}
	}
	self.popups = Popups:new(popups)

	self.colors = {
		delete = tdengine.color32(150, 0, 25, 255),
		disabled = tdengine.color32(100, 100, 100, 255),
	}

	self.sizes = {
		preview = tdengine.vec2(100, 200)
	}

	self.old_name = ''
	self.current_name = ''
	self.import_dir = ''

	self.data = {
		speed = 0.1,
		frames = {}
	}

	self.animation = Animation:new()
	self.table_editor = {}
end

function AnimationEditor:update(dt)
	--self.animation:update(dt)
	self.popups:update()
end

function AnimationEditor:reinit_animation_component()
	self.animation:init({
		animation = self.current_name,
		data = self.data
	})
end

function AnimationEditor:create(name)
	local data = tdengine.animation.find('default')
	data = table.deep_copy(data)
	tdengine.animation.add(name, data)
end

function AnimationEditor:edit(name)
	-- Fetch the animation from our current canonical animation data
	local animation = tdengine.animation.find(name)

	-- Update our names to match
	self.original_name = name
	self.current_name = name
	imgui.InputText(self.ids.name, self, 'current_name')

	-- Pull frames from the animation, and add a callback for the frames' table editors to show
	-- the Delete Frame button
	self.data = table.deep_copy(animation)
	self.table_editor = imgui.extensions.TableEditor(self.data.frames, { depth = 2, enter_returns_true = true })
	for index, frame_editor in pairs(self.table_editor.children) do
		frame_editor.on_done = function() self:draw_delete_frame_button(index) end
	end

	self:reinit_animation_component()
end

function AnimationEditor:draw_delete_frame_button(index)
	local disabled = #self.data.frames == 1
	local color = disabled and self.colors.disabled or self.colors.delete
	imgui.PushStyleColor(ffi.C.ImGuiCol_Button, color)

	if imgui.Button('Delete') then
		if not disabled then
			tdengine.array.remove(self.data.frames, index)
			self.animation:restart()
		end
	end

	imgui.PopStyleColor()
end

function AnimationEditor:popup()
	local done = false

	local window = tdengine.vec2(450, 600)
	local wx = tdengine.screen_dimensions().x
	imgui.SetCursorPosX((wx / 2) - (window.x / 2))
	imgui.SetNextWindowSize(window:unpack())

	local flags = 0
	flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoMove)
	flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoResize)
	flags = bitwise(tdengine.op_or, flags, ffi.C.ImGuiWindowFlags_NoCollapse)

	if imgui.BeginPopupModal(self.popup_kind.edit, nil, flags) then
		local sprite = self:calc_preview_size()
		local center = (window.x / 2) - (sprite.x / 2)

		imgui.Dummy(0, 10)
		imgui.SetCursorPosX(center)

		self.animation:update()
		local image = self.animation:get_image()
		imgui.GameImage(image, sprite:unpack())

		local remaining = self.sizes.preview.y - sprite.y

		imgui.Dummy(10, remaining + 10)
		imgui.extensions.VariableName('name')
		imgui.SameLine()
		imgui.InputText(self.ids.name, self, 'current_name')

		imgui.extensions.VariableName('speed')
		imgui.SameLine()
		imgui.InputFloat(self.ids.speed, self.data, 'speed')


		imgui.Dummy(10, 10)

		add, change = self.table_editor:draw()
		if change then
			self:reinit_animation_component()
		end

		if imgui.Button('Add Frame') then
			local frame = {
				image = 'debug.png'
			}
			table.insert(self.data.frames, frame)

			local index = #self.data.frames
			self.table_editor:add_child(index)
			local frame_editor = self.table_editor.children[index]
			frame_editor.enter_returns_true = true
			frame_editor.on_done = function() self:draw_delete_frame_button(index) end
		end

		imgui.InputText(self.ids.import, self, 'import_dir')
		local import_path = tdengine.ffi.resolve_format_path('image', self.import_dir):to_interned()
		local import_valid = tdengine.does_path_exist(import_path) and #self.import_dir > 0
		imgui.SameLine()

		if imgui.Button('Import') then
			-- It's a directory, not a filename, but this function still just grabs the part after the final path separator.
			local name = tdengine.extract_filename(import_path)
			self.current_name = name

			self.data.speed = .25

			table.clear(self.data.frames)
			local files = tdengine.scandir(import_path)
			table.sort(files)
			for index, file in pairs(files) do
				local frame = {
					image = file,
					time = 0
				}
				table.insert(self.data.frames, frame)
			end

			self:reinit_animation_component()
		end

		if imgui.IsItemHovered() then
			imgui.BeginTooltip()
			imgui.PushTextWrapPos(imgui.GetFontSize() * 24)
			local message = [[
Import a folder as one animation. The folder is a relative path from the /assets/images. Every file in the folder should be a PNG that is specified in some texture atlas.
]]
			imgui.Text(message)
			imgui.PopTextWrapPos()
			imgui.EndTooltip()
		end

		imgui.Dummy(25, 25)

		-- Save
		if imgui.Button('Save', imgui.ImVec2(100, 25)) then
			tdengine.animation.add(self.current_name, self.data)
			tdengine.animation.save()
		end

		imgui.SameLine()

		-- Cancel
		if imgui.Button('Cancel', imgui.ImVec2(100, 25)) then
			done = true
		end

		-- Cleanup
		if done then
			self.popups:close_popup(self.popup_kind.edit)
		end
		imgui.EndPopup()
	end
end

function AnimationEditor:calc_preview_size()
	local image = self.animation:get_image()
	local size = tdengine.vec2(tdengine.sprite_size(image))

	if size.y == 0 then
		return tdengine.vec2(tdengine.sprite_size('debug.png'))
	end

	-- @hack: Now that I have larger assets, they don't fit in the window. This flat doesn't work
	-- for wider-than-tall, but it's OK for now
	if size.y > self.sizes.preview.y then
		local ratio = size.x / size.y
		size.y = self.sizes.preview.y
		size.x = size.y * ratio
	end

	return size
end

GameView = dn.class.define('GameView')

GameViewSize = dn.enum.define(
  'GameViewSize',
  {
    Force_16_9 = 0,
    ExactSize = 1
  }
)

GameViewPriority = dn.enum.define(
  'GameViewPriority',
  {
    Standard = 0,
    Main = 1
  }
)


function GameView:init(name, render_target, size_calculation, size, priority)
  self.name = name
  self.render_target = render_target
  self.size_calculation = size_calculation
  self.size = size:copy()
  self.priority = priority or GameViewPriority.Main

  self.position = dn.vec2()
end

function GameView:update()
  imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_WindowPadding, 0, 0)
  dn.editor.begin_window(self.name)

  self.focus = imgui.IsWindowFocused()
  self.hover = imgui.IsWindowHovered()

  if self.priority == dn.enums.GameViewPriority.Main then
    if self.size_calculation == dn.enums.GameViewSize.ExactSize then
      dn.window.set_game_area_size(self.size)

      self.position.x, self.position.y = imgui.GetCursorScreenPos()
      dn.window.set_game_area_position(self.position)
    elseif self.size_calculation == dn.enums.GameViewSize.Force_16_9 then

    end

  end

  local render_target = dn.ffi.gpu_render_target_find(self.render_target:to_qualified_string())
  imgui.Image(
    render_target.color_buffer,
    imgui.ImVec2(self.size.x, self.size.y),
    imgui.ImVec2(0, 1), imgui.ImVec2(1, 0))

  dn.editor.end_window()
  imgui.PopStyleVar()
end


-----------------------
-- GAME VIEW MANAGER --
-----------------------
local GameViewManager = dn.editor.define('GameViewManager')

function GameViewManager:init()
  self.game_views = dn.data_types.Array:new()
end

function GameViewManager:find_main_view()
  for game_view in self.game_views:iterate_values() do
    if game_view.priority == dn.enums.GameViewPriority.Main then
      return game_view
    end
  end
end

function GameViewManager:add_view(view)
  if view.priority == dn.enums.GameViewPriority.Main then
    local main_view = self:find_main_view()
    if main_view then
      main_view.priority = dn.enums.GameViewPriority.Standard
    end
  end

  self.game_views:add(view)
  dn.ffi.info('GameViewManager:add_view', '%s [%s, %s (%d, %d)]',
    view.name,
    view.priority:to_qualified_string(),
    view.render_target:to_qualified_string(),
    view.size.x, view.size.y
  )

end

function GameViewManager:update()
  for game_view in self.game_views:iterate_values() do
    game_view:update()
  end

  local main_view = self:find_main_view()
  self.focus = main_view.focus
  self.hover = main_view.hover
end
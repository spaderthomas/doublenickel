GameView = tdengine.class.define('GameView')

tdengine.enum.define(
  'GameViewSize',
  {
    Force_16_9 = 0,
    ExactSize = 1
  }
)

tdengine.enum.define(
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
  self.priority = priority

  self.position = tdengine.vec2()
end

function GameView:update()
  imgui.PushStyleVar_2(ffi.C.ImGuiStyleVar_WindowPadding, 0, 0)
  tdengine.editor.begin_window(self.name)

  self.focus = imgui.IsWindowFocused()
  self.hover = imgui.IsWindowHovered()

  if self.priority == tdengine.enums.GameViewPriority.Main then
    if self.size_calculation == tdengine.enums.GameViewSize.ExactSize then
      tdengine.window.set_game_area_size(self.size)

      self.position.x, self.position.y = imgui.GetCursorScreenPos()
      tdengine.window.set_game_area_position(self.position)
    elseif self.size_calculation == tdengine.enums.GameViewSize.Force_16_9 then

    end

    ffi.C.set_game_focus(self.focus and self.hover)
  end

  imgui.Image(
    self.render_target.color_buffer,
    imgui.ImVec2(self.size.x, self.size.y),
    imgui.ImVec2(0, 1), imgui.ImVec2(1, 0))

  tdengine.editor.end_window()
  imgui.PopStyleVar()
end




local GameViewManager = tdengine.editor.define('GameViewManager')

function GameViewManager:init()
  self.game_views = tdengine.data_types.Array:new()
end

function GameViewManager:find_main_view()
  for game_view in self.game_views:iterate_values() do
    if game_view.priority == tdengine.enums.GameViewPriority.Main then
      return game_view
    end
  end
end

function GameViewManager:add_view(view)
  if view.priority == tdengine.enums.GameViewPriority.Main then
    local main_view = self:find_main_view()
    if main_view then
      log.info('Setting main game view to %s', view.name)
      main_view.priority = tdengine.enums.GameViewPriority.Standard
    end
  end

  self.game_views:add(view)
end

function GameViewManager:update()
  for game_view in self.game_views:iterate_values() do
    game_view:update()
  end

  local main_view = self:find_main_view()
  self.focus = main_view.focus
  self.hover = main_view.hover
end
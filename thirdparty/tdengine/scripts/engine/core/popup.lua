Popups = tdengine.class.define('Popups')

function Popups:init(popups)
  self.popups = popups
end

function Popups:open_popup(popup)
  self.popups[popup].need_open = true
  self.popups[popup].open = true
end

function Popups:close_popup(popup)
  imgui.CloseCurrentPopup()
  self.popups[popup].open = true
end

function Popups:update()
  for id, popup in pairs(self.popups) do
    if popup.need_open then
      imgui.OpenPopup(id)
      popup.need_open = false
    end

    if popup.open then
      popup.callback()
    end
  end
end

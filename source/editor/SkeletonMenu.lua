SkeletonMenu = tdengine.editor.define('SkeletonMenu')

function SkeletonMenu:init()
end

function SkeletonMenu:on_main_menu()
  local skeleton_viewer = tdengine.find_entity('SkeletonViewer')
  if skeleton_viewer then
    if imgui.BeginMenu('Skeleton') then
      if imgui.BeginMenu('Open') then
        local skeleton_dir = tdengine.ffi.resolve_named_path('skeletons'):to_interned()
        local skeletons = tdengine.scandir(skeleton_dir)
        for index, name in pairs(skeletons) do
          skeletons[index] = string.gsub(name, '.lua', '')
        end
        table.sort(skeletons)

        for index, skeleton in pairs(skeletons) do
          if imgui.MenuItem(skeleton) then
            skeleton_viewer:load_from_file(skeleton)
          end
        end

        imgui.EndMenu()
      end

      if imgui.MenuItem('Save') then
        skeleton_viewer:save_skeleton()
      end

      imgui.EndMenu()
    end

    if imgui.BeginMenu('Skeletal Animation') then
        local directory = tdengine.ffi.resolve_named_path('skeletal_animations'):to_interned()
        local animations = tdengine.scandir(directory)
        for index, name in pairs(animations) do
          animations[index] = string.gsub(name, '.lua', '')
        end
        table.sort(animations)

        for index, animation in pairs(animations) do
          if imgui.MenuItem(animation) then
            skeleton_viewer:load_animation(animation)
          end
        end

      imgui.EndMenu()
    end
  end
end

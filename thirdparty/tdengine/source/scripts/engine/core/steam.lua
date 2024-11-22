function tdengine.steam.read_text_input()
  if not tdengine.ffi.is_text_input_dirty() then return false, nil end

  return true, ffi.string(tdengine.ffi.read_text_input())
end

function tdengine.steam.show_text_input(description, existing_text)
  tdengine.ffi.show_text_input(description, existing_text)
end

function tdengine.steam.open_store_page(utm)
  utm = utm or ffi.cast('const char*', nil)
  tdengine.ffi.open_steam_page(utm)
end

function tdengine.steam.open_discord_invite()
  os.execute('start https://discord.gg/uAUHBmGz7p')
end

--
-- log.lua
--
-- Copyright (c) 2016 rxi
--
-- This library is free software; you can redistribute it and/or modify it
-- under the terms of the MIT license. See LICENSE for details.
--

log = { _version = "0.1.0" }

log.use_color = true
log.outfile = nil
log.level = "trace"

log.stack_depth = nil
log.push_stack_depth = function(value)
  log.stack_depth = value
end

log.read_stack_depth = function()
  local stack_depth = log.stack_depth or 2
  log.stack_depth = nil
  return stack_depth
end


local modes = {
  { name = "trace", color = "\27[34m", },
  { name = "debug", color = "\27[36m", },
  { name = "info",  color = "\27[32m", },
  { name = "warn",  color = "\27[33m", },
  { name = "error", color = "\27[31m", },
  { name = "fatal", color = "\27[35m", },
}


local levels = {}
for i, v in ipairs(modes) do
  levels[v.name] = i
end


local round = function(x, increment)
  increment = increment or 1
  x = x / increment
  return (x > 0 and math.floor(x + .5) or math.ceil(x - .5)) * increment
end


local _tostring = tostring

local tostring = function(...)
  local t = {}
  for i = 1, select('#', ...) do
    local x = select(i, ...)
    if type(x) == "number" then
      x = round(x, .01)
    end
    t[#t + 1] = _tostring(x)
  end
  return table.concat(t, " ")
end


for i, x in ipairs(modes) do
  local nameupper = x.name:upper()
  log[x.name] = function(...)
    -- Return early if we're below the log level
    if i < levels[log.level] then
      return
    end

    local msg = string.format(...)

    local stack_depth = log.read_stack_depth()
    local info = debug.getinfo(stack_depth, "Sl")

    local install = tdengine.ffi.resolve_named_path('install'):to_interned()
    local relative_path = info.source:gsub(install, '')
    relative_path = relative_path:sub(2, #relative_path)

    -- Output to console
    print(string.format("%s[%-6s%s]%s %s::%d %s",
      log.use_color and x.color or "",
      nameupper,
      os.date("%H:%M:%S"),
      log.use_color and "\27[0m" or "",
      relative_path,
      info.currentline,
      msg))

    -- Output to log file
    if log.outfile then
      local fp = io.open(log.outfile, "a")
      local str = string.format("[%-6s%s] %s: %s\n",
        nameupper, os.date(), relative_path, msg)
      fp:write(str)
      fp:close()
    end
  end
end

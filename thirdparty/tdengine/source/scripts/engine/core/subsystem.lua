local module = tdengine.subsystem



function tdengine.subsystem.init()
  module.subsystems = {}
  for name, class in pairs(tdengine.subsystem.types) do
    module.subsystems[name] = class:new()
  end
end

function tdengine.subsystem.define(name)
  local class = tdengine.class.define(name)
  class:include_lifecycle()
  class:include_update()

  tdengine.subsystem.types[name] = class
  return class
end

function tdengine.subsystem.find(name)
  return module.subsystems[name]
end

function tdengine.subsystem.update()
  for _, subsystem in pairs(module.subsystems) do
    subsystem:update()
  end
end
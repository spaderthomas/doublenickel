local module = doublenickel.subsystem

function doublenickel.subsystem.init()
  dn.log('doublenickel.subsystem.init')
  module.subsystems = {}
  for name, class in pairs(doublenickel.subsystem.types) do
    module.subsystems[name] = class:new()
  end
end

function doublenickel.subsystem.define(name)
  local class = doublenickel.class.define(name)
  class:include_lifecycle()
  class:include_update()

  doublenickel.subsystem.types[name] = class
  return class
end

function doublenickel.subsystem.find(name)
  return module.subsystems[name]
end

function doublenickel.subsystem.update()
  for _, subsystem in pairs(module.subsystems) do
    subsystem:update()
  end
end
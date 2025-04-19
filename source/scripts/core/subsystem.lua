local module = dn.subsystem

function dn.subsystem.init()
  dn.ffi.log('dn.subsystem.init')
  module.subsystems = {}
  for name, class in pairs(dn.subsystem.types) do
    module.subsystems[name] = class:new()
  end
end

function dn.subsystem.define(name)
  local class = dn.class.define(name)
  class:include_lifecycle()
  class:include_update()

  dn.subsystem.types[name] = class
  return class
end

function dn.subsystem.find(name)
  return module.subsystems[name]
end

function dn.subsystem.update()
  for _, subsystem in pairs(module.subsystems) do
    subsystem:update()
  end
end
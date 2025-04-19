Hello = dn.entity.define('Hello')
function Hello:init()
  dn.ffi.log('Hello:init()')
end

function Hello:update()
  dn.ffi.log('Hello:update()')
end

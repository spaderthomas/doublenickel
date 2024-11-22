function tdengine.is_instance_of(self, class)
  if type(class) == 'table' then
    class = class.name
  end

  local mt = getmetatable(self)
  return mt.__type == class
end

function tdengine.class.define(name)
  tdengine.types[name] = {
    name = name,
    -- Static methods manipulate the class itself, like SomeClass:new()
    __static = {
      include = function(__class, mixin)
        for method_name, method in pairs(mixin) do
          tdengine.types[name].__instance[method_name] = method
        end
      end,

      include_lifecycle = function(__class, enum)
        tdengine.types[name].__static:include_enum(tdengine.enums.LifecycleCallback)
      end,

      include_update = function(__class, enum)
        tdengine.types[name].__static:include_enum(tdengine.enums.UpdateCallback)
      end,

      include_enum = function(__class, enum)
        for as_enum, as_string, as_number in enum:iterate() do
          tdengine.types[name].__instance[as_string] = function() end
        end
      end,

      include_fields = function(__class, fields)
        for field_name, field in pairs(fields) do
          tdengine.types[name].__fields[field_name] = field
        end
      end,

      set_field_metadata = function(__class, field, metadata)
        tdengine.editor.set_field_metadata(tdengine.types[name], field, metadata)
      end,

      set_field_metadatas = function(__class, metadatas)
        tdengine.editor.set_field_metadatas(tdengine.types[name], metadatas)
      end,

      set_metamethod = function(__class, metamethod, fn)
        -- Since the class system uses the indirection provided by __index to work, overriding it
        -- would render the instance unusable. We can hack it in, though, by calling the internal
        -- __index before we try whatever the user gave us
        if metamethod == '__index' then
          tdengine.types[name].__metamethods[metamethod] = function(__instance, key)
            return tdengine.types[name].__metamethods.__default_index(__instance, key) or fn(__instance, key)
          end
        else
          tdengine.types[name].__metamethods[metamethod] = fn
        end
        -- if metamethod_n
        -- tdengine.editor.set_field_metadatas(tdengine.types[name], metadatas)
      end,





      allocate = function(__class, ...)
        local instance = {}
        for field_name, field in pairs(tdengine.types[name].__fields) do
          instance[field_name] = field
        end

        setmetatable(instance, tdengine.types[name].__metamethods)
    
        -- setmetatable(instance, {
        --   __index = function(__instance, key)
        --     -- If some key isn't found on the instance, check the class' instance methods
        --     -- Look up the class in the global type table, so that it's not stale when hotloaded
        --     return tdengine.types[name].__instance[key]
        --   end,
        --   __type = name
        -- })
    
        return instance
      end,

      new = function(__class, ...)
        local instance = tdengine.types[name].__static.allocate(__class)
        return tdengine.types[name].__static.construct(__class, instance, ...)
      end,

      construct = function(__class, instance, ...)
        if tdengine.types[name].__instance.init then
          tdengine.types[name].__instance.init(instance, ...)
        end
    
        return instance
      end
    },

    -- Instance methods manipulate an instance of the class, like some_instance:do_something()
    __instance = {
      is_instance_of = function(self, class)
        if type(class) == 'table' then
          class = class.name
        end

        local mt = getmetatable(self)
        return mt.__type == class
      end,

      class = function(self)
        local metatable = getmetatable(self)
        return metatable and metatable.__type or ''
      end
    },

    -- This metatable is applied to an instance when it is allocated
    __metamethods = {
      __index = function(__instance, key)
        return rawget(tdengine.types[name].__instance, key)
      end,
      __default_index = function(__instance, key)
        return rawget(tdengine.types[name].__instance, key)
      end,
      __type = name
    },

    -- If a class should have some default fields populated, they end up here. When an instance is allocated,
    -- we copy any fields here onto the instance with a default value
    __fields = {

    }
  }

  -- This is the metatable for the class itself
  setmetatable(tdengine.types[name], {
    __index = function(_, key)
      return rawget(tdengine.types[name].__static, key) or rawget(tdengine.types[name].__instance, key)
    end,
    __newindex = function(__class, member_name, member)
      tdengine.types[name].__instance[member_name] = member
    end,
    __type = name
  })

  return tdengine.types[name]
end

function tdengine.class.metatype(ctype)
  -- Because LuaJIT (bafflingly and ostensibly for some optimization) makes metatypes immutable, we create the metatype once
  -- with a layer of indirection that looks up metamethods rather than calling them directly. Ultimately, we'd like all of the
  -- LuaJIT metatype's metamethods to point to the Lua class that we make
  if not tdengine.types[ctype] then
    local metatype_indirection = {
      __index = function(t, k)
        return getmetatable(tdengine.types[ctype]).__index(t, k)
      end
    }

    ffi.metatype(ctype, metatype_indirection)
  end
  
  -- Then, we create a plain old class. The only difference is that instead of creating and populating a table, we just need
  -- to return ffi.new().
  --
  -- This does mean that our metatypes can't have extra fields on them. There's no reason this isn't possible; it would just
  -- take more metatable wrangling than I feel like doing right now (i.e. store everything besides the ctype in a table, look
  -- up fields in that table if not found on the ctype -- annoying!)
  local type = tdengine.class.define(ctype)
  type.__static.allocate = function()
    return ffi.new(ctype)
  end

  return type
end


function tdengine.class.find(name)
  return tdengine.types[name]
end

function tdengine.class.get(t)
  if t.class then return tdengine.class.find(t:class()) end
end


function tdengine.add_class_metamethod(class, name, fn)
  local metatable = getmetatable(class)
  metatable[name] = fn
  setmetatable(class, metatable)
end

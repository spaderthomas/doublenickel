
struct LuaState {
	lua_State* state;
	FileMonitor* file_monitor;

	static constexpr u32 max_script_dirs = 32;
	Array<string> script_dirs;

	LuaState();
	void script_dir(string path);
	void script_named_dir(const_string name);
	bool script_file(string path);
	void dump_stack();

	void init_phase_0();
	void init_phase_1();
	void init_phase_2();

	void parse_string(i32 index, const char** value);
	void parse_string(i32 index, char** value);
	void parse_string(const char*, const char** value);
	void parse_string(const char*, char** value);
	void parse_bool(const char* key, bool* value);
	void parse_bool(i32 index, bool* value);
	void parse_int32(const char* key, i32* value);
	void parse_int32(i32 index, i32* value);
	void parse_uint32(const char* key, u32* value);
	void parse_uint32(i32 index, u32* value);
	void parse_float(const char* key, float32* value);
	void parse_float(i32 index, float32* value);
	void parse_float64(const char* key, float64* value);
	void parse_float64(i32 index, float64* value);
	void parse_vec2(Vector2* vec);
	void parse_vec2(const char* name, Vector2* vec);
	void parse_vec2(i32 index, Vector2* vec);
	void parse_color(Vector4* color);
	void parse_color(const char* name, Vector4* color);
	void parse_color(i32 index, Vector4* color);
	
	template<typename T>
	void parse_enum(const char* key, T* value);
};

LuaState& get_lua();
void init_lua();
void init_scripts();
void update_game();
i32 handle_error(lua_State* l);


FM_LUA_EXPORT void add_script_directory(const char* directory);

#define DEFER_POP(l) defer { lua_pop(l, 1); };

#define _ENSURE_ARG() fm_assert(lua_gettop(l) >= __arg)
#define _INDEX_ARG(ARG) const i32 IDX_##ARG = __arg++
#define _IS_TABLE() lua_istable(l, __arg)
#define _IS_STRING() lua_isstring(l, __arg)
#define _IS_NUMBER() lua_isnumber(l, __arg)
#define _IS_BOOL() lua_isboolean(l, __arg)
#define _IS_NIL() lua_isnil(l, __arg)
#define _CHECK_TABLE_ARG()  if (lua_gettop(l) >= __arg) { fm_assert(_IS_TABLE()); }
#define _CHECK_STRING_ARG() if (lua_gettop(l) >= __arg) { fm_assert(_IS_STRING()); }
#define _CHECK_NUMBER_ARG() if (lua_gettop(l) >= __arg) { fm_assert(_IS_NUMBER()); }
#define _CHECK_BOOL_ARG()   if (lua_gettop(l) >= __arg) { fm_assert(_IS_BOOL()); }
#define _CHECK_OPT_TABLE_ARG()  if (lua_gettop(l) >= __arg) { fm_assert(_IS_TABLE() || _IS_NIL()); }
#define _CHECK_OPT_STRING_ARG() if (lua_gettop(l) >= __arg) { fm_assert(_IS_STRING() || _IS_NIL(); }
#define _CHECK_OPT_NUMBER_ARG() if (lua_gettop(l) >= __arg) { fm_assert(_IS_NUMBER() || _IS_NIL()); }
#define _CHECK_OPTBOOL_ARG()   if (lua_gettop(l) >= __arg) { fm_assert(_IS_BOOLEAN() || _IS_NIL()); }

#define BEGIN_ARGS() i32 __arg = 1;
#define END_ARGS() fm_assert(lua_gettop(l) <= __arg);
#define NO_ARGS() fm_assert(lua_gettop(l) == 0);
#define HAS_ARG(IDX) lua_gettop(l) >= IDX && !(lua_isnil(l, IDX))

#define ADD_TABLE_ARG(ARG)  _ENSURE_ARG(); _CHECK_TABLE_ARG();      _INDEX_ARG(ARG);
#define ADD_STRING_ARG(ARG) _ENSURE_ARG(); _CHECK_STRING_ARG();     _INDEX_ARG(ARG);
#define ADD_NUMBER_ARG(ARG) _ENSURE_ARG(); _CHECK_NUMBER_ARG();     _INDEX_ARG(ARG);
#define ADD_BOOL_ARG(ARG)   _ENSURE_ARG(); _CHECK_BOOL_ARG();       _INDEX_ARG(ARG);
#define ADD_OPTIONAL_TABLE_ARG(ARG)        _CHECK_OPT_TABLE_ARG();  _INDEX_ARG(ARG);
#define ADD_OPTIONAL_STRING_ARG(ARG)       _CHECK_OPT_STRING_ARG(); _INDEX_ARG(ARG);
#define ADD_OPTIONAL_NUMBER_ARG(ARG)       _CHECK_OPT_NUMBER_ARG(); _INDEX_ARG(ARG);
#define ADD_OPTIONAL_BOOL_ARG(ARG)         _CHECK_OPT_BOOL_ARG();   _INDEX_ARG(ARG);

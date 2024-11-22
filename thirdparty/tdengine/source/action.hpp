namespace Steam {
	typedef InputHandle_t Controller;
	typedef InputDigitalActionHandle_t DigitalAction;
	typedef InputActionSetHandle_t ActionSet;
	typedef InputDigitalActionData_t DigitalActionData;
	typedef HTTPRequestHandle HttpRequest;
	typedef SteamAPICall_t ApiCall;
}

enum class InputDevice : u32 {
	MouseAndKeyboard = 0,
	Controller = 1
};

enum class KeyEvent : u32 {
	Press = 0,
	Down = 1
};

struct ActionSet {
	static constexpr u8 NAME_MAX = 64;
	char name [NAME_MAX];

	Steam::ActionSet handle;
};

struct Action {
	static constexpr u8 NAME_MAX = 64;
	char name [NAME_MAX];

	Steam::DigitalAction handle;
	ActionSet* set;

	bool state;
	bool old_state;
	
	u32 key;
	KeyEvent key_event;
};


struct SteamInputManager {
	static constexpr int32 MAX_ACTION_SETS = 8;
	static constexpr int32 MAX_ACTIONS = 128;

	Steam::Controller controller;
	
	Array<ActionSet> action_sets;
	ActionSet* current_action_set;
	ActionSet* queued_action_set;

	// @hack: This is a really gross hack, but I don't know what else to do. SteamInput seems to have a bug where
	// if two unique actions across different action sets are bound to the same button (e.g. OpenMenu and CloseMenu
	// both being bound to start), any button press that changes the action set will cause the event bound to
	// the same button in the new action set to also fire. In other words, if I pressed Start to open the menu, I
	// would get both an OpenMenu and CloseMenu event.
	//
	// I submitted a Steam support request and they were pretty unhelpful. I have no way to debounce these duplicated
	// press events, because the whole point of SteamInput is that any action can be mapped to any button. For this reason,
	// I just refuse to read inputs for N frames after I change action sets. It's supremely hacky, but luckily this game
	// couldn't care less about precise inputs in that way.
	//
	// 5 is picked more or less arbitrarily. I see the input happening the frame after I switch action sets, so theoretically
	// 2 or 3 should be enough.
	static constexpr int32 action_set_cooldown_max = 5;
	int32 action_set_cooldown = 0;

	Array<Action> actions;

	ISteamInput* steam;
	bool got_controller_input;
	InputDevice last_input_device;
	
	tstring get_controller_name(Steam::Controller controller);
	ActionSet* find_action_set(const char* name);
	ActionSet* find_action_set(Steam::ActionSet handle);
	ActionSet* find_current_action_set();
	Action* find_action(const char* name);
	bool is_action_active(Action* action);
	void poll_for_controllers();
	void poll_for_inputs();
	void register_actions();
	void load_controller_glyphs();
	bool update_cooldown_hack();
};
SteamInputManager steam_input;

void init_actions();
void update_actions();

FM_LUA_EXPORT void register_action(const char* name, u32 key, u32 key_even, const char* action_set);
FM_LUA_EXPORT void register_action_set(const char* name);
FM_LUA_EXPORT void activate_action_set(const char* name);
FM_LUA_EXPORT bool is_digital_active(const char* name);
FM_LUA_EXPORT bool was_digital_active(const char* name);
FM_LUA_EXPORT bool was_digital_pressed(const char* name);
FM_LUA_EXPORT int32 get_input_device();
FM_LUA_EXPORT int32 get_action_set_cooldown();
FM_LUA_EXPORT const char* get_active_action_set(); 

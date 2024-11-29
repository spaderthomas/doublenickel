#ifndef DN_STEAM_H
#define DN_STEAM_H

typedef enum {
  DN_STEAM_INIT_NONE = 0,
  DN_STEAM_INIT_FAILED = 1,
  DN_STEAM_INIT_OK = 2,
} dn_steam_init_state_t;

typedef enum {
  DN_STEAM_INPUT_TEXT_UNINITIALIZED = 0,
  DN_STEAM_INPUT_TEXT_OPEN = 1,
  DN_STEAM_INPUT_TEXT_UNREAD = 2,
  DN_STEAM_INPUT_TEXT_CLOSED = 3,
} dn_steam_text_input_state_t;

struct dn_steam_callbacks_t {
  STEAM_CALLBACK(dn_steam_callbacks_t, on_dismiss_text_input, GamepadTextInputDismissed_t);
};
dn_steam_callbacks_t dn_steam_callbacks;

typedef struct {
  u32 app_id;
} dn_steam_config_t;

typedef struct {
  u32 app_id;
  dn_steam_init_state_t state;

  struct {
    dn_steam_text_input_state_t state;
    dn_string_t contents;
  } text_input;
} dn_steam_t;
dn_steam_t dn_steam;

DN_API void                        dn_steam_init(dn_steam_config_t config);
DN_API bool                        dn_steam_initialized();
DN_API void                        dn_steam_open_store_page();
DN_API void                        dn_steam_open_store_page_utm(dn_string_t utm);
DN_API void                        dn_steam_open_store_page_ex(dn_string_t url);
DN_API void                        dn_steam_open_text_input();
DN_API dn_steam_text_input_state_t dn_steam_text_input_state();
DN_API dn_string_t                 dn_string_read_text_input();
DN_API bool                        dn_steam_is_deck();
DN_IMP void                        dn_steam_shutdown();
#endif



#ifdef DN_STEAM_IMPLEMENTATION
void dn_steam_init(dn_steam_config_t config) {
  dn_steam.app_id = config.app_id;
  dn_steam.state = DN_STEAM_INIT_NONE;
  dn_steam.text_input.state = DN_STEAM_INPUT_TEXT_UNINITIALIZED;
  
  SteamErrMsg steam_error = dn_zero_initialize();
  
  if (SteamAPI_InitEx(&steam_error) == k_ESteamAPIInitResult_OK) {
    dn_steam.state = DN_STEAM_INIT_OK;
    dn_steam.text_input.state = DN_STEAM_INPUT_TEXT_CLOSED;
    dn_log("%s: Successfully initialized Steam", __func__);
  } else {
    dn_steam.state = DN_STEAM_INIT_FAILED;
    dn_log("Could not initialize Steam. Error: %s", steam_error);
  }
}

void dn_steam_update() {
  if (dn_steam_initialized()) {
    SteamAPI_RunCallbacks();
  }
}

void dn_steam_shutdown() {
  if (dn_steam_initialized()) SteamAPI_Shutdown();

  if (steam_input.steam) steam_input.steam->Shutdown();
}

bool dn_steam_initialized() {
  return dn_steam.state == DN_STEAM_INIT_OK;
} 


void dn_steam_callbacks_t::on_dismiss_text_input(GamepadTextInputDismissed_t* callback) {
  DN_UNTESTED();
  
  if (!callback->m_bSubmitted) return;

  u32 len = SteamUtils()->GetEnteredGamepadTextLength();

  dn_steam.text_input.state = DN_STEAM_INPUT_TEXT_UNREAD;
  dn_steam.text_input.contents.len = len;
  dn_steam.text_input.contents.data = dn_allocators.standard.alloc<u8>(len);

  bool success = SteamUtils()->GetEnteredGamepadTextInput((char*)dn_steam.text_input.contents.data, len);
  if (!success) {
    dn_log("%s: Failed to read from Steam text input", __func__);
    return;
  }
}

 void dn_steam_open_store_page() {
   DN_UNTESTED();

   dn_string_builder_t builder = {
     .buffer = dn_zero_initialize(),
     .allocator = &dn_allocators.bump,
   };
   dn_string_builder_append_cstr(&builder, "https://store.steampowered.com/app/");
   dn_string_builder_append_fmt(&builder, dn_string_literal("%d"), dn_steam.app_id);
 }
 
 void dn_steam_open_store_page_utm(dn_string_t utm) {
   DN_UNTESTED();
  
   dn_string_builder_t builder = {
     .buffer = dn_zero_initialize(),
     .allocator = &dn_allocators.bump
   };
   dn_string_builder_append_cstr(&builder, "https://store.steampowered.com/app/");
   dn_string_builder_append_fmt(&builder,dn_string_literal("%d"), dn_steam.app_id);
   dn_steam_open_store_page_ex(dn_string_builder_write(&builder));
 }
 
 void dn_steam_open_store_page_ex(dn_string_t url) {
  DN_UNTESTED();

  if (dn_steam_initialized()) {
    SteamFriends()->ActivateGameOverlayToWebPage(dn_string_to_cstr(url, &dn_allocators.bump));
  }
  else {
    dn_string_builder_t builder = {
      .buffer = dn_zero_initialize(),
      .allocator = &dn_allocators.bump
    };
    dn_string_builder_append_cstr(&builder, "start ");
    dn_string_builder_append(&builder, url);
    system(dn_string_builder_write_cstr(&builder));
  }
 }

 void dn_steam_open_text_input() {
   DN_UNTESTED();

   if (!dn_steam_initialized()) return;
  
   SteamUtils()->ShowFloatingGamepadTextInput(
     k_EFloatingGamepadTextInputModeModeSingleLine, 
     100, 100, 
     600, 600
   );
   dn_steam.text_input.state = DN_STEAM_INPUT_TEXT_OPEN;
 }

 dn_string_t dn_steam_read_text_input() {
   DN_UNTESTED();

   if (!dn_steam_initialized()) return dn_zero_initialize();
   return dn_steam.text_input.contents;
 }

 dn_steam_text_input_state_t dn_steam_text_input_state() {
   return dn_steam.text_input.state;
 }

 bool dn_steam_is_deck() {
   if (!dn_steam_initialized()) return false;
   return SteamUtils()->IsSteamRunningOnSteamDeck();
 }
#endif
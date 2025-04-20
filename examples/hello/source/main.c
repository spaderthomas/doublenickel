#define DN_APP
#define DN_IMPLEMENTATION
#include "dn.h"

/*
A doublenickel app only deals in absolute paths. When we initialize the app, we give the framework the paths it needs to build absolute paths:
1. install_path: Where is the root of the project compared to the executable? In a packaged build, this is probably `.`, but this lets you put your project anywhere in an editor build
2. engine_path: Where are the doublenickel sources?
3. write_path: In packaged builds, this will be `AppData`. But, in editor builds, we'd still like to write data files at runtime. This is where they go.
4. app_path: Finally, this is the path to the app's Lua sources.

With the paths configured, doublenickel knows where to finds its sources to bootstrap first itself and then your app.

The next file is the main app entry point, App.lua
*/

int main(int num_args, char** args) {
  dn_init((dn_config_t) {
    .mode = DN_MODE_APP,
    .path = (dn_path_config_t) {
      .engine  = dn_string_literal("../../.."),
      .install = dn_string_literal("../../hello"),
      .write   = dn_string_literal("../../hello/data"),
    },
    .lua = (dn_lua_config_t) {
      .scripts = dn_string_literal("../../hello/source")
    },
  });

  return dn_main();  
}
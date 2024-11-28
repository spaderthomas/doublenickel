#define DN_IMPLEMENTATION
#define DN_USE_DEFAULT_PROJECT_STRUCTURE
#include "dn.hpp"

int main(int num_args, char** args) {
  dn_app_descriptor_t app = {
    .install_path = "../../../..",
    .engine_path = "thirdparty/tdengine",
    .write_path = "source/app/data",
    .app_path = "source/app",
  };
  dn_main(app);  
}
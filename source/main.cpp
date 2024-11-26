#define DN_IMPLEMENTATIONEMENTATION
#include "td.hpp"

int main(int num_args, char** args) {
  dn_app_descriptor_t app = {
    .install_path = "../../../..",
    .engine_path = "../../../../thirdparty/tdengine",
    .write_path = "../../../../source/app/data",
    .app_path = "../../../../source/app",
  };
  td_main(app);  
}
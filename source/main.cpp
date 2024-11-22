#define TD_IMPLEMENTATION
#include "td.hpp"

int main(int num_args, char** args) {
  TdAppDescriptor app = {
    .install_path = "../../../..",
    .engine_path = "../../../../thirdparty/tdengine",
    .write_path = "../../../../source/data",
    .app_path = "../../../../source",
  };
  td_main(app);  
}
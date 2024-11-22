#ifndef APP_H
#define APP_H
typedef struct {
  const char* install_path;
  const char* engine_path;
  const char* write_path;
  const char* app_path;
} TdAppDescriptor;

typedef struct {
  char install_path [TD_MAX_PATH_LEN];
  char engine_path [TD_MAX_PATH_LEN];
  char write_path [TD_MAX_PATH_LEN];
  char app_path [TD_MAX_PATH_LEN];
} TdApp;
TdApp app;

void init_app(TdAppDescriptor descriptor);
#endif

#ifdef APP_IMPLEMENTATION
void init_app(TdAppDescriptor descriptor) {
  copy_string(descriptor.engine_path, app.engine_path, TD_MAX_PATH_LEN);
  copy_string(descriptor.install_path, app.install_path, TD_MAX_PATH_LEN);
  copy_string(descriptor.write_path, app.write_path, TD_MAX_PATH_LEN);
  copy_string(descriptor.app_path, app.app_path, TD_MAX_PATH_LEN);
}
#endif
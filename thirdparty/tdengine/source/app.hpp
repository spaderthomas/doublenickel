#ifndef APP_H
#define APP_H
typedef struct {
  const char* install_path;
  const char* engine_path;
  const char* write_path;
  const char* app_path;
} dn_app_descriptor_t;

typedef struct {
  char install_path [DN_MAX_PATH_LEN];
  char engine_path [DN_MAX_PATH_LEN];
  char write_path [DN_MAX_PATH_LEN];
  char app_path [DN_MAX_PATH_LEN];
} dn_app_t;
dn_app_t dn_app;

void dn_app_init(dn_app_descriptor_t descriptor);
#endif

#ifdef APP_IMPLEMENTATION
void dn_app_init(dn_app_descriptor_t descriptor) {
  copy_string(descriptor.engine_path, dn_app.engine_path, DN_MAX_PATH_LEN);
  copy_string(descriptor.install_path, dn_app.install_path, DN_MAX_PATH_LEN);
  copy_string(descriptor.write_path, dn_app.write_path, DN_MAX_PATH_LEN);
  copy_string(descriptor.app_path, dn_app.app_path, DN_MAX_PATH_LEN);
}
#endif
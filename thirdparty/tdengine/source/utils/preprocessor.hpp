#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

typedef struct {
  const char* file_path;
  struct {
    dn_path_t* data;
    u32 count;
  } include_dirs;


  char error [256];

  FILE* file;
  String file_data;
  char* result;
} dn_preprocessor_context_t;

void dn_preprocess(dn_preprocessor_context_t* context);
#endif

#ifdef PREPROCESSOR_IMPLEMENTATION

void dn_preprocess(dn_preprocessor_context_t* context) {
  context->result = nullptr;

  context->file = fopen(context->file_path, "rb");
  if (!context->file) {
    snprintf(context->error, 256, "Failed to open file for preprocessing; file = %s", context->file_path);
    return;
  }

  fseek(context->file, 0, SEEK_END);
  context->file_data.len = ftell(context->file);
  context->file_data.data = bump_allocator.alloc<u8>(context->file_data.len + 1);
  char* file_data = (char*)context->file_data.data;

  fseek(context->file, 0, SEEK_SET);
  fread(file_data, 1, context->file_data.len, context->file);
  fclose(context->file);

  char* temp = bump_allocator.alloc<char>(4096);


  char* str = reinterpret_cast<char*>(context->file_data.data);
  include_info *inc_list;
  u32 num_includes = stb_include_find_includes(str, &inc_list);
  size_t source_len = context->file_data.len;
  size_t textlen=0, last=0;
  for (u32 i = 0; i < num_includes; ++i) {
    context->result = stb_include_append(context->result, &textlen, str+last, inc_list[i].offset - last);

    if (inc_list[i].filename) {
      char* included_file_data = nullptr;
      for (u32 dir_index = 0; dir_index < context->include_dirs.count; dir_index++) {
        dn_string_builder_t builder = { 
            .buffer = {0}, 
            .allocator = &bump_allocator 
        };

        dn_string_builder_append_cstr(&builder, context->include_dirs.data[dir_index]);
        dn_string_builder_append_cstr(&builder, "/");
        dn_string_builder_append_cstr(&builder, inc_list[i].filename);

        auto file_path = dn_string_builder_write_cstr(&builder);
        auto file = fopen(file_path, "rb");
        if (file) {
          fclose(file);
          dn_preprocessor_context_t subcontext = {
            .file_path = file_path,
            .include_dirs = context->include_dirs
          };
          dn_preprocess(&subcontext);
          included_file_data = subcontext.result;
          break;
        }
      }

      if (!included_file_data) {
        snprintf(context->error, 256, "Failed to find include; file = %s", inc_list[i].filename);
        stb_include_free_includes(inc_list, num_includes);
        return;
      }

      context->result = stb_include_append(context->result, &textlen, included_file_data, strlen(included_file_data));
      free(included_file_data);
    }
    
    last = inc_list[i].end;
  }
  context->result = stb_include_append(context->result, &textlen, str+last, source_len - last + 1); // append '\0'
  stb_include_free_includes(inc_list, num_includes);
}

#endif


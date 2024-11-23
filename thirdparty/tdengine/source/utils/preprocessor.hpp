#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

typedef struct {
  const char* file_path;
  struct {
    const char* data [8];
    u32 count;
  } include_dirs;


  char error [256];

  FILE* file;
  String file_data;
  char* result;
} IncludeContext;

void td_include(IncludeContext* context);
void td_include_free_context(IncludeContext* context);
#endif

#ifdef PREPROCESSOR_IMPLEMENTATION

void td_include(IncludeContext* context) {
  context->result = nullptr;
  
  context->file = fopen(context->file_path, "rb");
  if (!context->file) {
    copy_string("Failed to load file", context->error, 256);
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
        StringBuilder builder = { 
            .buffer = {0}, 
            .allocator = &bump_allocator 
        };

        string_builder_append_cstr(&builder, context->include_dirs.data[dir_index]);
        string_builder_append_cstr(&builder, "/");
        string_builder_append_cstr(&builder, inc_list[i].filename);

        auto file_path = string_builder_write_cstr(&builder);
        auto file = fopen(file_path, "rb");
        if (file) {
          fclose(file);
          IncludeContext subcontext = {
            .file_path = file_path,
            .include_dirs = context->include_dirs
          };
          td_include(&subcontext);
          included_file_data = subcontext.result;
          break;
        }
      }

      if (!included_file_data) {
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

void td_include_free_context(IncludeContext* context) {
   if (context->file_data.data) free(context->file_data.data);
}

#endif


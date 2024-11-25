///////////
// ENUMS //
///////////



DN_API i32  find_uniform_index(const char* name);
DN_API void set_uniform_immediate_mat4(const char* name, HMM_Mat4 value);
DN_API void set_uniform_immediate_mat3(const char* name, HMM_Mat3 value);
DN_API void set_uniform_immediate_vec4(const char* name, HMM_Vec4 value);
DN_API void set_uniform_immediate_vec3(const char* name, HMM_Vec3 value);
DN_API void set_uniform_immediate_vec2(const char* name, Vector2 value);
DN_API void set_uniform_immediate_i32(const char* name, i32 value);
DN_API void set_uniform_immediate_f32(const char* name, float value);
DN_API void set_uniform_immediate_texture(const char* name, i32 value);


////////////////////////////////////
// IMMEDIATE OPENGL CONFIGURATION //
////////////////////////////////////
i32 find_uniform_index(const char* name) {
	i32 program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	return glGetUniformLocation(program, name);
}

void set_uniform_immediate_vec4(const char* name, HMM_Vec4 vec) {
	i32 index = find_uniform_index(name);
	glUniform4f(index, vec.X, vec.Y, vec.Z, vec.W);
}

void set_uniform_immediate_vec3(const char* name, HMM_Vec3 vec) {
	i32 index = find_uniform_index(name);
	glUniform3f(index, vec.X, vec.Y, vec.Z);
}

void set_uniform_immediate_vec2(const char* name, Vector2 vec) {
	i32 index = find_uniform_index(name);
	glUniform2f(index, vec.x, vec.y);
}

void set_uniform_immediate_mat3(const char* name, HMM_Mat3 matrix) {
	i32 index = find_uniform_index(name);
	glUniformMatrix3fv(index, 1, GL_FALSE, (const float*)&matrix);
}

void set_uniform_immediate_mat4(const char* name, HMM_Mat4 matrix) {
	i32 index = find_uniform_index(name);
	glUniformMatrix4fv(index, 1, GL_FALSE, (const float*)&matrix);
}

void set_uniform_immediate_i32(const char* name, i32 val) {
	i32 index = find_uniform_index(name);
	glUniform1i(index, val);
}

void set_uniform_immediate_f32(const char* name, float val) {
	i32 index = find_uniform_index(name);
	glUniform1f(index, val);
}

void set_uniform_immediate_texture(const char* name, i32 val) {
	set_uniform_immediate_i32(name, val);
}



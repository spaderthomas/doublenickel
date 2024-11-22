////////////////////
// SHADER LOADING //
////////////////////
tstring build_shader_source(const char* file_path) {
	auto shader_file = copy_string(file_path, &bump_allocator);
	auto shader_directory = resolve_named_path("shaders");
	auto error = bump_allocator.alloc<char>(256);
	
	auto preprocessed_source = stb_include_file(shader_file, nullptr, shader_directory, error);
	if (!preprocessed_source) {
		tdns_log.write("shader preprocessor error; shader = %s, err = %s", shader_file, error);
		return copy_string("YOUR_SHADER_FAILED_TO_COMPILE", &bump_allocator);
	}
	
	auto source = copy_string(preprocessed_source, &bump_allocator);
	
	free(preprocessed_source);
	
	return source;
}

void check_shader_compilation(u32 shader, const char* file_path) {
	i32 success;
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		static constexpr u32 error_size = 512;
		auto compilation_status = bump_allocator.alloc<char>(error_size);
		
		glGetShaderInfoLog(shader, error_size, NULL, compilation_status);

		tdns_log.write("shader compile error; shader = %s, err = %s", file_path, compilation_status);
	}
}

void check_shader_linkage(u32 program, const char* file_path) {
	i32 success;
	
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		static constexpr u32 error_size = 512;
		auto compilation_status = bump_allocator.alloc<char>(error_size);
		
		glGetProgramInfoLog(program, error_size, NULL, compilation_status);
		tdns_log.write("shader link error; shader = %s, err = %s", file_path, compilation_status);
	}
}


////////////////
// GPU SHADER //
////////////////
void GpuShader::init(GpuShaderDescriptor descriptor) {
	if (descriptor.kind == GpuShaderKind::Compute) {
		auto compute_path = resolve_format_path_ex("compute_shader", descriptor.compute_shader, &bump_allocator);
		init_compute_ex(descriptor.name, compute_path);
	}
	else if (descriptor.kind == GpuShaderKind::Graphics) {
		auto vertex_path = resolve_format_path_ex("vertex_shader", descriptor.vertex_shader, &bump_allocator);
		auto fragment_path = resolve_format_path_ex("fragment_shader", descriptor.fragment_shader, &bump_allocator);
		init_graphics_ex(descriptor.name, vertex_path, fragment_path);
	}
}

void GpuShader::init_graphics(const char* name) {
	auto vertex_path = resolve_format_path_ex("vertex_shader", name, &bump_allocator);
	auto fragment_path = resolve_format_path_ex("fragment_shader", name, &bump_allocator);
	init_graphics_ex(name, vertex_path, fragment_path);
}

void GpuShader::init_graphics_ex(const char* name, const char* vertex_shader, const char* fragment_shader) {
	kind = GpuShader::Kind::Graphics;
	this->name = copy_string(name);
	this->vertex_path = copy_string(vertex_shader);
	this->fragment_path = copy_string(fragment_shader);
	

	const char* paths[] = {
		vertex_path,
		fragment_path
	};

	unsigned int shader_program = glCreateProgram();
	
	fox_for(index, 2) {
		auto file_path = paths[index];
		auto source = build_shader_source(file_path);
		if (!source) return;
		
		// Compile the shader
		unsigned int shader_kind = (index == 0) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		unsigned int shader = glCreateShader(shader_kind);
		if (shader_kind == GL_VERTEX_SHADER) {
			vertex = shader;
		}
		else if (shader_kind == GL_FRAGMENT_SHADER) {
			fragment = shader;
		}

		u32 num_shaders = 1;
		glShaderSource(shader, num_shaders, &source, NULL);
		glCompileShader(shader);
		check_shader_compilation(shader, file_path);

		glAttachShader(shader_program, shader);
	}
		
	// Link into a shader program
	glLinkProgram(shader_program);
	check_shader_linkage(shader_program, vertex_path);

	// Push the data into the shader. If anything fails, the shader won't get the new GL handles
	program = shader_program;
	glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, (int*)&num_uniforms);

	set_gl_name(static_cast<u32>(GlId::Program), program, strlen(name), name);
}

void GpuShader::init_compute(const char* name) {
	init_compute_ex(name, resolve_format_path_ex("compute_shader", name, &bump_allocator));
}

void GpuShader::init_compute_ex(const char* name, const char* compute_path) {
	this->kind = GpuShader::Kind::Compute;
	this->name = copy_string(name);
	this->compute_path = copy_string(compute_path);
	auto source = build_shader_source(this->compute_path);
	
	u32 num_shaders = 1;
	this->compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(this->compute, num_shaders, &source, NULL);
	glCompileShader(this->compute);
	check_shader_compilation(this->compute, this->compute_path);

	this->program = glCreateProgram();
	glAttachShader(this->program, this->compute);
	glLinkProgram(this->program);
	check_shader_linkage(this->program, vertex_path);
}

void GpuShader::reload() {
	//tdns_log.write("Reloading shader %s (%s)", name, kind == Shader::Kind::Graphics ? "Graphics" : "Compute");

	glDeleteProgram(program);

	if (kind == GpuShader::Kind::Graphics) {
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		
		init_graphics_ex(this->name, copy_string(this->vertex_path, &bump_allocator), copy_string(this->fragment_path, &bump_allocator));
	}
	else if (kind == GpuShader::Kind::Compute) {
		glDeleteShader(compute);
		
		init_compute_ex(this->name, copy_string(this->compute_path, &bump_allocator));
	}
}


/////////////
// UNIFORM //
/////////////
Uniform::Uniform() : kind(UniformKind::I32), as_i32(0) {}
Uniform::Uniform(const char* name) : kind(UniformKind::I32), as_i32(0) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, const HMM_Mat4& m) : kind(UniformKind::Matrix4), mat4(m) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, const HMM_Mat3& m) : kind(UniformKind::Matrix3), mat3(m) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, const HMM_Vec4& v) : kind(UniformKind::Vector4), vec4(v) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, const HMM_Vec3& v) : kind(UniformKind::Vector3), vec3(v) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, const Vector2& v) : kind(UniformKind::Vector2), vec2(v) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, int32 i) : kind(UniformKind::I32), as_i32(i) {
	strncpy(this->name, name, max_name_len);
}
Uniform::Uniform(const char* name, float32 f) : kind(UniformKind::F32), f32(f) {
	strncpy(this->name, name, max_name_len);
}

bool are_uniforms_equal(Uniform& a, Uniform& b) {
	if (a.kind != b.kind) return false;
	if (std::strncmp(a.name, b.name, Uniform::max_name_len) != 0) return false;

	// Then, compare the union members based on `kind`
	switch (a.kind) {
		case UniformKind::Matrix4:
			return !std::memcmp(&a.mat4, &b.mat4, sizeof(HMM_Mat4));
		case UniformKind::Matrix3:
			return std::memcmp(&a.mat3, &b.mat3, sizeof(HMM_Mat3));
		case UniformKind::Vector4:
			return a.vec4 == b.vec4;
		case UniformKind::Vector3:
			return a.vec3 == b.vec3;
		case UniformKind::Vector2:
			return HMM_EqV2(a.vec2, b.vec2);
		case UniformKind::I32:
			return a.as_i32 == b.as_i32;
		case UniformKind::F32:
			return a.f32 == b.f32;
		case UniformKind::Texture:
			return a.texture == b.texture;
		default:
			return true;
	}
}
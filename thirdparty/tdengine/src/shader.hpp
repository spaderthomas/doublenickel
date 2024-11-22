#define MAX_UNIFORMS 32
#define MAX_UNIFORM_LEN 32

enum class UniformKind : u32 {
	None = 0,
	Matrix4 = 1,
	Matrix3 = 2,
	Vector4 = 3,
	Vector3 = 4,
	Vector2 = 5,
	I32 = 6,
	F32 = 7,
	Texture = 100,
	PipelineOutput = 101,
	RenderTarget = 102,
};


struct Uniform {
	UniformKind kind = UniformKind::None;

	static constexpr u32 max_name_len = 64;
	char name [max_name_len];
	
	union {
		HMM_Mat4 mat4;
		HMM_Mat3 mat3;
		HMM_Vec4 vec4;
		HMM_Vec3 vec3;
		Vector2 vec2;
		i32 as_i32;
		float32 f32;
		i32 texture;
	};

	Uniform();
	Uniform(const char* name);
	Uniform(const char* name, const HMM_Mat4& m);
	Uniform(const char* name, const HMM_Mat3& m);
	Uniform(const char* name, const HMM_Vec4& v);
	Uniform(const char* name, const HMM_Vec3& v);
	Uniform(const char* name, const Vector2& v);
	Uniform(const char* name, i32 i);
	Uniform(const char* name, float32 f);
};
bool are_uniforms_equal(Uniform& a, Uniform& b);

enum class GpuShaderKind : u32 {
	Graphics = 0,
	Compute = 1
};

struct GpuShaderDescriptor {
	string name;
	string vertex_shader;
	string fragment_shader;
	string compute_shader;

	GpuShaderKind kind;
};

struct GpuShader {
	enum class Kind : i32 {
		Graphics,
		Compute
	};

	Kind kind;
	string name;
	
	u32 program = 0;
	
	string vertex_path;
	string fragment_path;
	u32 vertex = 0;
	u32 fragment = 0;

	string compute_path;
	u32 compute = 0;

	u32 num_uniforms = 0;
	
	static int active;

	void init(GpuShaderDescriptor descriptor);
	void init_graphics(const char* name);
	void init_graphics_ex(const char* name, const char* vertex_path, const char* fragment_path);
	void init_compute(const char* name);
	void init_compute_ex(const char* name, const char* compute_path);
	void reload();	
};
int GpuShader::active = -1;
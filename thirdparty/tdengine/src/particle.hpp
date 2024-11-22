enum class ParticleKind : i32 {
	Quad,
	Circle,
	Image,
	Invalid
};

struct ParticleQuad {
	Vector2 size;
};

struct ParticleCircle {
	float radius;
};

struct ParticleImage {
	Sprite* sprite;
	Vector2 size;
};

union ParticleData {
	ParticleQuad quad;
	ParticleCircle circle;
	ParticleImage image;
};

struct Particle {
	Vector2 position;
	Interpolator2 velocity;

	float lifetime;
	float accumulated;

	ParticleKind kind;
	ParticleData data;
	Vector4 base_color;
	Vector4 color;

	bool occupied = false;
	Particle* next = nullptr;
};

enum class ParticlePositionMode {
	Bottom,
};

struct ParticleSystemFrame {
	int spawned;
	int despawned;
	int alive;
};

struct ParticleSystem {
	bool occupied;
	int32 generation;

	static constexpr int max_particles = 4096;
	Array<Particle> particles;
	Particle* free_list;

	// RUNTIME
	ParticleSystemFrame frame_stats;
	int num_spawned;
	float spawn_accumulated;
	bool warm;
	bool emit;

	// PARAMETERS
	ParticleKind particle_kind;
	ParticleQuad quad;
	ParticleCircle circle;
	ParticleImage image;
	Vector4 color;
	ParticlePositionMode position_mode;
	Vector2 position;
	Vector2 area;
	Interpolator2 velocity;
	Vector2 velocity_jitter;
	bool jitter_base_velocity;
	bool jitter_max_velocity;
	int32 layer;
	int max_spawn;
	float spawn_rate;
	float lifetime;
	int warmup_iter;
	Vector2 gravity_source;
	float gravity_intensity;
	bool gravity_enabled;
	float size_jitter;
	bool jitter_size;
	float master_opacity;
	float opacity_jitter;
	bool jitter_opacity;
	float opacity_interpolate_target;
	float opacity_interpolate_time;
	bool opacity_interpolate_active;

	void init();
	void deinit();
	void update();
	void despawn_particle(Particle* particle);
	bool spawn_particle();
};

struct ParticleSystemHandle {
	int32 index = -1;
	int32 generation = -1;

	operator bool();
};

void init_particles();

ParticleSystem* find_particle_system(ParticleSystemHandle handle);
FM_LUA_EXPORT ParticleSystemHandle make_particle_system();
FM_LUA_EXPORT void free_particle_system(ParticleSystemHandle handle);
FM_LUA_EXPORT ParticleSystemFrame check_particle_system(ParticleSystemHandle handle);
FM_LUA_EXPORT void stop_particle_emission(ParticleSystemHandle handle);
FM_LUA_EXPORT void start_particle_emission(ParticleSystemHandle handle);
FM_LUA_EXPORT void clear_particles(ParticleSystemHandle handle);
FM_LUA_EXPORT void update_particles(ParticleSystemHandle handle);
FM_LUA_EXPORT void draw_particles(ParticleSystemHandle handle);
FM_LUA_EXPORT void stop_all_particles();

FM_LUA_EXPORT void set_particle_lifetime(ParticleSystemHandle handle, float lifetime);
FM_LUA_EXPORT void set_particle_max_spawn(ParticleSystemHandle handle, int max_spawn);
FM_LUA_EXPORT void set_particle_spawn_rate(ParticleSystemHandle handle, float spawn_rate);
FM_LUA_EXPORT void set_particle_size(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_radius(ParticleSystemHandle handle, float r);
FM_LUA_EXPORT void set_particle_sprite(ParticleSystemHandle handle, const char* sprite);
FM_LUA_EXPORT void set_particle_position_mode(ParticleSystemHandle handle, ParticlePositionMode mode);
FM_LUA_EXPORT void set_particle_position(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_area(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_kind(ParticleSystemHandle handle, ParticleKind kind);
FM_LUA_EXPORT void set_particle_color(ParticleSystemHandle handle, float r, float g, float b, float a);
FM_LUA_EXPORT void set_particle_layer(ParticleSystemHandle handle, int32 layer);
FM_LUA_EXPORT void set_particle_velocity_fn(ParticleSystemHandle handle, InterpolationFn function);
FM_LUA_EXPORT void set_particle_velocity_base(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_velocity_max(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_velocity_jitter(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_jitter_base_velocity(ParticleSystemHandle hanel, bool jitter);
FM_LUA_EXPORT void set_particle_jitter_max_velocity(ParticleSystemHandle handle, bool jitter);
FM_LUA_EXPORT void set_particle_size_jitter(ParticleSystemHandle handle, float jitter);
FM_LUA_EXPORT void set_particle_jitter_size(ParticleSystemHandle handle, bool jitter);
FM_LUA_EXPORT void set_particle_master_opacity(ParticleSystemHandle handle, float opacity);
FM_LUA_EXPORT void set_particle_opacity_jitter(ParticleSystemHandle handle, float jitter);
FM_LUA_EXPORT void set_particle_jitter_opacity(ParticleSystemHandle handle, bool jitter);
FM_LUA_EXPORT void set_particle_opacity_interpolation(ParticleSystemHandle handle, bool active, float start_time, float interpolate_to);
FM_LUA_EXPORT void set_particle_warm(ParticleSystemHandle handle, bool warm);
FM_LUA_EXPORT void set_particle_warmup(ParticleSystemHandle handle, int32 iter);
FM_LUA_EXPORT void set_particle_gravity_source(ParticleSystemHandle handle, float x, float y);
FM_LUA_EXPORT void set_particle_gravity_intensity(ParticleSystemHandle handle, float intensity);
FM_LUA_EXPORT void set_particle_gravity_enabled(ParticleSystemHandle handle, bool enabled);


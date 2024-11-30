enum class ParticleKind : i32 {
	Quad,
	Circle,
	Image,
	Invalid
};

struct ParticleQuad {
	dn_vector2_t size;
};

struct ParticleCircle {
	float radius;
};

struct ParticleImage {
	Sprite* sprite;
	dn_vector2_t size;
};

union ParticleData {
	ParticleQuad quad;
	ParticleCircle circle;
	ParticleImage image;
};

struct Particle {
	dn_vector2_t position;
	Interpolator2 velocity;

	float lifetime;
	float accumulated;

	ParticleKind kind;
	ParticleData data;
	dn_vector4_t base_color;
	dn_vector4_t color;

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
	i32 generation;

	static constexpr int max_particles = 4096;
	dn_array_t<Particle> particles;
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
	dn_vector4_t color;
	ParticlePositionMode position_mode;
	dn_vector2_t position;
	dn_vector2_t area;
	Interpolator2 velocity;
	dn_vector2_t velocity_jitter;
	bool jitter_base_velocity;
	bool jitter_max_velocity;
	i32 layer;
	int max_spawn;
	float spawn_rate;
	float lifetime;
	int warmup_iter;
	dn_vector2_t gravity_source;
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
	i32 index = -1;
	i32 generation = -1;

	operator bool();
};

dn_array_t<ParticleSystem, 64> particle_systems;

void init_particles();

ParticleSystem* find_particle_system(ParticleSystemHandle handle);
DN_API ParticleSystemHandle make_particle_system();
DN_API void free_particle_system(ParticleSystemHandle handle);
DN_API ParticleSystemFrame check_particle_system(ParticleSystemHandle handle);
DN_API void stop_particle_emission(ParticleSystemHandle handle);
DN_API void start_particle_emission(ParticleSystemHandle handle);
DN_API void clear_particles(ParticleSystemHandle handle);
DN_API void update_particles(ParticleSystemHandle handle);
DN_API void draw_particles(ParticleSystemHandle handle);
DN_API void stop_all_particles();

DN_API void set_particle_lifetime(ParticleSystemHandle handle, float lifetime);
DN_API void set_particle_max_spawn(ParticleSystemHandle handle, int max_spawn);
DN_API void set_particle_spawn_rate(ParticleSystemHandle handle, float spawn_rate);
DN_API void set_particle_size(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_radius(ParticleSystemHandle handle, float r);
DN_API void set_particle_sprite(ParticleSystemHandle handle, const char* sprite);
DN_API void set_particle_position_mode(ParticleSystemHandle handle, ParticlePositionMode mode);
DN_API void set_particle_position(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_area(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_kind(ParticleSystemHandle handle, ParticleKind kind);
DN_API void set_particle_color(ParticleSystemHandle handle, float r, float g, float b, float a);
DN_API void set_particle_layer(ParticleSystemHandle handle, i32 layer);
DN_API void set_particle_velocity_fn(ParticleSystemHandle handle, InterpolationFn function);
DN_API void set_particle_velocity_base(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_velocity_max(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_velocity_jitter(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_jitter_base_velocity(ParticleSystemHandle hanel, bool jitter);
DN_API void set_particle_jitter_max_velocity(ParticleSystemHandle handle, bool jitter);
DN_API void set_particle_size_jitter(ParticleSystemHandle handle, float jitter);
DN_API void set_particle_jitter_size(ParticleSystemHandle handle, bool jitter);
DN_API void set_particle_master_opacity(ParticleSystemHandle handle, float opacity);
DN_API void set_particle_opacity_jitter(ParticleSystemHandle handle, float jitter);
DN_API void set_particle_jitter_opacity(ParticleSystemHandle handle, bool jitter);
DN_API void set_particle_opacity_interpolation(ParticleSystemHandle handle, bool active, float start_time, float interpolate_to);
DN_API void set_particle_warm(ParticleSystemHandle handle, bool warm);
DN_API void set_particle_warmup(ParticleSystemHandle handle, i32 iter);
DN_API void set_particle_gravity_source(ParticleSystemHandle handle, float x, float y);
DN_API void set_particle_gravity_intensity(ParticleSystemHandle handle, float intensity);
DN_API void set_particle_gravity_enabled(ParticleSystemHandle handle, bool enabled);


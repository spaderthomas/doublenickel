//////////////////////////
// LAGRANGIAN FLUID SIM //
//////////////////////////
namespace LagrangianFluidSim {
	constexpr u32 threads_per_workgroup = 32;

	namespace Kernel {
		constexpr i32 init_base = 1;
		constexpr i32 init_density = 2;
		constexpr i32 update_density = 10;
		constexpr i32 update_predicted_position = 11;
		constexpr i32 update_viscosity = 12;
		constexpr i32 update_acceleration = 13;
	}
	
	using ssbo = u32;

	struct IndirectDrawCommand {
		u32 count;
		u32 instance_count;
		u32 first;
		u32 base_instance;
	};
	
	struct Particle {
		 Vector4 color;
		 Vector2 position;
		 Vector2 predicted_position;
		 Vector2 velocity;
		 Vector2 local_velocity;
	};

	struct FluidProperties {
		float density;
		float pressure;
		float padding;
		float padding_;
	};
	
	struct GpuSystem {
		Vector2 pa;
		Vector2 pb;
		Vector2 velocity;
		float radius;

		float smoothing_radius_px = 4.0;
		float particle_mass = 1.0;
		float viscosity = 50.0;
		float pressure = 100.0;
		float gravity = 100.0;
		float dt = 1.f / 60.f;

		u32 next_unprocessed_index;
		u32 num_particles;
	};
	
	struct System {
		GpuSystem gpu;

		u32 num_workgroups;
		
		ssbo particles;
		ssbo fluid;
		ssbo indirect_draw_command;
		ssbo system;
	};

	GenerationalArena<System> systems;

	void init();
	void update();
	void draw();
}

FM_LUA_EXPORT ArenaHandle lf_create(u32 num_particles);
FM_LUA_EXPORT void lf_destroy(ArenaHandle handle);
FM_LUA_EXPORT void lf_destroy_all();
FM_LUA_EXPORT void lf_init(ArenaHandle handle);
FM_LUA_EXPORT void lf_inspect(ArenaHandle handle);
FM_LUA_EXPORT void lf_set_volume(ArenaHandle handle, float ax, float ay, float bx, float by, float radius);
FM_LUA_EXPORT void lf_set_velocity(ArenaHandle handle, float x, float y);
FM_LUA_EXPORT void lf_set_smoothing_radius(ArenaHandle handle, float r);
FM_LUA_EXPORT void lf_set_particle_mass(ArenaHandle handle, float mass);
FM_LUA_EXPORT void lf_set_viscosity(ArenaHandle handle, float viscosity);
FM_LUA_EXPORT void lf_set_pressure(ArenaHandle handle, float pressure);
FM_LUA_EXPORT void lf_set_gravity(ArenaHandle handle, float gravity);
FM_LUA_EXPORT void lf_set_timestep(ArenaHandle handle, float dt);
FM_LUA_EXPORT void lf_draw_all();
FM_LUA_EXPORT void lf_bind(ArenaHandle handle);
FM_LUA_EXPORT void lf_update(ArenaHandle handle);
FM_LUA_EXPORT void lf_draw(ArenaHandle handle);

////////////////////////
// EULERIAN FLUID SIM //
////////////////////////
namespace EulerianFluidSim {
	constexpr u32 threads_per_workgroup = 32;

	namespace Kernel {
		constexpr i32 init = 1;
		constexpr i32 update_sources = 10;
		constexpr i32 update_buffered_density = 12;
		constexpr i32 update_diffuse = 13;
		constexpr i32 update_diffuse_bounds = 14;
		constexpr i32 update_diffuse_corners = 15;
		constexpr i32 update_advect = 16;
	}
	
	using ssbo = u32;
	
	struct Fluid {
		Vector2 velocity;
		float density;
		float buffered_density;
		float padding [4];
	};

	struct Source {
		Vector2 velocity;
		float density;
		float padding [1];
	};

	struct GpuSystem {
		u32 next_unprocessed_index;
		u32 grid_size;
	};
	
	struct System {
		GpuSystem gpu;

		u32 num_workgroups;
		u32 num_boundary_workgroups;
		u32 render_size;
		u32 num_cells;
		u32 num_sim_cells;
		u32 gauss_seidel_iterations;

		Array<Source> sources;
		
		ssbo fluid;
		ssbo source;
		ssbo system;
	};
	
	GenerationalArena<System> systems;

	void init();
	void update();
	void draw();
}

FM_LUA_EXPORT ArenaHandle ef_create(u32 grid_size);
FM_LUA_EXPORT void ef_destroy(ArenaHandle handle);
FM_LUA_EXPORT void ef_destroy_all();
FM_LUA_EXPORT void ef_init(ArenaHandle handle);
FM_LUA_EXPORT void ef_inspect(ArenaHandle handle);
FM_LUA_EXPORT u32 ef_pair_to_index(u32 grid_size, u32 x, u32 y);
FM_LUA_EXPORT void ef_set_render_size(ArenaHandle handle, u32 render_size);
FM_LUA_EXPORT void ef_clear_density_source(ArenaHandle handle);
FM_LUA_EXPORT void ef_set_density_source(ArenaHandle handle, u32 x, u32 y, float amount);
FM_LUA_EXPORT void ef_set_velocity(ArenaHandle handle, u32 x, u32 y, float vx, float vy);
FM_LUA_EXPORT void ef_set_gauss_seidel(ArenaHandle handle, u32 iterations);
FM_LUA_EXPORT void ef_bind(ArenaHandle handle);
FM_LUA_EXPORT void ef_update(ArenaHandle handle);
FM_LUA_EXPORT void ef_draw(ArenaHandle handle);


///////////////
// INTERNALS //
///////////////
void init_fluid();
void draw_gpu_particles();

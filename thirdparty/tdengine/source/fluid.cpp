//////////////
// INTERNAL //
//////////////
void init_fluid() {
	LagrangianFluidSim::init();
	EulerianFluidSim::init();
}

void sync_gpu_buffer(u32 handle, void* data, u32 size) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void zero_gpu_buffer(u32 handle, u32 size) {
	auto data = dn::allocator::alloc<u8>(&dn_allocators.bump, size);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, (void*)data, GL_STATIC_DRAW);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

//////////////////////////
// LAGRANGIAN FLUID SIM //
//////////////////////////
namespace LagrangianFluidSim {
	void inspect(LagrangianFluidSim::System& system) {
		dn_gpu_error_clear();
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, system.particles);
		auto particles = (LagrangianFluidSim::Particle*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, system.fluid);
		auto fluid = (LagrangianFluidSim::FluidProperties*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, system.indirect_draw_command);
		auto idc = (LagrangianFluidSim::IndirectDrawCommand*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, system.system);
		auto gpu_system = (LagrangianFluidSim::GpuSystem*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void bind_ssbos(LagrangianFluidSim::System& system) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, system.system);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, system.particles);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, system.fluid);
	}

	void run_kernel(LagrangianFluidSim::System& system, i32 kernel) {
		LagrangianFluidSim::GpuSystem gpu_system = system.gpu;
		gpu_system.next_unprocessed_index = 0;
		sync_gpu_buffer(system.system, &gpu_system, sizeof(LagrangianFluidSim::GpuSystem));
	
		// set_uniform_immediate_i32("fluid_kernel", kernel);
		bind_ssbos(system);
		glDispatchCompute(system.num_workgroups, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void init() {
		systems.init(32);
	}
}

dn_gen_arena_handle_t lf_create(u32 num_particles) {
	dn_gpu_error_clear();
	auto handle = LagrangianFluidSim::systems.insert();
	auto system = LagrangianFluidSim::systems[handle];
	
	system->gpu.num_particles = num_particles;
	system->num_workgroups = dn_math_ceil_divide(system->gpu.num_particles, LagrangianFluidSim::threads_per_workgroup);
	
	// SSBO 1: Particle data. We don't zero the memory because this could be a huge block.
	glGenBuffers(1, &system->particles);
	sync_gpu_buffer(system->particles, nullptr, system->gpu.num_particles * sizeof(LagrangianFluidSim::Particle));

	// SSBO 2: Indirect draw command
	glGenBuffers(1, &system->indirect_draw_command);
	LagrangianFluidSim::IndirectDrawCommand draw_command;
	draw_command.count = 6;
	draw_command.instance_count = system->gpu.num_particles;
	draw_command.first = 0;
	draw_command.base_instance = 0;
	sync_gpu_buffer(system->indirect_draw_command, &draw_command, sizeof(LagrangianFluidSim::IndirectDrawCommand));
	
	// SSBO 3: Particle system parameters		
	glGenBuffers(1, &system->system);
	zero_gpu_buffer(system->system, sizeof(LagrangianFluidSim::GpuSystem));

	// SSBO 4: Fluid properties		
	glGenBuffers(1, &system->fluid);
	sync_gpu_buffer(system->fluid, nullptr, system->gpu.num_particles * sizeof(LagrangianFluidSim::FluidProperties));

	return handle;
}

void lf_destroy(dn_gen_arena_handle_t handle) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glDeleteBuffers(1, &system->particles);
	glDeleteBuffers(1, &system->indirect_draw_command);
	glDeleteBuffers(1, &system->system);
	glDeleteBuffers(1, &system->fluid);

	LagrangianFluidSim::systems.remove(handle);
}

void lf_destroy_all() {
	LagrangianFluidSim::systems.clear();
}

void lf_init(dn_gen_arena_handle_t handle) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	// set_shader_immediate("fluid_init");
	LagrangianFluidSim::run_kernel(*system, LagrangianFluidSim::Kernel::init_base);
	LagrangianFluidSim::run_kernel(*system, LagrangianFluidSim::Kernel::init_density);
}

void lf_inspect(dn_gen_arena_handle_t handle) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	LagrangianFluidSim::inspect(*system);
}

void lf_set_volume(dn_gen_arena_handle_t handle, float ax, float ay, float bx, float by, float radius) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.pa = Vector2(ax, ay);
	gpu_system.pb = Vector2(bx, by);
	gpu_system.radius = radius;
}

void lf_set_velocity(dn_gen_arena_handle_t handle, float x, float y) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.velocity = Vector2(x, y);
}

void lf_set_smoothing_radius(dn_gen_arena_handle_t handle, float radius) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.smoothing_radius_px = radius;
}

void lf_set_particle_mass(dn_gen_arena_handle_t handle, float mass) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.particle_mass = mass;
}

void lf_set_viscosity(dn_gen_arena_handle_t handle, float viscosity) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.viscosity = viscosity;
}

void lf_set_pressure(dn_gen_arena_handle_t handle, float pressure) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.pressure = pressure;
}

void lf_set_gravity(dn_gen_arena_handle_t handle, float gravity) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.gravity = gravity;
}

void lf_set_timestep(dn_gen_arena_handle_t handle, float dt) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	auto& gpu_system = system->gpu;
	gpu_system.dt = dt;
}

void lf_draw(dn_gen_arena_handle_t handle) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	// Render the particles
	// set_shader_immediate("particle");
	// set_uniform_immediate_mat4("projection", render.projection);
	// set_uniform_immediate_mat4("view", HMM_Translate(HMM_V3(-render.camera.x, -render.camera.y, 0.f)));

	LagrangianFluidSim::bind_ssbos(*system);
		
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, system->indirect_draw_command);
	glDrawArraysIndirect(GL_TRIANGLES, 0);
}

void lf_update(dn_gen_arena_handle_t handle) {
	auto system = LagrangianFluidSim::systems[handle];
	if (!system) return;

	// set_shader_immediate("fluid_update");
	LagrangianFluidSim::run_kernel(*system, LagrangianFluidSim::Kernel::update_predicted_position);
	LagrangianFluidSim::run_kernel(*system, LagrangianFluidSim::Kernel::update_density);
	LagrangianFluidSim::run_kernel(*system, LagrangianFluidSim::Kernel::update_viscosity);
	LagrangianFluidSim::run_kernel(*system, LagrangianFluidSim::Kernel::update_acceleration);

}


////////////////////////
// EULERIAN FLUID SIM //
////////////////////////
namespace EulerianFluidSim {	
	void inspect(EulerianFluidSim::System& system) {
		dn_gpu_error_clear();
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, system.fluid);
		auto fluid = (EulerianFluidSim::Fluid*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, system.system);
		auto gpu_system = (EulerianFluidSim::GpuSystem*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	void bind_ssbos(EulerianFluidSim::System& system) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, system.system);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, system.fluid);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, system.source);
	}

	void run_kernel(EulerianFluidSim::System& system, i32 kernel, u32 num_workgroups = 0) {
		if (!num_workgroups) num_workgroups = system.num_workgroups;
		
		EulerianFluidSim::GpuSystem gpu_system = system.gpu;
		gpu_system.next_unprocessed_index = 0;
		sync_gpu_buffer(system.system, &gpu_system, sizeof(EulerianFluidSim::GpuSystem));
	
		// set_uniform_immediate_i32("fluid_kernel", kernel);
		bind_ssbos(system);
		glDispatchCompute(num_workgroups, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void init() {
		systems.init(32);
	}
}

dn_gen_arena_handle_t ef_create(u32 grid_size) {
	auto handle = EulerianFluidSim::systems.insert();
	auto system = EulerianFluidSim::systems[handle];
	
	system->gpu.grid_size = grid_size;
	system->num_sim_cells = grid_size * grid_size;
	system->num_cells = (grid_size + 2) * (grid_size + 2);
	system->num_workgroups = dn_math_ceil_divide(system->num_sim_cells, EulerianFluidSim::threads_per_workgroup);
	system->num_boundary_workgroups = dn_math_ceil_divide(system->gpu.grid_size, EulerianFluidSim::threads_per_workgroup);
	system->gauss_seidel_iterations = 20;
	
	arr_init(&system->sources, system->num_cells);
	system->sources.size = system->sources.capacity;

	glGenBuffers(1, &system->system);
	zero_gpu_buffer(system->system, sizeof(EulerianFluidSim::GpuSystem));

	glGenBuffers(1, &system->fluid);
	zero_gpu_buffer(system->fluid, system->num_cells * sizeof(EulerianFluidSim::Fluid));

	glGenBuffers(1, &system->source);
	zero_gpu_buffer(system->source, system->num_cells * sizeof(EulerianFluidSim::Source));

	return handle;
}

void ef_destroy(dn_gen_arena_handle_t handle) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glDeleteBuffers(1, &system->system);
	glDeleteBuffers(1, &system->fluid);
	glDeleteBuffers(1, &system->source);

	arr_free(&system->sources);

	EulerianFluidSim::systems.remove(handle);
}

void ef_destroy_all() {
	EulerianFluidSim::systems.clear();
}

void ef_init(dn_gen_arena_handle_t handle) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	// set_shader_immediate("fluid_eulerian_init");
	EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::init);
}

void ef_inspect(dn_gen_arena_handle_t handle) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	EulerianFluidSim::inspect(*system);
}

u32 ef_pair_to_index(u32 grid_size, u32 x, u32 y) {
	u32 bx = x + 1;
	u32 by = y + 1;
	return ((grid_size + 2) * by) + bx;
}

void ef_set_render_size(dn_gen_arena_handle_t handle, u32 render_size) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	system->render_size = render_size;
}

void ef_set_velocity(dn_gen_arena_handle_t handle, u32 x, u32 y, float vx, float vy) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	auto index = ef_pair_to_index(system->gpu.grid_size, x, y);
	system->sources[index]->velocity.x = vx;
	system->sources[index]->velocity.y = vy;
}

void ef_clear_density_source(dn_gen_arena_handle_t handle) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	arr_clear(&system->sources);
	system->sources.size = system->sources.capacity;
}

void ef_set_density_source(dn_gen_arena_handle_t handle, u32 x, u32 y, float amount) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;
	
	auto index = ef_pair_to_index(system->gpu.grid_size, x, y);
	system->sources[index]->density = amount;
}

void ef_set_gauss_seidel(dn_gen_arena_handle_t handle, u32 iterations) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	system->gauss_seidel_iterations = iterations;
}

void ef_bind(dn_gen_arena_handle_t handle) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	EulerianFluidSim::bind_ssbos(*system);
}

void ef_update(dn_gen_arena_handle_t handle) {
	auto system = EulerianFluidSim::systems[handle];
	if (!system) return;

	sync_gpu_buffer(system->source, system->sources.data, sizeof(EulerianFluidSim::Source) * system->num_cells);
	// set_shader_immediate("fluid_eulerian_update");

	EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_sources);
	
	for (u32 i = 0; i < system->gauss_seidel_iterations; i++) {
		EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_buffered_density);
		EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_diffuse);
		EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_diffuse_bounds, system->num_boundary_workgroups);
		EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_diffuse_corners, 1);
	}
	
	EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_buffered_density);
	EulerianFluidSim::run_kernel(*system, EulerianFluidSim::Kernel::update_advect);
}

void ef_draw(dn_gen_arena_handle_t handle) {

}

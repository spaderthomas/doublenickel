////////////////////////////
// PARTICLE SYSTEM HANDLE //
////////////////////////////
ParticleSystemHandle::operator bool() {
	return index >= 0;
}


/////////
// FFI //
/////////
ParticleSystemHandle make_particle_system() {
	ParticleSystemHandle handle;
	for (int index = 0; index < particle_systems.size; index++) {
		auto particle_system = particle_systems[index];
		if (!particle_system->occupied) {
			particle_system->occupied = true;
			particle_system->generation++;
			particle_system->init();

			handle.index = index;
			handle.generation = particle_system->generation;
			return handle;
		}
	}

	return handle;
}

void free_particle_system(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->occupied = false;
	particle_system->generation++;
	particle_system->deinit();
}

ParticleSystemFrame check_particle_system(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return ParticleSystemFrame();
	
	return particle_system->frame_stats;
}

void start_particle_emission(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->emit = true;
}

void stop_particle_emission(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->emit = false;
}

void clear_particles(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	arr_for(particle_system->particles, particle) {
		if (!particle->occupied) continue;
		particle_system->despawn_particle(particle);
	}
}

void update_particles(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->update();
}

void draw_particles(ParticleSystemHandle handle) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	arr_for(particle_system->particles, particle) {
		if (!particle->occupied) continue;

		// DRAW ONE PARTICLE
		auto color = particle->color;
		color.a *= particle_system->master_opacity;
		if (particle->kind == ParticleKind::Quad) {
			auto& quad = particle->data.quad;
			draw_quad(particle->position, quad.size, color);
		}
		else if (particle->kind == ParticleKind::Circle) {
			auto& circle = particle->data.circle;
			draw_circle(particle->position.x, particle->position.y, circle.radius, color);
		}
		else if (particle->kind == ParticleKind::Image) {
			auto& image = particle->data.image;
			draw_image(image.sprite, particle->position.x, particle->position.y, image.size.x, image.size.y, color.a);
		}
	}
}

void stop_all_particles() {
	arr_for(particle_systems, particle_system) {
		if (particle_system->occupied) {
			particle_system->emit = false;
		}
	}
}



void set_particle_lifetime(ParticleSystemHandle handle, float lifetime) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->lifetime = lifetime;
}

void set_particle_spawn_rate(ParticleSystemHandle handle, float spawn_rate) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->spawn_rate = spawn_rate;
}

void set_particle_max_spawn(ParticleSystemHandle handle, int max_spawn) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->max_spawn = std::min(max_spawn, ParticleSystem::max_particles);
}

void set_particle_size(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	if (particle_system->particle_kind == ParticleKind::Quad) {
		particle_system->quad.size = Vector2(x, y);
	}
	else if (particle_system->particle_kind == ParticleKind::Image) {
		particle_system->image.size = Vector2(x, y);
	}
}

void set_particle_radius(ParticleSystemHandle handle, float radius) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	if (particle_system->particle_kind == ParticleKind::Circle) {
		particle_system->circle.radius = radius;
	}
}

void set_particle_sprite(ParticleSystemHandle handle, const char* sprite) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	if (particle_system->particle_kind == ParticleKind::Image) {
		particle_system->image.sprite = find_sprite(sprite);
	}
}

void set_particle_position_mode(ParticleSystemHandle handle, ParticlePositionMode mode) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->position_mode = mode;
}

void set_particle_position(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->position = Vector2(x, y);
}

void set_particle_kind(ParticleSystemHandle handle, ParticleKind particle_kind) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	// These are coming in from Lua, where you can type anything into the editor.
	if (static_cast<int32>(particle_kind) >= static_cast<int32>(ParticleKind::Invalid)) {
		particle_kind = ParticleKind::Invalid;
	}
	particle_system->particle_kind = particle_kind;
}

void set_particle_color(ParticleSystemHandle handle, float r, float g, float b, float a) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->color = Vector4{r, g, b, a};
}

void set_particle_layer(ParticleSystemHandle handle, int32 layer) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->layer = layer;
}

void set_particle_area(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->area = Vector2(x, y);
}

void set_particle_velocity_fn(ParticleSystemHandle handle, InterpolationFn function) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->velocity.function = function;
}

void set_particle_velocity_base(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->velocity.start = Vector2(x, y);
}

void set_particle_velocity_max(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->velocity.target = Vector2(x, y);
}

void set_particle_velocity_jitter(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;
	
	particle_system->velocity_jitter = Vector2(x, y);
}

void set_particle_jitter_base_velocity(ParticleSystemHandle handle, bool jitter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->jitter_base_velocity = jitter;
}

void set_particle_jitter_max_velocity(ParticleSystemHandle handle, bool jitter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->jitter_max_velocity = jitter;
}

void set_particle_size_jitter(ParticleSystemHandle handle, float jitter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->size_jitter = jitter;
}

void set_particle_jitter_size(ParticleSystemHandle handle, bool jitter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->jitter_size = jitter;
}

void set_particle_master_opacity(ParticleSystemHandle handle, float opacity) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->master_opacity = opacity;
}

void set_particle_opacity_jitter(ParticleSystemHandle handle, float jitter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->opacity_jitter = jitter;
}

void set_particle_jitter_opacity(ParticleSystemHandle handle, bool jitter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->jitter_opacity = jitter;
}

void set_particle_opacity_interpolation(ParticleSystemHandle handle, bool active, float start_time, float interpolate_to) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->opacity_interpolate_active = active;
	particle_system->opacity_interpolate_time = start_time;
	particle_system->opacity_interpolate_target = interpolate_to;
}

void set_particle_warm(ParticleSystemHandle handle, bool warm) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->warm = warm;
}


void set_particle_warmup(ParticleSystemHandle handle, int32 iter) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->warmup_iter = iter;
}

void set_particle_gravity_source(ParticleSystemHandle handle, float x, float y) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->gravity_source = Vector2(x, y);
}

void set_particle_gravity_intensity(ParticleSystemHandle handle, float intensity) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->gravity_intensity = intensity;
}

void set_particle_gravity_enabled(ParticleSystemHandle handle, bool enabled) {
	auto particle_system = find_particle_system(handle);
	if (!particle_system) return;

	particle_system->gravity_enabled = enabled;
}


/////////////////////
// PARTICLE SYSTEM //
/////////////////////
void ParticleSystem::init() {
	// INTERNAL DATA STRUCTURES
	arr_init(&particles, ParticleSystem::max_particles);
	particles.size = ParticleSystem::max_particles;
	
	for (int index = 0; index < particles.size - 1; index++) {
		particles[index]->next = particles[index + 1];
	}
	free_list = particles[0];

	// RESET TIMERS
	num_spawned = 0;
	spawn_accumulated = 0.f;
	warm = false;
	emit = true;

	// DEFAULT PARTICLE SYSTEM PARAMETERS
	particle_kind = ParticleKind::Quad;
	position_mode = ParticlePositionMode::Bottom;
	position = Vector2(0.f, 0.f);
	area = Vector2(100.f, 100.f);
	velocity.function = InterpolationFn::Linear;
	velocity.start = Vector2(1.f, 1.f);
	velocity.target = Vector2(10.f, 10.f);
	velocity.speed = 1.f;
	max_spawn = 100;
	spawn_rate = 1.f;
	lifetime = 1.f;
	layer = 31;
	warmup_iter = 0; 
	gravity_source = Vector2(0.f, 0.f); 
	gravity_intensity = 1.f; 
	gravity_enabled = false;
	master_opacity = 1.f;
	jitter_opacity = false;
	opacity_jitter = 0.f;
	opacity_interpolate_active = false;
	opacity_interpolate_time = 0.f;
	opacity_interpolate_target = 0.f;
}

void ParticleSystem::deinit() {
	arr_free(&particles);
}

void ParticleSystem::update() {
	frame_stats = ParticleSystemFrame();

	auto do_update = [&]() {
		spawn_accumulated += engine.dt;
		auto spawn_target = 1.f / spawn_rate;

		while (spawn_accumulated >= spawn_target && num_spawned < max_spawn) {
			spawn_accumulated -= spawn_target;
			if (!spawn_particle()) break;
		}

		float max_gravity_distance = HMM_LenV2(HMM_SubV2(gravity_source, position));
		float distance_threshold = 0.1f;
		float alignment_threshold = 0.95f;
		float deceleration = .99f;
		
		arr_for(particles, particle) {
			if (!particle->occupied) continue;

			frame_stats.alive++;

			// UPDATE ONE PARTICLE
			particle->accumulated += engine.dt;
			if (particle->accumulated >= particle->lifetime) {
				despawn_particle(particle);
			}
			else {
				if (gravity_enabled) {
					auto direction = HMM_SubV2(gravity_source, particle->position);
					auto direction_normal = HMM_NormV2(direction);
					
					auto distance = HMM_LenV2(direction);
					auto distance_ratio = std::min(distance / max_gravity_distance, 1.f);

					// Base gravity
					auto gravity_strength = gravity_intensity / 100.f;
					gravity_strength *= distance_ratio; // Accelerate more the farther you are from the source
					auto gravity = HMM_MulV2F(direction_normal, gravity_strength);

					particle->velocity.target = HMM_AddV2(particle->velocity.target, gravity);

					// Deceleration
					auto velocity_normal = HMM_NormV2(particle->velocity.target);
					auto alignment = HMM_DotV2(velocity_normal, direction_normal);
					if (distance_ratio < distance_threshold || alignment < alignment_threshold) {
						particle->velocity.target = HMM_MulV2F(particle->velocity.target, deceleration);
					}
				}

				particle->velocity.update();
				auto velocity = particle->velocity.get_value();
				particle->position.x += velocity.x;
				particle->position.y += velocity.y;

				if (opacity_interpolate_active) {
					auto accumulated = particle->accumulated - opacity_interpolate_time;
					auto remaining = particle->lifetime - opacity_interpolate_time;
					if (accumulated >= 0.f) {
						particle->color.a = interpolate_linear(particle->base_color.a, opacity_interpolate_target, accumulated / remaining);
					}
				}
			}
		}	
	};

	if (!warm && warmup_iter) {
		for (int i = 0; i < warmup_iter; i++) {
			do_update();
		}
	}
	warm = true;

	do_update();
}

bool ParticleSystem::spawn_particle() {
	if (!emit) return false;
	if (!free_list) return false;

	// INTERNAL DATA STRUCTURES
	auto particle = free_list;
	free_list = free_list->next;
	
	num_spawned++;

	frame_stats.spawned++;

	// ARENA
	particle->occupied = true;

	// PARTICLE DATA
	static constexpr float lifetime_jitter = .05f;
	particle->lifetime = lifetime + random_float(-1 * lifetime_jitter * lifetime, lifetime_jitter * lifetime);
	particle->accumulated = 0;
	
	if (position_mode == ParticlePositionMode::Bottom) {
		particle->position.x = random_float(position.x, position.x + area.x);
		particle->position.y = position.y;
	}
	else {
		particle->position = position;
	}

	particle->velocity = velocity;
	
	if (jitter_base_velocity) {
		particle->velocity.start.x += random_float(-1 * velocity_jitter.x, velocity_jitter.x);
		particle->velocity.start.y += random_float(-1 * velocity_jitter.y, velocity_jitter.y);
	}
	if (jitter_max_velocity) {
		particle->velocity.target.x += random_float(-1 * velocity_jitter.x, velocity_jitter.x);
		particle->velocity.target.y += random_float(-1 * velocity_jitter.y, velocity_jitter.y);
	}

	particle->kind = particle_kind;
	if (particle_kind == ParticleKind::Quad) {
		particle->data.quad = quad;
	}
	else if (particle_kind == ParticleKind::Circle) {
		particle->data.circle = circle;
	}
	else if (particle_kind == ParticleKind::Image) {
		particle->data.image = image;
	}

	if (jitter_size) {
		auto jitter = random_float(-1 * size_jitter, size_jitter);
		if (particle_kind == ParticleKind::Quad) {
			auto& size = particle->data.quad.size;
			size.x += jitter;
			size.y += jitter;
		}
		else if (particle_kind == ParticleKind::Circle) {
			auto& radius = particle->data.circle.radius;
			radius += jitter;
		}
		else if (particle_kind == ParticleKind::Image) {
			auto& size = particle->data.image.size;
			size.x += jitter;
			size.y += jitter;
		}
	}

	particle->base_color = color;
	if (jitter_opacity) {
		auto jitter = random_float(-1 * opacity_jitter, opacity_jitter);
		particle->base_color.a = std::min(particle->base_color.a + jitter, 1.f);
	}
	particle->color = particle->base_color;

	return true;
}

void ParticleSystem::despawn_particle(Particle* particle) {
	if (!particle) return;

	// INTERNAL DATA STRUCTURES
	particle->next = free_list;
	free_list = particle;
	
	num_spawned--;

	frame_stats.despawned++;

	// ARENA
	particle->occupied = false;
}


//////////////////
// ENTRY POINTS //
//////////////////
void init_particles() {
	particle_systems.size = particle_systems.capacity;
}

ParticleSystem* find_particle_system(ParticleSystemHandle handle) {
	if (!handle) return nullptr;

	auto particle_system = particle_systems[handle.index];
	if (!particle_system->occupied) return nullptr;
	if (particle_system->generation != handle.generation) return nullptr;
	return particle_system;
}

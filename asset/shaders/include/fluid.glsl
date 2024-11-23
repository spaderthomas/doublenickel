/////////////
// STRUCTS //
/////////////
struct FluidProperties {
	float density;
	float pressure;
	float padding;
	float padding_;
};
	
struct Particle {
	vec4 color;
	vec2 position;
	vec2 predicted_position;
	vec2 velocity;
	vec2 local_velocity;
};

struct ParticleSystem {
	vec2 pa;
	vec2 pb;
	vec2 velocity;
	float radius;
	
	float smoothing_radius_px;
	float particle_mass;
	float viscosity;
	float pressure;
	float gravity;
	float dt;

	uint next_unprocessed_index;
	uint num_particles;
};

struct CapsuleHitResult {
	bool hit;
	float distance;
	vec2 closest_point;
};

struct BoxHitResult {
	bool hit_x;
	bool hit_y;
	bool hit;
};


//////////
// SSBO //
//////////
layout (std430, binding = 0) buffer ParticleSystemBuffer {
	ParticleSystem system;
};

layout (std430, binding = 1) buffer ParticleBuffer {
	Particle particles [];
};

layout (std430, binding = 2) buffer FluidPropertiesBuffer {
	FluidProperties fluid [];
};

////////////////
// PARAMETERS //
////////////////
const vec2 fluid_tank = vec2(1920.0, 1080.0) * vec2(.125, .5);


///////////////
// COLLISION //
///////////////
CapsuleHitResult is_within_capsule(vec2 position, vec2 pa, vec2 pb, float radius) {
	CapsuleHitResult result;
	
	vec2 v = pb - pa;
	vec2 u = pa - position;

	float t = -1 * dot(v, u) / dot(v, v);
	t = clamp(t, 0.0, 1.0);
	result.closest_point = mix(pa, pb, t);
	
	float dm = distance(position, result.closest_point);
	float da = distance(position, pa);
	float db = distance(position, pb);
	result.distance = min(min(dm, da), db);
	result.hit = result.distance < radius + .001;
	
	if (result.distance == da) result.closest_point = pa;
	if (result.distance == db) result.closest_point = pb;

	result.closest_point += normalize(position - result.closest_point) * radius * 1.0;
	
	return result;
}

BoxHitResult is_within_box(vec2 point, vec2 position, vec2 size) {
	BoxHitResult result;
	result.hit_x = point.x >= position.x && point.x <= position.x + size.x;
	result.hit_y = point.y >= position.y && point.y <= position.y + size.y;
	result.hit = result.hit_x && result.hit_y;
	return result;
}

BoxHitResult is_within_system_box(vec2 point) {
	return is_within_box(point, vec2(system.pa.x, system.pa.y), vec2(system.radius));
}

CapsuleHitResult is_within_system(vec2 point) {
	return is_within_capsule(point, system.pa, system.pb, system.radius);
}

void resolve_box_collision(uint i, vec2 box_position, vec2 box_size) {
	BoxHitResult result = is_within_box(particles[i].position, box_position, box_size);
	if (!result.hit_x) {
		particles[i].position.x = clamp(particles[i].position.x, box_position.x, box_position.x + box_size.x);
		particles[i].local_velocity.x *= -1;
	}
	
	if (!result.hit_y) {
		particles[i].position.y = clamp(particles[i].position.y, box_position.y, box_position.y + box_size.y);
		particles[i].local_velocity.y *= -1;
	}
}

void resolve_capsule_collision(uint i) {
	CapsuleHitResult result = is_within_system(particles[i].position);
	if (!result.hit) {
		particles[i].local_velocity = vec2(particles[i].local_velocity.y, -particles[i].local_velocity.x) * .9;
		particles[i].position = result.closest_point;
		particles[i].color = green;
	} else {
		particles[i].color = white;
	}
}
   

void resolve_system_collision(uint i) {
	resolve_capsule_collision(i);
	//resolve_box_collision(i, vec2(system.pa.x, system.pa.y), vec2(system.radius));
}


///////////////////////
// SMOOTHING KERNELS //
///////////////////////
const float smooth_h = 1.0;

float convert_px_distance(float px) {
	return (px / system.smoothing_radius_px) * smooth_h;
}

// SMOOTHING_KERNEL_LAGUE_SMOOTH
const float SMOOTHING_KERNEL_LAGUE_SMOOTH_VOLUME = pi * pow(smooth_h, 8) / 4.0;
const float SMOOTHING_KERNEL_LAGUE_SMOOTH_DERIVATIVE_SCALE = -24.0 / pi * pow(smooth_h, 8);

float SMOOTHING_KERNEL_LAGUE_SMOOTH(float x) {
	if (x > system.smoothing_radius_px) return 0.0;
	
	x = convert_px_distance(x);
	return pow(smooth_h * smooth_h - x * x, 3) / SMOOTHING_KERNEL_LAGUE_SMOOTH_VOLUME;
}

float SMOOTHING_KERNEL_LAGUE_SMOOTH_DERIVATIVE(float x) {
	if (x > system.smoothing_radius_px) return 0.0;
	
	x = convert_px_distance(x);
	float f = smooth_h * smooth_h - x * x;
	return SMOOTHING_KERNEL_LAGUE_SMOOTH_DERIVATIVE_SCALE * f * f * x;
}

// SMOOTHING_KERNEL_LAGUE_SPIKY
const float SMOOTHING_KERNEL_LAGUE_SPIKY_VOLUME = pi * pow(smooth_h, 4.0) / 6.0;
const float SMOOTHING_KERNEL_LAGUE_SPIKY_DERIVATIVE_SCALE = 12.0 / (pi * pow(smooth_h, 4));

float SMOOTHING_KERNEL_LAGUE_SPIKY(float x) {
	if (x > system.smoothing_radius_px) return 0.0;
	
	x = convert_px_distance(x);
	return pow(smooth_h - x, 2) / SMOOTHING_KERNEL_LAGUE_SMOOTH_VOLUME;
}

float SMOOTHING_KERNEL_LAGUE_SPIKY_DERIVATIVE(float x) {
	if (x > system.smoothing_radius_px) return 0.0;
	
	x = convert_px_distance(x);
	return (x - smooth_h) * SMOOTHING_KERNEL_LAGUE_SMOOTH_DERIVATIVE_SCALE;
}


// SMOOTHING_KERNEL_VISCOSITY_DERIVATIVE_2
const float SMOOTHING_KERNEL_VISCOSITY_DERIVATIVE_2_SCALE = 45.0 / (pi * pow(smooth_h, 6));
float SMOOTHING_KERNEL_VISCOSITY_DERIVATIVE_2(float x) {
	if (x > system.smoothing_radius_px) return 0.0;
	
	x = convert_px_distance(x);
	return SMOOTHING_KERNEL_VISCOSITY_DERIVATIVE_2_SCALE * (smooth_h - x);
}

float SMOOTHING_KERNEL_MULLER_SPIKY(float x) {
	return 0.0;
}

float SMOOTHING_KERNEL_MULLER_SPIKY_DERIVATIVE(float x) {
	return 0.0;	
}

float SMOOTHING_KERNEL_MULLER_SMOOTH(float x) {
	return 0.0;	
}

float SMOOTHING_KERNEL_MULLER_SMOOTH_DERIVATIVE(float x) {
	return 0.0;
}


//////////////////////
// FLUID SIMULATION //
//////////////////////
FluidProperties calc_fluid_properties(vec2 position) {	
	FluidProperties result;

	for (uint i = 0; i < system.num_particles; i++) {
		float dist = distance(position, particles[i].position);
		float influence = SMOOTHING_KERNEL_LAGUE_SMOOTH(dist);
		result.density += system.particle_mass * influence;
		
		result.pressure += fluid[i].pressure * system.particle_mass * influence / fluid[i].density;
	}

	return result;
}

float calc_target_density() {
	float capsule_length = length(system.pa - system.pb) + 1.0;
	float radius = system.radius / system.smoothing_radius_px;
	float area =  pi * radius * radius * capsule_length;
	float target = system.num_particles / area;
	target /= 2.0;
	return target;
	///////////////////////////////////////////////////////
	// float ax = fluid_tank.x / system.smoothing_radius_px;	 //
	// float ay = fluid_tank.y / system.smoothing_radius_px;	 //
	// float area = ax * ay;							 //
	// float mass = system.num_particles * mass;		 //
	// return mass / area;								 //
	///////////////////////////////////////////////////////
}


/////////////////////////
// FUNCTION ESTIMATION //
/////////////////////////
vec2 world_to_example(vec2 world) {
	return world / 64;
}

float example_function(vec2 position) {
	vec2 scaled_position = world_to_example(position);
	return cos(scaled_position.y + sin(scaled_position.x));
} 


float calc_example(vec2 position) {
	float example = 0;

	for (uint i = 0; i < system.num_particles; i++) {
		float dist = distance(position, particles[i].position);
		float influence = SMOOTHING_KERNEL_LAGUE_SMOOTH(dist);
		example += fluid[i].pressure * system.particle_mass * influence / fluid[i].density;
	}

	return example;
}


/////////////
// KERNELS //
/////////////
#define FLUID_KERNEL_INIT_BASE 1
#define FLUID_KERNEL_INIT_DENSITY 2
#define FLUID_KERNEL_UPDATE_DENSITY 10
#define FLUID_KERNEL_UPDATE_PREDICTED_POSITION 11
#define FLUID_KERNEL_UPDATE_VISCOSITY 12
#define FLUID_KERNEL_UPDATE_ACCELERATION 13

uniform int fluid_kernel;

uint claim_index() {
	return atomicAdd(system.next_unprocessed_index, 1);
}

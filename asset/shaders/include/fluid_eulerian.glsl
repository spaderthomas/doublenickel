/////////////
// STRUCTS //
/////////////
struct Fluid {
	vec2 velocity;
	float density;
	float buffered_density;
	float padding [4];
};

struct Source {
	vec2 velocity;
	float density;
	float padding [1];
};

struct EulerianFluidSystem {
	uint next_unprocessed_index;
	uint grid_size;
};


//////////
// SSBO //
//////////
layout (std430, binding = 0) buffer SystemBuffer {
	EulerianFluidSystem system;
};

layout (std430, binding = 1) buffer FluidBuffer {
	Fluid fluid [];
};

layout (std430, binding = 2) buffer SourceBuffer {
	Source source [];
};


/////////////
// KERNELS //
/////////////
#define EULERIAN_FLUID_KERNEL_INIT 1
#define EULERIAN_FLUID_KERNEL_UPDATE_SOURCES 10
#define EULERIAN_FLUID_KERNEL_UPDATE_BUFFERED_DENSITY 12
#define EULERIAN_FLUID_KERNEL_UPDATE_DIFFUSE 13
#define EULERIAN_FLUID_KERNEL_UPDATE_DIFFUSE_BOUNDS 14
#define EULERIAN_FLUID_KERNEL_UPDATE_DIFFUSE_CORNERS 15
#define EULERIAN_FLUID_KERNEL_UPDATE_ADVECT 16

uniform int fluid_kernel;

const float dt = 1.0 / 60.0;
const float diffusion = 0.1;

void enforce_boundary(uint i) {
	
}

/////////////////////
// INDEX CRAZINESS //
/////////////////////
#define sim_width (system.grid_size)
#define buffer_width (system.grid_size + 2)
#define num_sim_cells (sim_width * sim_width)

#define SIM_MIN 1
#define SIM_MAX system.grid_size
#define BUFFER_MIN 0
#define BUFFER_MAX system.grid_size + 1
#define IX(x, y) buffer2_to_buffer(ivec2((x), (y)))

uint sim_to_buffer(uint sim) {
    ivec2 sim2 = ivec2(sim % system.grid_size, sim / system.grid_size);
    ivec2 buffer2 = sim2 + ivec2(1, 1);
    return buffer2.x + (system.grid_size + 2) * buffer2.y;
}

uint buffer_to_sim(uint index) {
    ivec2 buffer2 = ivec2(index % (system.grid_size + 2), index / (system.grid_size + 2));
	ivec2 sim2 = buffer2 - ivec2(1, 1);
	return sim2.x + system.grid_size * sim2.y;
}

ivec2 buffer_to_buffer2(uint index) {
	uint x = index % (system.grid_size + 2);
	uint y = index / (system.grid_size + 2);
    return ivec2(x, y);
}

uint buffer2_to_buffer(ivec2 buffer2) {
	uint col = buffer2.x;
	uint row = buffer2.y;
	return col + ((system.grid_size + 2) * row);
}

uint uv_to_buffer(float u, float v) {
	uint col = uint(floor(u * (system.grid_size + 2)));
	uint row = uint(floor(v * (system.grid_size + 2)));
	return col + (system.grid_size + 2) * row;
}

float buffer_to_01(uint index) {
	return float(index) / ((system.grid_size + 2) * (system.grid_size + 2));
}


uint buffer_up(uint index) {
	ivec2 buffer2 = buffer_to_buffer2(index);
    uint offset = uint(buffer2.x > 0u);
	
	return index + (offset * buffer_width);
}

uint buffer_left(uint index) {
	ivec2 buffer2 = buffer_to_buffer2(index);
    uint offset = uint(buffer2.y > 0u);
	
	return index - offset;
}

uint buffer_right(uint index) {
	ivec2 buffer2 = buffer_to_buffer2(index);
    uint offset = uint(buffer2.y < buffer_width - 1);
	
	return index + offset;
}

uint buffer_down(uint index) {
	ivec2 buffer2 = buffer_to_buffer2(index);
    uint offset = uint(buffer2.x < buffer_width - 1);
	
	return index - (offset * buffer_width);
}

struct BufferNeighbors {
	uint up;
	uint down;
	uint left;
	uint right;
};

BufferNeighbors buffer_neighbors(uint index) {
	BufferNeighbors neighbors;
	neighbors.up = buffer_up(index);
	neighbors.down = buffer_down(index);
	neighbors.left = buffer_left(index);
	neighbors.right = buffer_right(index);
	return neighbors;
}

struct BufferCorners {
	uint bl;
	uint tl;
	uint br;
	uint tr;
};

BufferCorners buffer_corners() {
	BufferCorners corners;
	corners.bl = IX(BUFFER_MIN, BUFFER_MIN);
	corners.tl = IX(BUFFER_MIN, BUFFER_MAX);
	corners.br = IX(BUFFER_MAX, BUFFER_MIN);
	corners.tr = IX(BUFFER_MAX, BUFFER_MAX);
	return corners;
}

struct BufferBoundary {
	uint buffer_left;
	uint buffer_right;
	uint buffer_top;
	uint buffer_bottom;
	
	uint sim_left;
	uint sim_right;
	uint sim_top;
	uint sim_bottom;
};

BufferBoundary buffer_boundary(uint i) {
	BufferBoundary boundary;
	boundary.buffer_left = IX(BUFFER_MIN, i);
	boundary.sim_left = IX(SIM_MIN, i);
	
	boundary.buffer_right = IX(BUFFER_MAX, i);
	boundary.sim_right = IX(SIM_MAX, i);
	
	boundary.buffer_bottom = IX(i, BUFFER_MIN);
	boundary.sim_bottom = IX(i, SIM_MIN);
	
	boundary.buffer_top = IX(i, BUFFER_MAX);
	boundary.sim_top = IX(i, SIM_MAX);
	
	return boundary;
}

bool is_boundary(uint i) {
	ivec2 b = buffer_to_buffer2(i);
	return b.x == BUFFER_MIN || b.y == BUFFER_MIN || b.x == BUFFER_MAX || b.y == BUFFER_MAX;
}

void clear_debug_fields(uint buffer_index) {
	for (uint i = 0; i < 4; i++) {
		fluid[buffer_index].padding[i] = 0.0;
	}
}

uint claim_index() {
	return atomicAdd(system.next_unprocessed_index, 1);
}

#define SDF_CIRCLE 0
#define SDF_RING 1
#define SDF_BOX 2   
#define SDF_ORIENTED_BOX 3
#define SDF_COMBINE 100

#define SDF_COMBINE_OP_UNION 0
#define SDF_COMBINE_OP_INTERSECTION 1
#define SDF_COMBINE_OP_SUBTRACTION 2

#define SDF_SMOOTH_KERNEL_NONE 0
#define SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC 1

layout (std430, binding = 0) readonly buffer SdfBuffer {
    float sdf_data [];
};

layout (std430, binding = 1) readonly buffer SdfCombineBuffer {
    uint sdf_combine_data [];
};


////////////////
// SDF HEADER //
////////////////
struct SdfHeader {
    vec3 color;
    vec2 position;
    float rotation;
    float edge_thickness;
};

SdfHeader pull_header(inout uint buffer_index) {
    SdfHeader header;
    header.color          = PULL_VEC3(sdf_data, buffer_index);
    header.position       = PULL_VEC2(sdf_data, buffer_index);
    header.rotation       = PULL_F32(sdf_data, buffer_index);
    header.edge_thickness = PULL_F32(sdf_data, buffer_index);
    return header;
}


////////////////
// SDF CIRCLE //
////////////////
struct SdfCircle {
    float radius;
};

SdfCircle pull_circle(inout uint buffer_index) {
    SdfCircle circle;
    circle.radius = PULL_F32(sdf_data, buffer_index);
    return circle;
}

float sdf_circle(vec2 point, vec2 center, float radius) {
	return length(point - center) - radius;	
}


//////////////
// SDF RING //
//////////////
struct SdfRing {
    float inner_radius;
    float outer_radius;
};

SdfRing pull_ring(inout uint buffer_index) {
    SdfRing ring;
    ring.inner_radius = PULL_F32(sdf_data, buffer_index);
    ring.outer_radius = PULL_F32(sdf_data, buffer_index);
    return ring;
}

float sdf_ring(vec2 world_point, vec2 center, float inner_radius, float outer_radius) {
    float dist_to_center = length(world_point - center);
    float dist_to_outer = dist_to_center - outer_radius;
    float dist_to_inner = inner_radius - dist_to_center;
    return max(dist_to_outer, dist_to_inner);
}


//////////////////////
// SDF ORIENTED BOX //
//////////////////////
struct SdfOrientedBox {
    vec2 size;
};

SdfOrientedBox pull_oriented_box(inout uint buffer_index) {
    SdfOrientedBox box;
    box.size = PULL_VEC2(sdf_data, buffer_index);
    return box;
}

float sdf_box(vec2 world_point, vec2 top_left, vec2 size) {
    vec2 edge_distances = abs(world_point - top_left) - size;
    float edge_distance = max(edge_distances.x, edge_distances.y);
    edge_distance = min(edge_distance, 0.0);
    return length(max(edge_distances, 0.0)) + edge_distance;
}

float sdf_oriented_box(vec2 p, vec2 a, vec2 b, float th)
{
    float l = length(b-a);
    vec2  d = (b-a)/l;
    vec2  q = (p-(a+b)*0.5);
          q = mat2(d.x,-d.y,d.y,d.x)*q;
          q = abs(q)-vec2(l,th)*0.5;
    return length(max(q,0.0)) + min(max(q.x,q.y),0.0);    
}



float sdf_triangle_isosceles(vec2 point, vec2 size) {
	size.y = -size.y;
    point.x = abs(point.x);
    vec2 a = point - size * clamp(dot(point, size) / dot(size, size), 0.0, 1.0);
    vec2 b = point - size * vec2(clamp(point.x / size.x, 0.0, 1.0), 1.0);
    float s = -sign(size.y);

    vec2 da = vec2(dot(a, a), s * (point.x * size.y - point.y * size.x));
    vec2 db = vec2(dot(b, b), s * (point.y - size.y));
    vec2 d = min(da, db);
                  
    return -sqrt(d.x) * sign(d.y);
}


///////////////
// SDF INDEX //
///////////////
struct SdfIndex {
    uint shape;
    uint buffer_index;
};


struct SdfCombineEntry {
    SdfIndex index;
    uint combine_op;
    uint kernel;
};

SdfCombineEntry pull_sdf_combine_entry(inout uint index) {
    SdfCombineEntry entry;
    entry.index.buffer_index = PULL_U32(sdf_combine_data, index);
    entry.index.shape = PULL_U32(sdf_combine_data, index);
    entry.combine_op = PULL_U32(sdf_combine_data, index);
    entry.kernel = PULL_U32(sdf_combine_data, index);
    return entry;
}


////////////////////
// SDF OPERATIONS //
////////////////////
float sdf_op_union(float a, float b, uint kernel) {
    if (kernel == SDF_SMOOTH_KERNEL_NONE) {
        return min(a, b);
    }
    else if (kernel == SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC) {
        float k = 8.0;
        float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
        return mix( b, a, h ) - k*h*(1.0-h);
    }
    return a;
}

float sdf_op_subtraction(float a, float b, uint kernel) {
    if (kernel == SDF_SMOOTH_KERNEL_NONE) {
        return max(-a, b);
    }
    else if (kernel == SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC) {
        float k = 4.0;
        float h = clamp( 0.5 - 0.5*(b + a)/k, 0.0, 1.0 );
        return mix( b, -a, h ) + k*h*(1.0-h);
    }
    return a;
}

float sdf_op_intersection(float a, float b, uint kernel) {
    if (kernel == SDF_SMOOTH_KERNEL_NONE) {
        return max(a, b);
    }
    else if (kernel == SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC) {
        float k = 4.0;
        float h = clamp( 0.5 - 0.5*(b - 1)/k, 0.0, 1.0 );
        return mix( b, a, h ) + k*h*(1.0-h);
    }
    return a;
}


////////////////////////
// SDF DECODE CONTEXT //
////////////////////////
struct SdfDecodeContext {
    SdfIndex index;
    SdfHeader header;
    float sdf_distance;
    vec4 color;
};

SdfDecodeContext sdf_decode_context(uvec2 index) {
    SdfDecodeContext context;
    context.index.shape = index.x;
    context.index.buffer_index = index.y;
    return context;
}
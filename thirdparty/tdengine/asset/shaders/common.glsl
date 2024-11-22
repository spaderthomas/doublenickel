#version 450 core
 
//////////////
// UNIFORMS //
//////////////
uniform float master_time;
uniform vec2 camera;
uniform vec2 output_resolution;
uniform vec2 native_resolution;


///////////////
// CONSTANTS //
///////////////
const float pi = 3.14159265359;
const float gamma = 2.2;
#define THREADS_PER_WORKGROUP 32


////////////
// COLORS //
////////////
const vec4 red   = vec4(1.0, 0.0, 0.0, 1.0);
const vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
const vec4 blue  = vec4(0.0, 0.0, 1.0, 1.0);
const vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
const vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 clear = vec4(0.0, 0.0, 0.0, 0.0);

const vec4 gunmetal     = vec4(43.0 / 255.0, 61.0 / 255.0, 65.0 / 255.0, 255.0);
const vec4 paynes_gray  = vec4(76.0 / 255.0, 95.0 / 255.0, 107.0 / 255.0, 255.0);
const vec4 cadet_gray   = vec4(131.0 / 255.0, 160.0 / 255.0, 160.0 / 255.0, 255.0);
const vec4 celadon      = vec4(183.0 / 255.0, 227.0 / 255.0, 204.0 / 255.0, 255.0);
const vec4 spring_green = vec4(89.0 / 255.0, 255.0 / 255.0, 160.0 / 255.0, 255.0);
const vec4 mindaro      = vec4(188.0 / 255.0, 231.0 / 255.0, 132.0 / 255.0, 255.0);
const vec4 light_green  = vec4(161.0 / 255.0, 239.0 / 255.0, 139.0 / 255.0, 255.0);
const vec4 indian_red   = vec4(180.0 / 255.0, 101.0 / 255.0, 111.0 / 255.0, 255.0);

#define DBG(debug_color) color = (vec4((debug_color).rgb, 1.0)); return;
#define DBG_TEX(debug_texture) color = texture(debug_texture, f_uv); return;
#define DBG_FLOAT(value) color = make_red(value); return;
#define DBG_VEC2(value) color = (vec4((value).rg, 0.0, 1.0)); return;
#define DBG_VEC3(value) color = (vec4((value).rgb, 1.0)); return;
#define DBG_MIX(sample_color, debug_color, t) color = mix((sample_color), (debug_color), (t)); return;
#define DBG_SCALAR_GEQ(scalar, threshold) if ((scalar) >= threshold) { DBG(green); } else { DBG(red); }
#define DBG_SCALAR_EQ(scalar, threshold)  if ((scalar) == value)     { DBG(green); } else { DBG(red); }

#define FN_DBG(debug_color) return (vec4((debug_color).rgb, 1.0));
#define FN_DBG_TEX(debug_texture) return texture(debug_texture, f_uv);
#define FN_DBG_FLOAT(value) return make_red(value);
#define FN_DBG_VEC2(value) return (vec4((value).rg, 0.0, 1.0));
#define FN_DBG_VEC3(value) return (vec4((value).rgb, 1.0));
#define FN_DBG_MIX(sample_color, debug_color, t) return mix((sample_color), (debug_color), (t));
#define FN_DBG_BOOL(expr)  if (expr) { FN_DBG(green); } else { FN_DBG(red); }
#define FN_DBG_SCALAR_GEQ(scalar, threshold) FN_DBG_BOOL((scalar) >= (threshold))
#define FN_DBG_SCALAR_EQ(scalar, value)  FN_DBG_BOOL((scalar) == (value))

vec4 gray(float v) {
	return vec4(vec3(v), 1.0);
}

vec4 make_red(float r) {
	return vec4(r, 0.0, 0.0, 1.0);
}

vec4 make_green(float g) {
	return vec4(0.0, g, 0.0, 1.0);
}

vec4 make_blue(float b) {
	return vec4(0.0, 0.0, b, 1.0);
}

vec4 sign_color(float value, vec4 positive, vec4 negative) {
	if (value >= 0) {
		return vec4(positive.xyz * value, positive.a);
	}
	else {
		return vec4(negative.xyz * abs(value), negative.a);
	}
}

vec4 color_255(float r, float g, float b, float a) {
	return vec4(r, g, b, a) / 255.0;
}


////////////////////////
// COLOR CALCULATIONS //
////////////////////////
float calc_brightness(vec4 color) {
	const vec3 weights = vec3(0.2126, 0.7152, 0.0722);
	return dot(color.rgb, weights);
}

float calc_perceived_lightness(vec4 color) {
	float luminance = calc_brightness(color);

	const float lightness_threshold = 216.0 / 24389.0;
	const float k1 = 24389.0 / 27.0;

	float lightness = 0.0;
	if (luminance <= lightness_threshold) {
		lightness = luminance * k1;
	}
	else {
		lightness = pow(luminance, .333) * 116 - 16;
	}

	return lightness / 100.0;
}

vec3 rgb_to_hsv(vec3 c) {
	// All components are in the range [0…1], including hue.
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec4 rgb_to_hsv_4(vec4 c) {
	return vec4(rgb_to_hsv(c.rgb), c.a);
}


vec3 hsv_to_rgb(vec3 c) {
	// All components are in the range [0…1], including hue.
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 hsv_to_rgb_4(vec4 c) {
	return vec4(hsv_to_rgb(c.rgb), c.a);
}

#define HSV_VALUE(color) ((color).z)
#define HSV_SATURATION(color) ((color).y)
#define HSV_HUE(color) ((color).x)

float linear_to_srgb(float linear) {
    if (linear <= 0.0031308) {
        return linear * 12.92;
    } else {
        return 1.055 * pow(linear, 1.0 / gamma) - 0.055;
    }
}

vec4 linear_to_srgb(vec4 linear) {
	return vec4(
		linear_to_srgb(linear.r),
		linear_to_srgb(linear.g),
		linear_to_srgb(linear.b),
		linear.a
	);
}

bool is_edge_3(vec4 sample_center, vec4 sample_left, vec4 sample_right, float edge_threshold) {
	float brightness = calc_brightness(sample_center);
	float brightness_right = calc_brightness(sample_right);
	float brightness_left = calc_brightness(sample_left);
	float brightness_delta = max(abs(brightness - brightness_left), abs(brightness - brightness_right));

	bool is_edge = brightness_delta > edge_threshold;
	return is_edge;
}


/////////////////
// RANDOM SHIT //
/////////////////
float nonlinear_weight(float x, float exp, float low) {
	return max(pow(x, exp), low);
}

vec2 scaled_pixels(float pixels) {
	return pixels * native_resolution / output_resolution;
}

float quantize(float x, float bands) {
	return floor(x * bands) / bands;
}

vec2 quantize(vec2 v, float bands) {
	return vec2(quantize(v.x, bands), quantize(v.y, bands));
}

vec2 quantize_uv(vec2 uv, float horizontal_bands) {
	return vec2(
		quantize(uv.x, horizontal_bands), 
		quantize(uv.y, horizontal_bands * output_resolution.y / output_resolution.x)
	);
}

float plot(vec2 uv, float pct){
  return  smoothstep( pct-0.02, pct, uv.y) -
          smoothstep( pct, pct+0.02, uv.y);
}

#define PULL_U32(buf, offset) (buf)[(offset)++]
#define PULL_F32(buf, offset) (buf)[(offset)++]
#define PULL_VEC2(buf, offset) vec2(PULL_F32(buf, offset), PULL_F32(buf, offset))
#define PULL_VEC3(buf, offset) vec3(PULL_F32(buf, offset), PULL_F32(buf, offset), PULL_F32(buf, offset))

//////////
// MATH //
//////////
float radians_to_turns(float theta) {
	return theta / (2 * pi);
}

float turns_to_radians(float theta) {
	return theta * (2 * pi);
}

float atan_turns(float y, float x) {
	return atan(y, x) / (2 * pi);
}

float atan_cyclic(float y, float x) {
	return abs(atan(y, x));
}

float atan_cyclic_turns(float y, float x) {
	float theta = abs(atan(y, x));
	return radians_to_turns(theta);
}

mat3 scale_matrix(vec2 scale) {
	return mat3(
		scale.x, 0.0,     0.0,
		0.0,     scale.y, 0.0,
		0.0,     0.0,     1.0
	);
}

mat3 translation_matrix(vec2 translation) {
	return mat3(
		1.0,           0.0,           0.0,
		0.0,           1.0,           0.0,
		translation.x, translation.y, 1.0
	);
}

mat3 rotation_matrix(float rotation) {
	float c = cos(rotation);
	float s = sin(rotation);
	return mat3(
		c,    s,   0.0,
		-s,   c,   0.0,
		0.0,  0.0, 1.0
	);
}

mat3 transformation_matrix(vec2 translation, float rotation, vec2 scale) {
	return translation_matrix(translation) * rotation_matrix(rotation) * scale_matrix(scale);
}
// mat3 transform_2(vec2 translation, float rotation, vec2 scale) {
// 	float c = cos(rotation);
// 	float s = sin(rotation);
// 	return mat3(
// 		c * scale.x,  0.0, 0.0,
// 		0.0,  c * scale.y, 0.0,
// 		translation.x,         translation.y,        1.0
// 	);

// }


/////////
// RNG //
/////////
float random_float(vec2 seed, float min, float max) {
	const vec2 hash_vector = vec2(12.9898, 78.233);
	const float large_multiplier = 43758.5453;
	
	float hashed_seed = dot(seed, hash_vector) + mod(master_time, 1.1374);
	float value_01 = fract(sin(hashed_seed) * large_multiplier);

    return min + (max - min) * value_01;
}

void jitter_rng_seed(inout vec2 seed) {
	seed += random_float(seed, 12.8594, 23.9576);
}

#define RANDOM_FLOAT(seed, vmin, vmax) random_float((seed), (vmin), (vmax)); jitter_rng_seed(seed);


/////////////////
// OSCILLATORS //
/////////////////
float ranged_sin(float x, float vmin, float vmax) {
	float coefficient = (vmax - vmin) / 2.0;
	float offset = (vmax + vmin) / 2.0;
	float sinx = sin(x);
	return coefficient * sinx + offset;
}

float timed_sin_ex(float speed, float vmin, float vmax, float jitter) {
	return ranged_sin(master_time * speed + jitter, vmin, vmax);
}

float timed_sin(float speed, float vmin, float vmax) {
	return timed_sin_ex(speed, vmin, vmax, 0.0);
}

float triangle_wave(float x) {
    return clamp(1.0 - abs(x), 0.0, 1.0);
}

float map_range(float x, float min_a, float max_a, float min_b, float max_b) {
    float coefficient = (max_b - min_b) / (max_a - min_a);
    float offset = min_b - coefficient * min_a;
    return clamp(coefficient * x + offset, min_b, max_b);
}


////////////
// EASING //
////////////
float ease_in_out_cubic(float t) {
  return t < 0.5
    ? 4.0 * t * t * t
    : 0.5 * pow(2.0 * t - 2.0, 3.0) + 1.0;
}

float interp_in_out_cubic(float x, float y, float t) {
	return mix(x, y, ease_in_out_cubic(t));
}

float ease_out_sine(float t) {
	return sin(t * pi / 2);
}

float ease_out_quadratic(float t) {
	return 1.0 - pow(1.0 - t, 2);
}

float ease_out_cubic(float t) {
	return 1.0 - pow(1.0 - t, 3);
}

float ease_out_quartic(float t) {
	return 1.0 - pow(1.0 - t, 4);
}

float ease_in_quadratic(float t) {
	return pow(t, 2);
}

float ease_in_cubic(float t) {
	return pow(t, 3);
}


//////////////////
// THRESHOLDING //
//////////////////
float is_higher_than(float x, float threshold) {
	return step(threshold, x);
}

float high_pass(float x, float threshold) {
	return x * is_higher_than(x, threshold);
}

float is_lower_than(float x, float threshold) {
	return step(x, threshold);
}

float low_pass(float x, float threshold) {
	return x * is_lower_than(x, threshold);
}

float double_pass(float x, float low, float high) {
	return high_pass(low_pass(x, high), low);
}


//////////////////
// BLUR HELPERS //
//////////////////
vec4 sample_neighbor_h(sampler2D source_texture, vec2 uv, float pixel_offset) {
	const vec2 uv_per_px = 1.0 / native_resolution;

	uv.x += pixel_offset * uv_per_px.x;
	uv = clamp(uv, 0.0, 0.999);
	return texture(source_texture, uv);
}

vec4 sample_neighbor_v(sampler2D source_texture, vec2 uv, float pixel_offset) {
	const vec2 uv_per_px = 1.0 / native_resolution;

	uv.y += pixel_offset * uv_per_px.y;
	uv = clamp(uv, 0.0, 0.999);
	return texture(source_texture, uv);
}

float gauss(float x, float sigma) {
	float power = -(pow(x, 2) / 2 * pow(sigma, 2));
	return exp(power);
}

void add_gaussian_blur_h(sampler2D source_texture, vec2 uv, float sigma, float pixel_offset, inout float total_weights, inout vec4 blurred_color) {
	float weight = gauss(pixel_offset, sigma);
	blurred_color += weight * sample_neighbor_h(source_texture, uv, pixel_offset);
	total_weights += weight;
}

void add_gaussian_blur_v(sampler2D source_texture, vec2 uv, float sigma, float pixel_offset, inout float total_weights, inout vec4 blurred_color) {
	float weight = gauss(pixel_offset, sigma);
	blurred_color += weight * sample_neighbor_v(source_texture, uv, pixel_offset);
	total_weights += weight;
}

void add_nonlinear_blur_h(sampler2D source_texture, vec2 uv, float pixel_offset, float power, inout float total_weights, inout vec4 blurred_color) {
	vec4 sampled_color = sample_neighbor_h(source_texture, uv, pixel_offset);

	// Pixels contribute to the final color based on how much color they already have.
	const vec3 weights = vec3(0.4, 0.2, 0.4);
	float weight = dot(sampled_color.rgb, weights);

	// Since we're weighting based on the color components, black will always have a weight of zero. We can just clamp
	// the weight to some small value to allow black to actually propagate. The lower this value, the more saturated
	// the result, since we're only allowing pixels with lots of color to contribute.
	weight = max(pow(weight, power), 0.25);

	blurred_color += weight * sampled_color;
	total_weights += weight;
}


//////////////////
// BLUR KERNELS //
//////////////////
vec4 box_blur_n_h(sampler2D source_texture, vec2 uv, int num_taps, float pixel_offset) {
	vec4 blurred_color = vec4(0.0);

	for (int i = 1; i <= num_taps / 2; i++) {
		blurred_color += sample_neighbor_h(source_texture, uv, i * pixel_offset);
		blurred_color += sample_neighbor_h(source_texture, uv, -i * pixel_offset);
	}
	blurred_color += sample_neighbor_h(source_texture, uv, 0);

	return blurred_color / num_taps;
}

vec4 gaussian_blur_n_h(sampler2D source_texture, vec2 uv, float sigma, uint num_taps, float pixel_offset) {
	float total_weights = 0.0;
	vec4 blurred_color = vec4(0.0);

	for (int i = 1; i <= num_taps / 2; i++) {
		add_gaussian_blur_h(source_texture, uv, sigma, i * pixel_offset, total_weights, blurred_color);
		add_gaussian_blur_h(source_texture, uv, sigma, -i * pixel_offset, total_weights, blurred_color);
	}

	add_gaussian_blur_h(source_texture, uv, sigma, 0, total_weights, blurred_color);

	return blurred_color / total_weights;
}

vec4 gaussian_blur_n_v(sampler2D source_texture, vec2 uv, float sigma, uint num_taps, float pixel_offset) {
	float total_weights = 0.0;
	vec4 blurred_color = vec4(0.0);

	for (int i = 1; i <= num_taps / 2; i++) {
		add_gaussian_blur_v(source_texture, uv, sigma, i * pixel_offset, total_weights, blurred_color);
		add_gaussian_blur_v(source_texture, uv, sigma, -i * pixel_offset, total_weights, blurred_color);
	}

	add_gaussian_blur_v(source_texture, uv, sigma, 0, total_weights, blurred_color);

	return blurred_color / total_weights;
}


vec4 nonlinear_blur_n_h(sampler2D source_texture, vec2 uv, uint num_taps, float pixel_offset, float power) {
	float total_weights = 0.0;
	vec4 blurred_color = vec4(0.0);

	for (int i = 1; i <= num_taps / 2; i++) {
		add_nonlinear_blur_h(source_texture, uv, i * pixel_offset, power, total_weights, blurred_color);
		add_nonlinear_blur_h(source_texture, uv, -i * pixel_offset, power, total_weights, blurred_color);
	}

	add_nonlinear_blur_h(source_texture, uv, 0, power, total_weights, blurred_color);

	return blurred_color / total_weights;
}
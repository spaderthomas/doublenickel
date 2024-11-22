struct Light {
    vec4 color;
    vec2 position;
    float radial_falloff;
    float angular_falloff;
    float intensity;
    float volumetric_intensity;
    float angle;
    float padding [1];
};

layout (std430, binding = 0) buffer LightBuffer {
	Light lights [16];
};

uniform int num_lights;

const float light_radius = 4;


float calc_radial_falloff(Light light, vec2 position) {
    float d = distance(position, light.position);
    float normalized_distance = distance(position, light.position) / light_radius;

    float radial_falloff_power = map_range(light.radial_falloff, 0.0, 1.0, 0.0, 2.0);
    float radial_falloff = 1.0 / (1.0 + pow(normalized_distance, radial_falloff_power));
    // float radial_falloff = smoothstep(light_radius, light_radius * light.radial_falloff, d);
    // radial_falloff = quantize(radial_falloff, light_resolution);

    return radial_falloff;
}


float calc_angular_falloff(Light light, vec2 position) {
  vec2 light_direction = normalize(position - light.position);

  float fragment_angle = atan_turns(light_direction.y, light_direction.x);
  fragment_angle = fragment_angle < 0.0 ? fragment_angle + 1.0 : fragment_angle;

  float angular_delta = fragment_angle - light.angle;
  angular_delta = angular_delta > 0.5 ? angular_delta - 1.0 : angular_delta;
  angular_delta = angular_delta < -0.5 ? angular_delta + 1.0 : angular_delta;
  angular_delta = abs(angular_delta);

  return smoothstep(1.0 - light.angular_falloff, 0, angular_delta);
} 


const bool band_uvs = true;
const int max_uv_bands = 96;

const bool warp_uv_bands = true;
const float uv_band_warp = 0.25;

vec2 band_light_uvs_grid(Light light, vec2 uv, float num_bands) {
  return quantize_uv(uv, num_bands);
}

vec2 band_light_uvs_warp(Light light, vec2 uv, int max_bands, vec2 world_position, float warp) {
  float normalized_distance = distance(world_position, light.position) / length(output_resolution);
  float light_resolution = pow(1.0 - normalized_distance, warp);
  float num_uv_bands = max_bands * light_resolution;
  return band_light_uvs_grid(light, uv, num_uv_bands);
}

vec2 band_light_uvs(Light light, vec2 uv, vec2 world_position) {
  if (band_uvs) {
    if (warp_uv_bands) {
        return band_light_uvs_warp(light, uv, max_uv_bands, world_position, uv_band_warp);
    }
    else {
        return band_light_uvs_grid(light, uv, max_uv_bands);
    }
  }

  return uv;
}

struct LitFragment {
  vec3 light_color;
  vec3 volumetric_color;
  float light_strength;
};

LitFragment light_fragment(Light light, vec2 world_position, vec3 normal) {
  LitFragment result;

  vec2 light_direction = normalize(light.position - world_position);

  float radial_falloff = calc_radial_falloff(light, world_position);
  float normal_falloff = clamp(dot(light_direction, normal.xy), 0.0, 1.0) * (normal.z);
  float angular_falloff = calc_angular_falloff(light, world_position);
  result.light_strength = light.intensity * radial_falloff * angular_falloff * normal_falloff;
  result.light_color = light.color.rgb * result.light_strength;

  float volumetric_strength = light.intensity * radial_falloff * angular_falloff * light.volumetric_intensity;
  result.volumetric_color = light.color.rgb * volumetric_strength;

  return result;
}
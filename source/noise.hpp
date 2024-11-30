#ifndef DN_NOISE_H
#define DN_NOISE_H
typedef struct {
  struct {
    u32 perlin;
    u32 chaoric;
  } textures;
} dn_noise_t;
dn_noise_t dn_noise;

static const int dn_perlin_permutation_raw[256] = {
  151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,172,9,
  129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
  49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

static int dn_perlin_permutation[512];


DN_IMP void dn_noise_init();
DN_API double dn_noise_perlin(double x, double y);
DN_API double dn_noise_perlin_scaled(double x, double y, double vmin, double vmax);
DN_API double dn_noise_chaotic(double x, double y);
DN_API double dn_noise_chaotic_scaled(double x, double y, double vmin, double vmax);
#endif


#ifdef DN_NOISE_IMPLEMENTATION
void dn_noise_init() {
  for (int i = 0; i < 256; i++) {
      dn_perlin_permutation[i] = dn_perlin_permutation_raw[i];
      dn_perlin_permutation[i + 256] = dn_perlin_permutation_raw[i];
  }

	const char* file_name = "perlin.png";
	auto file_path = dn_paths_resolve_format("dn_image", file_name);
	create_sprite(file_name , file_path);

	auto rx = static_cast<u32>(window.native_resolution.x);
	auto ry = static_cast<u32>(window.native_resolution.y);
	auto data = dn::allocator::alloc<u32>(&dn_allocators.bump, rx * ry);

	for (int x = 0; x < rx; x++) {
		for (int y = 0; y < ry; y++) {
            u32 r = dn_math_random_i32(0, 255);
            u32 g = dn_math_random_i32(0, 255) << 8;
            u32 b = dn_math_random_i32(0, 255) << 16;
            u32 a = 255 << 24;
            data[y * rx + x] = r + g + b + a;
        }
	}

	create_sprite("chaotic_noise.png", (u8*)data, rx, ry, 4);
}

static inline double dn_perlin_fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline double dn_perlin_lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double dn_perlin_grad(int hash, double x, double y, double z) {
    int h = hash & 0xF;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float dn_perlin_random(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

double dn_noise_perlin(double x, double y) {
  y = y == 0 ? 0 : y;
  float z = 0;
	int wrap = 256;

    // Wrap the integer coordinates within the tiling period
    int xi = ((int)floor(x) % wrap) & 255;
    int yi = ((int)floor(y) % wrap) & 255;
    int zi = ((int)floor(z) % wrap) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = dn_perlin_fade(x);
    double v = dn_perlin_fade(y);
    double w = dn_perlin_fade(z);

    int A = dn_perlin_permutation[xi] + yi;
    int AA = dn_perlin_permutation[A] + zi;
    int AB = dn_perlin_permutation[A + 1] + zi;
    int B = dn_perlin_permutation[xi + 1] + yi;
    int BA = dn_perlin_permutation[B] + zi;
    int BB = dn_perlin_permutation[B + 1] + zi;

    return dn_perlin_lerp(w,
        dn_perlin_lerp(v,
            dn_perlin_lerp(u,
                dn_perlin_grad(dn_perlin_permutation[AA], x, y, z),
                dn_perlin_grad(dn_perlin_permutation[BA], x - 1, y, z)
            ),
            dn_perlin_lerp(u,
                dn_perlin_grad(dn_perlin_permutation[AB], x, y - 1, z),
                dn_perlin_grad(dn_perlin_permutation[BB], x - 1, y - 1, z)
            )
        ),
        dn_perlin_lerp(v,
            dn_perlin_lerp(u,
                dn_perlin_grad(dn_perlin_permutation[AA + 1], x, y, z - 1),
                dn_perlin_grad(dn_perlin_permutation[BA + 1], x - 1, y, z - 1)
            ),
            dn_perlin_lerp(u,
                dn_perlin_grad(dn_perlin_permutation[AB + 1], x, y - 1, z - 1),
                dn_perlin_grad(dn_perlin_permutation[BB + 1], x - 1, y - 1, z - 1)
            )
        )
    );
}

double dn_noise_perlin_scaled(double x, double y, double vmin, double vmax) {
    return ((dn_noise_perlin(x, y) + 1) / 2) * (vmax - vmin) + vmin;
}

float dn_noise_chaotic(float x, float y) {
    // Scale coordinates to make the noise finer
    float scale = 1000.0f;
    x *= scale;
    y *= scale;

    // Combine Perlin noise with random noise
    float perlin_value = dn_noise_perlin_scaled(x, y, -1, 1);
    float random_value = dn_perlin_random(static_cast<int>(x), static_cast<int>(y));

    // Add randomization to Perlin noise to make it more chaotic
    return dn_perlin_lerp(.2, perlin_value, random_value);
}

float dn_noise_chaotic_scaled(float x, float y, float vmin, float vmax) {
    return ((dn_noise_chaotic(x, y) + 1) / 2) * (vmax - vmin) + vmin;
}

#endif

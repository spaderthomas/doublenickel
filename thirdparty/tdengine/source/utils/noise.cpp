static const int permutation_raw[256] = {
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

static int permutation[512];


static inline float64 fade(float64 t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static inline float64 lerp(float64 t, float64 a, float64 b) {
    return a + t * (b - a);
}

float64 grad(int hash, float64 x, float64 y, float64 z) {
    int h = hash & 0xF;
    float64 u = h < 8 ? x : y;
    float64 v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// A simple random function based on integer coordinates
float random(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

// Chaotic noise function
float chaotic_noise(float x, float y, float vmin, float vmax) {
    // Scale coordinates to make the noise finer
    float scale = 1000.0f;
    x *= scale;
    y *= scale;

    // Combine Perlin noise with random noise
    float perlin_value = perlin(x, y, -1, 1);
    float random_value = random(static_cast<int>(x), static_cast<int>(y));

    // Add randomization to Perlin noise to make it more chaotic
	auto res = lerp(.2, perlin_value, random_value);
	
    // Map the result from [-1, 1] to [vmin, vmax]
    if (vmin != 0 || vmax != 1) {
        res = ((res + 1) / 2) * (vmax - vmin) + vmin;
    }
	
	return res;
}

void init_perlin_texture() {
	#if 0
	auto rx = static_cast<u32>(window.native_resolution.x);
	auto ry = static_cast<u32>(window.native_resolution.y);
	auto data = standard_allocator.alloc<u32>(rx * ry);
	
    float scale_low = 2;
    float scale_mid = 10;
    float scale_high = 100;
    auto sample_perlin = [&](int x, int y, float scale) {
        float px = static_cast<float>(x) * scale / rx;
        float py = static_cast<float>(y) * scale / ry;
        auto nf = perlin(px, py, 0, 255);
        return static_cast<u32>(nf);
    };

	for (int x = 0; x < rx; x++) {
		for (int y = 0; y < ry; y++) {
            u32 r = sample_perlin(x, y, scale_low);
            u32 g = sample_perlin(x, y, scale_mid) << 8;
            u32 b = sample_perlin(x, y, scale_high) << 16;
            u32 a = 255 << 24;
            data[y * rx + x] = r + g + b + a;
        }
	}
#endif
	
	const char* file_name = "perlin.png";
	auto file_path = resolve_format_path("image", file_name);
	create_sprite(file_name , file_path);
}

void init_chaotic_texture() {
	auto rx = static_cast<u32>(window.native_resolution.x);
	auto ry = static_cast<u32>(window.native_resolution.y);
	auto data = bump_allocator.alloc<u32>(rx * ry);

	for (int x = 0; x < rx; x++) {
		for (int y = 0; y < ry; y++) {
            u32 r = random_int(0, 255);
            u32 g = random_int(0, 255) << 8;
            u32 b = random_int(0, 255) << 16;
            u32 a = 255 << 24;
            data[y * rx + x] = r + g + b + a;
        }
	}

	create_sprite("chaotic_noise.png", (u8*)data, rx, ry, 4);
}


void init_noise() {
    for (int i = 0; i < 256; i++) {
        permutation[i] = permutation_raw[i];
        permutation[i + 256] = permutation_raw[i];
    }

	init_perlin_texture();
	init_chaotic_texture();
}

float64 perlin(float64 x, float64 y, float64 vmin, float64 vmax) {
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

    float64 u = fade(x);
    float64 v = fade(y);
    float64 w = fade(z);

    int A = permutation[xi] + yi;
    int AA = permutation[A] + zi;
    int AB = permutation[A + 1] + zi;
    int B = permutation[xi + 1] + yi;
    int BA = permutation[B] + zi;
    int BB = permutation[B + 1] + zi;

    float64 res = lerp(w,
        lerp(v,
            lerp(u,
                grad(permutation[AA], x, y, z),
                grad(permutation[BA], x - 1, y, z)
            ),
            lerp(u,
                grad(permutation[AB], x, y - 1, z),
                grad(permutation[BB], x - 1, y - 1, z)
            )
        ),
        lerp(v,
            lerp(u,
                grad(permutation[AA + 1], x, y, z - 1),
                grad(permutation[BA + 1], x - 1, y, z - 1)
            ),
            lerp(u,
                grad(permutation[AB + 1], x, y - 1, z - 1),
                grad(permutation[BB + 1], x - 1, y - 1, z - 1)
            )
        )
    );

    // Map the result from [-1, 1] to [vmin, vmax]
    if (vmin != 0 || vmax != 1) {
        res = ((res + 1) / 2) * (vmax - vmin) + vmin;
    }

    return res;
}

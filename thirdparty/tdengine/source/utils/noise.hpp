uint32 perlin_texture = 0;
uint32 chaotic_texture = 0;

void init_noise();

DN_API float64 perlin(float64 x, float64 y, float64 vmin, float64 vmax);
DN_API float64 chaotic_noise(float64 x, float64 y, float64 vmin, float64 vmax);

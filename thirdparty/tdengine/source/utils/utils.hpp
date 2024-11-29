
typedef int GLFW_KEY_TYPE;
typedef GLFW_KEY_TYPE key_t;
#define GLFW_KEY_CONTROL 349
#define GLFW_KEY_SUPER 350
#define GLFW_KEY_SHIFT 351
#define GLFW_KEY_ALT 352

#define ASCII_OPAREN     40
#define ASCII_CPAREN     41
#define ASCII_QMARK      63
#define ASCII_UNDERSCORE 95

void dn_random_init() {
  srand(time(NULL));
}

float32 dn_math_lerp(float32 a, float32 b, float32 t) {
  DN_ASSERT(t >= 0.f);
  DN_ASSERT(t <= 1.f);

  return (a * (1 - t)) + (b * t);
}

float32 dn_math_clamp(float32 value, float32 lower, float32 upper) {
  return std::min(std::max(value, lower), upper);
}

constexpr u32 dn_math_ceilf(float f) {
    const u32 i = static_cast<u32>(f);
    return f > i ? i + 1 : i;
}

constexpr u32 dn_math_ceil_divide(u32 a, u32 b) {
  return dn_math_ceilf(static_cast<float>(a) / static_cast<float>(b));
}

float dn_math_random_float_slow(float min, float max) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(generator);
}

int dn_math_random_int(int min, int max) {
  static std::random_device rd;
  static std::mt19937 generator(rd());
  
  std::uniform_int_distribution<int32> distribution(min, max);
  return distribution(generator);
}
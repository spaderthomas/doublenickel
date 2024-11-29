#ifndef DN_MATH_H
#define DN_MATH_H

DN_API f32  dn_math_lerp(f32 a, f32 b, f32 t);
DN_API f32  dn_math_clamp(f32 value, f32 lower, f32 upper);
DN_API u32  dn_math_ceilf(f32 f);
DN_API u32  dn_math_ceil_divide(u32 a, u32 b);
DN_API f32  dn_math_random_f32_slow(f32 min, f32 max);
DN_API i32  dn_math_random_i32(i32 min, i32 max);
DN_IMP void dn_random_init();

#endif



#ifdef DN_MATH_IMPLEMENTATION

f32 dn_math_lerp(f32 a, f32 b, f32 t) {
  DN_ASSERT(t >= 0.f);
  DN_ASSERT(t <= 1.f);

  return (a * (1 - t)) + (b * t);
}

f32 dn_math_clamp(f32 value, f32 lower, f32 upper) {
  return std::min(std::max(value, lower), upper);
}

u32 dn_math_ceilf(f32 f) {
    const u32 i = static_cast<u32>(f);
    return f > i ? i + 1 : i;
}

u32 dn_math_ceil_divide(u32 a, u32 b) {
  return dn_math_ceilf(static_cast<f32>(a) / static_cast<f32>(b));
}

f32 dn_math_random_f32_slow(f32 min, f32 max) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_real_distribution<f32> distribution(min, max);
  return distribution(generator);
}

i32 dn_math_random_i32(i32 min, i32 max) {
  static std::random_device rd;
  static std::mt19937 generator(rd());

  std::uniform_int_distribution<i32> distribution(min, max);
  return distribution(generator);
}

void dn_random_init() {
  srand(time(NULL));
}

#endif
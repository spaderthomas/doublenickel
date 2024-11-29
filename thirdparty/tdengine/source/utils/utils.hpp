
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

#define FM_FLOATEQ_EPSILON .00005
bool fm_floateq(float a, float b) {
  return abs(a - b) < FM_FLOATEQ_EPSILON;
}

float32 fm_lerp(float32 a, float32 b, float32 t) {
  fm_assert(t >= 0.f);
  fm_assert(t <= 1.f);

  return (a * (1 - t)) + (b * t);
}

float32 clamp(float32 value, float32 lower, float32 upper) {
  return std::min(std::max(value, lower), upper);
}

int32 fm_floor(float32 f) {
  return static_cast<int32>(floor(f));
}

int32 fm_ceil(float32 f) {
  return static_cast<int32>(ceil(f));
}

constexpr u32 ceiling(float f) {
    const u32 i = static_cast<u32>(f);
    return f > i ? i + 1 : i;
}

constexpr u32 ceiling_divide(u32 a, u32 b) {
  return ceiling(static_cast<float>(a) / static_cast<float>(b));
}

float random_float(float min, float max) {
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



void* ogl_offset_to_ptr(int32 offset) {
  return (char*)nullptr + offset;
}

template<typename T, uint32 N>
T* arr_to_ptr(T (&array)[N]) {
  return &array[0];
}


// STL extensions 
std::vector<std::string> split(const std::string &str, char delim) {
  std::stringstream stream(str);
  std::string item;
  std::vector<std::string> tokens;
  while (getline(stream, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

void string_replace(std::string& str, std::string from, std::string to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

GLFWwindow* g_window;

/* Some utilities for dealing with files, directories, and paths */

double pi = 3.14159;

size_t hash_label(const char* label) {
  constexpr size_t prime = 31;
  
  size_t result = 0;
  size_t len = strlen(label);
  for (int i = 0; i < len; i++) {
        result = label[i] + (result * prime);
    }
    return result;
}

void init_random() {
  srand(time(NULL));
}


void hex_encode(char* destination, const char* data, size_t len) {
  char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  for (int i = 0; i < len; i++) {
    char const byte = data[i];

    int base_index = i * 2;
    destination[base_index] = hex_chars[(byte & 0xF0) >> 4];
    destination[base_index + 1]     = hex_chars[(byte & 0x0F) >> 0];
  }
}

void base64_encode(char* destination, const char* source, size_t len) {
  const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  uint32_t val = 0;
  int valb = -6;

  int i = 0;
  for (i; i < len; ++i) {
    val = (val << 8) + source[i];
    valb += 8;
    while (valb >= 0) {
      destination[i] = base64_chars[(val >> valb) & 0x3F];
      valb -= 6;
    }
  }
  if (valb > -6) {
    destination[i] = base64_chars[((val << 8) >> (valb + 8)) & 0x3F];
  }

  while (i % 4) destination[i++] = '=';
}

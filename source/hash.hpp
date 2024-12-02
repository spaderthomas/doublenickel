#ifndef DN_HASH_H
#define DN_HASH_H

// Thank you to John Jackson's framework Gunslinger, from which I took much inspiration but also
// ripped this piece of hashing code off wholesale.
#define GS_SIZE_T_BITS  ((sizeof(size_t)) * 8)
#define GS_SIPHASH_C_ROUNDS 1
#define GS_SIPHASH_D_ROUNDS 1
#define gs_rotate_left(__V, __N)   (((__V) << (__N)) | ((__V) >> (GS_SIZE_T_BITS - (__N))))
#define gs_rotate_right(__V, __N)  (((__V) >> (__N)) | ((__V) << (GS_SIZE_T_BITS - (__N))))

#define DN_HASH_SEED 0x9de341c9

dn_hash_t dn_hash_siphash_bytes(const void *p, size_t len, size_t seed);
dn_hash_t dn_hash_bytes(const void *p, size_t len);
dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed);
dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b);
dn_hash_t dn_hash_cstr_dumb(const char* str);
dn_hash_t dn_hash_str_dumb(dn_string_t str);
dn_hash_t dn_hash_string(dn_string_t str);
void      dn_hash_encode_hex(char* destination, const char* data, size_t len);
void      dn_hash_encode_base64(char* destination, const char* source, size_t len);

#define dn_hash_type(t) dn_hash_string_dumb((const char*)(#t))
#endif

#ifdef DN_HASH_IMPLEMENTATION
size_t hash_siphash_bytes(const void *p, size_t len, size_t seed) {
  unsigned char *d = (unsigned char *) p;
  size_t i,j;
  size_t v0,v1,v2,v3, data;

  // hash that works on 32- or 64-bit registers without knowing which we have
  // (computes different results on 32-bit and 64-bit platform)
  // derived from siphash, but on 32-bit platforms very different as it uses 4 32-bit state not 4 64-bit
  v0 = ((((size_t) 0x736f6d65 << 16) << 16) + 0x70736575) ^  seed;
  v1 = ((((size_t) 0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
  v2 = ((((size_t) 0x6c796765 << 16) << 16) + 0x6e657261) ^  seed;
  v3 = ((((size_t) 0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

  #ifdef STBDS_TEST_SIPHASH_2_4
  // hardcoded with key material in the siphash test vectors
  v0 ^= 0x0706050403020100ull ^  seed;
  v1 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  v2 ^= 0x0706050403020100ull ^  seed;
  v3 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  #endif

  #define gs_sipround() \
    do {                   \
      v0 += v1; v1 = gs_rotate_left(v1, 13);  v1 ^= v0; v0 = gs_rotate_left(v0,GS_SIZE_T_BITS/2); \
      v2 += v3; v3 = gs_rotate_left(v3, 16);  v3 ^= v2;                                                 \
      v2 += v1; v1 = gs_rotate_left(v1, 17);  v1 ^= v2; v2 = gs_rotate_left(v2,GS_SIZE_T_BITS/2); \
      v0 += v3; v3 = gs_rotate_left(v3, 21);  v3 ^= v0;                                                 \
    } while (0)

  for (i=0; i+sizeof(size_t) <= len; i += sizeof(size_t), d += sizeof(size_t)) {
    data = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    data |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24)) << 16 << 16; // discarded if size_t == 4

    v3 ^= data;
    for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
      gs_sipround();
    v0 ^= data;
  }
  data = len << (GS_SIZE_T_BITS-8);
  switch (len - i) {
    case 7: data |= ((size_t) d[6] << 24) << 24; // fall through
    case 6: data |= ((size_t) d[5] << 20) << 20; // fall through
    case 5: data |= ((size_t) d[4] << 16) << 16; // fall through
    case 4: data |= (d[3] << 24); // fall through
    case 3: data |= (d[2] << 16); // fall through
    case 2: data |= (d[1] << 8); // fall through
    case 1: data |= d[0]; // fall through
    case 0: break;
  }
  v3 ^= data;
  for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
    gs_sipround();
  v0 ^= data;
  v2 ^= 0xff;
  for (j=0; j < GS_SIPHASH_D_ROUNDS; ++j)
    gs_sipround();

#if 0
  return v0^v1^v2^v3;
#else
  return v1^v2^v3; // slightly stronger since v0^v3 in above cancels out final round operation? I tweeted at the authors of SipHash about this but they didn't reply
#endif
}

#define DN_HASH_SEED 0x9de341c9
dn_hash_t dn_hash_bytes(const void *p, size_t len) {
  return hash_siphash_bytes(p, len, DN_HASH_SEED);
}

dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed) {
  return hash_siphash_bytes(p,len,seed);
}

dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b) {
  return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

dn_hash_t dn_hash_cstr_dumb(const char* str) {
  return dn_hash_str_dumb(dn_string_literal(str));
}

dn_hash_t dn_hash_str_dumb(dn_string_t str) {
  constexpr size_t prime = 31;
  
  size_t result = 0;
  for (int i = 0; i < str.len; i++) {
        result = str.data[i] + (result * prime);
    }
    return result;
}

dn_hash_t dn_hash_string(dn_string_t str) {
  return dn_hash_bytes(str.data, str.len);
}

void dn_hash_encode_hex(char* destination, const char* data, size_t len) {
  static char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  for (int i = 0; i < len; i++) {
    char const byte = data[i];

    int base_index = i * 2;
    destination[base_index] = hex_chars[(byte & 0xF0) >> 4];
    destination[base_index + 1]     = hex_chars[(byte & 0x0F) >> 0];
  }
}

void dn_hash_encode_base64(char* destination, const char* source, size_t len) {
  static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  u32 val = 0;
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


#define dn_hash_type(t) dn_hash_string_dumb((const char*)(#t))
#endif
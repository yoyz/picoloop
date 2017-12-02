#include <stdint.h>
static inline int32_t signed_saturate_rshift(int32_t val, int bits, int rshift)
{
  int32_t out, max;
  out = val >> rshift;
  max = 1 << (bits - 1);
  if   (out >= 0)
    { if (out > max - 1) out = max - 1;     }
  else
    { if (out < -max) out = -max;           }
  return out;
}


static inline int16_t saturate16(int32_t val)
{
  return 0;
}

static inline int32_t signed_multiply_32x16b(int32_t a, uint32_t b)
{
        return ((int64_t)a * (int16_t)(b & 0xFFFF)) >> 16;
}

static inline uint32_t pack_16t_16t(int32_t a, int32_t b)
{
        return (a & 0xFFFF0000) | ((uint32_t)b >> 16);
}

static inline uint32_t pack_16b_16b(int32_t a, int32_t b)
{
        return (a << 16) | (b & 0x0000FFFF);

}

// computes ((a[31:0] * b[31:16]) >> 16)
static inline int32_t signed_multiply_32x16t(int32_t a, uint32_t b)
{
        return ((int64_t)a * (int16_t)(b >> 16)) >> 16;
}

// computes (((a[31:16] + b[31:16]) << 16) | (a[15:0 + b[15:0]))  (saturates)
//static inline uint32_t signed_add_16_and_16(uint32_t a, uint32_t b) __attribute__((always_inline, unused));
static inline uint32_t signed_add_16_and_16(uint32_t a, uint32_t b)
{
  return a+b;
}

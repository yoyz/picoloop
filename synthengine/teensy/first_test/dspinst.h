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

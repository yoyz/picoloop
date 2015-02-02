#ifndef __MYTYPES_H__
#define __MYTYPES_H__

#include "fixed.h"

#ifdef FIXED
#define mfloat PBSynthFixed
#define MFLOAT_HALF(f) f.v >>= 1;
#else
#define mfloat float
#define MFLOAT_HALF(f) f /= 2.0f;
#endif

#endif

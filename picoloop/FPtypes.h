#ifndef __FPYTYPES_H__
#define __FPYTYPES_H__

#include "FixedPoint.h"

#ifdef FIXEDPOINT
#define FPfloat FixedPoint
#define FIXEDPOINT_HALF(f) f.v >>= 1;
#else
#define FPfloat float
#define MFLOAT_HALF(f) f /= 2.0f;
#endif

#endif

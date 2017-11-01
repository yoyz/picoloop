#ifndef __FIXEDPOINT_H__
#define __FIXEDPOINT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define FTYPE int32_t
#define NUMBITS (sizeof(FTYPE) * 8)
#define FBITS 20
#define CBITS 10

class FixedPoint {
	
public:
	FTYPE v;
	
	inline FixedPoint() { }
	inline FixedPoint(const FTYPE b)                       { v = b << FBITS; }
	inline FixedPoint(const FTYPE b, int d)                { v = b; }
	inline FixedPoint(float a)                             { v = (FTYPE)(a * ((FTYPE)1 << FBITS)); }
	inline operator float()                                { return ((float)v) / ((FTYPE)1 << FBITS); }
	inline FixedPoint& operator =(const float &b)          { v = (FTYPE) (b * ((FTYPE)1 << FBITS)); return *this; }
	inline FixedPoint& operator =(const int32_t b)         { v = (FTYPE)b << FBITS; return *this; }
	inline FixedPoint operator *(const FixedPoint &b)      { return FixedPoint((v >> CBITS) * (b.v >> CBITS), 0); 	  }
	inline FixedPoint operator *(const float b)            { return FixedPoint(b * (float)*this); }
	inline FixedPoint operator *(const int b)              { return FixedPoint((FTYPE)b * v, 0); }
	inline FixedPoint operator /(const FixedPoint &b)      { return FixedPoint((v << CBITS) / (b.v >> CBITS), 0); 	}
	inline FixedPoint operator /(const int b)              { return FixedPoint(v / (FTYPE)b, 0); }
	inline FixedPoint operator +(const FixedPoint &b)      { return FixedPoint(b.v + v, 0); }
	inline FixedPoint operator +(const float b)            { return FixedPoint(b + (float)*this); }
	inline FixedPoint operator +(const int b)              { return FixedPoint(((FTYPE)b << FBITS) + v, 0); }

	inline FixedPoint operator -(const float b)            { return FixedPoint((float)*this - b); }
	inline FixedPoint operator -(const int b)              { return FixedPoint(v - ((FTYPE)b << FBITS), 0); }

	inline FixedPoint& operator +=(const FixedPoint &b)    { v += b.v; return *this; }
	inline FixedPoint& operator -=(const FixedPoint &b)    { v -= b.v; return *this; }
	inline FixedPoint& operator -=(const int b)            { v -= (FTYPE)b << FBITS; return *this; }
	
	inline bool operator ==(const FixedPoint &b)           { return v == b.v; }
	inline bool operator ==(const float b)                 { return (float)*this == b; }
	inline bool operator <=(const FixedPoint &b)           { return v <= b.v; }
	inline bool operator <=(const float b)                 { return (float)*this <= b; }
	inline bool operator <(const float b)                  { return (float)*this < b; }
	inline bool operator >=(const FixedPoint &b)           { return v >= b.v; }
	inline bool operator >=(const float b)                 { return (float)*this >= b; }
	inline bool operator >(const float b)                  { return (float)*this > b; }
	inline bool operator <(const FixedPoint &b)            { return v < b.v; }
	inline bool operator >(const FixedPoint &b)            { return v > b.v; }
	inline bool operator <=(const int b)                   { return v <= ((FTYPE)b << FBITS); }
	inline bool operator ==(const int b)                   { return v == ((FTYPE)b << FBITS); }
	inline bool operator <(const int b)                    { return v <  ((FTYPE)b << FBITS); }
	inline bool operator >=(const int b)                   { return v >= ((FTYPE)b << FBITS); }
	inline bool operator >(const int b)                    { return v >  ((FTYPE)b << FBITS); }
};

#endif


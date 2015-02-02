#ifndef __FIXED_H__
#define __FIXED_H__

#include <stdio.h>
#include <stdlib.h>

//#define FTYPE signed long
#define FTYPE int32_t
#define NUMBITS (sizeof(FTYPE) * 8)
//#define FBITS 20
#define FBITS 20
#define CBITS (FBITS / 2)

class fixed {
	
public:
	FTYPE v;
	
	inline fixed() { }
	inline fixed(const FTYPE b) { v = b << FBITS; }
	inline fixed(const FTYPE b, int d) { v = b; }
  inline fixed(float a) { v = (FTYPE)(a * ((FTYPE)1 << FBITS)); }

	inline operator float() { return ((float)v) / ((FTYPE)1 << FBITS); }
	
  inline fixed& operator =(const float &b) { v = (FTYPE) (b * ((FTYPE)1 << FBITS)); return *this; }
  inline fixed& operator =(const int b) { v = (FTYPE)b << FBITS; return *this; }

	inline fixed operator *(const fixed &b) {

		return fixed((v >> CBITS) * (b.v >> CBITS), 0);
	}
	inline fixed operator *(const float b) { return fixed(b * (float)*this); }
	inline fixed operator *(const int b) { return fixed((FTYPE)b * v, 0); }

	inline fixed operator /(const fixed &b) {
	
		return fixed((v << CBITS) / (b.v >> CBITS), 0);
	}
	inline fixed operator /(const int b) { return fixed(v / (FTYPE)b, 0); }
	inline fixed operator +(const fixed &b) { return fixed(b.v + v, 0); }
	inline fixed operator +(const float b) { return fixed(b + (float)*this); }
	inline fixed operator +(const int b) { return fixed(((FTYPE)b << FBITS) + v, 0); }

	inline fixed operator -(const float b) { return fixed((float)*this - b); }
	inline fixed operator -(const int b) { return fixed(v - ((FTYPE)b << FBITS), 0); }

  inline fixed& operator +=(const fixed &b) { v += b.v; return *this; }
  inline fixed& operator -=(const fixed &b) { v -= b.v; return *this; }
  inline fixed& operator -=(const int b) { v -= (FTYPE)b << FBITS; return *this; }

  inline bool operator ==(const fixed &b) { return v == b.v; }
  inline bool operator ==(const float b) { return (float)*this == b; }
  inline bool operator <=(const fixed &b) { return v <= b.v; }
  inline bool operator <=(const float b) { return (float)*this <= b; }
  inline bool operator <(const float b) { return (float)*this < b; }
  inline bool operator >=(const fixed &b) { return v >= b.v; }
  inline bool operator >=(const float b) { return (float)*this >= b; }
  inline bool operator >(const float b) { return (float)*this > b; }
  inline bool operator <(const fixed &b) { return v < b.v; }
  inline bool operator >(const fixed &b) { return v > b.v; }
  inline bool operator <=(const int b) { return v <= ((FTYPE)b << FBITS); }
  inline bool operator ==(const int b) { return v == ((FTYPE)b << FBITS); }
  inline bool operator <(const int b) { return v < ((FTYPE)b << FBITS); }
  inline bool operator >=(const int b) { return v >= ((FTYPE)b << FBITS); }
  inline bool operator >(const int b) { return v > ((FTYPE)b << FBITS); }
};

#endif

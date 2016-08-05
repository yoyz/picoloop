#ifndef __FIXED_H__
#define __FIXED_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#define FTYPE signed long
#define FTYPE int32_t
//#define FTYPE int64_t
//#define NUMBITS (sizeof(FTYPE) * 8)
#define NUMBITS (sizeof(FTYPE) * 8)
//#define FBITS 20
//#define FBITS 20
//#define CBITS (FBITS / 2)

#define FBITS 20
#define CBITS 10

class PBSynthFixed {
	
public:
	FTYPE v;
	
	inline PBSynthFixed() { }
	inline PBSynthFixed(const FTYPE b)                          { v = b << FBITS; }
	inline PBSynthFixed(const FTYPE b, int d)                   { v = b; }
	inline PBSynthFixed(float a)                                { v = (FTYPE)(a * ((FTYPE)1 << FBITS)); }
	inline operator float()                                     { return ((float)v) / ((FTYPE)1 << FBITS); }       
	inline PBSynthFixed& operator =(const float &b)             { v = (FTYPE) (b * ((FTYPE)1 << FBITS)); return *this; }
	inline PBSynthFixed& operator =(const int b)                { v = (FTYPE)b << FBITS; return *this; }
	inline PBSynthFixed operator *(const PBSynthFixed &b)       { return PBSynthFixed((v >> CBITS) * (b.v >> CBITS), 0); 	  }
	inline PBSynthFixed operator *(const float b)               { return PBSynthFixed(b * (float)*this); }
	inline PBSynthFixed operator *(const int b)                 { return PBSynthFixed((FTYPE)b * v, 0); }
	inline PBSynthFixed operator /(const PBSynthFixed &b)       { return PBSynthFixed((v << CBITS) / (b.v >> CBITS), 0); 	}
	inline PBSynthFixed operator /(const int b)                 { return PBSynthFixed(v / (FTYPE)b, 0); }
	inline PBSynthFixed operator +(const PBSynthFixed &b)       { return PBSynthFixed(b.v + v, 0); }
	inline PBSynthFixed operator +(const float b)               { return PBSynthFixed(b + (float)*this); }
	inline PBSynthFixed operator +(const int b)                 { return PBSynthFixed(((FTYPE)b << FBITS) + v, 0); }

	inline PBSynthFixed operator -(const float b)               { return PBSynthFixed((float)*this - b); }
	inline PBSynthFixed operator -(const int b)                 { return PBSynthFixed(v - ((FTYPE)b << FBITS), 0); }

	inline PBSynthFixed& operator +=(const PBSynthFixed &b)     { v += b.v; return *this; }
	inline PBSynthFixed& operator -=(const PBSynthFixed &b)     { v -= b.v; return *this; }
	inline PBSynthFixed& operator -=(const int b)               { v -= (FTYPE)b << FBITS; return *this; }
	
	inline bool operator ==(const PBSynthFixed &b)              { return v == b.v; }
	inline bool operator ==(const float b)                      { return (float)*this == b; }
	inline bool operator <=(const PBSynthFixed &b)              { return v <= b.v; }
	inline bool operator <=(const float b)                      { return (float)*this <= b; }
	inline bool operator <(const float b)                       { return (float)*this < b; }
	inline bool operator >=(const PBSynthFixed &b)              { return v >= b.v; }
	inline bool operator >=(const float b)                      { return (float)*this >= b; }
	inline bool operator >(const float b)                       { return (float)*this > b; }
	inline bool operator <(const PBSynthFixed &b)               { return v < b.v; }
	inline bool operator >(const PBSynthFixed &b)               { return v > b.v; }
	inline bool operator <=(const int b)                        { return v <= ((FTYPE)b << FBITS); }
	inline bool operator ==(const int b)                        { return v == ((FTYPE)b << FBITS); }
	inline bool operator <(const int b)                         { return v <  ((FTYPE)b << FBITS); }
	inline bool operator >=(const int b)                        { return v >= ((FTYPE)b << FBITS); }
	inline bool operator >(const int b)                         { return v >  ((FTYPE)b << FBITS); }
};

#endif


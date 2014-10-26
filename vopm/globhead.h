/*
 *  globhead.h
 *  VOPM_test
 *
 */

#ifdef __GNUC__
	#ifndef __int64
		#define __int64 long long
	#endif
#endif

#ifdef __APPLE__
//case of WIN(mingw32),define in windef.h
//define MAX_PATH 260
	#ifndef MAX_PATH
		#define MAX_PATH PATH_MAX
	#endif
#endif

extern int OpmRate;
extern int Samprate;
extern unsigned int irnd(void);
 
 


/****************************
some wanabe filter code by Beausoleil S Guillemette
hope it will work :D
[2006-09-25 .. 2006-09-30]
****************************/

/*-------------------------------------------
some useful abstract info:
 there are 8 filters, one per voice. each filter
 is stereo, so there are 2 height and speed vars.
 The filters are static/globals, so that when
 calling the action can be resumed when calling
 buffer rendering again (removing clics)
-------------------------------------------*/

#include "lgptfixed.h"

typedef enum 
{
	FLT_LOWPASS,		//only lowpass is implemented so far
	FLT_HIGHPASS,
	FLT_BANDPASS,
	FLT_NOTCH,
}filterType_t;


typedef struct
{
  lgptfixed height[2];
  lgptfixed speed[2];
  lgptfixed hipdelay[2] ;
  filterType_t type;
  lgptfixed parm1,parm2;
  lgptfixed freq,reso;
  lgptfixed dirt ;
  lgptfixed mix ;
} filter_t;

void set_filter(int channel, filterType_t type, lgptfixed parm1, lgptfixed parm2,int mix,bool bassyMapping);

void init_filters(void) ;


filter_t *get_filter(int channel) ;

/* amSynth
 * (c) 2001-2005 Nick Dowell
 */

#include "AMSynthLowPassFilter.h"
#include "Synth--.h"		// for PI and TWO_PI
#include <math.h>

AMSynthLowPassFilter::AMSynthLowPassFilter() :
	rate (4100.0)
,	nyquist (22050.0)
{
  d1=d2=d3=d4=0;
  i_d1=i_d2=i_d3=i_d4=0;
}

void
AMSynthLowPassFilter::reset()
{
	d1 = d2 = d3 = d4 = 0;
	i_d1=i_d2=i_d3=i_d4=0;
}


void AMSynthLowPassFilter::calc(float fc, float res)
{
	// constrain cutoff
#define SAFE 0.99f // filter is unstable _AT_ PI
	if (fc>(nyquist*SAFE))
		fc=nyquist*SAFE;
	if (fc<10)
		{fc = 10;/*d1=d2=d3=d4=0;*/}
	w = (fc/(float)rate); // cutoff freq [ 0 <= w <= 0.5 ]
	
	// find final coeff values for end of this buffer
	//register double k, k2, bh, a0, a1, a2, b1, b2;
	r = 2*(1-res);
	if(r==0.0) r = 0.001;

	k=tan(w*PI);
	k2 = k*k;
	bh = 1 + (r*k) + k2;

	a0 = a2 = float(k2/bh);
	a1 = a0 * 2;

	b1 = float(2*(k2-1)/-bh);
	b2 = float((1-(r*k)+k2)/-bh);         

	i_a0=a0             * AM_DECAL;
	i_a1=a1             * AM_DECAL;
	i_a2=a2             * AM_DECAL;
	
	i_b1=b1             * AM_DECAL;
	i_b2=b2             * AM_DECAL;

	//i_d1=d1             * AM_DECAL;
	//i_d2=d2             * AM_DECAL;

}

void
AMSynthLowPassFilter::ProcessSamples	(float *buffer, int numSamples)
{
  for (int i=0; i<numSamples; i++) {
    x = buffer[i];
    
    // first 2nd-order unit
    y = ( a0*x ) + d1;
    d1 = d2 + ( (a1)*x ) + ( (b1)*y );
    d2 = ( (a2)*x ) + ( (b2)*y );
    x=y;
    // and the second
    
    y = ( a0*x ) + d3;
    d3 = d4 + ( a1*x ) + ( b1*y );
    d4 = ( a2*x ) + ( b2*y );
    
    buffer[i] = (float) y;
  }
}


float
AMSynthLowPassFilter::ProcessSample	(float buffer)
{
    x = buffer;
    
    // first 2nd-order unit
    y = ( a0*x ) + d1;
    d1 = d2 + ( (a1)*x ) + ( (b1)*y );
    d2 = ( (a2)*x ) + ( (b2)*y );
    x=y;
    // and the second
    
    y = ( a0*x ) + d3;
    d3 = d4 + ( a1*x ) + ( b1*y );
    d4 = ( a2*x ) + ( b2*y );

    //printf("y:%.8f buffer:%.8f\n",y,buffer);    
    buffer = y;

    return buffer;
    //return buffer;

    //a0:0.29714334 a1:0.59428668 a2:0.29714334
    //b1:1.99715245 b2:-0.99715459
    //d1:1736.26477051 d2713.18225098 d3:1743.07446289 d4:716.03179932

    //printf("a0:%.8f a1:%.8f a2:%.8f\n",a0,a1,a2);    
    //printf("b0:%.8f b1:%.8f b2:%.8f\n",b1,b2);
    //printf("d1:%.8f d2%.8f d3:%.8f d4:%.8f\n",d1,d2,d3,d4);
}


int 
AMSynthLowPassFilter::ProcessSample	(int buffer)
{
    i_x = buffer;
    i_x = i_x << AM_SHIFT;
    
    // first 2nd-order unit
    i_y  =        ( ( i_a0*i_x )>> AM_SHIFT )  +    i_d1;
    i_d1 = i_d2 + ( ( i_a1*i_x )>> AM_SHIFT )  + ( (i_b1*i_y ) >> AM_SHIFT );
    i_d2 =        ( ( i_a2*i_x )>> AM_SHIFT )  + ( (i_b2*i_y ) >> AM_SHIFT );
    i_x  = i_y;
    // and the second
    
    i_y  =        ( (i_a0*i_x) >> AM_SHIFT)    +    i_d3;
    i_d3 = i_d4 + ( (i_a1*i_x) >> AM_SHIFT)    + ( (i_b1*i_y) >> AM_SHIFT  );
    i_d4 =        ( (i_a2*i_x) >> AM_SHIFT )   + ( (i_b2*i_y) >> AM_SHIFT  );

    //printf("y:%.8f buffer:%.8f\n",y,buffer);    
    buffer = i_y >> AM_SHIFT;

    return buffer;
    //return buffer;

    //a0:0.29714334 a1:0.59428668 a2:0.29714334
    //b1:1.99715245 b2:-0.99715459
    //d1:1736.26477051 d2713.18225098 d3:1743.07446289 d4:716.03179932

    //printf("a0:%.8f a1:%.8f a2:%.8f\n",a0,a1,a2);    
    //printf("b0:%.8f b1:%.8f b2:%.8f\n",b1,b2);
    //printf("d1:%.8f d2%.8f d3:%.8f d4:%.8f\n",d1,d2,d3,d4);
}


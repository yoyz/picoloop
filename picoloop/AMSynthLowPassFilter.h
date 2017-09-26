/* amSynth
 * (c) 2001-2005 Nick Dowell
 */
#include <stdio.h>
#include <stdint.h>
#ifndef _AMSYNTHLOWPASSFILTER_H
#define _AMSYNTHLOWPASSFILTER_H


//#define    DECAL 16777216
//#define      DECAL 2147483648

/*
#define      AM_SHIFT 15
#define      AM_DECAL 32768
*/
#define      AM_SHIFT 16
#define      AM_DECAL 65536



/**
 * A 24 dB/ocatave resonant low-pass filter.
 **/
class AMSynthLowPassFilter
{
public:
	AMSynthLowPassFilter();

	void	SetSampleRate	(int rateIn) { rate = (float) rateIn; nyquist = rate/(float)2; }
	
	/**
	 * Reset the filter - clear anything in the delay units of the filter.
	 */
	void reset();

	void calc(float fc, float res);
	void	process_samples(float*, int);
	float	process_one_sample(float);
	int32_t	process_one_sample(int32_t);
	//int	process_one_sample(int);
private:
	float rate;
	float nyquist;
	float d1, d2, d3, d4;
	float a0, a1, a2;
	float b1, b2;
	float x,y;
	float k, k2, bh;
	float r;
	float w;

	int64_t   i_a0, i_a1, i_a2, i_b1, i_b2;
	int64_t   i_d1, i_d2, i_d3, i_d4;
	int64_t   i_x,i_y;
	/*
	int32_t   i_a0, i_a1, i_a2, i_b1, i_b2;
	int32_t   i_d1, i_d2, i_d3, i_d4; 
	int32_t   i_x,i_y; 
	*/
};

#endif

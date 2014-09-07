/* amSynth
 * (c) 2001-2005 Nick Dowell
 */
#include <stdio.h>
#ifndef _AMSYNTHLOWPASSFILTER_H
#define _AMSYNTHLOWPASSFILTER_H

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
	void	ProcessSamples	(float*, int);
	float	ProcessSample	(float);
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
};

#endif

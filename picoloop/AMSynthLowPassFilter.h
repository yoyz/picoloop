/* amSynth
 * (c) 2001-2005 Nick Dowell
 */

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
private:
	float rate;
	float nyquist;
	double d1, d2, d3, d4;
	double a0, a1, a2, b0, b1, b2;
	double x,y;
	double k, k2, bh;
	double r;
	float w;
};

#endif

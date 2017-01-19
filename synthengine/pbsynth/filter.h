#ifndef __IIR_H__
#define __IIR_H__

#include "mytypes.h"

class Filter {

public:
	Filter();
	~Filter();
	void setCutoff(mfloat cutoff, mfloat reso);
	void process(mfloat *in, mfloat *out, int n);
	void reset();
	void *getParameters(int *size);
	void setParameters(void *p);
	mfloat getCutoff();
	mfloat getResonance();

protected:
	struct {
		
		mfloat cutoff, resonance;
	} parameters;

	mfloat coeff[5]; /* a0, a1, a2, b1, b2 */
	mfloat xbuffer[4];
	mfloat ybuffer[4];
};

#endif

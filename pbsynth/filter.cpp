#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filter.h"
#include "coeff.h"

#define IIR_A0 0
#define IIR_A1 1
#define IIR_A2 2
#define IIR_B1 3
#define IIR_B2 4

Filter::Filter() {

	reset();
}

Filter::~Filter() {
}

void Filter::setCutoff(mfloat cutoff, mfloat resonance) {

	int c, r;
	long ofs, i;

	if (cutoff < -1)
		cutoff = -1;
	else
	if (cutoff > 1)
		cutoff = 1;
	
	if (resonance < -1)
		resonance = -1;
	else
	if (resonance > 1)
		resonance = 1;

	parameters.cutoff = cutoff;
	parameters.resonance = resonance;
		
	c = (int)(((cutoff + 1) / 2) * 1023);
	r = (int)(((resonance + 1) / 2) * 127);

	ofs = 5 * (c + (1024 * r));
	
	for (i = 0; i < 5; i++)
		coeff[i] = lpcoeff[ofs++];
}


void Filter::reset() {
	
	int i;

	for (i = 0; i < 4; i++)
		xbuffer[i] = ybuffer[i] = 0;
		
	setCutoff(1.0f, -1.0f);
}


void Filter::process(mfloat *in, mfloat *out, int n) {
	
	int i;
	mfloat a0, a1, a2, b1, b2;
	mfloat *xb;
	mfloat *yb;
	
	a0 = coeff[IIR_A0];
	a1 = coeff[IIR_A1];
	a2 = coeff[IIR_A2];
	b1 = coeff[IIR_B1];
	b2 = coeff[IIR_B2];
	
	xb = xbuffer;
	yb = ybuffer;

	for (i = 0; i < n; i++) {

		xb[3] = xb[3 - 1];
		yb[3] = yb[3 - 1];
		xb[2] = xb[2 - 1];
		yb[2] = yb[2 - 1];
		xb[1] = xb[1 - 1];
		yb[1] = yb[1 - 1];

		xbuffer[0] = in[i];

		out[i] = ((a0 * xb[0]) +
		          (a1 * xb[1]) +
		          (a2 * xb[2]) +
	  		      (b1 * yb[1]) +
	  		      (b2 * yb[2]));

		yb[0] = out[i];
	}
}

void *Filter::getParameters(int *size) {

	void *p;

	*size = sizeof(parameters);
	p = malloc(sizeof(parameters));
	memcpy(p, &parameters, sizeof(parameters));

	return p;
}

void Filter::setParameters(void *p) {
	
	memcpy(&parameters, p, sizeof(parameters));
	setCutoff(parameters.cutoff, parameters.resonance);
}

mfloat Filter::getCutoff() {
	
	return parameters.cutoff;
}

mfloat Filter::getResonance() {
	
	return parameters.resonance;
}

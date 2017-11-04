#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "osc.h"
#include "osctab.h"

PBSynthOscillator::PBSynthOscillator() {
	
  reset();
}

PBSynthOscillator::~PBSynthOscillator() {
}

void PBSynthOscillator::processTriangle(mfloat *in, mfloat *out, int n, int owrite) {
	
	int i;
	mfloat triInc;
	mfloat loval;
	mfloat lamp;
	mfloat ovalInc;
	
	loval = oval;
	lamp = parameters.amp;
	triInc = sawInc;
	ovalInc = triInc * 2;

	if (owrite)
	for (i = 0; i < n; i++) {
		
		out[i] = loval * lamp;
		
		loval = loval + ovalInc;
		
		if (loval > 1) {
			
			loval = 1 - (loval - 1);
			triInc = -triInc;
			ovalInc = triInc * 2;
		} else
		if (loval < -1) {
			
			loval = -(1 - (-loval - 1));
			triInc = -triInc;
			ovalInc = triInc * 2;
		}
	}
	else
	for (i = 0; i < n; i++) {
		
		out[i] = in[i] + (loval * lamp);
		
		loval = loval + ovalInc;
		
		if (loval > 1) {
			
			loval = 1 - (loval - 1);
			triInc = -triInc;
			ovalInc = triInc * 2;
		} else
		if (loval < -1) {
			
			loval = -(1 - (-loval - 1));
			triInc = -triInc;
			ovalInc = triInc * 2;
		}
	}

	sawInc = triInc;
	oval = loval;
}

void PBSynthOscillator::processSawtooth(mfloat *in, mfloat *out, int n, int owrite) {
	
	int i=0;
	mfloat lsawInc=0;
	mfloat loval=0;
	mfloat lamp=0;
	
	loval = oval;
	lamp = parameters.amp;
	
	lsawInc = sawInc;
	
	if (owrite)
	for (i = 0; i < n; i++) {
		
		out[i] = loval * lamp;

		loval = loval + lsawInc;
		
		if (loval > 1) {

			loval = loval - 2;
		}
	}
	else
	for (i = 0; i < n; i++) {
		
		out[i] = in[i] + (loval * lamp);
		
		loval = loval + lsawInc;
		
		if (loval > 1) {
			
			loval = loval - 2;
		}
	}
	
	oval = loval;
}

void PBSynthOscillator::processRectangle(mfloat *in, mfloat *out, int n, int owrite) {
	
	int i;
	mfloat lwlength = wlength;
	mfloat loval;
	mfloat lcounter;
	mfloat pw;
	mfloat count[2];


	pw = parameters.pulsewidth + 1;
	pw = pw / 2;
		
	count[0] = (lwlength * pw) / 2;
	count[1] = (lwlength * (1 - pw)) / 2;
	
	lcounter = counter;
	loval = parameters.amp * sqp;

	if (owrite)
	for (i = 0; i < n; i++) {
		
		out[i] = loval;
		lcounter -= 1;

		if (lcounter <= 0) {
			
			sqc ^= 1;
			lcounter += count[sqc];
			loval = -loval;
		}
	}
	else
	for (i = 0; i < n; i++) {
		
		out[i] = in[i] + loval;
		lcounter -= 1;

		if (lcounter <= 0) {

			sqc ^= 1;
			lcounter += count[sqc];
			loval = -loval;
		}
	}

	if (loval < 0)
		sqp = -1;
	else
		sqp = 1;

	counter = lcounter;
}

void PBSynthOscillator::process(mfloat *in, mfloat *out, int n, int owrite) {

	if (parameters.wave == OSC_RECTANGLE)
		processRectangle(in, out, n, owrite);
	else
	if (parameters.wave == OSC_SAWTOOTH)
		processSawtooth(in, out, n, owrite);
	else
	if (parameters.wave == OSC_TRIANGLE)
		processTriangle(in, out, n, owrite);
}

void PBSynthOscillator::setNote(int note) {
	
	long i;

	i = ((note * 128) + parameters.detune) * 2;
	if (i<0) i=0;
	if (i>32000) i=32000;
	wlength = osctab[i];
	sawInc = osctab[i + 1];

	if (parameters.amp == 0)
		oval = 0;
		
	this->note = note;
}

void PBSynthOscillator::reset() {
	
	parameters.amp = 0.5f;
	parameters.wave = OSC_SAWTOOTH;
	counter = 0.0f;
	sqp = 1.0f;
	sqc = 0;
	oval=0;
	sawInc=0;
	parameters.pulsewidth = 0.0f;
	parameters.detune = 0;
	setAmp(1.0f);
	setWave(OSC_SAWTOOTH);
	setPulseWidth(0.0f);
	setNote(12);
	setDetune(-128);
}

void PBSynthOscillator::setDetune(long detune) {
	
	parameters.detune = detune;
	setNote(note);
}

void PBSynthOscillator::setAmp(mfloat amp) {
	
	if (amp > 1)
		amp = 1;
	else
	if (amp < -1)
		amp = -1;
		
	parameters.amp = amp;
}

void PBSynthOscillator::setWave(int wave) {
	
	parameters.wave = wave;
}

void PBSynthOscillator::setPulseWidth(mfloat pw) {
	
	parameters.pulsewidth = pw;
}

void *PBSynthOscillator::getParameters(int *size) {

	void *p;
	
	*size = sizeof(parameters);
	p = malloc(sizeof(parameters));
	memcpy(p, &parameters, sizeof(parameters));

	return p;
}

void PBSynthOscillator::setParameters(void *p) {
	
	memcpy(&parameters, p, sizeof(parameters));
}

int PBSynthOscillator::getWave() {
	
	return parameters.wave;
}

long PBSynthOscillator::getDetune() {
	
	return parameters.detune;
}

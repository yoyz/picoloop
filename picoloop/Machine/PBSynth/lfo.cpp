#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lfo.h"
#include "lfotab.h"

LFO::LFO(int samplesPerTick) {
	
	this->samplesPerTick = samplesPerTick;
	reset();
}

LFO::~LFO() {
}

mfloat LFO::param2IncVal(mfloat x) {
	
	mfloat ret;
	int i;
	
	if (x < -1)
		x = -1;
	else
	if (x > 1)
		x = 1;
	
	i = (int)(((x + 1) / 2) * 127);

	ret = lfotab[i];
	
	return ret;
}


void LFO::reset() {
	
	out = 0;
	inc = 1;
	setRate(0.5f);
}

void LFO::process() {
	
	if (inc)
		out += incVal * samplesPerTick;
	else
		out -= incVal * samplesPerTick;
	
	if (out > 1) {
		
		out = 1 - (out - 1);
		inc = !inc;
	} else
	if (out < -1) {
		
		out = -(1 - (-out - 1));
		inc = !inc;
	}
}

void LFO::setRate(mfloat rate) {

	incVal = param2IncVal(-rate);
	parameters.rate = rate;
}

mfloat LFO::getValue() {
	
	return out;
}

void *LFO::getParameters(int *size) {

	void *p;
	
	*size = sizeof(parameters);
	p = malloc(sizeof(parameters));
	memcpy(p, &parameters, sizeof(parameters));

	return p;
}

void LFO::setParameters(void *p) {
	
	memcpy(&parameters, p, sizeof(parameters));
	setRate(parameters.rate);
}

mfloat LFO::getRate() {
	
	return parameters.rate;
}

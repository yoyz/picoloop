#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "envtab.h"
/*
#define ENV_MAX	2.0f
#define ENV_MIN (2.0f / (44100.0f * 10.0f))
#define ENV_M (((1.0f / ENV_MIN) - (1.0f / ENV_MAX)) / 2.0f)
#define ENV_B (((1.0f / ENV_MIN) + (1.0f / ENV_MAX)) / 2.0f)
*/

Envelope::Envelope(int samplesPerTick) {
	
	reset();
	this->samplesPerTick = samplesPerTick;
}

Envelope::~Envelope() {
}

mfloat Envelope::param2IncVal(mfloat x) {
	
	mfloat ret;
	int i;
	
	if (x < -1)
		x = -1;
	else
	if (x > 1)
		x = 1;
	
	i = (int)(((x + 1) / 2) * 127);
	ret = envtab[i];

	return ret;
}

void Envelope::setA(mfloat a) {
	
	this->a = param2IncVal(a);
	parameters.a = a;
}

void Envelope::setD(mfloat d) {
	
	this->d = param2IncVal(d);
	parameters.d = d;
}

void Envelope::setS(mfloat s) {
	
	if (s < -1)
		s = -1;
	else
	if (s > 1)
		s = 1;
		
	this->s = s;
	parameters.s = s;
}

void Envelope::setR(mfloat r) {
	
	this->r = param2IncVal(r);
	parameters.r = r;
}

void Envelope::setADSR(mfloat a, mfloat d, mfloat s, mfloat r) {
	
	setA(a);
	setD(d);
	setS(s);
	setR(r);
}

void Envelope::reset() {
	
	setADSR(-1.0f, -1.0f, 1.0f, -1.0f);
	out = -1;
	point = 4;
	hold = 0;
	triggered = 0;
}

void Envelope::trigger() {

	triggered = 1;
	hold = 1;
	point = 0;
	out = -1;
}

void Envelope::release() {
	
	hold = 0;
	point = 3;
}

void Envelope::process() {
	
	if (point == 0) {
		
		out += a * samplesPerTick;
		if (out >= 1) {
			
			out = 1;
			point++;
		}
	} else
	if (point == 1) {
		
		out -= d * samplesPerTick;
		if (out <= s) {
			
			out = s;
			point++;
		}
	} else
	if (point == 2) {
		
		out = s;
	} else
	if (point == 3) {
		
		out -= r * samplesPerTick;
		if (out <= -1) {
			
			out = -1;
			point++;
		}
	} else
	if (point == 4) {
		
		out = -1;
	}
}

mfloat Envelope::getValue() {
	
	return out;
}

void *Envelope::getParameters(int *size) {

	void *p;
	
	*size = sizeof(parameters);
	p = malloc(sizeof(parameters));
	memcpy(p, &parameters, sizeof(parameters));

	return p;
}

void Envelope::setParameters(void *p) {
	
	memcpy(&parameters, p, sizeof(parameters));
	setADSR(parameters.a, parameters.d, parameters.s, parameters.r);
}

mfloat Envelope::getA() {
	
	return parameters.a;
}

mfloat Envelope::getD() {
	
	return parameters.d;
}
mfloat Envelope::getS() {
	
	return parameters.s;
}
mfloat Envelope::getR() {
	
	return parameters.r;
}

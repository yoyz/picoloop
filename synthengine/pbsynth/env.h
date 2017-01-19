#ifndef __ENV_H__
#define __ENV_H__

#include "mytypes.h"

class Envelope {
	
public:
	Envelope(int samplesPerTick);
	~Envelope();
	void trigger();
	void release();
	void process();
	void reset();
	void setA(mfloat a);
	void setD(mfloat d);
	void setS(mfloat s);
	void setR(mfloat r);
	mfloat getA();
	mfloat getD();
	mfloat getS();
	mfloat getR();
	void setADSR(mfloat a, mfloat d, mfloat s, mfloat r);
	mfloat getValue();
	void *getParameters(int *size);
	void setParameters(void *p);

protected:
	mfloat param2IncVal(mfloat x);
	
	struct {

		mfloat a, d, s, r;
	} parameters;

	mfloat a, d, s, r;
	int samplesPerTick;
	mfloat out;
	int triggered, hold;
	int point;
};

#endif

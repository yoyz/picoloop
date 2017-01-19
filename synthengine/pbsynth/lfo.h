#ifndef __LFO_H__
#define __LFO_H__

#include "mytypes.h"

class LFO {
	
public:
	LFO(int samplesPerTick);
	~LFO();
	void reset();
	void process();
	void setRate(mfloat rate);
	mfloat getRate();
	mfloat getValue();
	void *getParameters(int *size);
	void setParameters(void *p);

protected:
	struct {
		
		mfloat rate;
	} parameters;

	mfloat param2IncVal(mfloat x);
	
	mfloat incVal, out;
	int samplesPerTick, inc;
};

#endif

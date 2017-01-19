#ifndef __OSC_H__
#define __OSC_H__

#include "mytypes.h"

#define OSC_RECTANGLE 0
#define OSC_TRIANGLE 1
#define OSC_SAWTOOTH 2

class Oscillator {

public:
	Oscillator();
	~Oscillator();
	void process(mfloat *in, mfloat *out, int n, int owrite);
	void setNote(int note);
	void reset();
	void setAmp(mfloat amp);
	void setDetune(long detune);
	void setWave(int wave);
	void setPulseWidth(mfloat pw);
	int getWave();
	long getDetune();
	void *getParameters(int *size);
	void setParameters(void *p);

protected:
	struct {
		
		int wave;
		long detune;
		mfloat pulsewidth, amp;
	} parameters;

	int note, sqc;
	mfloat wlength, /* in samples */
					counter,
					sqp, oval, sawInc;

private:
	void processRectangle(mfloat *in, mfloat *out, int n, int owrite);
	void processSawtooth(mfloat *in, mfloat *out, int n, int owrite);
	void processTriangle(mfloat *in, mfloat *out, int n, int owrite);
};

#endif

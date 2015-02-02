#ifndef __SYNTH_H__
#define __SYNTH_H__
#include "fixed.h"
#include "mytypes.h"
#include "filter.h"
#include "osc.h"
#include "env.h"
#include "lfo.h"
#include "echo.h"

enum SynthEngineMod {

	SENGINE_LFO2_TO_CUTOFF,
	SENGINE_LFO1_TO_AMP,
	SENGINE_ENV1_TO_OSC1PW,
	SENGINE_ENV1_TO_OSC2PW,
	SENGINE_OSCMIX,
	SENGINE_AMPLEVEL,
	SENGINE_FILTFREQ,
	SENGINE_FILTRESO,
	SENGINE_ENV2_TO_CUTOFF
};

class SynthEngine {

public:
	SynthEngine(int samplesPerTick, long max_delaylen);
	~SynthEngine();
	void init();
	void setParameter(SynthEngineMod i, mfloat value);
	mfloat getParameter(SynthEngineMod i);
	void setPBSynthFilter24dB(int i);
	int getPBSynthFilter24dB();
	void process(mfloat *x, int l);
	void triggerNote(int note);
	void triggerNoteOsc(int osc, int note);
	void releaseNote();
	int loadProgram(char *filename);
	int saveProgram(char *filename);
	PBSynthOscillator *getPBSynthOscillator(int i);
	Envelope *getEnvelope(int i);
	PBSynthFilter *getPBSynthFilter(int i);
	LFO *getLFO(int i);
	Echo *getEcho();
	void *getParameters(int *size);
	void setParameters(void *p);
	void reset();

private:
	PBSynthOscillator *oscillators[2];
	Envelope *envelopes[2];
	PBSynthFilter *filters[2];
	LFO *lfos[2];
	Echo *echo;

	struct {

		mfloat amp_level;
		mfloat filter_freq;
		mfloat filter_reso;
		mfloat filter_env2amt;
		mfloat osc_mix;
		mfloat osc1_pw_env1mod;
		mfloat osc2_pw_env1mod;
		mfloat lfo1_ampamt;
		mfloat lfo2_filteramt;
		int filter_24db;
	} parameters;

	int granu_counter;
	int samplesPerTick;
	int base;
	int mdl;
};

#endif

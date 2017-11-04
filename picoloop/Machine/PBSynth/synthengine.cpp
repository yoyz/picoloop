#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "synthengine.h"
#include "archdep.h"

SynthEngine::SynthEngine(int samplesPerTick, long max_delaylen) {

  mdl=max_delaylen;
  granu_counter = samplesPerTick;
  this->samplesPerTick = samplesPerTick;

  base = 3;

  //granu_counter = samplesPerTick;
  //this->samplesPerTick = samplesPerTick;
  /*
  echo = new Echo(max_delaylen);
  lfos[0] = new LFO(samplesPerTick);
  lfos[1] = new LFO(samplesPerTick);
  envelopes[0] = new Envelope(samplesPerTick);
  envelopes[1] = new Envelope(samplesPerTick);
  oscillators[0] = new PBSynthOscillator();
  oscillators[1] = new PBSynthOscillator();
  filters[0] = new PBSynthFilter();
  filters[1] = new PBSynthFilter();
  
  reset();
  */

}

SynthEngine::~SynthEngine() {

  delete lfos[0];
  delete lfos[1];
  delete envelopes[0];
  delete envelopes[1];
  delete oscillators[0];
  delete oscillators[1];
  delete filters[0];
  delete filters[1];
  delete echo;
}

void SynthEngine::init()
{
  base = 3;
  
  granu_counter = samplesPerTick;
  //this->samplesPerTick = samplesPerTick;

  echo = new Echo(mdl);
  lfos[0] = new LFO(samplesPerTick);
  lfos[1] = new LFO(samplesPerTick);
  envelopes[0] = new Envelope(samplesPerTick);
  envelopes[1] = new Envelope(samplesPerTick);
  oscillators[0] = new PBSynthOscillator();
  oscillators[1] = new PBSynthOscillator();
  filters[0] = new PBSynthFilter();
  filters[1] = new PBSynthFilter();

  echo->init();

  reset();
}

PBSynthOscillator *SynthEngine::getPBSynthOscillator(int i) {

	if (i < 0 || i > 1)
		return 0;
	else
		return oscillators[i];
}

Envelope *SynthEngine::getEnvelope(int i) {

	if (i < 0 || i > 1)
		return 0;
	else
		return envelopes[i];
}

PBSynthFilter *SynthEngine::getPBSynthFilter(int i) {

	if (i < 0 || i > 1)
		return 0;
	else
		return filters[i];
}

LFO *SynthEngine::getLFO(int i) {

	if (i < 0 || i > 1)
		return 0;
	else
		return lfos[i];
}

Echo *SynthEngine::getEcho() {

	return echo;
}

int SynthEngine::getPBSynthFilter24dB() {

	return parameters.filter_24db;
}

void SynthEngine::setPBSynthFilter24dB(int i) {

	parameters.filter_24db = i != 0;
}

mfloat SynthEngine::getParameter(SynthEngineMod i) {

	mfloat t;

	t = 0;

	switch (i) {

		case SENGINE_LFO2_TO_CUTOFF:
			return parameters.lfo2_filteramt;
			break;
		case SENGINE_LFO1_TO_AMP:
			return parameters.lfo1_ampamt;
			break;
		case SENGINE_ENV1_TO_OSC1PW:
			return parameters.osc1_pw_env1mod;
			break;
		case SENGINE_ENV1_TO_OSC2PW:
			return parameters.osc2_pw_env1mod;
			break;
		case SENGINE_OSCMIX:
			return parameters.osc_mix;
			break;
		case SENGINE_AMPLEVEL:
			return parameters.amp_level;
			break;
		case SENGINE_FILTFREQ:
			return parameters.filter_freq;
			break;
		case SENGINE_FILTRESO:
			return parameters.filter_reso;
			break;
		case SENGINE_ENV2_TO_CUTOFF:
			return parameters.filter_env2amt;
			break;

		default:
			t = 0;
			return t;
			break;
	}
}

void SynthEngine::setParameter(SynthEngineMod i, mfloat value) {

	switch (i) {

		case SENGINE_LFO2_TO_CUTOFF:
			parameters.lfo2_filteramt = value;
			break;
		case SENGINE_LFO1_TO_AMP:
			parameters.lfo1_ampamt = value;
			break;
		case SENGINE_ENV1_TO_OSC1PW:
			parameters.osc1_pw_env1mod = value;
			break;
		case SENGINE_ENV1_TO_OSC2PW:
			parameters.osc2_pw_env1mod = value;
			break;
		case SENGINE_OSCMIX:
			parameters.osc_mix = value;
			break;
		case SENGINE_AMPLEVEL:
			parameters.amp_level = value;
			break;
		case SENGINE_FILTFREQ:
			parameters.filter_freq = value;
			break;
		case SENGINE_FILTRESO:
			parameters.filter_reso = value;
			break;
		case SENGINE_ENV2_TO_CUTOFF:
			parameters.filter_env2amt = value;
			break;

		default:
			return;
			break;
	}
}

void SynthEngine::process(mfloat *x, int l) {

	int o = 0;

	while (l > 0) {

		int c;
		mfloat t1, t2, t3;

		envelopes[0]->process();
		envelopes[1]->process();

		if (granu_counter <= l)
			c = granu_counter;
		else
			c = l;

		oscillators[0]->setPulseWidth(envelopes[0]->getValue() * parameters.osc1_pw_env1mod);
		oscillators[1]->setPulseWidth(envelopes[0]->getValue() * parameters.osc2_pw_env1mod);

		t1 = envelopes[0]->getValue() + 1;
		MFLOAT_HALF(t1);
		t2 = parameters.lfo1_ampamt * lfos[0]->getValue();
		t3 = (t1 + (t1 * t2)) * parameters.amp_level;

		oscillators[0]->setAmp(t3 * (1 - parameters.osc_mix));
		oscillators[1]->setAmp(t3 * parameters.osc_mix);

		t1 = (envelopes[1]->getValue() * parameters.filter_env2amt) +
		      parameters.filter_freq + (parameters.lfo2_filteramt * lfos[1]->getValue());
		filters[0]->setCutoff(t1, parameters.filter_reso);
		filters[1]->setCutoff(t1, parameters.filter_reso);

		oscillators[0]->process(&x[o], &x[o], c, 1);
		oscillators[1]->process(&x[o], &x[o], c, 0);

		filters[0]->process(&x[o], &x[o], c);
		if (parameters.filter_24db)
			filters[1]->process(&x[o], &x[o], c);

		echo->process(&x[o], &x[o], c);

		lfos[0]->process();
		lfos[1]->process();

		l -= c;
		o += c;
		granu_counter -= c;

		if (granu_counter <= 0)
			granu_counter = samplesPerTick;
	}
}

void SynthEngine::triggerNote(int note) {

	oscillators[0]->setNote(note);
	oscillators[1]->setNote(note);
	envelopes[0]->trigger();
	envelopes[1]->trigger();
}

void SynthEngine::triggerNoteOsc(int osc, int note) {

	oscillators[osc]->setNote(note);
	envelopes[osc]->trigger();
}

void SynthEngine::releaseNote() {

	envelopes[0]->release();
	envelopes[1]->release();
}

int SynthEngine::loadProgram(char *filename) {

	FILE *fh;
	int size;
	void *parameters;

	if (!(fh = fopen(filename, "rb")))
		return 1;

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	oscillators[0]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	oscillators[1]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	envelopes[0]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	envelopes[1]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	filters[0]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	filters[1]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	lfos[0]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	lfos[1]->setParameters(parameters);
	free(parameters);

	fread(&size, sizeof(size), 1, fh);
	parameters = malloc(size);
	fread(parameters, size, 1, fh);
	echo->setParameters(parameters);
	free(parameters);

	// fread(&arpMode, sizeof(arpMode), 1, fh);
	// fread(&arpSpeed, sizeof(arpSpeed), 1, fh);
	// fread(&arpOctaveMax, sizeof(arpOctaveMax), 1, fh);

	// fread(&oscBase[0], sizeof(oscBase), 1, fh);
	// fread(&note[0], sizeof(note), 1, fh);

	fclose(fh);

	return 0;
}

int SynthEngine::saveProgram(char *filename) {

	FILE *fh;
	int size;
	void *parameters;

	if (!(fh = fopen(filename, "wb")))
		return 1;

	parameters = getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = oscillators[0]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = oscillators[1]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = envelopes[0]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = envelopes[1]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = filters[0]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = filters[1]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = lfos[0]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = lfos[1]->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	parameters = echo->getParameters(&size);
	fwrite(&size, sizeof(size), 1, fh);
	fwrite(parameters, size, 1, fh);
	free(parameters);

	// fwrite(&arpMode, sizeof(arpMode), 1, fh);
	// fwrite(&arpSpeed, sizeof(arpSpeed), 1, fh);
	// fwrite(&arpOctaveMax, sizeof(arpOctaveMax), 1, fh);

	// fwrite(&oscBase[0], sizeof(oscBase), 1, fh);
	// fwrite(&note[0], sizeof(note), 1, fh);

	fclose(fh);

	return 0;
}

void *SynthEngine::getParameters(int *size) {

	void *p;

	*size = sizeof(parameters);
	p = malloc(sizeof(parameters));
	memcpy(p, &parameters, sizeof(parameters));

	return p;
}

void SynthEngine::setParameters(void *p) {

	memcpy(&parameters, p, sizeof(parameters));
}

void SynthEngine::reset() {

	setParameter(SENGINE_LFO2_TO_CUTOFF, 0.0f);
	setParameter(SENGINE_LFO1_TO_AMP, 0.0f);
	setParameter(SENGINE_ENV1_TO_OSC1PW, 0.0f);
	setParameter(SENGINE_ENV1_TO_OSC2PW, 0.0f);
	setParameter(SENGINE_OSCMIX, 0.5f);
	setParameter(SENGINE_AMPLEVEL, 1.0f);
	setParameter(SENGINE_FILTFREQ, 1.0f);
	setParameter(SENGINE_FILTRESO, -1.0f);
	setParameter(SENGINE_ENV2_TO_CUTOFF, 0.0f);
	setPBSynthFilter24dB(0);

	oscillators[0]->reset();
	oscillators[1]->reset();
	envelopes[0]->reset();
	envelopes[1]->reset();
	filters[0]->reset();
	filters[1]->reset();
	lfos[0]->reset();
	lfos[1]->reset();
	echo->reset();
}

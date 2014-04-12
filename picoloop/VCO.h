#ifndef __VCO____
#define __VCO____

#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
#include "PulseOscillator.h"
#include "WaveTableSineOscillator.h"
#include "NoiseOscillator.h"

class VCO
{
public:
  VCO();
  ~VCO();

  int checkSevenBitBoundarie(int val);
  void setSynthFreq(int sfreq);
  void setOscillator(int oscillator_number,int oscillator_type);
  /*
  void setSineOsc();
  void setSawOsc();
  void setFuzzyPulseOsc();
  */

  

  void                  reset();
  Sint16                tick();
  void                  init();
  void                  setVCOMix(int mix);
  Oscillator          * getOscillatorOne();

  SawOscillator           sawOsc1;
  SawOscillator           sawOsc2;
  SineOscillator          sineOsc1;
  SineOscillator          sineOsc2;
  FuzzyPulseOscillator    fuzzyPulseOsc1;
  FuzzyPulseOscillator    fuzzyPulseOsc2;
  PulseOscillator         pulseOsc1;
  PulseOscillator         pulseOsc2;
  NoiseOscillator         noiseOsc1;
  NoiseOscillator         noiseOsc2;
  WaveTableSineOscillator waveTableSineOsc1;
  WaveTableSineOscillator waveTableSineOsc2;

  //NoiseOscillator       noiseosc;
  Oscillator            * s1;
  Oscillator            * s2;

  int freq;
  int current;
  int vcomix;
};

#endif



#ifndef __VCO____
#define __VCO____

#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
#include "PulseOscillator.h"
//#include "NoiseOscillator.h"

class VCO
{
public:
  VCO();
  ~VCO();
  
  void setSynthFreq(int sfreq);
  
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

  SawOscillator         sawosc;
  SineOscillator        sineosc;
  FuzzyPulseOscillator  fuzzypulseosc;
  PulseOscillator       pulseosc;
  Oscillator          * s1;
  Oscillator          * s2;

  int freq;
  int current;
  int vcomix;
};

#endif



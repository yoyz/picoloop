#ifndef __VCO____
#define __VCO____

#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"

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
  Oscillator          * getOscillatorOne();

  SawOscillator         sawosc;
  SineOscillator        sineosc;
  FuzzyPulseOscillator  fuzzypulseosc;
  Oscillator          * s1;
  Oscillator          * s2;

  int freq;
  int current;
};

#endif



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

  SawOscillator         sawosc;
  SineOscillator        sineosc;
  FuzzyPulseOscillator  fuzzypulseosc;
  PulseOscillator       pulseosc;
  //NoiseOscillator       noiseosc;
  Oscillator          * s1;
  Oscillator          * s2;

  int freq;
  int current;
  int vcomix;
};

#endif



using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"
//#include "AudioEngine.h"
#include "Oscillator.h"

#ifndef __FUZZYPULSEOSCILLATOR__
#define __FUZZYPULSEOSCILLATOR__

class FuzzyPulseOscillator : public Oscillator
{
 public:
  FuzzyPulseOscillator();
  ~FuzzyPulseOscillator();
  //  void   setFreq(int freq);
  //  void   setAmplitude(int amplitude);
  Sint16 tick();
 private:
  int old_phase;
  int new_phase;
  
};

#endif

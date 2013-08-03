using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"
#include "AudioEngine.h"
#include "Oscillator.h"

#ifndef __NOISEOSCILLATOR__
#define __NOISEOSCILLATOR__

class NoiseOscillator : public Oscillator
{
 public:
  NoiseOscillator();
  ~NoiseOscillator();
  //  void   setFreq(int freq);
  //  void   setAmplitude(int amplitude);
  Sint16 tick();
 private:
  int old_phase;
  int new_phase;
  
};

#endif

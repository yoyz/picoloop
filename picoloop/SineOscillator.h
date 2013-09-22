using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"
//#include "AudioEngine.h"
#include "Oscillator.h"

#ifndef __SINEOSCILLATOR__
#define __SINEOSCILLATOR__
 
class SineOscillator : public Oscillator
{
 public:
  SineOscillator();
  ~SineOscillator();
  //  void   setFreq(int freq);
  //  void   setAmplitude(int amplitude);
  Sint16 tick();
  Sint16 * table;
  //  int    table_fill;
 
 
};

#endif

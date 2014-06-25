using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"
//#include "AudioEngine.h"
#include "Oscillator.h"

#ifndef __SAWOSCILLATOR__
#define __SAWOSCILLATOR__

class SawOscillator : public Oscillator
{
 public:
  SawOscillator();
  ~SawOscillator();
  //  void   setFreq(int freq);
  //  void   setAmplitude(int amplitude);
  Sint16 * table;
  void     init();
  Sint16   tick();
  //  int      phase;  
  int      table_size;
  
};

#endif

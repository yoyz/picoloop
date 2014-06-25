using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"
//#include "AudioEngine.h"
#include "Oscillator.h"

#ifndef __PULSEOSCILLATOR__
#define __PULSEOSCILLATOR__

class PulseOscillator : public Oscillator
{
 public:
  PulseOscillator();
  ~PulseOscillator();
  //  void   setFreq(int freq);
  //  void   setAmplitude(int amplitude);
  Sint16 * table;
  void     init();
  Sint16   tick();


  int      table_size;
  //int      index;
 
};

#endif

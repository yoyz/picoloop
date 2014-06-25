using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"
//#include "AudioEngine.h"
#include "Oscillator.h"

#ifndef __TRIANGLEOSCILLATOR__
#define __TRIANGLEOSCILLATOR__

class TriangleOscillator : public Oscillator
{
 public:
  TriangleOscillator();
  ~TriangleOscillator();
  //  void   setFreq(int freq);
  //  void   setAmplitude(int amplitude);
  Sint16 * table;
  void     init();
  Sint16   tick();
  //  int      phase;  
  int      table_size;
  //  int      index;
  
};

#endif

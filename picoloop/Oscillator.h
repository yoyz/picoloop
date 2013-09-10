using namespace std;

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <SDL/SDL.h>

#include "PatternElement.h"

#ifndef __OSCILLATOR__
#define __OSCILLATOR__

class Oscillator
{
 public:
  Oscillator();
  ~Oscillator();
  void   setAmplitude(int amp);
  void   setFreq(int freq);
  void   reset();
  //virtual Sint16 tick();
  
  virtual Sint16 tick()
  {
    Sint16 trash;
    printf("Sint16 Oscillator::tick() freq:%d amp:%d\n",frequency,amplitude);
    //  exit(0);
    //return super.tick();
    //return trash;
  }
  


  //Sint16* buffer;
  Sint16 last_tick;
  int    length;
  int    amplitude;
  int    frequency;
  int    bit;
  int    sample_num;

   
};

#endif


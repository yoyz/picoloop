using namespace std;

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <SDL/SDL.h>

#include "PatternElement.h"
#include "Master.h"
#ifndef __OSCILLATOR__
#define __OSCILLATOR__

class Oscillator
{
 public:
  Oscillator();
  ~Oscillator();
  void   setAmplitude(int amp);
  void   reset();
  void   setFreq(int freq);
  int    getFreq();

  
  virtual Sint16 tick();



  Sint16 last_tick;
  int    length;
  int    amplitude;
  int    frequency;
  int    bit;
  int    sample_num;
  int    sample_num_index;
  int    table_fill;
  int    table_size;
  Uint16 u;
  Uint16 last_u;
  Sint16 s;
  
};

#endif


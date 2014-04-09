

using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"

#include "Oscillator.h"


#ifndef __WAVETABLESINEOSCILLATOR__
#define __WAVETABLESINEOSCILLATOR__


class WaveTableSineOscillator : public Oscillator
{
 public:
  WaveTableSineOscillator();
  ~WaveTableSineOscillator();
  void   init();
  Sint16 tick();
  Sint16 * table;
  int    table_size;
  int    index;
};

#endif

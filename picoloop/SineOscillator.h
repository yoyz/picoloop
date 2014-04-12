

using namespace std;

#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"

#include "Oscillator.h"


#ifndef __SINEOSCILLATOR__
#define __SINEOSCILLATOR__


class SineOscillator : public Oscillator
{
 public:
  SineOscillator();
  ~SineOscillator();
  void   init();
  Sint16 tick();
  Sint16 * table;
  int    table_size;
  int    index;
};

#endif

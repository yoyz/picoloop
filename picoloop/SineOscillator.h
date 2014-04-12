

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
  Sint16 * table;
  void     init();
  Sint16   tick();
  int      table_size;
  int      index;
};

#endif

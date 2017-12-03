#include <string>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "Wave.h"

#include "Oscillator.h"


#ifndef __ONEOSCILLATOR__
#define __ONEOSCILLATOR__


class OneOscillator : public Oscillator
{
 public:
  OneOscillator();
  ~OneOscillator();
  Sint16 * table;
  void     init();
  Sint16   tick();
  int      table_size;
  //int      index;
};

#endif

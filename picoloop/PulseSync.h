#ifndef __PULSESYNC____
#define __PULSESYNC____
#include "Master.h"

class PulseSync
{
public:
  PulseSync();

  int setNbTickBeforeStepChange(int val);
  int tick();

  int nb_tick_before_step_change;
  int nb_tick;
  int nb_tick_before_step_change_real;
  int tick_length_high;
  int tick_length_low;
  int tick_height_high;
  int tick_height_low;
  int tick_height_std;
  int step;

  int out;
  int last_out;
  int stepdec1;
  int stepdec2;
};


#endif

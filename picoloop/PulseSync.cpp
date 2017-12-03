#include "PulseSync.h"

PulseSync::PulseSync()
{
  nb_tick=0;
  nb_tick_before_step_change=0;
  nb_tick_before_step_change_real=0;

  tick_length_high=3;
  tick_length_low=tick_length_high*2;

  tick_length_high=3;
  tick_length_low=3;
  tick_length_high=128;
  tick_length_low=nb_tick_before_step_change_real-128;



  tick_height_std=0;

  //tick_height_low=-8000;
  // tick_height_high=20000;
  // tick_height_low=-20000;

  tick_height_high=18000;
  tick_height_low=-18000;

  tick_height_high=32000;
  tick_height_low=-32000;



  step=0;
}

int PulseSync::setNbTickBeforeStepChange(int val)
{
  nb_tick_before_step_change=val;
  nb_tick_before_step_change_real=val*2;
  tick_length_high=nb_tick_before_step_change_real/2;
  tick_length_high=nb_tick_before_step_change_real/2;
  stepdec1=tick_height_high/tick_length_high;

  // tick_length_low=nb_tick_before_step_change_real-(nb_tick_before_step_change_real/8);
  // stepdec2=tick_length_low/(nb_tick_before_step_change_real/16);

  tick_length_high=(nb_tick_before_step_change_real*1)/2;
  tick_length_low=((nb_tick_before_step_change_real*1)/4)+tick_length_high;

  stepdec2=tick_height_high/(nb_tick_before_step_change_real/3);
  //tick_length_high=nb_tick_before_step_change_real/2;
  return 0;
}

int PulseSync::tick() // volca
{
  //Sint16 out=tick_height_std;
  //Sint16 last=128;
  nb_tick++;
  //printf("nb_tick:%d\n tick_length_high:%d tick_length_low:%d\n",	 nb_tick,	 tick_length_high, 	 tick_length_low);
  if (nb_tick<tick_length_high)
    {  
      out=tick_height_high;
      last_out=out;
    }
    
    if (nb_tick > tick_length_high &&
	nb_tick < tick_length_low)
      {
	out=last_out-stepdec2;
	last_out=out;
      }

    if (nb_tick > tick_length_low)
      {
	out=tick_height_low;
	last_out=out;
      }

        
    if (nb_tick>nb_tick_before_step_change_real)
      {
	DPRINTF("STEP CHANGE\n");
	//nb_tick=0;
	step++;
	if (step>=2)
	  {
	    step=0;
	    nb_tick=0;
	  }
      }

  return out;
}



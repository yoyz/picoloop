#include <stdlib.h>
#include "Machine.h"

#ifndef __TWEAKABLEKNOB__
#define __TWEAKABLEKNOB__

#define NBELEM_TWEAKABLEKNOB 512

class TweakableKnob
{  
public:
  TweakableKnob();
  ~TweakableKnob();

  void init();
  void reset();



  void set(int machineParam,int value);
  void setAll(int machineParam,int value);


  int get(int machineParam);
  int getAll(int machineParam);

  int getNonZero();
  int getAllNonZero();

 private:
  int * machineParamArray;
  int * machineParamArrayAll;

  /* int bpm;               // change from -10 to +10 */
  /* int divider;           // divider - => /1 /2 /4 /8  ; divider + => /8 /4 /2 /1 */
  /* int swing; */
  /* int invert_trig; */



  /* int note; */
  /* int note_all; */
  
  /* int amp;                  // variation of the amp of monomixer */
  /* int amp_all;              // variation of the amp of monomixer */
  
  /* int attack_amp; */
  /* int decay_amp; */
  /* int sustain_amp; */
  /* int release_amp; */
  
  /* int attack_fltr; */
  /* int decay_fltr; */
  /* int sustain_fltr; */
  /* int release_fltr; */
  
  
  /* int attack_amp_all; */
  /* int decay_amp_all; */
  /* int sustain_amp_all; */
  /* int release_amp_all; */
  
  /* int attack_fltr_all; */
  /* int decay_fltr_all; */
  /* int sustain_fltr_all; */
  /* int release_fltr_all; */
  
  
  /* int pshift_all; */
  /* int plength_all; */
  
  /* int trig_time; */
  /* int trig_time_all; */
  
  
  /* int lfo_depth; */
  /* int lfo_depth_all; */
  /* int lfo_speed; */
  /* int lfo_speed_all; */
  
  /* int pb_depth; */
  /* int pb_depth_all; */
  /* int pb_speed; */
  /* int pb_speed_all; */
  
  /* int lfo_type; */
  /* int lfo_type_all; */
  
  /* int cutoff; */
  /* int resonance; */
  
  /* int cutoff_all; */
  /* int resonance_all; */
  
  /* int fx_depth; */
  /* int fx_speed; */
  
  /* int fx_depth_all; */
  /* int fx_speed_all; */
  
  /* int vcomix; */
  /* int vcomix_all; */
  
  /* int osconetype; */
  /* int osctwotype; */
  
  /* int osconetype_all; */
  /* int osctwotype_all; */
  
  /* int machine_type; */
  /* int machine_type_all; */
  
  /* int phase_osc1; */
  /* int phase_osc1_all; */
  
  /* int filter_algo; */
  /* int filter_algo_all; */
  
  /* int filter_type; */
  /* int filter_type_all; */
  
  /* int osc1_amp; */
  /* int osc1_amp_all; */
  
  /* int osc2_amp; */
  /* int osc2_amp_all; */
  
  /* int fmtype; */
  /* int fmtype_all; */
    
};

#endif

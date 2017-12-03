#include "../../Machine.h"
#include "synthengine.h"
#include "fixed.h"
#include "../../NoteFreq.h"
#include "../../fixed.h"



#ifndef __PBSYNTHMACHINE____
#define __PBSYNTHMACHINE____

class PBSynthMachine : public Machine
{
 public:
  PBSynthMachine();
  ~PBSynthMachine();

  void init();
  void reset();
  Sint32  tick();
  Fixed  tick_fixed();
  Fixed  * tick_fixed_buffer();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  int checkI(int what,int   val);
  //Biquad & getBiquad();

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:
  //Hiopl                 * HO;
  SynthEngine           * SE;

  //Biquad                bq;
  //PBSynthFilter                filter;

  int                   cutoff;
  int                   resonance;


  Sint32                sample_num;
  Sint32                last_sample;

  Sint16              * tanh_table;
  mfloat              * buffer_f;
  Sint16              * buffer_i;
  Fixed               * buffer_fix;
  int                   index;

  //int                   freq;
  float                 freq;
  int                   keyon;

  //SinePBSynthOscillator          sineLfoOsc1;
  //PBSynthOscillator          sineLfoOsc1;

  int lfo_depth;
  int lfo_depth_shift;

  float lfo_speed;

  int note;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;


  int                   osc1_scale;
  int                   osc2_scale;

  int                   osc1_detune;
  int                   osc2_detune;


};

#endif
  

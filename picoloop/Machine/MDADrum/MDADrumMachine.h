#include "Machine.h"
#include "Filter.h"
#include "NoteFreq.h"
#include "drumsynth_oop5.h"



#ifndef __MDADRUMMACHINE____
#define __MDADRUMMACHINE____

class MDADrumMachine : public Machine
{
 public:
  MDADrumMachine();
  ~MDADrumMachine();

  void init();
  void reset();
  Sint32  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  int checkI(int what,int   val);
  int checkITwoVal(int what,int   val1, int val2);
  //Biquad & getBiquad();

  const char * getMachineParamCharStar(int machineParam,int paramValue);
  const char * getMachineTwoParamCharStar(int machineParam,int paramValue1,int paramValue2);

 protected:


  //Biquad                bq;
  Filter                filter;

  int                   cutoff;
  int                   resonance;


  Sint32                sample_num;
  Sint32                last_sample;

  Sint16              * tanh_table;
  Sint16              * buffer;
  int                   index;
  
  float                 param_t;
  float                 param_o;
  float                 param_n;
  float                 param_b;

  float                 param_tune;
  float                 param_time;

  //int                   freq;
  float                 freq;
  int                   keyon;

  //SineOscillator          sineLfoOsc1;
  //Oscillator          sineLfoOsc1;

  int lfo_depth;
  int lfo_depth_shift;

  float lfo_speed;

  int note;
  int buffer_size;

  int need_note_on;
  int note_on;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;

  drumsynth             dsoop;

};

#endif
  

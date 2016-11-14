//using namespace std;

#include "../../Machine.h"
//#include "synthengine.h"
//#include "fixed.h"
//#include <SDL/SDL.h>
#include "../../NoteFreq.h"
#include "sid.h"



#ifndef __SIDSYNTHMACHINE____
#define __SIDSYNTHMACHINE____

class SIDSynthMachine : public Machine
{
 public:
  SIDSynthMachine();
  ~SIDSynthMachine();

  void init();
  void reset();
  Sint16  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  int checkI(int what,int   val);
  //Biquad & getBiquad();

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:
  //Hiopl                 * HO;
  //SynthEngine           * SE;

  //Biquad                bq;
  //SIDSynthFilter                filter;

  int                   cutoff;
  int                   resonance;


  Sint32                sample_num;
  Sint32                last_sample;

  Sint16              * tanh_table;
  float               * buffer_f;
  Sint16              * buffer_i;
  int                   index;

  SID                 * sid;
  //int                   freq;
  float                 freq;
  int                   keyon;

  //SineSIDSynthOscillator          sineLfoOsc1;
  //SIDSynthOscillator          sineLfoOsc1;

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

  int                   osc1_mod; // Pulse wave voice 1 here
  int                   osc2_mod; // Pulse wave voice 2 here

  cycle_count           delta_t;

  int                   attack;
  int                   decay;
  int                   sustain;
  int                   release;
  double   *            sid_note_frqs;
};

#endif
  

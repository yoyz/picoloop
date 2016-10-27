using namespace std;

#include "../../Machine.h"
#include "hiopl.h"

#include "../../Filter.h"
#include <SDL/SDL.h>
#include "../../NoteFreq.h"
//#include "Biquad.h"
//#include "SineOscillator.h"
//#include "dbopl/mixer.h"
//#include "dbopl/dbopl.h"
//#include "dbopl/dosbox.h"





#ifndef __DBOPLMACHINE____
#define __DBOPLMACHINE____

class dboplMachine : public Machine
{
 public:
  dboplMachine();
  ~dboplMachine();

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
  Hiopl                 * HO;

  //Biquad                bq;
  Filter                filter;

  int                   cutoff;
  int                   resonance;


  Sint32                sample_num;
  Sint32                last_sample;

  Sint16              * tanh_table;
  Sint16              * buffer;
  int                   index;

  //int                   freq;
  float                 freq;
  int                   keyon;

  //SineOscillator          sineLfoOsc1;
  Oscillator          sineLfoOsc1;

  int lfo_depth;
  int lfo_depth_shift;

  float lfo_speed;

  int note;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;


};

#endif
  

using namespace std;

#include "Machine.h"
#include "twytch_helm_engine.h"
#include "NoteFreq.h"
#ifndef __TWITCHSYNTHMACHINE____
#define __TWITCHSYNTHMACHINE____

class TwytchsynthMachine : public Machine
{
 public:
  TwytchsynthMachine();
  ~TwytchsynthMachine();

  void init();
  void reset();
  int  tick();

  void setI(int what,int val);
  void setF(int what,float val);
  int  getI(int what);
  int  checkI(int what,int val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:

  mopotwytchsynth::HelmEngine * TWE;

  int                   cutoff;
  int                   resonance;

  Sint16              * buffer_i;
  mopotwytchsynth::mopo_float    * buffer_f;

  Sint32                sample_num;
  Sint32                index;
  Sint16                last_sample;

  Sint16              * tanh_table;

  int                   note;
  int                   old_note;
  int                   note_on;
  int                   detune;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;

  int                   velocity;

  float                 f_env1_amount;
  float                 f_env2_amount;

};

#endif
  

using namespace std;

#include "Machine.h"
#include "cursynth_engine.h"
#include "NoteFreq.h"
#ifndef __CURSYNTHMACHINE____
#define __CURSYNTHMACHINE____

class CursynthMachine : public Machine
{
 public:
  CursynthMachine();
  ~CursynthMachine();

  void init();
  void reset();
  Sint16 tick();

  void setI(int what,int val);
  void setF(int what,float val);
  int  getI(int what);
  int  checkI(int what,int val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:

  mopocursynth::CursynthEngine * CSE;

  int                   cutoff;
  int                   resonance;

  Sint16              * buffer_i;
  mopocursynth::mopo_float    * buffer_f;

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

  int                   osc1_scale;
  int                   osc2_scale;

  int                   velocity;

};

#endif
  

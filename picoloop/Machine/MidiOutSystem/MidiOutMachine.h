//using namespace std;

#include "Machine.h"
#ifndef __MIDIOUTMACHINE____
#define __MIDIOUTMACHINE____

#include "MidiOutSystem.h"

class MidiOutMachine : public Machine
{
 public:
  MidiOutMachine();
  ~MidiOutMachine();

  void init();
  void reset();
  Sint16  tick();

  void setI(int what,int val);
  void setF(int what,float val);
  int  getI(int what);
  int  checkI(int what,int val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:
  
  int getCC(int machine,int cc);
  int                   midi_machine_type;
  int                   cutoff;
  int                   resonance;



  Sint32                sample_num;
  Sint16                last_sample;

  Sint16              * tanh_table;

  int                   note;
  int                   old_note;
  int                   detune;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;
  
  int                   need_to_noteOn;
  int                   need_to_noteOff;
  int                   delay_to_noteOn;
};

#endif
  

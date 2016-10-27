using namespace std;

#include "Machine.h"
#include "rosic_Open303.h"
#include "NoteFreq.h"
#ifndef __OPEN303MACHINE____
#define __OPEN303MACHINE____

class Open303Machine : public Machine
{
 public:
  Open303Machine();
  ~Open303Machine();
  
  void init();
  void reset();
  Sint16  tick();

  void setI(int what,int val);
  void setF(int what,float val);
  int  getI(int what);
  int  checkI(int what,int val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:

  rosic::Open303      * O303E;

  int                   cutoff;
  int                   resonance;

  Sint16              * buffer_i;
  //mopo::mopo_float    * buffer_f;

  Sint32                sample_num;
  Sint32                index;
  Sint16                last_sample;

  Sint16              * tanh_table;

  int                   note;
  int                   note_on;
  int                   detune;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;

  int                   velocity;
  int                   old_note;

};

#endif
  

using namespace std;

#include "Machine.h"
#include "dbopl/hiopl.h"
//#include "Biquad.h"
#include "Filter.h"
#include <SDL/SDL.h>
#include "SineOscillator.h"
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
  int  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  //Biquad & getBiquad();

 protected:
  Hiopl                 * HO;

  //Biquad                bq;
  Filter                filter;

  int                   cutoff;
  int                   resonance;


  int                   sample_num;
  Sint16                last_sample;

  Sint16              * tanh_table;
  Sint16              * buffer;
  int                   index;

  //int                   freq;
  float                 freq;
  int                   keyon;

  SineOscillator          sineLfoOsc1;

  int lfo_depth;
  int lfo_depth_shift;

  float lfo_speed;


};

#endif
  

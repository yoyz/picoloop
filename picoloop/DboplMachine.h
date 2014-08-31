using namespace std;

#include "Machine.h"
#include "dbopl/hiopl.h"
#include "Biquad.h"
#include <SDL/SDL.h>
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

  void set(int what,int val);
  int  get(int what);

  Biquad & getBiquad();

 protected:
  Hiopl                 * HO;

  Biquad                bq;

  int                   cutoff;
  int                   resonance;


  int                   sample_num;
  Sint16                last_sample;

  Sint16              * tanh_table;
  Sint16              * buffer;
  int                   index;

  int                   freq;
  int                   keyon;
};

#endif
  

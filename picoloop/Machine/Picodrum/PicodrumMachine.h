using namespace std;

#include "Machine.h"
#include "PicodrumVCO.h"
#include "PicodrumADSR.h"
#include "Filter.h"

#ifndef __PICODRUMMACHINE____
#define __PICODRUMMACHINE____

class PicodrumMachine : public Machine
{
 public:
  PicodrumMachine();
  ~PicodrumMachine();

  void init();
  void reset();
  int  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

 protected:
  
  PicodrumADSR   & getADSRAmp();
  PicodrumVCO    & getPicodrumVCO();
  Biquad & getBiquad();

  int                   cutoff;
  int                   resonance;

  PicodrumVCO           vco;
  PicodrumADSR          adsr_amp;
  Filter                filter;

  int                   sample_num;
  Sint16                last_sample;
};

#endif
  

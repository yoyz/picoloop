using namespace std;

#include "../../Machine.h"
#include "PicodrumVCO.h"
#include "PicodrumADSR.h"
#include "../../Filter.h"

#ifndef __PICODRUMMACHINE____
#define __PICODRUMMACHINE____

class PicodrumMachine : public Machine
{
 public:
  PicodrumMachine();
  ~PicodrumMachine();

  void init();
  void reset();
  Sint16  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  int checkI(int what,int   val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

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

  int                   note;
  int                   detune;

  int                   osc1_type;
  int                   osc2_type;

  int                   phase; 

};

#endif
  

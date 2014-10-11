using namespace std;

#include "Machine.h"
#include "VCO.h"
#include "ADSR.h"
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
  
  ADSR   & getADSRAmp();
  VCO    & getVCO();
  Biquad & getBiquad();

  int                   cutoff;
  int                   resonance;

  VCO                   vco;
  ADSR                  adsr_amp;
  Filter                filter;

  int                   sample_num;
  Sint16                last_sample;
};

#endif
  

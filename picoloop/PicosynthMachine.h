using namespace std;

#include "Machine.h"
#include "VCO.h"
#include "ADSR.h"
#include "Filter.h"
#include "OneOscillator.h"

#ifndef __PICOSYNTHMACHINE____
#define __PICOSYNTHMACHINE____

class PicosynthMachine : public Machine
{
 public:
  PicosynthMachine();
  ~PicosynthMachine();

  void init();
  void reset();
  int  tick();

  void set(int what,int val);
  int  get(int what);

 protected:
  
  ADSR   & getADSRAmp();
  ADSR   & getADSRFltr();
  VCO    & getVCO();

  int                   cutoff;
  int                   resonance;

  VCO                   vco;
  ADSR                  adsr_amp;
  ADSR                  adsr_fltr;
  Filter                filter;

  OneOscillator         one_osc;

  int                   sample_num;
  Sint16                last_sample;

  Sint16              * tanh_table;
};

#endif
  

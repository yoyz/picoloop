using namespace std;

#include "../../Machine.h"
#include "SampleInstrument.h"
#include "SamplePool.h"
#include "DEBSystem.h"
#include "lgptfixed.h"
#ifndef __LGPTSAMPLERMACHINE____
#define __LGPTSAMPLERMACHINE____

class LgptsamplerMachine : public Machine
{
 public:
  LgptsamplerMachine();
  ~LgptsamplerMachine();

  void init();
  void reset();
  Sint16  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  int checkI(int what,int   val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);


  DEBSystem DEB;
  SampleInstrument SI;

 protected:
  
  //PicodrumADSR   & getADSRAmp();
  //  PicodrumVCO    & getPicodrumVCO();
  //  Biquad & getBiquad();

  int                   cutoff;
  int                   resonance;

  //PicodrumVCO           vco;
  //PicodrumADSR          adsr_amp;
  //Filter                filter;

  int                   sample_num;
  Sint16                last_sample;

  int                   note;
  int                   detune;

  int                   osc1_type;
  int                   osc2_type;

  int                   phase;

  //Sint16              * buffer_lgpt;
  lgptfixed   * buffer;
  int16_t     * buffer16;

  int                   index;
  int                   afterinit;

};

#endif
  

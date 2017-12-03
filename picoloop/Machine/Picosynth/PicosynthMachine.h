#include "../../Machine.h"
#include "PicosynthVCO.h"
#include "PicosynthADSR.h"
#include "../../Filter.h"
#include "../../OneOscillator.h"

#ifndef __PICOSYNTHMACHINE____
#define __PICOSYNTHMACHINE____

class PicosynthMachine : public Machine
{
 public:
  PicosynthMachine();
  ~PicosynthMachine();

  void init();
  void reset();
  Sint32  tick();

  void setI(int what,int val);
  void setF(int what,float val);
  int  getI(int what);
  int  checkI(int what,int val);

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:
  
  PicosynthADSR   & getADSRAmp();
  PicosynthADSR   & getADSRFltr();
  PicosynthVCO    & getVCO();

  int                   cutoff;
  int                   resonance;

  PicosynthVCO          * vco;
  PicosynthADSR         * adsr_amp;
  PicosynthADSR         * adsr_fltr;
  Filter                * filter;
  OneOscillator         * one_osc;

  Sint32                sample_num;
  Sint16                last_sample;

  Sint16              * tanh_table;
  //Sint16              * buffer;
  int32_t               * buffer;
  Sint16                index;
  int                   note;
  int                   osc1_scale;
  int                   osc2_scale;
  int                   detune;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;

};

#endif
  

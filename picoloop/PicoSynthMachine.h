using namespace std;


#include "VCO.h"
/*
#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
*/

#include "ADSR.h"
#include "Biquad.h"
#include "OneOscillator.h"

#ifndef __MACHINE____
#define __MACHINE____

class Machine
{
 public:
  Machine();
  ~Machine();

  //  void setOscillator(SineOscillator NS);
  /*
  void setSynthFreq(int sfreq);
  void setSineOsc();
  void setSawOsc();
  void setFuzzyPulseOsc();
  */
  void init();
  ADSR   & getADSRAmp();
  ADSR   & getADSRFltr();
  VCO    & getVCO();
  Biquad & getBiquad();
  //  Oscillator * getOscillator();

  void reset();
  int  tick();

  //  VCO                   vco_osc;
  //  VCO                 * vco_pointer;
  VCO                   vco;
  ADSR                  adsr_amp;
  ADSR                  adsr_fltr;
  Biquad                bq;
  Biquad                bq2;

  OneOscillator         one_osc;

  int                   sample_num;
  Sint16                last_sample;

  Sint16              * tanh_table;
  /*
  Oscillator          * s;
  SawOscillator         sawosc;
  SineOscillator        sineosc;
  FuzzyPulseOscillator  fuzzypulseosc;
  ADSR                  adsr;
  //SineOscillator SINEOSC;
  //  Oscillator S;
  //  SineOscillator S;
   //SineOscillator S;
  //SineOscillator S;

  int freq;
  int current;
  */
  /*
  enum OSC 
  { 
    NOOSC,
    SINE,
    SAW,
  };
  */

};

#endif
  

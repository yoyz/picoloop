using namespace std;


#include "VCO.h"
/*
#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
*/

#include "ADSR.h"

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

  ADSR & getADSR();
  VCO  * getVCO();
  //  Oscillator * getOscillator();

  int tick();

  VCO                 * vco;
  ADSR                  adsr;
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
  

using namespace std;

#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
#include "ADSR.h"

#ifndef __MACHINE____
#define __MACHINE____

class Machine
{
 public:
  Machine();

  //  void setOscillator(SineOscillator NS);
  void setSynthFreq(int sfreq);
  void setSineOsc();
  void setSawOsc();
  void setFuzzyPulseOsc();

  ADSR & getADSR();
  Oscillator * getOscillator();

  int tick();



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
  

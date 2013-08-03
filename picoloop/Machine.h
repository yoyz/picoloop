using namespace std;

//#include "Oscillator.h"
#include "SineOscillator.h"
//#include "SawOscillator.h"
//#include "FuzzyPulseOscillator.h"


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

  int tick();


  
  //SineOscillator SINEOSC;
  //SawOscillator SAWOSC;
  //  Oscillator S;
  //  SineOscillator S;
  // SineOscillator SINEOSC;
  //  SineOscillator S;
  SineOscillator S;
  //  FuzzyPulseOscillator  S;
  int freq;
  

};

#endif
  

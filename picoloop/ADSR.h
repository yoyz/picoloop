#include <string>
#include <stdio.h>
#include <math.h>
#include "Wave.h"
#include "AudioEngine.h"
#include "Oscillator.h"
#include "SineOscillator.h"

#ifndef __ADSR__
#define __ADSR__

class ADSR
{
 public:
  ADSR();
  ~ADSR();
  void setInput(Oscillator * S);
  void setAttack(int atk);
  void setDecay(int dc);
  void setSustain(int sust);
  void setRelease(int rls);
  void setOscillator(Oscillator * osc);
  int  getPlaying();
  Sint16 tick();
  void reset();  
  int getSize();

 private:


  int attack;
  int decay;
  int sustain;
  int release;
  int sample_num;
  Oscillator * S;

  int ca;
  int cd;
  int cs;
  int cr;
  int playing;

  int size;
  float fseconds;  
};

#endif

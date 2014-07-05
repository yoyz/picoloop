#include <string>
#include <stdio.h>
#include <math.h>
//#include "Wave.h"
//#include "AudioEngine.h"
//#include "Oscillator.h"
//#include "SineOscillator.h"
#include "VCO.h"

#ifndef __ADSR__
#define __ADSR__

class ADSR
{
 public:
  ADSR();
  ~ADSR();
  //  void setInput(Oscillator * S);
  void init();
  void setInput(VCO * vcoosc);

  void setAttack(int atk);
  void setDecay(int dc);
  void setSustain(int sust);
  void setRelease(int rls);

  int getAttack();
  int getDecay();
  int getSustain();
  int getRelease();

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
  //  Oscillator * S;
  VCO * vco;

  Sint32 ca;
  Sint32 cd;
  Sint32 cs;
  Sint32 cr;

  Sint32 car;

  Sint32 ca_segment;
  Sint32 ca_next_segment;
  Sint32 cr_segment;
  Sint32 cr_next_segment;

  Sint32 ca_div;
  Sint32 cr_div;
  //Sint32 cr_div;

  int playing;

  int size_release;
  int size_attack;
  //  int size;
  float fseconds_attack;  
  float fseconds_release;  
};

#endif

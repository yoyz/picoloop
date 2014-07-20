#include <string>
#include <stdio.h>
#include <math.h>
#include "Wave.h"
#include "AudioEngine.h"
//#include "Oscillator.h"
//#include "SineOscillator.h"
#include "VCO.h"

#ifndef __ADSR__
#define __ADSR__

class ADSR : public Oscillator
{
 public:
  ADSR();
  ~ADSR();
  //  void setInput(Oscillator * S);
  void   init();
  //void   setInput(VCO * vcoosc);
  void   setInput(Oscillator * vcoosc);

  void   setAttack(int atk);
  void   setDecay(int dc);
  void   setSustain(int sust);
  void   setRelease(int rls);

  void   setNoteOn();
  void   setNoteOff();
  int    getNoteOn();
  void   setADSRRelease();

  int    getAttack();
  int    getDecay();
  int    getSustain();
  int    getRelease();
  int    setSegment(int segment);

  void   setOscillator(Oscillator * osc);
  int    getPlaying();
  Sint16 tick();
  void   reset();  
  int    getSize();

 private:


  int attack;
  int decay;
  int sustain;
  int release;
  int sample_num;

  int current_segment;
  //  Oscillator * S;
  VCO * vco;

  Sint32 ca;
  Sint32 cd;
  Sint32 cs;
  Sint32 cr;

  Sint32 cadr;

  Sint32 ca_segment;
  Sint32 ca_next_segment;

  Sint32 cr_segment;
  Sint32 cr_next_segment;

  Sint32 cd_segment;
  Sint32 cd_next_segment;

  Sint32 sustain_index_begin;

  Sint32 ca_div;
  Sint32 cd_div;
  Sint32 cr_div;
  //Sint32 cr_div;

  int noteOn_value;

  int size_release;
  int size_attack;
  //  int size;
  float fseconds_attack;  
  float fseconds_release; 

  Sint16 * tanh_table;

 
};

#endif

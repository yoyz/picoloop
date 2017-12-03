#include <string>
#include <stdio.h>
#include <math.h>
#include "../../Wave.h"
#include "../../AudioEngine.h"
#include "PicosynthVCO.h"

#ifndef __PicosynthADSR__
#define __PicosynthADSR__

class PicosynthADSR
{
 public:
  PicosynthADSR();
  ~PicosynthADSR();
  //  void setInput(Oscillator * S);
  void   init();
  //void   setInput(PicosynthVCO * vcoosc);
  void   setInput(PicosynthVCO * vcoosc);

  void   setAttack(int atk);
  void   setDecay(int dc);
  void   setSustain(int sust);
  void   setRelease(int rls);

  void   setNoteADSR(int mode);
  int    getNoteADSR();


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
  Sint16 tick_note();
  Sint16 tick_trig();
  void   reset();  
  int    getSize();

 private:

  Sint32 s_in;
  Sint32 old_s_sin;

  //int old_s_sinpole;
  int pole;
  int old_pole;

  int adsr_note;

  int attack;
  int decay;
  int sustain;
  int release;
  int sample_num;
  int sample_num_debug;

  int current_segment;
  //  Oscillator * S;
  PicosynthVCO * vco;

  Sint32 ca;
  Sint32 cd;
  Sint32 cs;
  Sint32 cr;

  Sint32 cadr;

  Sint32 ca_segment;
  Sint32 ca_next_segment;

  Sint32 cd_segment;
  Sint32 cd_next_segment;

  Sint32 cs_segment;
  Sint32 cs_next_segment;

  Sint32 cr_segment;
  Sint32 cr_next_segment;


  Sint32 sustain_index_begin;

  Sint32 ca_div;
  Sint32 cd_div;
  Sint32 cs_div;
  Sint32 cr_div;

  Sint32 ca_div_woalias;
  Sint32 cd_div_woalias;
  Sint32 cr_div_woalias;
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

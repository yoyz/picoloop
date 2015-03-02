using namespace std;

//#include "Note.h"

#include <string>
#include "Master.h"
#ifndef __PATTERNELEMENT__
#define __PATTERNELEMENT__




class PatternElement
{
public:
  PatternElement();
  ~PatternElement();

  void   init();

  const char * getMachineTypeCharStar();
  const char * getNoteCharStar();
  /* const char * getFMTypeCharStar(); */


  void   set(int what, int val);
  int    get(int what);

  string getStr();
  void   print(); 
private:
  int  checkSevenBitBoundarie(int val);
  int  note;
  int  note_on;
  int  Channel;

  int  vcomix;

  int  amp;

  int  amp_attack;
  int  amp_decay;
  int  amp_sustain;
  int  amp_release;

  int  fltr_attack;
  int  fltr_decay;
  int  fltr_sustain;
  int  fltr_release;

  int osc1_amp;
  int osc2_amp;

  int fltr_cutoff;
  int fltr_resonance;

  int lfo1_depth;
  int lfo1_speed;

  int lfo2_depth;
  int lfo2_speed;

  int pb_depth;
  int pb_speed;

  int lfo_type;

  int oscOneType;
  int oscTwoType;

  int phaseOsc1;

  int adsr_note;       //NOT USED ANYMORE NEED TO BE REMOVED

  int machine_type;

  int trig_time;

  int filterType;
  int filterAlgo;

  int fx_depth;
  int fx_speed;
  
  int fm_type;

  int lfo1_env_amount;
  int lfo2_env_amount;

  int lfo1Waveform;
  int lfo2Waveform;

  int osc1_detune;

  int velocity;
};

#endif

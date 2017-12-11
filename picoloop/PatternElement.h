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


  void   set(int what, int val);
  int    get(int what);

  void   inc(int what);
  void   dec(int what);
  std::string getStr();
  void   print(); 
private:
  int  checkSevenBitBoundarie(int val);
  int  checkThreeOctaveScale(int val);
  int  note1;
  int  note2;
  int  note3;
  int  note4;
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

  int osc3_amp;
  int osc4_amp;


  int osc1_unison;
  int osc2_unison;

  int osc1_unisondt;
  int osc2_unisondt;


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

  int oscThreeType;

  int phaseOsc1;

  int adsr_note;       //NOT USED ANYMORE NEED TO BE REMOVED

  int machine_type;

  int trig_time;

  int filterType;
  int filterAlgo;

  int filter1_saturation;
  int filter1_feedback;

  int fx_depth;
  int fx_speed;
  
  int fm_type;

  int lfo1_env_amount;
  int lfo2_env_amount;

  int lfo1Waveform;
  int lfo2Waveform;

  int osc1_detune;
  int osc2_detune;

  int velocity;

  int osc1_mod;
  int osc2_mod;

  int polyphony;

  int osc1_scale;
  int osc2_scale;
  int osc3_scale;
  int osc4_scale;

  int env1_depth;
  int env2_depth;
  int env3_depth;
  int env4_depth;

  int keytrack;
};

#endif

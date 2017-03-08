#ifndef __PicodrumVCO____
#define __PicodrumVCO____

#include "../../Oscillator.h"
#include "../../PitchBend.h"

class PicodrumVCO : public Oscillator
{
public:
  PicodrumVCO();
  ~PicodrumVCO();

  int checkSevenBitBoundarie(int val);
  void setNoteDetune(int note,int detune);
  //void setSynthFreq(float sfreq);
  void setOscillator(int oscillator_number,int oscillator_type);
  /*
  void setSineOsc();
  void setSawOsc();
  void setFuzzyPulseOsc();
  */

  

  void                  reset();
  Sint16                tick();
  void                  init();
  void                  setPicodrumVCOMix(int mix);
  void                  setPicodrumVCOPhase(int ph);
  void                  setLfoDepth(int val);
  void                  setLfoSpeed(float val);

  void                  setLfoType(int val);

  void                  setPitchBendDepth(int val);
  void                  setPitchBendSpeed(int val);

  Oscillator          * getOscillatorOne();



  Oscillator          osc1;
  Oscillator          osc2;
  Oscillator          lfoOsc1;
  
  Oscillator            * s1;
  Oscillator            * s2;
  Oscillator            * lfo1;
  
  PitchBend               pb;

  float freq;

  int freqOsc1;
  int freqOsc2;

  int                   note;
  int                   detune;


  int current;
  int vcomix;
  int phase;

  int lfo_depth;
  int lfo_depth_shift;

  int lfo_speed;

  int lfo_refresh;
  int lfo_counter;

  int pb_depth;
  int pb_speed;

  int lfo_type;
    
  int tmp_i;

  Sint16 * tanh_table;
};

#endif



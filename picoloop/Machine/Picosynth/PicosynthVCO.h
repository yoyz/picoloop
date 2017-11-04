#include "../../Oscillator.h"
#include "../../PitchBend.h"

#ifndef __PICOSYNTHVCO__
#define __PICOSYNTHVCO__


//#include "NoteFreq.h"
//#include "SineOscillator.h"
//#include "SawOscillator.h"
//#include "FuzzyPulseOscillator.h"
//#include "PulseOscillator.h"
//#include "TriangleOscillator.h"
//#include "WaveTableSineOscillator.h"
//#include "NoiseOscillator.h"

class PicosynthVCO
{
public:
  PicosynthVCO();
  ~PicosynthVCO();

  int checkSevenBitBoundarie(int val);
  //void setFSynthFreq(float ffreq);
  //void setISynthFreq(int   ifreq);
  //void setNoteDetune(int note,int detune);
  void setNoteDetune(int noteosc1,int noteosc2,int detune);
  void setOscillator(int oscillator_number,int oscillator_type);
  /*
  void setSineOsc();
  void setSawOsc();
  void setFuzzyPulseOsc();
  */

  

  void                  reset();
  Sint16                tick();
  void                  init();
  void                  setVCOMix(int mix);
  void                  setVCOPhase(int ph);
  void                  setLfoDepth(int val);
  void                  setLfoSpeed(int val);

  void                  setLfoType(int val);

  void                  setPitchBendDepth(int val);
  void                  setPitchBendSpeed(int val);



  Oscillator          * getOscillatorOne();


  /*
  SineOscillator          sineOsc1;
  SineOscillator          sineOsc2;

  SawOscillator           sawOsc1;
  SawOscillator           sawOsc2;

  PulseOscillator         pulseOsc1;
  PulseOscillator         pulseOsc2;

  TriangleOscillator      triangleOsc1;
  TriangleOscillator      triangleOsc2;

  WaveTableSineOscillator waveTableSineOsc1;
  WaveTableSineOscillator waveTableSineOsc2;

  NoiseOscillator         noiseOsc1;
  NoiseOscillator         noiseOsc2;

  SineOscillator          sineLfoOsc1;
  SawOscillator           sawLfoOsc1;
  */

  Oscillator          * osc1;
  Oscillator          * osc2;
  Oscillator          * oscLfo1;


  Oscillator            * s1; 
  Oscillator            * s2; 
  Oscillator            * lfo1;
  PitchBend             *  pb;

  int freqOsc1;
  int freqOsc2;

  int                   note;
  int                   detune;

  //int current;
  int vcomix;
  int phase;

  int lfo_depth;
  int lfo_depth_shift;
  int lfo_speed;
  int lfo_refresh;
  int lfo_counter;
  int lfo_type;    

  //int lfo_speed;

  int pb_depth;
  int pb_speed;


  //int tmp_i;
};

#endif



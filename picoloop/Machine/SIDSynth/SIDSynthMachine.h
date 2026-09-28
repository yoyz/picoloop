#include "../../Machine.h"
#include "../../NoteFreq.h"
#include "sid.h"



#ifndef __SIDSYNTHMACHINE____
#define __SIDSYNTHMACHINE____

class SIDSynthMachine : public Machine
{
 public:
  SIDSynthMachine();
  ~SIDSynthMachine();

  void init();
  void reset();
  Sint32  tick();

  void setI(int what,int   val);
  void setF(int what,float val);
  int  getI(int what);

  int checkI(int what,int   val);
  //Biquad & getBiquad();

  const char * getMachineParamCharStar(int machineParam,int paramValue);

 protected:
  //Hiopl                 * HO;
  //SynthEngine           * SE;

  //Biquad                bq;
  //SIDSynthFilter                filter;

  int                   cutoff;
  int                   resonance;


  Sint32                sample_num;
  Sint32                last_sample;

  Sint16              * tanh_table;
  float               * buffer_f;
  Sint16              * buffer_i;
  int                   index;

  SIDCHIP             * sid;
  //int                   freq;
  float                 freq;
  int                   keyon;

  //SineSIDSynthOscillator          sineLfoOsc1;
  //SIDSynthOscillator          sineLfoOsc1;

  int lfo_depth;
  int lfo_depth_shift;

  float lfo_speed;

  // Software LFO -> pitch bend/vibrato. LFO1 bends osc1, LFO2 bends osc2.
  int    lfo1_depth;   // 0..127 -> depth in semitones (0..24)
  float  lfo1_freq;    // 0..1   -> LFO rate in Hz (0..20)
  double lfo1_phase;
  int    lfo2_depth;
  float  lfo2_freq;
  double lfo2_phase;
  // Unmodulated SID FREQ register values computed at NOTE_ON; the LFO
  // writes pitch-shifted versions of these every buffer in tick().
  int    base_reg1;
  int    base_reg2;

  int note;

  int                   trig_time_mode;
  int                   trig_time_duration;
  Sint32                trig_time_duration_sample;

  int                   osc1_type;
  int                   osc2_type;


  int                   osc1_scale;
  int                   osc2_scale;

  int                   osc1_detune;
  int                   osc2_detune;

  int                   osc1_mod; // Pulse wave voice 1 here
  int                   osc2_mod; // Pulse wave voice 2 here

  // SID hard sync / ring modulation on voice 2 (osc2), which syncs/rings to
  // osc1 (voice 0 is the sync/ring source for voice 1 in reSID).
  // Driven by OSC1_PHASE (sync) and OSC2_PHASE (ring) pattern params.
  int                   osc2_sync;
  int                   osc2_ring;

  cycle_count           delta_t;

  int                   attack;
  int                   decay;
  int                   sustain;
  int                   release;
  // Second envelope generator: drives voice 2 (oscillator 2). The SID chip
  // has one ADSR per voice.
  int                   attack2;
  int                   decay2;
  int                   sustain2;
  int                   release2;
  double   *            sid_note_frqs;
};

#endif
  

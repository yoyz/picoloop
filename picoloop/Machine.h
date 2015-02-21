using namespace std;
#include <stdio.h>

#define ADSR_ENV0_ATTACK    0 
#define ADSR_ENV0_DECAY     1  
#define ADSR_ENV0_SUSTAIN   2  
#define ADSR_ENV0_RELEASE   3  

#define ADSR_AMP_ATTACK     0 
#define ADSR_AMP_DECAY      1  
#define ADSR_AMP_SUSTAIN    2  
#define ADSR_AMP_RELEASE    3  


#define ADSR_ENV1_ATTACK    4 
#define ADSR_ENV1_DECAY     5  
#define ADSR_ENV1_SUSTAIN   6  
#define ADSR_ENV1_RELEASE   7  

#define ADSR_FLTR_ATTACK    4 
#define ADSR_FLTR_DECAY     5  
#define ADSR_FLTR_SUSTAIN   6  
#define ADSR_FLTR_RELEASE   7  


#define ADSR_ENV2_ATTACK    8 
#define ADSR_ENV2_DECAY     9  
#define ADSR_ENV2_SUSTAIN   10  
#define ADSR_ENV2_RELEASE   11 

#define ADSR_ENV3_ATTACK    12
#define ADSR_ENV3_DECAY     13 
#define ADSR_ENV3_SUSTAIN   14 
#define ADSR_ENV3_RELEASE   15 

#define ADSR_ENV4_ATTACK    16
#define ADSR_ENV4_DECAY     17  
#define ADSR_ENV4_SUSTAIN   18  
#define ADSR_ENV4_RELEASE   19  

#define OSC1_TYPE           20
#define OSC2_TYPE           21
#define OSC3_TYPE           22  
#define OSC4_TYPE           23

#define OSC1_PHASE          24
#define OSC2_PHASE          25
#define OSC3_PHASE          26
#define OSC4_PHASE          27

#define OSC12_MIX           28  // mix the two oscillator
#define VCO_MIX             28  // mix the two oscillator

//#define OSC34_MIX           29
//#define OSC1234_MIX         30

//#define VCO_MIX             24

#define OSC1_AMP            31
#define OSC2_AMP            32
#define OSC3_AMP            33
#define OSC4_AMP            34

#define AMP                 35

#define LFO1_ENV_AMOUNT     38
#define LFO2_ENV_AMOUNT     39

#define LFO1_DEPTH          41
#define LFO1_FREQ           42

#define LFO2_DEPTH          43
#define LFO2_FREQ           44

#define PITCHBEND_DEPTH     45
#define PITCHBEND_SPEED     46

#define LFO_TYPE            47


#define FILTER1_TYPE        50
#define FILTER1_ALGO        51
#define FILTER1_CUTOFF      52
#define FILTER1_RESONANCE   53

#define FILTER2_TYPE        54
#define FILTER2_ALGO        55
#define FILTER2_CUTOFF      56
#define FILTER2_RESONANCE   57

#define FM_TYPE             60



#define OSC1_NOTE           70
#define OSC2_NOTE           71
#define OSC3_NOTE           72
#define OSC4_NOTE           73

#define OSC1_DETUNE         74
#define OSC2_DETUNE         75
#define OSC3_DETUNE         76
#define OSC4_DETUNE         77

#define FX1_TYPE            80
#define FX1_DEPTH           81
#define FX1_SPEED           82
#define FX1_PARAM1          83
#define FX1_PARAM2          84

#define FX2_TYPE            85
#define FX2_DEPTH           86
#define FX2_SPEED           87
#define FX2_PARAM1          88
#define FX2_PARAM2          89


/*   HERE IS THE NOT RELATED TO MACHINE STUFF     */
#define INVERT_TRIG         144  // invert the trig to on/off
#define SWING               145  // used to store the +/- of swing
#define BPM_DIVIDER         146  // used to store the +/- of bpm divider of pattern
#define BPM                 147  // used to store the +/- of bpm
#define PATTERN_LENGTH      148  // used to store the +/- of pattern size
#define PATTERN_SHIFT       149  // used to shift the +/- of the pattern shift
#define NOTE_ON             150  // Used by PatternPlayer sequencer 
                                 // external hardware/WaveSynth

#define TRIG_TIME_MODE      101  // Used by PatternPlayer to predefined the time of NOTE_ON
#define TRIG_TIME_DURATION  102  // Used by PatternPlayer to predefined the time of NOTE_ON

#define NOTE                200   // Store note value
#define MACHINE_TYPE        210   // Store the machine type dbOpl, Picosynth...
#define NOTE_ADSR           211   // Note used but still here...       //NOT USED ANYMORE NEED TO BE REMOVED

#ifndef __MACHINE____
#define __MACHINE____

class Machine
{
 public:
  Machine();
  ~Machine();

  virtual void setI(int what,int val);
  virtual void setF(int what,float val);
  virtual int  getI(int what);

  virtual void init();
  virtual void reset();
  virtual int  tick();

  virtual int  checkI(int what,int val);
  virtual const char * getMachineParamCharStar(int machineParam,int paramValue);

};

#endif
  

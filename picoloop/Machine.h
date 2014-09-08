using namespace std;
#include <stdio.h>

#define ADSR_ENV0_ATTACK    0 
#define ADSR_ENV0_DECAY     1  
#define ADSR_ENV0_SUSTAIN   2  
#define ADSR_ENV0_RELEASE   3  

#define ADSR_ENV1_ATTACK    4 
#define ADSR_ENV1_DECAY     5  
#define ADSR_ENV1_SUSTAIN   6  
#define ADSR_ENV1_RELEASE   7  

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

#define OSC12_MIX           28
#define OSC34_MIX           29
#define OSC1234_MIX         30

#define OSC1_FREQ           37
#define OSC2_FREQ           38
#define OSC3_FREQ           39
#define OSC4_FREQ           40


#define LFO1_DEPTH          41
#define LFO1_FREQ           42
#define LFO2_DEPTH          43
#define LFO2_FREQ           44


#define FILTER1_TYPE        50
#define FILTER1_ALGO        51
#define FILTER1_CUTOFF      52
#define FILTER1_RES         53

#define FILTER2_TYPE        54
#define FILTER2_ALGO        55
#define FILTER2_CUTOFF      56
#define FILTER2_RES         57



#define NOTE_ON             100


#ifndef __MACHINE____
#define __MACHINE____

class Machine
{
 public:
  Machine();
  ~Machine();

  virtual void set(int what,int val);
  virtual int  get(int what);

  virtual void init();
  virtual void reset();
  virtual int  tick();


};

#endif
  

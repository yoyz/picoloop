using namespace std;
#include <stdio.h>
#include <stdint.h>
#include "Biquad.h"
#include "AMSynthLowPassFilter.h"
#ifndef __FILTER____
#define __FILTER____


#define LOWPASS   0
#define BANDPASS  1
#define HIPASS    2

class Filter
{
 public:
  Filter();
  ~Filter();

  void    init();
  void    reset();

  void    setAlgo(int val);       // Biquad,  Moog ....
  void    setType(int val);       // LOWPASS, HIPASS
  void    setCutoff(int val);     // 0..127
  void    setResonance(int val);  // 0..127
  

  int16_t process(int16_t in);

 protected:

  int cutoff;
  int resonance;
  int type;
  int algo;
  int needCalc;

  Biquad                bq;
  AMSynthLowPassFilter  lp;
};

#endif
 

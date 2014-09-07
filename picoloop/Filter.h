using namespace std;
#include <stdio.h>
#include <stdint.h>
#include "Biquad.h"
#include "AMSynthLowPassFilter.h"
#ifndef __FILTER____
#define __FILTER____


#define TYPE_LOWPASS   0
#define TYPE_BANDPASS  1
#define TYPE_HIPASS    2

#define ALGO_NOFILTER  0
#define ALGO_BIQUAD    1
#define ALGO_AMSYNTH   2

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

  int16_t process_biquad(int16_t in);
  int16_t process_amsynth(int16_t in);

  int cutoff;
  int resonance;
  int type;
  int filterAlgo;
  int needCalc;

  Biquad                bq;
  AMSynthLowPassFilter  lp;
};

#endif
 

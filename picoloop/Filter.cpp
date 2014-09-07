#include "Filter.h"

Filter::Filter() : bq()
{
  printf("Filter::Filter()\n");  
  cutoff=120;
  resonance=10;
  needCalc=1;
}

Filter::~Filter()
{
  printf("Filter::~Filter()\n");  
}

void Filter::init()
{
  cutoff=120;
  resonance=10;
  type=LOWPASS;
  needCalc=1;
}

void Filter::reset()
{
  cutoff=120;
  resonance=10;
  type=LOWPASS;
  needCalc=1;
}


void Filter::setCutoff(int val)
{
  if (cutoff!=val)
    {
      cutoff=val;
      needCalc=1;
    }
}

void Filter::setResonance(int val)
{
  if (resonance!=val)
    {
      resonance=val;
      needCalc=1;
    }
}

void Filter::setType(int val)
{
  if (type!=val)
    {
      type=val;  
      needCalc=1;
    }
}


/*
int16_t Filter::process(int16_t in)
{
  if (needCalc)
    {
      float f_val_cutoff=cutoff;
      float f_val_resonance=resonance;
      //needCalc=1;
      bq.setBiquad(0, (f_val_cutoff/256), (f_val_resonance/8)+0.005, 0.0);
      bq.calcBiquad(); 
      needCalc=0;
    }
  return bq.process(in);
}
*/

int16_t Filter::process(int16_t in)
{
  float f_in[0];
  float f_out;
  int16_t  out;
  float f_val_cutoff=cutoff;
  float f_val_resonance=resonance;

  f_in[0]=in;
  if (needCalc)
    {
      //needCalc=1;
      //bq.setBiquad(0, (f_val_cutoff/256), (f_val_resonance/8)+0.005, 0.0);
      //bq.calcBiquad(); 
      
      needCalc=0;      
      lp.SetSampleRate(44100);
      lp.reset();
      //lp.calc(100,0.8);
      lp.calc(cutoff*256,resonance/128);
    }
  //return bq.process(in);
  //lp.ProcessSamples(f_in,1,f_val_cutoff/127,f_val_resonance/127);

  lp.ProcessSamples(f_in,1);
  f_out=f_in[0];
  out=f_out;
  return out;
}


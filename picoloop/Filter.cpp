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


#include "Filter.h"

Filter::Filter() : bq(), lp()
{
  DPRINTF("Filter::Filter()");  
  cutoff=120;
  resonance=10;
  needCalc=1;
  filterType=FILTER_TYPE_LP;
  filterAlgo=FILTER_ALGO_AMSYNTH;
  //filterAlgo=ALGO_BIQUAD;
  //filterAlgo=ALGO_NOFILTER;
}

Filter::~Filter()
{
  DPRINTF("Filter::~Filter()");  
}

void Filter::init()
{
  cutoff=120;
  resonance=10;
  filterType=FILTER_TYPE_LP;
  filterAlgo=FILTER_ALGO_AMSYNTH;
  needCalc=1;
}

void Filter::reset()
{
  //cutoff=120;
  //resonance=10;
  //type=TYPE_LOWPASS;
  //filterAlgo=ALGO_AMSYNTH;
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

void Filter::setFilterType(int val)
{
  if (filterType!=val)
    {
      this->bq.reset();
      lp.reset();
      filterType=val;  
      needCalc=1;
    }
}

void Filter::setFilterAlgo(int val)
{
  if (filterAlgo!=val)
    {
      filterAlgo=val;  
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




void Filter::process_samples(float * in, int nb_sample)
{
  if (filterAlgo==FILTER_ALGO_NOFILTER) return;
  if (filterAlgo==FILTER_ALGO_BIQUAD)   
    { 
      this->process_biquad_need_calc();
      return bq.process_samples(in,nb_sample);
    }
  if (filterAlgo==FILTER_ALGO_AMSYNTH)  
    {
      this->process_amsynth_need_calc();
      return lp.process_samples(in,nb_sample);
    }
}

void Filter::process_samples(int32_t * in, int nb_sample)
{
  if (filterAlgo==FILTER_ALGO_NOFILTER) return;
  if (filterAlgo==FILTER_ALGO_BIQUAD)   
    { 
      this->process_biquad_need_calc();
      return bq.process_samples(in,nb_sample);
    }
  if (filterAlgo==FILTER_ALGO_AMSYNTH)  
    {
      this->process_amsynth_need_calc();
      return lp.process_samples(in,nb_sample);
    }
}



int16_t Filter::process_one_sample(int16_t in)
{ 
  if (filterAlgo==FILTER_ALGO_NOFILTER)
    return in;
  if (filterAlgo==FILTER_ALGO_BIQUAD)
    return this->process_biquad_one_sample(in);
  if (filterAlgo==FILTER_ALGO_AMSYNTH)
    return this->process_amsynth_one_sample(in);
  return 0;
}


inline void Filter::process_biquad_need_calc()
{
  if (needCalc)
    {
      float f_val_cutoff=cutoff;
      float f_val_resonance=resonance;
      bq.setBiquad(filterType,
		   (f_val_cutoff/256)*(f_val_cutoff/256),
		   (f_val_resonance/8)+0.005, 0.0);
      bq.calcBiquad();       
      needCalc=0;      
    }
}

int16_t Filter::process_biquad_one_sample(int16_t in)
{
  float f_in;
  float f_out;
  int16_t  out;

  f_in=in;
  this->process_biquad_need_calc();
  return bq.process_one_sample((int32_t)in);
}



inline void Filter::process_amsynth_need_calc()
{
  if (needCalc)
    {
      float f_val_cutoff=cutoff;
      float f_val_resonance=resonance;
      needCalc=0;      
      lp.SetSampleRate(44100);
      lp.reset();
      lp.calc(((f_val_cutoff/128)*(f_val_cutoff/128))*64*128,f_val_resonance/160);
    }
}

int16_t Filter::process_amsynth_one_sample(int16_t in)
{
  float    f_in;
  float    f_out;
  int32_t  i_in;
  int      i_out;
  int16_t    out;

  f_in=in;
  i_in=in;
  this->process_amsynth_need_calc();
  i_out=lp.process_one_sample(i_in);
  out=f_out;
  out=i_out;
  
  return out;
}


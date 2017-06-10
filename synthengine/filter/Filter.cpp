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

int16_t Filter::process(int16_t in)
{
  //return this->process_amsynth(in);

  
  if (filterAlgo==FILTER_ALGO_NOFILTER)
    return in;
  if (filterAlgo==FILTER_ALGO_BIQUAD)
    return this->process_biquad(in);
  if (filterAlgo==FILTER_ALGO_AMSYNTH)
    return this->process_amsynth(in);
  
    
}


int16_t Filter::process_biquad(int16_t in)
{
  //float f_in[0];
  float f_in;
  float f_out;
  int16_t  out;
  float f_val_cutoff=cutoff;
  float f_val_resonance=resonance;

  f_in=in;
  if (needCalc)
    {
      //needCalc=1;
      //bq.setType(filterType);
      //bq.setBiquad(filterType, (f_val_cutoff/256), (f_val_resonance/8)+0.005, 0.0;)
      bq.setBiquad(filterType,
		   (f_val_cutoff/256)*(f_val_cutoff/256),
		   (f_val_resonance/8)+0.005, 0.0);
      bq.calcBiquad(); 
      
      needCalc=0;      
      //lp.SetSampleRate(44100);
      //lp.reset();
      //lp.calc(100,0.8);
      //lp.calc(cutoff*256,resonance/128);
    }
  return bq.process(in);
  //lp.ProcessSamples(f_in,1,f_val_cutoff/127,f_val_resonance/127);

  //lp.ProcessSample(f_in);
   //f_out=f_in;
  //out=f_out;
  //return out;
}




int16_t Filter::process_amsynth(int16_t in)
{
  //float f_in[0];
  float    f_in;
  float    f_out;

  int      i_in;
  int      i_out;

  float    f_val_cutoff=cutoff;
  float    f_val_resonance=resonance;
  int16_t    out;

  //f_in[0]=in;
  f_in=in;
  i_in=in;
  if (needCalc)
    {
      //needCalc=1;
      //bq.setBiquad(0, (f_val_cutoff/256), (f_val_resonance/8)+0.005, 0.0);
      //bq.calcBiquad(); 
      
      needCalc=0;      
      //lp.SetSampleRate(4100);
      lp.SetSampleRate(44100);
      lp.reset();
      //lp.calc(f_val_cutoff*256,f_val_resonance/128);
      //lp.calc(f_val_cutoff*32,f_val_resonance/160);
      lp.calc(((f_val_cutoff/128)*(f_val_cutoff/128))*64*128,f_val_resonance/160);
    }
  //return bq.process(in);
  //lp.ProcessSamples(f_in,1,f_val_cutoff/127,f_val_resonance/127);

  //f_out=lp.ProcessSample(f_in);
  i_out=lp.ProcessSample(i_in);
  //DPRINTF("f_in:%.8f f_out:%.8f\n",f_in,f_out);
  //lp.ProcessSamples(f_in,1);
  //f_out=f_in[0];
  out=f_out;
  out=i_out;
  
  return out;
}


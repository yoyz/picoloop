using namespace std;

#include "Oscillator.h"

Oscillator::Oscillator()
{
  frequency=0;
  amplitude=0;
  sample_num=0;
  last_tick=0;
}

Oscillator::~Oscillator()
{
  
}

void Oscillator::reset()
{
  if (sample_num>0)
    {
      sample_num=0;
    }
}


void Oscillator::setFreq(int freq)
{
  if (freq==frequency)
    return;

  if (freq < 0)          freq=1;
  else if (freq > 20000) freq=20000;
  frequency=freq;
  table_fill=0;
  //  printf("setFreq:%d\n",freq);
}



void Oscillator::setAmplitude(int amp)
{
  int debug=1;
  if (amp>=127) {amplitude=127; }
  if (amp<=0)   {amplitude=0;   }
  if (amp>0   && amp<127){ amplitude=amp; }
  printf("amplitude:%d\n",amplitude);
}



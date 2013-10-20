using namespace std;

#include "Oscillator.h"

Oscillator::Oscillator()
{
  frequency=0;
  amplitude=0;
  sample_num=0;
  sample_num_index=0
  last_tick=0;
  table_size=0;
}

Oscillator::~Oscillator()
{
  printf("Oscillator::~Oscillator()\n");
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

  if (freq <= 0)          freq=0;
  else if (freq > 20000) freq=20000;
  frequency=freq;
  table_fill=0;
  if (freq!=0)
    table_size=DEFAULTFREQ/freq;
  else
    table_size=0;
  //  printf("setFreq:%d\n",freq);
}



void Oscillator::setAmplitude(int amp)
{
  int debug=1;
  if (amp>=127) {amplitude=127; }
  if (amp<=0)   {amplitude=0;   }
  if (amp>0   && amp<127){ amplitude=amp; }
  printf("Oscillator::setAmplitude(amplitude=%d)\n",amplitude);
}



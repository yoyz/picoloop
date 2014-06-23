using namespace std;
#include "Oscillator.h"

Oscillator::Oscillator()
{
  frequency=0;
  amplitude=0;
  sample_num=0;
  sample_num_index=0;
  last_tick=0;
  table_size=0;
  offset_next_index=0;
  table_size=WAVETABLE_SIZE;
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


int Oscillator::getFreq()
{
  return frequency;
}

void Oscillator::setFreq(int freq)
{
  if (freq==frequency)
    return;

  if (freq <= 0)          freq=0;
  else 
    if (freq > 20000) freq=20000;
  frequency=freq;
  /*
  table_fill=0;
  if (freq!=0)
    table_size=DEFAULTFREQ/freq;
  else
    table_size=0;
  */
  //  printf("setFreq:%d\n",freq);
  if (freq>0)
    {
      offset_next_index=(freq*table_size)/44100;
      /*
      printf("*************freq:%d\n",freq);
      printf("*************table_size:%d\n",table_size);
      printf("*************offset_next_index:%d\n",offset_next_index);
      */
    }
}



void Oscillator::setAmplitude(int amp)
{
  int debug=1;
  if (amp>=127          ){ amplitude=127; }
  if (amp<=0            ){ amplitude=0;   }
  if (amp>0   && amp<127){ amplitude=amp; }
  printf("Oscillator::setAmplitude(amplitude=%d)\n",amplitude);
}



Sint16 Oscillator::tick()
{
  Sint16 trash;
  printf("Sint16 Oscillator::tick() freq:%d amp:%d\n",frequency,amplitude);
}


using namespace std;

#include "Oscillator.h"

Oscillator::Oscillator()
{
  frequency=0;
  amplitude=0;
}

Oscillator::~Oscillator()
{
  
}

void Oscillator::setFreq(int freq)
{
  if (freq < 0)          freq=1;
  else if (freq > 20000) freq=20000;
  else                   frequency=freq;

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



/*
virtual Sint16 Oscillator::tick()
{
  Sint16 trash;
  printf("Sint16 Oscillator::tick() freq:%d amp:%d\n",frequency,amplitude);
  //  exit(0);
  return trash;
}
*/

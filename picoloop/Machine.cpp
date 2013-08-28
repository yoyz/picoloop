#include "Machine.h"

Machine::Machine()
{
  S = &SINEOSC;
  S->setFreq(0);
  S->setAmplitude(127);
  printf("&S:%d\n",S);
}

void Machine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  S->setFreq(sfreq);
}



/*
void Machine::setOscillator(SineOscillator NS)
{
  S=NS;
}
*/


void Machine::setSineOsc()
{
  S=&SINEOSC;
  S->setFreq(freq);
}


void Machine::setSawOsc()
{
  S=&SAWOSC;
  S->setFreq(freq);
}

void Machine::setFuzzyPulseOsc()
{
  S=&FUZZYPULSEOSC;
  S->setFreq(freq);
}


int Machine::tick()
{
  //printf("&S:%d\n",S);
  //exit(0);
  return S->tick();
}

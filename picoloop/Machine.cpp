#include "Machine.h"

Machine::Machine()
{
  //  S = SINEOSC;
  S.setFreq(880);
  S.setAmplitude(127);
}

void Machine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  S.setFreq(sfreq);
}

/*
void Machine::setOscillator(SineOscillator NS)
{
  S=NS;
}

void Machine::setSineOsc()
{
  S=SINEOSC;
  S.setFreq(freq);
}

void Machine::setSawOsc()
{
  S=SAWOSC;
  S.setFreq(freq);
}
*/

int Machine::tick()
{
  return S.tick();
}

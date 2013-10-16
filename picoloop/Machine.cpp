#include "Machine.h"


//Machine::Machine() : sineosc(), sawosc(), fuzzypulseosc(), adsr()
Machine::Machine() : adsr()
{
  vco = new VCO();
  /*
  s = &sineosc;
  s->setFreq(0);
  s->setAmplitude(127);

  */
  //  adsr.setOscillator(s);
  adsr.setVCO(vco);
  //  printf("&S:%d\n",S);
}

Machine::~Machine()
{
  printf("Machine::~Machine()\n");  
}





/*
void Machine::setOscillator(SineOscillator NS)
{
  S=NS;
}
*/

ADSR & Machine::getADSR()
{
  return adsr;
}

VCO * Machine::getVCO()
{
  return vco;
}
/*
void Machine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  vco->setFreq(sfreq);
  //adsr.setOscillator(s);
  adsr.setVCO(vco);
}
*/

/*

void Machine::setSynthFreq(int sfreq)
{
  freq=sfreq;
  s->setFreq(sfreq);
  adsr.setOscillator(s);
}


Oscillator * Machine::getOscillator()
{
  return s;
}

void Machine::setSineOsc()
{
  s=&sineosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}


void Machine::setSawOsc()
{
  s=&sawosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}

void Machine::setFuzzyPulseOsc()
{
  s=&fuzzypulseosc;
  s->setFreq(freq);
  adsr.setOscillator(s);
}
*/

int Machine::tick()
{
  //if (&s==0) { printf("sine:%d\n",sineosc.tick()); exit(0);  }
  //  return s->tick();
  return adsr.tick();
}

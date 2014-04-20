#include "Machine.h"


//Machine::Machine() : sineosc(), sawosc(), fuzzypulseosc(), adsr()

//Machine::Machine()// : adsr(), vco()
//Machine::Machine() : adsr(), vco_osc()
Machine::Machine() : adsr(), vco(), bq()
{
  printf("Machine::Machine()\n");  
  //  adsr=new ADSR();
  //  vco=new VCO();

  //  vco = new VCO();
  //vco_pointer = new VCO();
  /*
  s = &sineosc;
  s->setFreq(0);
  s->setAmplitude(127);

  */
  //  adsr.setOscillator(s);
  //  vco_pointer=&vco_osc;
  //  adsr.setVCO(vco_pointer);

  //  printf("&S:%d\n",S);
}



Machine::~Machine()
{
  printf("Machine::~Machine()\n");
}


void Machine::init()
{
  adsr.setInput(&vco);
  bq.setBiquad(0, 0.2, 0.5, 0.1);
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

VCO & Machine::getVCO()
{
  //  printf("Machine::getVCO() this=0x%08.8X\n",this);
  //  return vco_pointer;
  return vco;
}

Biquad & Machine::getBiquad()
{
  return bq;
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
  float  f_in;
  float  f_out;
  Sint16 s_in;
  Sint16 s_out;


  s_in=adsr.tick();
  s_in=s_in/4;
  s_out=bq.process(s_in);
  //  s_out=bq.process(s_out);
  //s_out=bq.process(s_out);
  return s_out;


  /*
  s_in=adsr.tick();

  f_in=s_in;
  f_in=f_in/32768.0;
  f_out=bq.process(f_in);
  f_out=bq.process(f_out);
  f_out=bq.process(f_out);
  f_out=f_out*32768.0;
  s_out=f_out;
  return s_out;
  */

  /*    
  s_in=adsr.tick();
  f_in=s_in;
  //f_in=f_in/32768.0;
  f_in=f_in*0.0000305;
  f_out=bq.process(f_in);
  f_out=f_out*32768.0;
  s_out=f_out;
  return s_out;
  */

  //  printf("%d %d %f %f\n",s_in,s_out,f_in,f_out);
  
  
  //return adsr.tick();
}

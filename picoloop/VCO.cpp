#include "VCO.h"


VCO::VCO() : sineosc(), sawosc(), fuzzypulseosc()
{
  printf("VCO::VCO()\n");
  s1=NULL;
  s2=NULL;
  //  s1 = &sineosc;
  //  s1->setFreq(0);
  //  s1->setAmplitude(32);
  /*
  s2 = &sineosc;
  s2->setFreq(0);
  s2->setAmplitude(32);
  */
  //  s1=NULL;
  //  s2=NULL;
}

VCO::~VCO()
{
  printf("VCO::~VCO()\n");
}


void VCO::init()
{
  printf("VCO::init() begin s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);
  s1 = &sineosc;
  s1->setFreq(0);
  s1->setAmplitude(32);

  s2 = &fuzzypulseosc;
  s2->setFreq(0);
  s2->setAmplitude(32);
  printf("VCO::init() end s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);
}


void VCO::reset()
{
  printf("VCO::reset() this=0x%08.8X\n",this); // <==== FAILS allways the same this pointers
  s1->reset();
  s2->reset();
}

Oscillator * VCO::getOscillatorOne()
{
  printf("Oscillator * VCO::getOscillatorOne() Oscillator=0x%08.8X\n",s1);
  return s1;
}

void VCO::setSynthFreq(int sfreq)
{
  /*
  s1->setSynthFreq(sfreq);
  s2->setSynthFreq(sfreq);
  */
  s1->setFreq(sfreq);
  s2->setFreq(sfreq);
}


Sint16 VCO::tick()
{
  //  printf("VCO::tick() this=0x%08.8X\n",this); 
  // return s1->tick()+s2->tick();
  if (s1==NULL)
    { 
      printf("[s1 is NULL]\n"); 
      //  exit(1); 
    } /*
  else
    {
      printf("[s1 is NOT NULL]\n"); 
      } */
  return s1->tick()+s2->tick();
}

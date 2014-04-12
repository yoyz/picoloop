#include "VCO.h"


VCO::VCO() : sineOsc1(), sineOsc2(), sawOsc1(), sawOsc2(), pulseOsc1(), pulseOsc2(), waveTableSineOsc1(), waveTableSineOsc2() //, noiseosc()
{
  printf("VCO::VCO()\n");
  s1=NULL;
  s2=NULL;
  vcomix=64;
}

void VCO::init()
{
  printf("VCO::init() begin s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);

  waveTableSineOsc1.init();
  waveTableSineOsc2.init();

  sineOsc1.init();
  sineOsc2.init();


  //  s1 = &sineosc;
  s1 = &pulseOsc1;
  s2 = &sawOsc2;

  s1->setFreq(0);
  s1->setAmplitude(32);
  //  s2 = &pulseosc;
  s2->setFreq(0);
  s2->setAmplitude(32);


  printf("VCO::init() end s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);
}


VCO::~VCO()
{
  printf("VCO::~VCO()\n");
}

int VCO::checkSevenBitBoundarie(int val)
{
  if (val < 0)
    return 0;
  if (val > 127)
    return 127;
  return val;
}


void VCO::setVCOMix(int mix)
{

  vcomix=this->checkSevenBitBoundarie(mix);
}

void VCO::setOscillator(int oscillator_number,int oscillator_type)
{
  int s1freq=s1->getFreq();
  int s2freq=s2->getFreq();

  if (oscillator_number %2==0)
    {
      if (oscillator_type%4==0) s1=&sineOsc1;
      if (oscillator_type%4==1) s1=&sawOsc1;
      if (oscillator_type%4==2) s1=&pulseOsc1;
      if (oscillator_type%4==3) s1=&waveTableSineOsc1;

      //      if (oscillator_type%4==2) s1=&noiseosc;
    }
  if (oscillator_number %2==1)
    {
      if (oscillator_type%4==0) s2=&sineOsc2;
      if (oscillator_type%4==1) s2=&sawOsc2;
      if (oscillator_type%4==2) s2=&pulseOsc2;
      if (oscillator_type%4==3) s2=&waveTableSineOsc2;
      //      if (oscillator_type%4==3) s2=&noiseosc;
    }
  s1->setFreq(s1freq);
  s2->setFreq(s2freq);
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
  s1->setFreq(sfreq*1);
  s2->setFreq(sfreq*1);
  //  s2->setFreq(sfreq*3);
}




Sint16 VCO::tick()
{
  //  printf("VCO::tick() this=0x%08.8X\n",this); 
  // return s1->tick()+s2->tick();
  Sint32 sa;
  Sint32 sb;
  Sint32 sc;
  Sint16 s;
  if (vcomix==0) vcomix=1;
  if (s1==NULL)
    { 
      printf("[s1 is NULL]\n"); 
      //  exit(1); 
    } /*
  else
    {
      printf("[s1 is NOT NULL]\n"); 
      } */
  //  return s1->tick()+s2->tick();
  
  //sa=s1->tick()*(128/vcomix);

  //vcomix=0    sa full
  //vcomix=128  sa 0

  //vcomix=0    sb 0
  //vcomix=128  sb full

  //  sa=(s1->tick()*((128-vcomix))/128);
  //  sb=(s2->tick()*((vcomix-128))/128);
  
  sa=(s1->tick()*((128-vcomix))/(128));

  sb=(s2->tick()*((vcomix))/(128));

  sc=sa+sb;
  s=sc;
  //  sb=(s2->tick())

  //return s1->tick()/(128-vcomix)+s2->tick()/(vcomix);  
    
  
  //return s1->tick()+s2->tick();
  return s;
}

#include "VCO.h"


VCO::VCO() : sineOsc1(), sineOsc2(), sawOsc1(), sawOsc2(), pulseOsc1(), pulseOsc2(), triangleOsc1(), triangleOsc2(), waveTableSineOsc1(), waveTableSineOsc2(), noiseOsc1(), noiseOsc2(), sineLfoOsc1(), sawLfoOsc1() //, noiseosc()
{
  printf("VCO::VCO()\n");
  s1=NULL;
  s2=NULL;
  vcomix=64;

  lfo_counter=0;
  lfo_refresh=2;

  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;

  freqOsc1=0;
  freqOsc2=0;
}

void VCO::init()
{
  printf("VCO::init() begin s1:=0x%08.8X s2:=0x%08.8X\n",s1,s2);


  lfo_depth=0;
  lfo_depth_shift=20;

  lfo_speed=0;


  waveTableSineOsc1.init();
  waveTableSineOsc2.init();

  sineOsc1.init();
  sineOsc2.init();

  sawOsc1.init();
  sawOsc2.init();

  triangleOsc1.init();
  triangleOsc2.init();

  pulseOsc1.init();
  pulseOsc2.init();

  noiseOsc1.init();
  noiseOsc2.init();

  sineLfoOsc1.init();
  sawLfoOsc1.init();

  //lfo1=&sineLfoOsc1;
  lfo1=&sawLfoOsc1;

  lfo1->setFreq(0);
  lfo1->setAmplitude(32);

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
  if (val <= 0)
    return 0;
  if (val >= 127)
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
      if (oscillator_type%5==0) s1=&sineOsc1;
      if (oscillator_type%5==1) s1=&sawOsc1;
      if (oscillator_type%5==2) s1=&pulseOsc1;
      //if (oscillator_type%5==3) s1=&waveTableSineOsc1;
      if (oscillator_type%5==3) s1=&triangleOsc1;
      if (oscillator_type%5==4) s1=&noiseOsc1;

      //      if (oscillator_type%4==2) s1=&noiseosc;
    }
  if (oscillator_number %2==1)
    {
      if (oscillator_type%5==0) s2=&sineOsc2;
      if (oscillator_type%5==1) s2=&sawOsc2;
      if (oscillator_type%5==2) s2=&pulseOsc2;
      //if (oscillator_type%5==3) s2=&waveTableSineOsc2;
      if (oscillator_type%5==3) s2=&triangleOsc2;
      if (oscillator_type%5==4) s2=&noiseOsc2;
      //      if (oscillator_type%4==3) s2=&noiseosc;
    }
  s1->setFreq(s1freq);
  s2->setFreq(s2freq);
}

void VCO::setLfoDepth(int val)
{
  lfo_depth=val;
  if (val > 0   && val <=  8  )  { lfo_depth=val ; lfo_depth_shift=20;       }
  if (val > 8   && val <= 16  )  { lfo_depth=val ; lfo_depth_shift=12;       }
  if (val > 17  && val <= 32  )  { lfo_depth=val ; lfo_depth_shift=11;       } 
  if (val > 33  && val <= 48  )  { lfo_depth=val ; lfo_depth_shift=10;       }
  if (val > 49  && val <= 64  )  { lfo_depth=val ; lfo_depth_shift=9;        }
  if (val > 65  && val <= 80  )  { lfo_depth=val ; lfo_depth_shift=8;        }
  if (val > 81  && val <= 96  )  { lfo_depth=val ; lfo_depth_shift=7;        }
  if (val > 97  && val <= 112 )  { lfo_depth=val ; lfo_depth_shift=6;        }
  if (val > 113 && val <= 128 )  { lfo_depth=val ; lfo_depth_shift=5;        }

}

void VCO::setLfoSpeed(int val)
{
  lfo_speed=val;
  lfo1->setFreq(val);
  //lfo1->setAmplitude(0);
}

void VCO::reset()
{
  printf("VCO::reset() this=0x%08.8X\n",this); // <==== FAILS allways the same this pointers
  s1->reset();
  s2->reset();
  lfo1->reset();
  
  //this->setLfoDepth(0);
  //this->setLfoSpeed(0);
  //  lfo_counter=0;
}

Oscillator * VCO::getOscillatorOne()
{
  printf("Oscillator * VCO::getOscillatorOne() Oscillator=0x%08.8X\n",s1);
  return s1;
}


void VCO::setSynthFreq(float sfreq)
{

  freqOsc1=sfreq;
  freqOsc2=sfreq;
  /*
  s1->setSynthFreq(sfreq);
  s2->setSynthFreq(sfreq);
  */
  s1->setFreq(freqOsc1);
  s2->setFreq(freqOsc2);
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
  int    tmp=0;
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
  
  //sa=(s1->tick()*((128-vcomix))/(128));
  lfo_counter++;
  if (lfo_counter>lfo_refresh)
    lfo_counter=0;

  //lfo_counter=0;
  
  if (lfo_counter==0)
    {
      //tmp=lfo1->tick() >> ( lfo_depth >> 4 ) ;
      //tmp=lfo1->tick() >> (lfo_depth /32 ) ;
      if (lfo_depth==0)
	tmp=0;
      else
	tmp=lfo1->tick() >> lfo_depth_shift;

      s1->setFreq(freqOsc1+tmp);
      s2->setFreq(freqOsc2+tmp);
      //      tmp_i++;
      //      if (tmp_i>=128)
      //      	{
      //	  if (tmp!=0)
      //	    printf("tmp:\t%.5d speed:%.5d depth:%.5d shift:%.5d\n",tmp,lfo_speed,lfo_depth,lfo_depth_shift);
      //	  tmp_i=0;
      //	}
    }


  sa=(s1->tick()*((128-vcomix)));

  //sb=(s2->tick()*((vcomix))/(128));
  sb=(s2->tick()*((vcomix)));

  //sc=sa+sb;
  //sc=(sa+sb)/128;
  sc=(sa+sb)>>7;

  s=sc;
  //  sb=(s2->tick())

  //return s1->tick()/(128-vcomix)+s2->tick()/(vcomix);  
    
  
  //return s1->tick()+s2->tick();
  return s;
}

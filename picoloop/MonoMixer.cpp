#include "MonoMixer.h"



			//MonoMixer::MonoMixer(): M()
//MonoMixer::MonoMixer()
MonoMixer::MonoMixer(): PD(), PM(), OPLM(), FXDelay()
{
  printf("MonoMixer::MonoMixer()\n");  
  amplitude=127;
  //  M=NULL;
  //M=&PM;
  //printf("MonoMixer::MonoMixer()  PM=0x%08.8X\n",PM);
  //printf("MonoMixer::MonoMixer()  M=0x%08.8X\n",M);
  //M=&OPLM;
  machine_type=0;
  M=&PM;


  //FX=&FXDelay;

  fx_depth=125;
  fx_speed=90;
}

MonoMixer::~MonoMixer()
{
  printf("MonoMixer::~MonoMixer()\n");  
}


void MonoMixer::init()
{
  PM.init();
  PD.init();
  OPLM.init();

  //FX=&FXDelay;
  FX=&FXDelay;
  FX->init();
  FX->setDepth(fx_depth);
  FX->setSpeed(fx_speed);
  
  //FXDelay.init();

  //FXDelay.setDepth(fx_depth);
  //FXDelay.setSpeed(fx_speed);

  

  if (machine_type==0)
    M=&PM;
  if (machine_type==1)
    M=&OPLM;
  if (machine_type==2)
    M=&PD;

  //M=&OPLM;
  //M=&PM;
}


void MonoMixer::setMachineType(int type)
{
  printf("MonoMixer::setMachineType :%d\n",type);  
  machine_type=type;

  if (type==0)
      M=&PM;

  if (type==1)
      M=&OPLM;

  if (type==2)
      M=&PD;

}

void MonoMixer::setAmplitude(int amp)
{
  int debug=0;
  if (amp>=127)           { amplitude=127; }
  if (amp<=0)             { amplitude=0;   }
  if (amp>0   && amp<127) { amplitude=amp; }
  printf("MonoMixer::setAmplitude(amplitude=%d)\n",amplitude);

  //M->setAmplitude(amp);
  amplitude=amp;
}
/*
void MonoMixer::setInput(Machine * Mach)
{
  M=Mach;
}
*/
Machine * MonoMixer::getInput()
{
  //M->init();
  return M;
  
}

Effect * MonoMixer::getEffect()
{
  //M->init();
  return FX;
  
}


Sint16 MonoMixer::tick()
{
  int debug=0;
  //  /*
  Sint32 res32=0;
  Sint16 tick=0;
  Sint16 res16=0;
  //tick=M->tick();
  //tick=FX.process(M->tick());
  //tick=FXDelay.process(M->tick());
  tick=FX->process(M->tick());
  res32=tick*amplitude;
  //  res32=tick*127;
  //res32=res32/127;
  
  res32=res32>>5;

  //if (res32>32000)  res32=32000-(res32>>6);
  //if (res32<-32000) res32=-32000+(res32>>6);

  if (res32>32000)  res32=32000;
  if (res32<-32000) res32=-32000;

  //if (res32>32000)  res32=32000-(res32-32000);
  //if (res32<-32000) res32=-32000-(res32+32000);

  //  return M->tick();
  res16=res32;
  //if (0) printf("amplitude:%d tick:%d res32:%d res16:%d\n",amplitude,tick,res32,res16);
  return res16;
  //*/
  //return M->tick();
}

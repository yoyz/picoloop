#include "MonoMixer.h"


MonoMixer::MonoMixer(): M()
{
  amplitude=127;
}

MonoMixer::~MonoMixer()
{
  printf("MonoMixer::~MonoMixer()\n");  
}


void MonoMixer::setAmplitude(int amp)
{
  int debug=0;
  if (amp>=127)           { amplitude=127; }
  if (amp<=0)             { amplitude=0;   }
  if (amp>0   && amp<127) { amplitude=amp; }
  printf("amplitude:%d\n",amplitude);

  //M->setAmplitude(amp);
  amplitude=amp;
}

void MonoMixer::setInput(Machine * Mach)
{
  M=Mach;
}

Sint16 MonoMixer::tick()
{
  int debug=0;
  //  /*
  Sint32 res32;
  Sint16 res16;
  Sint16 tick;
  tick=M->tick();
  res32=tick*amplitude;
  //  res32=tick*127;
  res32=res32/127;
  if (res32>32000)  res32=32000;
  if (res32<-32000) res32=-32000;
  //  return M->tick();
  res16=res32;
  if (debug) printf("amplitude:%d tick:%d res32:%d res16:%d\n",amplitude,tick,res32,res16);
  return res16;
  //*/
  //return M->tick();
}

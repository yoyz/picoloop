#include "AudioMixer.h"

AudioMixer::AudioMixer()
{
}

Track & AudioMixer::getTrack(int numTrack)
{
  if (numTrack==0)
    return T0;
  else
    return T1;
}

Sint16 AudioMixer::tick()
{
  //Sint16 inttick=T.tick();
  Sint32 sint32tick=(T0.tick()/2)+(T1.tick()/2);
  Sint16 sint16tick;
  
  sint16tick=sint32tick;
  //  printf("%d\n",inttick);
  //  return S.tick();
  //return inttick;
  return sint16tick;
}



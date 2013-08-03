#include "AudioMixer.h"

AudioMixer::AudioMixer()
{
}

Track & AudioMixer::getTrack()
{
  return T;
}

int AudioMixer::tick()
{
  int inttick=T.tick();
  //  printf("%d\n",inttick);
  //  return S.tick();
  return inttick;
}



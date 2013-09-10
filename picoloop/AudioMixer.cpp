#include "AudioMixer.h"

AudioMixer::AudioMixer() : T0(), T1()
{
}

Track & AudioMixer::getTrack(int numTrack)
{
  if (numTrack==0)
    return T0;
  else
    return T1;
}


Sint16 AudioMixer::twoChannel(Sint16 a,Sint16 b)
{
  Sint16 res;
  /*
  if (a > 0 && b > 0)
    {
      res=(((a+b))-((a*b)/32768));
      printf("a:%d\tb:%d\tres:%d\n",a,b,res);
      return res;
    }
  if (a < 0 && b < 0)
    {
      //return 2*(a+b)-((a*b)/-32768)+65535;
      res=(((a+b))-((a*b)/-32768));
      printf("a:%d\tb:%d\tres:%d\n",a,b,res);
      return res;
    }
  */
  return a*0.5+b*0.5;
  //return a;
}



Sint16 AudioMixer::tick()
{
  //Sint16 inttick=T.tick();
  //Sint32 sint32tick=(T0.tick()/2)+(T1.tick()/2);
  Sint32 sint32tick=twoChannel(T0.tick(),T1.tick());
  Sint16 sint16tick;
  
  sint16tick=sint32tick;
  //  printf("%d\n",inttick);
  //  return S.tick();
  //return inttick;
  return sint16tick;
}



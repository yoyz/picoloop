#include "AudioMixer.h"

//AudioMixer::AudioMixer() : T0(), T1()
AudioMixer::AudioMixer() : T(TRACK_MAX)
{
  printf("AudioMixer::AudioMixer()\n");  
}

AudioMixer::~AudioMixer()
{
  printf("AudioMixer::~AudioMixer()\n");  
}


Track & AudioMixer::getTrack(int numTrack)
{
  return T[numTrack];
  /*
  if (numTrack==0)
    return T0;
  else
    return T1;
  */
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
  Sint32 val=0;
  int i;
  Sint16 sint16tick;

  for (i=0;i<TRACK_MAX;i++)
    val=val+T[i].tick();
  //Sint16 inttick=T.tick();
  //Sint32 sint32tick=(T0.tick()/2)+(T1.tick()/2);
  //Sint32 sint32tick=this->twoChannel(T[0].tick(),T[1].tick());

  
  //  sint16tick=sint32tick;
  sint16tick=val;
  //  printf("%d\n",inttick);
  //  return S.tick();
  //return inttick;
  return sint16tick;
}



#include "AudioMixer.h"

//AudioMixer::AudioMixer() : T0(), T1()
AudioMixer::AudioMixer() : MM(TRACK_MAX)
{
  DPRINTF("AudioMixer::AudioMixer()\n");  
  volume=64;
}

AudioMixer::~AudioMixer()
{
  DPRINTF("AudioMixer::~AudioMixer()\n");  
}


MonoMixer * AudioMixer::getMonoMixer(int numMonoMixer)
{
  //MonoMixer * MMtmp=MM[numTrack];
  return &MM[numMonoMixer];
}


Sint16 AudioMixer::twoChannel(Sint16 a,Sint16 b)
{
  Sint16 res;
  /*
  if (a > 0 && b > 0)
    {
      res=(((a+b))-((a*b)/32768));
      DPRINTF("a:%d\tb:%d\tres:%d\n",a,b,res);
      return res;
    }
  if (a < 0 && b < 0)
    {
      //return 2*(a+b)-((a*b)/-32768)+65535;
      res=(((a+b))-((a*b)/-32768));
      DPRINTF("a:%d\tb:%d\tres:%d\n",a,b,res);
      return res;
    }
  */
  return a*0.5+b*0.5;
  //return a;
}


void AudioMixer::setAudioVolume(int v)
{
  if (v>=0 &&
      v<=127)
    {
      volume=v;
      volumeFixed=Fixed(Fixed(v)/Fixed(127));
    }
  DPRINTF("AudioVolume:%d\n",volume);
}

int AudioMixer::getAudioVolume()
{
  return volume;
}
/*
Sint16 AudioMixer::tick()
{
  Sint32 val=0;
  Sint16 sint16tick;
  int i;


  for (i=0;i<TRACK_MAX;i++)
    val=val+Sint32(MM[i].tick_fixed());
  //Sint16 inttick=T.tick();
  //Sint32 sint32tick=(T0.tick()/2)+(T1.tick()/2);
  //Sint32 sint32tick=this->twoChannel(T[0].tick(),T[1].tick());
  val=(val*volume)>>7;
  
  //  sint16tick=sint32tick;

  if (val>32000)  val=32000-(val-32000);
  if (val<-32000) val=-32000-(val+32000);

  //if (val>32000)  val=32000;
  //if (val<-32000) val=-32000;
  sint16tick=val;
  //  DPRINTF("%d\n",inttick);
  //  return S.tick();
  //return inttick;
  return sint16tick;
}
*/

Sint16 AudioMixer::tick()
{
  Sint32 val=0;
  Sint16 sint16tick;
  int i;


  for (i=0;i<TRACK_MAX;i++)
    val=val+Sint32(MM[i].tick_fixed());
  //Sint16 inttick=T.tick();
  //Sint32 sint32tick=(T0.tick()/2)+(T1.tick()/2);
  //Sint32 sint32tick=this->twoChannel(T[0].tick(),T[1].tick());
  val=(val*volume)>>7;
  
  //  sint16tick=sint32tick;

  if (val>32000)  val=32000-(val-32000);
  if (val<-32000) val=-32000-(val+32000);

  //if (val>32000)  val=32000;
  //if (val<-32000) val=-32000;
  sint16tick=val;
  //  DPRINTF("%d\n",inttick);
  //  return S.tick();
  //return inttick;
  return sint16tick;
}

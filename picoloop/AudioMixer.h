#include "MonoMixer.h"
//#include <SDL/SDL_types.h>
#include <vector>
#include "Master.h"
#ifndef __AUDIOMIXER____
#define __AUDIOMIXER____


class AudioMixer
{
 public:
  AudioMixer();
  ~AudioMixer();
  MonoMixer * getMonoMixer(int numTrack);
  Sint16 twoChannel(Sint16 a,Sint16 b);
  Sint16  tick();
  void    setAudioVolume(int v);
  int     getAudioVolume();

 private:
  //Track T0;
  //Track T1;
  std::vector <MonoMixer> MM;
  int volume;
  Fixed volumeFixed;
  Fixed * buffer_fix;
  int index;
};


#endif


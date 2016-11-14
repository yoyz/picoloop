using namespace std;

#include "Track.h"
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
  Track & getTrack(int numTrack);
  Sint16 twoChannel(Sint16 a,Sint16 b);
  Sint16  tick();
  void    setAudioVolume(int v);
  int     getAudioVolume();

 private:
  //Track T0;
  //Track T1;
  vector <Track> T;
  int volume;
};


#endif


using namespace std;

#include "Track.h"

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
 private:
  //Track T0;
  //Track T1;
  vector <Track> T;

};


#endif


using namespace std;

#ifndef __AUDIOMIXER____
#define __AUDIOMIXER____
#include "Track.h"

class AudioMixer
{
 public:
  AudioMixer();
  Track & getTrack();
  Track T;
  int  tick();
};


#endif


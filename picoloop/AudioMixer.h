using namespace std;

#ifndef __AUDIOMIXER____
#define __AUDIOMIXER____
#include "Track.h"

class AudioMixer
{
 public:
  AudioMixer();
  Track & getTrack(int numTrack);
  Sint16  tick();
 private:
  Track T0;
  Track T1;

};


#endif


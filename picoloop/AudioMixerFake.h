#ifndef __AUDIOMIXER____
#define __AUDIOMIXER____
#include "Master.h"

class AudioMixer
{
 public:
  AudioMixer();
  ~AudioMixer();
  Sint16  tick();
};
#endif

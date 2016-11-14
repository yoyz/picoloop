#ifndef __AUDIODRIVERSDL____
#define __AUDIODRIVERSDL____


#include "Master.h"


class AudioDriver
{
 public:
  AudioDriver();
  
  int  closeAudio();
  int  openAudio();
  int  stopAudio();
  int  startAudio();

  int     getNumberOfAudioOutputDevice();
  char *  getAudioOutputName(int deviceNumber);
  void    setAudioOutput(int deviceNumber);

  
  int  getBufferFrame();


 public:
  SDL_AudioSpec * sdlAudioSpecWanted;
  SDL_AudioSpec * sdlAudioSpecObtained;

  //AudioEngine   * AE;

};

#endif

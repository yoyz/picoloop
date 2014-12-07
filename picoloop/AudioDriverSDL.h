#ifndef __AUDIODRIVERSDL____
#define __AUDIODRIVERSDL____

#include "SDL/SDL.h"
#include "Master.h"
//#include "AudioEngine.h"

class AudioDriver
{
 public:
  AudioDriver();
  
  int  closeAudio();
  int  openAudio();
  int  stopAudio();
  int  startAudio();
  
  int  getBufferFrame();


 public:
  SDL_AudioSpec * sdlAudioSpecWanted;
  SDL_AudioSpec * sdlAudioSpecObtained;

  //AudioEngine   * AE;

};

#endif

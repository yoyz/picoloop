#ifndef __AUDIODRIVERRTAUDIO____
#define __AUDIODRIVERRTAUDIO____

//#include "SDL/SDL.h"
#include "RtAudio.h"
#include "Master.h"
//#include "AudioEngine.h"

typedef int(* RtAudioCallback)(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);


class AudioDriver
{
 public:
  AudioDriver();
  
  int  closeAudio();
  int  openAudio();
  int  stopAudio();
  int  startAudio();


 public:
  RtAudio::StreamParameters rtAudioOutputParams;
  RtAudio::StreamOptions    rtAudioStreamOptions;
  RtAudio dac;
  int     FORMAT;
  int     bufferFrames;
  RtAudioCallback internal_callback;
  void  * userdata;
  //SDL_AudioSpec * sdlAudioSpecWanted;
  //SDL_AudioSpec * sdlAudioSpecObtained;

  //AudioEngine   * AE;

};

#endif

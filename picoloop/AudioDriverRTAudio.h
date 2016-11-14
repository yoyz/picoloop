#ifndef __AUDIODRIVERRTAUDIO____
#define __AUDIODRIVERRTAUDIO____

#include "RtAudio.h"
#include "Master.h"

typedef int(* RtAudioCallback)(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);


class AudioDriver
{
 public:
  AudioDriver();
  
  int     closeAudio();
  int     openAudio();
  int     stopAudio();
  int     startAudio();
  int     getNumberOfAudioOutputDevice();
  char *  getAudioOutputName(int deviceNumber);
  void    setAudioOutput(int deviceNumber);

 public:
  RtAudio::StreamParameters rtAudioOutputParams;
  RtAudio::StreamOptions    rtAudioStreamOptions;
  RtAudio dac;
  int     FORMAT;
  unsigned int     bufferFrames;
  RtAudioCallback internal_callback;
  void  * userdata;
  //SDL_AudioSpec * sdlAudioSpecWanted;
  //SDL_AudioSpec * sdlAudioSpecObtained;

  //AudioEngine   * AE;

};

#endif

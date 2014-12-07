#include "AudioDriverSDL.h"



AudioDriver::AudioDriver()
{
  sdlAudioSpecWanted   = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));  
  sdlAudioSpecObtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));

  //sdlAudioSpecWanted->freq = 48000;
  //sdlAudioSpecWanted->freq = 44100;
  sdlAudioSpecWanted->freq = DEFAULTFREQ;
  sdlAudioSpecWanted->format = AUDIO_S16SYS;
  sdlAudioSpecWanted->channels = 2;    /* 1 = mono, 2 = stereo */
  sdlAudioSpecWanted->samples = BUFFER_FRAME;  /* Good low-latency value for callback */
  //sdlAudioSpecWanted->callback = fakesdlcallback;
  //sdlAudioSpecWanted->userdata = this;
}

int AudioDriver::getBufferFrame()
{
  //return sdlAudioSpecWanted->samples;
  return sdlAudioSpecObtained->samples;
}


int AudioDriver::startAudio()
{
  SDL_PauseAudio(0);
}

int AudioDriver::stopAudio()
{
  SDL_PauseAudio(1);
}

int AudioDriver::openAudio()
{
  //SDL_InitSubSystem(SDL_INIT_AUDIO);
  if ( SDL_OpenAudio(sdlAudioSpecWanted, sdlAudioSpecObtained) < 0 ) 
    {
      fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
      return(-1);
    }
  
  printf("SDL freq     : %d\n",sdlAudioSpecObtained->freq);
  printf("SDL format   : %d\n",sdlAudioSpecObtained->format);
  printf("SDL channels : %d\n",sdlAudioSpecObtained->channels);
  printf("SDL samples  : %d\n",sdlAudioSpecObtained->samples);
}


int AudioDriver::closeAudio()
{
  //SDL_CloseAudio();
  //SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

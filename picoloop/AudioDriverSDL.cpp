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

// Do nothing today
void AudioDriver::setAudioOutput(int deviceNumber)
{
  
}

char * AudioDriver::getAudioOutputName(int deviceNumber)
{
  //static const char * audioOutputName="SDL";
  static char * audioOutputName="SDL";
  return audioOutputName;
}

int AudioDriver::getNumberOfAudioOutputDevice()
{
  // dac.getDeviceCount() return count+1
  return 0;
}


int AudioDriver::getBufferFrame()
{
  //return sdlAudioSpecWanted->samples;
  return sdlAudioSpecObtained->samples;
}


int AudioDriver::startAudio()
{
  SDL_PauseAudio(0);
  return 0;
}

int AudioDriver::stopAudio()
{
  SDL_PauseAudio(1);
  return 0;
}

int AudioDriver::openAudio()
{
  //SDL_InitSubSystem(SDL_INIT_AUDIO);
  if ( SDL_OpenAudio(sdlAudioSpecWanted, sdlAudioSpecObtained) < 0 ) 
    {
      DPRINTF("Couldn't open audio: %s\n", SDL_GetError());
      return(-1);
    }
  
  DPRINTF("SDL freq     : %d\n",sdlAudioSpecObtained->freq);
  DPRINTF("SDL format   : %d\n",sdlAudioSpecObtained->format);
  DPRINTF("SDL channels : %d\n",sdlAudioSpecObtained->channels);
  DPRINTF("SDL samples  : %d\n",sdlAudioSpecObtained->samples);
  return 0;
}


int AudioDriver::closeAudio()
{
  SDL_CloseAudio();
  //SDL_QuitSubSystem(SDL_INIT_AUDIO);
  return 0;
}

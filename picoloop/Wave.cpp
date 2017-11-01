using namespace std;

#include "Wave.h"
#include "Master.h"




Wave::Wave()
{
  desire_audio_spec.freq=DEFAULTFREQ;
  desire_audio_spec.channels=DEFAULTCHANNELS;
  desire_audio_spec.format=DEFAULTFORMAT;
}


int Wave::loadWave(char * filename)
{
  file_name=filename;
  //SDL_AudioSpec loaded;       /* format of the loaded data */
  printf("DEBUG: Wave::loadWave:%s\n",filename);
  if (SDL_LoadWAV(filename, 
		  &obtained_audio_spec, 
		  &sound_sample, 
		  &sound_length) == NULL) 
    {
      printf("Unable to load sound: %s\n", SDL_GetError());
      return 1;
    }

  if (SDL_BuildAudioCVT(&sdl_audio_cvt,
			obtained_audio_spec.format,
			obtained_audio_spec.channels,
			obtained_audio_spec.freq,
			desire_audio_spec.format, 
			desire_audio_spec.channels, 
			desire_audio_spec.freq) < 0) 
    {
      printf("Unable to convert sound: %s\n", SDL_GetError());
      return 1;
    }
  return 0;
}

Uint32 Wave::getLength()
{
  return sound_length;
}

Uint8* Wave::getBuffer(Uint32 offset)
{

  Uint8* new_buff=sound_sample;
  return new_buff+offset;
  
}





//#include <SDL/SDL.h>

#ifdef   __SDL12__
#include <SDL/SDL.h>
#endif // __SDL12__
#ifdef  __SDL20__
#include <SDL2/SDL.h>
#endif // __SDL20__

#ifndef __WAVE__
#define __WAVE__




class Wave
{
 public:
  Wave();
  int loadWave(char * filename);
  Uint32 getLength();
  Uint8* getBuffer(Uint32 offset);

 private:
  SDL_AudioCVT  sdl_audio_cvt;         // audio format conversion structure

  SDL_AudioSpec desire_audio_spec;     // desired  audio object
  SDL_AudioSpec obtained_audio_spec;   // obtained audio object



  Uint8 *sound_sample;
  Uint32 sound_length;

  char * file_name;
  
  
};

#endif

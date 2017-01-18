#ifndef __AUDIODRIVERPSVITA____
#define __AUDIODRIVERPSVITA____


#include "Master.h"
#include <psp2/kernel/threadmgr.h> 
#include <psp2/audioout.h>

//void psvitacallback(void *unused, Uint8 *stream, int len);
void psvitaaudiopushthread(SceSize argc, void *argp);
void psvitaaudiogeneratethread(SceSize argc, void *argp);

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
  SceUID thid_generateBuffer;
  SceUID thid_pushToAudioCard;
  int sce_size;
  int sce_freq;
  int sce_mode;
  int sce_vol;
  int sce_port;
  Sint16 * audio_buffer0;
  Sint16 * audio_buffer1;
  int    * buffer_used_by_card;
  void   ** argp_push;
  void   ** argp_generate;
  void   *  userdata;
};

#endif

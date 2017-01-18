#include "AudioDriverPSVita.h"




AudioDriver::AudioDriver()
{
  sdlAudioSpecWanted   = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
  audio_buffer         = (Sint16 *) malloc(sizeof(Sint16)*8192);  

  //sdlAudioSpecWanted->freq = 48000;
  //sdlAudioSpecWanted->freq = 44100;
  sdlAudioSpecWanted->freq = DEFAULTFREQ;
  sdlAudioSpecWanted->format = AUDIO_S16SYS;
  sdlAudioSpecWanted->channels = 2;    /* 1 = mono, 2 = stereo */
  sdlAudioSpecWanted->samples = BUFFER_FRAME;  /* Good low-latency value for callback */
  //sdlAudioSpecWanted->callback = fakesdlcallback;
  //sdlAudioSpecWanted->userdata = this;

  sce_size = 2048;
  sce_freq = 8;
  sce_mode = SCE_AUDIO_OUT_MODE_MONO;
  sce_vol = SCE_AUDIO_VOLUME_0DB;
  sce_port;

}

// Do nothing today
void AudioDriver::setAudioOutput(int deviceNumber)
{
  
}

char * AudioDriver::getAudioOutputName(int deviceNumber)
{
  static const char * audioOutputName="PSVITA";
  return audioOutputName;
}

int AudioDriver::getNumberOfAudioOutputDevice()
{
  // dac.getDeviceCount() return count+1
  return 0;
}


int AudioDriver::getBufferFrame()
{
  return sdlAudioSpecWanted->samples;
  //return sdlAudioSpecObtained->samples;
}


/*

// http://wololo.net/talk/viewtopic.php?t=26133&p=226425

module_start(SceSize args, void *argp)
{
int array[200];
array[0]=1234;

SceUID thid = sceKernelCreateThread("my_thread", my_thread, 0x18, 0x10000, 0, NULL);
sceKernelStartThread(thid, sizeof(array), array);
}

int my_thread(SceSize args, void *argp)
{
  printf("%d\n", argp[0]);

  return 0;
}
*/

int AudioDriver::startAudio()
{
  //SDL_PauseAudio(0);
  //sceKernelStartThread(thid,0,NULL);
  //void * args[3] = { sdlAudioSpecWanted->userdata=this, audio_buffer, BUFFER_FRAME };
  //sceKernelStartThread(thid,3,args);
  argp=malloc(sizeof(void**)*5);

  int i;
  argp[0]=sdlAudioSpecWanted->userdata=this;
  argp[1]=audio_buffer;
  argp[2]=2048;
  argp[3]=sce_port;
  DPRINTF("AudioDriverPSVita Before starting thread (argp[0]:0x%08.8X argp[1]:0x%08.8X argp[2]:0x%08.8X,argp[3]:0x%08.8X)\n",argp[0], argp[1], argp[2],argp[3]);
  sceKernelStartThread(thid,24,argp);

}

int AudioDriver::stopAudio()
{
  //SDL_PauseAudio(1);
}



int AudioDriver::openAudio()
{
  int sce_vol_tmp[2] = {sce_vol,sce_vol};
  int sce_freqs[] = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};
  sce_port = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, sce_size, sce_freqs[sce_freq], sce_mode);
  sceAudioOutSetVolume(sce_port, SCE_AUDIO_VOLUME_FLAG_L_CH |SCE_AUDIO_VOLUME_FLAG_R_CH, sce_vol_tmp);
  sceAudioOutSetConfig(sce_port, sce_size, sce_freqs[sce_freq], sce_mode);

  DPRINTF("PSVITA Before creating thread\n");
  thid = sceKernelCreateThread("audio_thread", psvitaaudiothread, 0x10000100, 0x10000, 0, 0, NULL);
  DPRINTF("PSVITA After creating thread\n");    
}


int AudioDriver::closeAudio()
{
  SDL_CloseAudio();
  //SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

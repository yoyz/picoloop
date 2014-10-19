#include "AudioEngine.h"


//#include "SineOscillator.h"

void sdlcallback(void *unused, Uint8 *stream, int len);
int   rtcallback( 
	       void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *user_data );

//AudioEngine::AudioEngine() : inst(), AM()
AudioEngine::AudioEngine() : AM(),
			     AD(),
			     buffer_out( new Sint16[INTERNAL_BUFFER_SIZE])
			     //buffer_out( new Sint16[BUFFER_FRAME])
{
  freq=DEFAULTFREQ;
  samples=DEFAULTSAMPLES;
  channels=DEFAULTCHANNELS;
  polyphony=DEFAULTPOLYPHONY;
  tick=0;

  FORMAT=RTAUDIO_SINT16;
  bufferFrames = 512;          // Weird ?
  bufferFrames = BUFFER_FRAME; // Weird ?
  nbCallback=0;
  debug_audio=1;
  if (debug_audio) 
    {
      fd = fopen("audioout","w+");
    }
  //buffer_out=(Sint16*)malloc(sizeof(Sint16)*BUFFER_FRAME);
  bufferGenerated=0;

  nb_tick=0;
  nb_tick_before_step_change=0;
  seqCallback=0;

  #ifdef __SDL_AUDIO__
  AD.sdlAudioSpecWanted->callback=sdlcallback;
  AD.sdlAudioSpecWanted->userdata=this;
  #endif

  #ifdef __RTAUDIO__
  AD.internal_callback=rtcallback;
  AD.userdata=this;
  #endif
}



int AudioEngine::getNbCallback()
{
  return nbCallback;
}

// related to  sequencer callbackw
int AudioEngine::setNbTickBeforeStepChange(int val) 
{
  nb_tick_before_step_change=val;
}


// a function which need to be trigged when "nb_tick_before_step_change" sample are generated
void AudioEngine::setupSequencerCallback(void (*ptrfunc)(void)) 
{
  seqCallback=ptrfunc;
}

// process BUFFER_FRAME sample and call the callback when needed
void AudioEngine::processBuffer(int len)
{
  //for (int i=0;i<BUFFER_FRAME;i++)
  for (int i=0;i<len;i++)
    {
      nb_tick++;
      if (nb_tick<nb_tick_before_step_change)
	buffer_out[i]=AM.tick();
      else
        {
          //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!CALLL\n");                                                                                                                      
          //PP->seq_callback_update_step();
	  if (seqCallback)
	    (*seqCallback)();
          nb_tick=0;
	  buffer_out[i]=AM.tick();
        }
    }
  bufferGenerated=0;
}


int AudioEngine::bufferIsGenerated()
{
  return bufferGenerated;
}

Sint16 * AudioEngine::getBufferOut()
{
  return buffer_out;
}



/*
void AudioEngine::setSynthFreq(int sfreq)
{
  S.setFreq(sfreq);
}
*/

AudioMixer & AudioEngine::getAudioMixer()
{
  return AM;
}

void AudioEngine::setDefault()
{
  freq=DEFAULTFREQ;
  samples=DEFAULTSAMPLES;
  channels=DEFAULTCHANNELS;
  polyphony=DEFAULTPOLYPHONY;
}

int AudioEngine::getTick()
{
  return tick;
}


void AudioEngine::setTick(int t)
{
  tick=t;
}



//void AudioEngine::sdlcallback(void *unused, Uint8 *stream, int len)
void AudioEngine::callback(void *unused, Uint8 *stream, int len)
{
  //  printf("AudioEngine::calback() begin nBufferFrame=%d nbCallback=%d\n",nBufferFrames,nbCallback);
  //printf("AudioEngine::callback() len=%d\n",len);
  int     buffer_size;
  //int     buffer_size=len;

  //Workaround a linux sdl 1.2 audio bug 
  //                   sdl seem to have a bug on this...
  #ifdef __SDL_AUDIO__
  buffer_size=BUFFER_FRAME;
  #endif

  //Should be the "Normal case" because we use ...,int len) provided by the caller
  #ifdef __RTAUDIO__
  buffer_size=len;
  #endif

  typedef Sint16 MY_TYPE;
  MY_TYPE *buffer = (MY_TYPE *) stream;
  
  //  this->bufferGenerated=0;
  if (bufferGenerated==0)
    //this->processBuffer(BUFFER_FRAME);
    this->processBuffer(buffer_size);

  #ifdef LINUX_DESKTOP
  if (debug_audio)
    fwrite(buffer_out,buffer_size,sizeof(Sint16),fd);
  #endif

    
    //for (int i=0;i<len;i++)
    for (int i=0;i<buffer_size;i++)
    {
      //int tick = S.tick();
      //      int tick = AM.tick();
      //Sint16 tick = AM.tick();
      tick=buffer_out[i];
      //      printf("%d\n",tick);
      /*
      #ifdef LINUX_DESKTOP
      if (debug_audio)
	{	  
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  //printf("%d\t",tick);
	}
      #endif
      */
      buffer[(2*i)]=    tick;
      buffer[(2*i)+1]=  tick;

      //buffer[i+1]=tick;
      //buffer[i+1]=0;
      i++;
    }
  //if (debug_audio)
  //fwrite(buffer,sizeof(MY_TYPE)*nBufferFrames,sizeof(MY_TYPE),fd);

  //  printf("AudioEngine::calback() end\n");
  //bufferGenerated=0;
  //return nbCallback++;  
}

void sdlcallback(void *unused, Uint8 *stream, int len)
{
  ((AudioEngine*)unused)->callback(unused,stream,len);
}


int rtcallback( 
	       void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *user_data )
{
  //printf("callback\n");
  ((AudioEngine*)user_data)->callback(user_data,
				      (Uint8*)outputBuffer,
				      nBufferFrames);

  return 0;
}



 int AudioEngine::startAudio()
 {
  //return 1;
  //SDL_PauseAudio(0);
   AD.startAudio();
}

// enable audio callback
//int AudioEngine::startAudio()
 //{
  //dac.startStream();
 //}

// disable audio callback
int AudioEngine::stopAudio()
{
  //  dac.stopStream();

}


// disable audio callback
int AudioEngine::stopAudioSdl()
{
  //SDL_CloseAudio();
  //return 1;
  //SDL_PauseAudio(1);
  AD.stopAudio();
}

int AudioEngine::openAudio()
{
  AD.openAudio();
}

int AudioEngine::closeAudio()
{
  //dac.closeStream();  
  AD.closeAudio();
}

int AudioEngine::closeAudioSdl()
{
  //SDL_LockAudio();
  //SDL_CloseAudio();
  //SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

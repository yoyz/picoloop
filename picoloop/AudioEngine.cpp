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
			     buffer_out( new Sint16[BUFFER_FRAME])
{
  freq=DEFAULTFREQ;
  samples=DEFAULTSAMPLES;
  channels=DEFAULTCHANNELS;
  polyphony=DEFAULTPOLYPHONY;
  tick=0;

  FORMAT=RTAUDIO_SINT16;
  bufferFrames = 512;          // Weird ?
  bufferFrames = BUFFER_FRAME; // Weird ?
  /*
  sdlAudioSpecWanted   = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));  
  sdlAudioSpecObtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));

  if (sdlAudioSpecWanted==NULL)
    exit(1);

  if (sdlAudioSpecObtained==NULL)
    exit(1);

  sdlAudioSpecWanted->freq = 48000;
  sdlAudioSpecWanted->format = AUDIO_S16;
  sdlAudioSpecWanted->channels = 2;    /* 1 = mono, 2 = stereo */
  //sdlAudioSpecWanted->samples = BUFFER_FRAME;  /* Good low-latency value for callback */
  //  sdlAudioSpecWanted->callback = fakesdlcallback;
  //  sdlAudioSpecWanted->userdata = this;

  /*
  rtAudioOutputParams.deviceId=dac.getDefaultOutputDevice();
  rtAudioOutputParams.firstChannel=0;
  rtAudioOutputParams.nChannels=2;

  rtAudioStreamOptions.flags  = RTAUDIO_HOG_DEVICE;
  rtAudioStreamOptions.flags |= RTAUDIO_SCHEDULE_REALTIME;
  */

  /*
  SineOscillator S;
  S.setFreq(110);
  S.setAmplitude(127);
  */
  //AM=new AudioMixer;
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

/*void AudioEngine::processBuffer()
{
  for (int i=0;i<BUFFER_FRAME-1;i++)
    {
      buffer_out[i]=AM.tick();      
    }
  bufferGenerated=0;
  }*/


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
void AudioEngine::processBuffer()
{
  for (int i=0;i<BUFFER_FRAME-1;i++)
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
/*
int AudioEngine::probeDevice()
{
    // Create an api map.
  std::map<int, std::string> apiMap;
  apiMap[RtAudio::MACOSX_CORE] = "OS-X Core Audio";
  apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
  apiMap[RtAudio::WINDOWS_DS] = "Windows Direct Sound";
  apiMap[RtAudio::UNIX_JACK] = "Jack Client";
  apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtAudio::LINUX_PULSE] = "Linux PulseAudio";
  apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
  apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";

  std::vector< RtAudio::Api > apis;
  RtAudio :: getCompiledApi( apis );
  
  std::cout << "\nCompiled APIs:\n";
  for ( unsigned int i=0; i<apis.size(); i++ )
    std::cout << "  " << apiMap[ apis[i] ] << std::endl;
  
  RtAudio audio;
  RtAudio::DeviceInfo info;
  
  std::cout << "\nCurrent API: " << apiMap[ audio.getCurrentApi() ] << std::endl;
  
  unsigned int devices = audio.getDeviceCount();
  std::cout << "\nFound " << devices << " device(s) ...\n";
  if (devices<0) { return -1; }

  for (unsigned int i=0; i<devices; i++) {
    info = audio.getDeviceInfo(i);

    std::cout << "\nDevice Name = " << info.name << '\n';
    if ( info.probed == false )
      std::cout << "Probe Status = UNsuccessful\n";
    else {
      std::cout << "Probe Status = Successful\n";
      std::cout << "Output Channels = " << info.outputChannels << '\n';
      std::cout << "Input Channels = " << info.inputChannels << '\n';
      std::cout << "Duplex Channels = " << info.duplexChannels << '\n';
      if ( info.isDefaultOutput ) std::cout << "This is the default output device.\n";
      else std::cout << "This is NOT the default output device.\n";
      if ( info.isDefaultInput ) std::cout << "This is the default input device.\n";
      else std::cout << "This is NOT the default input device.\n";
      if ( info.nativeFormats == 0 )
        std::cout << "No natively supported data formats(?)!";
      else {
        std::cout << "Natively supported data formats:\n";
        if ( info.nativeFormats & RTAUDIO_SINT8 )
          std::cout << "  8-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT16 )
          std::cout << "  16-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT24 )
          std::cout << "  24-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT32 )
          std::cout << "  32-bit int\n";
        if ( info.nativeFormats & RTAUDIO_FLOAT32 )
          std::cout << "  32-bit float\n";
        if ( info.nativeFormats & RTAUDIO_FLOAT64 )
          std::cout << "  64-bit float\n";
      }
      if ( info.sampleRates.size() < 1 )
        std::cout << "No supported sample rates found!";
      else {
        std::cout << "Supported sample rates = ";
        for (unsigned int j=0; j<info.sampleRates.size(); j++)
          std::cout << info.sampleRates[j] << " ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;

  return 0;
}
*/
/*
void AudioEngine::setEngineFreq(int frequency)
{
  
}
*/

int AudioEngine::getTick()
{
  return tick;
}


void AudioEngine::setTick(int t)
{
  tick=t;
}
/*
void fake_sdl_audio_callback(void *user_data, Uint8 *audio, int length)
{
  //printf("simple sdl_callback\n");
  ((AudioEngine*)user_data)->sdl_audio_callback(user_data,audio,length);
}
*/
 /*
void AudioEngine::sdl_audio_callback(void *user_data, Uint8 *audio, int length)
{
  tick++;
  printf("sdl_callback %d\n",tick);
  //inst.generateSamples(audio,length);

}
 */
//*/

  //unsigned int channel = 1;

/*
int mycallback( 
	       void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *data )
{
  printf("callback\n");
  
  return 0;
}
*/




 /*
int AudioEngine::callback( void *outputBuffer, 
			   void *inputBuffer, 
			   unsigned int nBufferFrames,
			   double streamTime, 
			   RtAudioStreamStatus status, 
			   void *user_data )
{
  //  printf("AudioEngine::calback() begin nBufferFrame=%d nbCallback=%d\n",nBufferFrames,nbCallback);
  typedef Sint16 MY_TYPE;
  MY_TYPE *buffer = (MY_TYPE *) outputBuffer;
  
  //  this->bufferGenerated=0;
  if (bufferGenerated==0)
    this->processBuffer();

#ifdef LINUX_DESKTOP
  if (debug_audio)
    fwrite(buffer_out,nBufferFrames,sizeof(Sint16),fd);
#endif

  
  for (int i=0;i<nBufferFrames-1;i++)
    {
      //int tick = S.tick();
      //      int tick = AM.tick();
      //Sint16 tick = AM.tick();
      tick=buffer_out[i];
      //      printf("%d\n",tick);

      #ifdef LINUX_DESKTOP
      if (debug_audio)
	{	  
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  //printf("%d\t",tick);
	}
      #endif

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
  return nbCallback++;  
}
*/





  //void AudioEngine::sdlcallback(void *unused, Uint8 *stream, int len)
void AudioEngine::callback(void *unused, Uint8 *stream, int len)
{
  //  printf("AudioEngine::calback() begin nBufferFrame=%d nbCallback=%d\n",nBufferFrames,nbCallback);
  typedef Sint16 MY_TYPE;
  MY_TYPE *buffer = (MY_TYPE *) stream;
  
  //  this->bufferGenerated=0;
  if (bufferGenerated==0)
    this->processBuffer();

#ifdef LINUX_DESKTOP
  if (debug_audio)
    fwrite(buffer_out,len,sizeof(Sint16),fd);
#endif

  
  for (int i=0;i<len-1;i++)
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



/*
int fakecallback( 
	       void *outputBuffer, 
	       void *inputBuffer, 
	       unsigned int nBufferFrames,
	       double streamTime, 
	       RtAudioStreamStatus status, 
	       void *user_data )
{
  //printf("callback\n");
  ((AudioEngine*)user_data)->callback(outputBuffer,
				      inputBuffer,
				      nBufferFrames,
				      streamTime,
				      status,
				      user_data);
      return 0;
    }
*/


//void AudioEngine::set_instrument(Instrument instru)
//{
//  inst=inst;
//}
/*
void AudioEngine::callback()
{
  printf("AudioEngine::callback\n");  
}
*/


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

/*
int AudioEngine::openAudioSdl()
{
  SDL_InitSubSystem(SDL_INIT_AUDIO);
  if ( SDL_OpenAudio(sdlAudioSpecWanted, sdlAudioSpecObtained) < 0 ) 
    {
      fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
      return(-1);
    }
  //SDL_PauseAudio(1);
}
*/

 /*
int AudioEngine::openAudio()
{
  //int FORMAT=RTAUDIO_SINT16;
  //  int bufferFrames = 512;
  double *data = (double *) calloc( channels, sizeof( double ) );

    if ( dac.getDeviceCount() < 1 ) 
      {
	std::cout << "\nNo audio devices found!\n";
	exit( 1 );
      }
    else
      {

//	dac.openStream( &rtAudioOutputParams, 
//			NULL, 
//			FORMAT, 
//			DEFAULTFREQ, 
//			&bufferFrames, 			
//			&AudioEngine::callback,
//			(void *)this, 
//			&rtAudioStreamOptions );

		
	dac.openStream( &rtAudioOutputParams, 
			NULL, 
			FORMAT, 
			DEFAULTFREQ, 
			&bufferFrames, 
			&fakecallback,
			//&this->callback,
			(void *)this, 
			&rtAudioStreamOptions );
	
	//dac.startStream();  
	
      }
    return 0;
}
 */

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

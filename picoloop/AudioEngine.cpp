#include "AudioEngine.h"
#include "SineOscillator.h"

AudioEngine::AudioEngine() : inst(), AM()
{
  freq=DEFAULTFREQ;
  samples=DEFAULTSAMPLES;
  channels=DEFAULTCHANNELS;
  polyphony=DEFAULTPOLYPHONY;
  tick=0;

  FORMAT=RTAUDIO_SINT16;
  bufferFrames = 512;          // Weird ?
  bufferFrames = BUFFER_FRAME; // Weird ?

  rtAudioOutputParams.deviceId=dac.getDefaultOutputDevice();
  rtAudioOutputParams.firstChannel=0;
  rtAudioOutputParams.nChannels=2;

  rtAudioStreamOptions.flags  = RTAUDIO_HOG_DEVICE;
  rtAudioStreamOptions.flags |= RTAUDIO_SCHEDULE_REALTIME;
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
  buffer_out=(Sint16*)malloc(sizeof(Sint16)*BUFFER_FRAME);
}

int AudioEngine::getNbCallback()
{
  return nbCallback;
}

void AudioEngine::processBuffer()
{
  for (int i=0;i<BUFFER_FRAME-1;i++)
    {
      buffer_out[i]=AM.tick();      
    }
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

void AudioEngine::sdl_audio_callback(void *user_data, Uint8 *audio, int length)
{
  tick++;
  printf("sdl_callback %d\n",tick);
  //inst.generateSamples(audio,length);

}
*/

unsigned int channel = 1;

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





int AudioEngine::callback( void *outputBuffer, 
			   void *inputBuffer, 
			   unsigned int nBufferFrames,
			   double streamTime, 
			   RtAudioStreamStatus status, 
			   void *user_data )
{
  //printf("AudioEngine::calback nBufferFrame=%d nbCallback=%d\n",nBufferFrames,nbCallback);
  typedef Sint16 MY_TYPE;
  MY_TYPE *buffer = (MY_TYPE *) outputBuffer;
  
  this->processBuffer();
  
  for (int i=0;i<nBufferFrames-1;i++)
    {
      //int tick = S.tick();
      //      int tick = AM.tick();
      //Sint16 tick = AM.tick();
      tick=buffer_out[i];
      //      printf("%d\n",tick);
      if (debug_audio)
	{
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  fwrite(&tick,1,sizeof(Sint16),fd);
	  //printf("%d\t",tick);
	}

      buffer[(2*i)]=    tick;
      buffer[(2*i)+1]=  tick;

      //buffer[i+1]=tick;
      //buffer[i+1]=0;
      i++;
    }
  //if (debug_audio)
  //fwrite(buffer,sizeof(MY_TYPE)*nBufferFrames,sizeof(MY_TYPE),fd);

  return nbCallback++;  
}

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



void AudioEngine::set_instrument(Instrument instru)
{
  inst=inst;
}
/*
void AudioEngine::callback()
{
  printf("AudioEngine::callback\n");  
}
*/


int AudioEngine::startAudio()
{
  dac.startStream();
}


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

int AudioEngine::closeAudio()
{
  
}

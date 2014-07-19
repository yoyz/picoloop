#include "AudioDriverRTAudio.h"



AudioDriver::AudioDriver()
{
  bufferFrames = BUFFER_FRAME;
  rtAudioOutputParams.deviceId=dac.getDefaultOutputDevice();
  rtAudioOutputParams.firstChannel=0;
  rtAudioOutputParams.nChannels=2;

  rtAudioStreamOptions.flags  = RTAUDIO_HOG_DEVICE;
  rtAudioStreamOptions.flags |= RTAUDIO_SCHEDULE_REALTIME;
}


int AudioDriver::startAudio()
{
  dac.startStream();
}

int AudioDriver::stopAudio()
{
  dac.stopStream();
}

int AudioDriver::openAudio()
{

  int FORMAT=RTAUDIO_SINT16;
  //  int bufferFrames = 512;
  //double *data = (double *) calloc( channels, sizeof( double ) );

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
			internal_callback,
			//&this->callback,
			(void *)userdata,
			&rtAudioStreamOptions );
	
	//dac.startStream();  
	
      }
    return 0;


}


int AudioDriver::closeAudio()
{
  //#ifdef OPENDINGUX
  dac.closeStream();  
  //SDL_CloseAudio();
  //#endif
  //SDL_CloseAudio();
  //  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

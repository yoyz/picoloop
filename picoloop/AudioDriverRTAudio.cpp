#include "AudioDriverRTAudio.h"



AudioDriver::AudioDriver()
{
  bufferFrames = BUFFER_FRAME;

  // this could be override because now it is chooseable  by
  rtAudioOutputParams.deviceId=dac.getDefaultOutputDevice();
  rtAudioOutputParams.firstChannel=0;
  rtAudioOutputParams.nChannels=2;

  //rtAudioStreamOptions.flags  = RTAUDIO_HOG_DEVICE;
  //rtAudioStreamOptions.flags |= RTAUDIO_SCHEDULE_REALTIME;
}

void AudioDriver::setAudioOutput(int deviceNumber)
{
  if (deviceNumber >= 0 &&
      deviceNumber <= this->getNumberOfAudioOutputDevice())
    rtAudioOutputParams.deviceId=deviceNumber;
  else   
    rtAudioOutputParams.deviceId=dac.getDefaultOutputDevice();
}

int AudioDriver::startAudio()
{
  dac.startStream();
  return 0;
}

int AudioDriver::stopAudio()
{
  dac.stopStream();
  return 0;
}

char * AudioDriver::getAudioOutputName(int deviceNumber)
{
  static char audioOutputName[128];
  RtAudio::DeviceInfo rtdi;
  rtdi=dac.getDeviceInfo(deviceNumber);
  strcpy(audioOutputName,rtdi.name.c_str());
  //printf("****%s*****\n",rtdi.name.c_str());
  return audioOutputName;
}

int AudioDriver::getNumberOfAudioOutputDevice()
{
  // dac.getDeviceCount() return count+1
  return dac.getDeviceCount() - 1;
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
	std::cout << "\nOpening audio devices\n";
	try {
	  dac.openStream( &rtAudioOutputParams, 
			  NULL, 
			  FORMAT, 
			  DEFAULTFREQ, 
			  &bufferFrames, 
			  internal_callback,
			  //&this->callback,
			  (void *)userdata,
			  &rtAudioStreamOptions,
			  NULL);
	}
	catch ( RtAudioError& e ) 
	  {
	    e.printMessage();
	    exit( 1 );
	  }
	
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
  return 0;
}

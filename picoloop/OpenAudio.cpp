#include "Master.h"
#include "AudioEngine.h"

AudioEngine    AE;          // used to  init alsa/rtaudio

void donothing() {}

void openaudio()
{
  AE.setupSequencerCallback(donothing);
  AE.setNbTickBeforeStepChange(1000);
  AE.setAudioOutput(0);
  AE.openAudio();
  printf("openAudio start streaming\n");
  AE.startAudio();
}


int main(int argc,char ** argv)
{	  
  openaudio();
}

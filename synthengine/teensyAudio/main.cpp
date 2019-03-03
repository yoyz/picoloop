#include "AudioStream.h"
#include "synth_waveform.h"
#include "mixer.h"
AudioSynthWaveform       voice1; 
AudioMixer4              mixer1; 
AudioConnection          patchCord1(voice1, 0, mixer1, 0);
int main()
{
  audio_block_t * blk;
  //AudioMemory(160);
  voice1.begin(.3,440,WAVEFORM_SQUARE);
  mixer1.gain(3,0);
  mixer1.update();
  mixer1.update();
  mixer1.update();
  mixer1.update();
  blk = mixer1.receiveReadOnly(0);
  printf("%08.8X \n",blk->data);
  
  return 0;
}

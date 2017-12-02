//#include "dspinst.h"
#include <stdio.h>
#include "synth_waveform.h"
#include "AudioStream.h"

int main()
{
  int16_t a16;
  int16_t b16;
  int32_t c32;
  audio_block_t blk[20];  
  AudioSynthWaveform asw;
  int i;
  for (i=0;i<19;i++)
    memset(blk[i].data,0,AUDIO_BLOCK_SAMPLES*2);
  printf("");
  asw.initialize_memory(blk,20);
  //AudioMemory(20);
  //audio_block_t data[64];
  //AudioStream::initialize_memory(data,64);

  asw.begin(WAVEFORM_SAWTOOTH);
  asw.frequency(440);
  asw.update();
  //printf("%d",signed_saturate_rshift(-32,16,1));
  //blk=asw.receiveReadOnly();
  //printf("%d",blk->data[0]);
  return 0;
}

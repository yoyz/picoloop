#include <stdio.h>
#include "synth_waveform.h"
#include "synth_pwm.h"
#include "AudioStream.h"
AudioSynthWaveform asw;
AudioSynthWaveformPWM pwm;

int main()
{
  audio_block_t * blk_rcv;
  static audio_block_t blklst[20];
  AudioStream::initialize_memory(blklst, 20);
  asw.begin(WAVEFORM_SAWTOOTH);
  asw.begin(WAVEFORM_TRIANGLE);
  asw.frequency(440);
  asw.amplitude(0.75);
  asw.pulseWidth(0.15);

  pwm.frequency(440);
  pwm.amplitude(0.75);
    
  FILE * FD = fopen("file","w");
  while(1)
    {
      asw.update();
      fwrite(blklst[19].data,128,1,FD);
    }
  return 0;
}

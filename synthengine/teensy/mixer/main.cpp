//#include "dspinst.h"
#include <stdio.h>
#include "synth_waveform.h"
#include "synth_pwm.h"
#include "AudioStream.h"
#include "mixer.h"
/*
#define AudioMemory(num) ({					\
			 static audio_block_t data[num];	\
			 AudioStream::initialize_memory(data, num);	\
})
*/
AudioSynthWaveform    asw;
AudioSynthWaveformPWM pwm;
AudioMixer4           am4;

AudioConnection          patchCord1(asw, 0, am4, 1);
AudioConnection          patchCord2(pwm, 0, am4, 2);

int main()
{
  int16_t a16;
  int16_t b16;
  int32_t c32;
  audio_block_t * blk_rcv;
  int i;
  static audio_block_t blklst[1];
  i=280;
  AudioStream::initialize_memory(blklst, 20);
  asw.begin(WAVEFORM_SAWTOOTH);
  asw.begin(WAVEFORM_TRIANGLE);
  asw.frequency(220);
  asw.amplitude(0.25);
  asw.pulseWidth(0.15);

  pwm.frequency(i);
  pwm.amplitude(0.25);
    
  FILE * FD = fopen("file","w");
  //for (i=0;i<1000;i++)
  while(1)
    {
      asw.update();
      pwm.update();
      am4.update();
      pwm.frequency(i);
      i++;
      if (i>440) i=280;
      printf("%d %d %d %d\n",
	     blklst[19].data[0],
	     blklst[19].data[1],
	     blklst[19].data[2],
	     blklst[19].data[3]
	     );
      fwrite(blklst[19].data,128,1,FD);
    }
  return 0;
}

//#include "dspinst.h"
#include <stdio.h>
#include "synth_waveform.h"
#include "AudioStream.h"
/*
#define AudioMemory(num) ({					\
			 static audio_block_t data[num];	\
			 AudioStream::initialize_memory(data, num);	\
})
*/
AudioSynthWaveform asw;

int main()
{
  int16_t a16;
  int16_t b16;
  int32_t c32;
  audio_block_t * blk_rcv;
  int i;
  static audio_block_t blklst[20];
  AudioStream::initialize_memory(blklst, 20);
  asw.begin(WAVEFORM_SAWTOOTH);
  asw.begin(WAVEFORM_TRIANGLE);
  asw.frequency(440);
  asw.amplitude(0.75);
  asw.pulseWidth(0.15);
  FILE * FD = fopen("file","w");
  //for (i=0;i<1000;i++)
  while(1)
    {
      //asw.frequency(i);
      asw.update();
      //i++;
      //if (i>1000) i=100;
      //blk_rcv=asw.receiveReadOnly();
      //printf("%d ",blk_rcv->data[0]);
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

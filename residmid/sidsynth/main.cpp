#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "sid.h"

#define SAMPLERATE 44118
#define CLOCKFREQ (22.5 * SAMPLERATE) //nearest int to 985248
#define AUDIO_BLOCK_SAMPLES 8192
int main()
{
  float                      	* out_buffer_f;
  int16_t 			* out_buffer_i;
  
  int n_frames=256;
  int i;
  int j=255;
  int fd;
  int polyphony=1;
  
  FILE     * FD; 
  SID  mysid;
  cycle_count cycles;
  //cycle_count delta_t = CLOCKFREQ / (SAMPLERATE / AUDIO_BLOCK_SAMPLES);
  cycle_count delta_t = CLOCKFREQ / (SAMPLERATE / AUDIO_BLOCK_SAMPLES);
  int nbsample=0;

  //cycle_count delta_t=AUDIO_BLOCK_SAMPLES;

  out_buffer_f=(float*)malloc(sizeof(float)*8192);
  out_buffer_i=(int16_t*)malloc(sizeof(int16_t*)*8192);
  
  //mysid.set_chip_model(MOS8580);
  mysid.set_chip_model(MOS6581);

  mysid.set_sampling_parameters(CLOCKFREQ, SAMPLE_FAST, SAMPLERATE);
  mysid.reset();
  /*
  mysid.write(0x18, 0x08);  // MODE/VOL

  mysid.write(0x00,0x0);    // v1 freq lo
  mysid.write(0x01,0x4);    // v1 freq hi
  
  mysid.write(0x05,0x0F);    // ATK/DCY
  mysid.write(0x06,0xFF);    // STN/RLS
  mysid.write(0x04,0x21);    // CONTROL
  */

  //mysid.write(0x18, 0x08);  // MODE/VOL
  mysid.write(0x18, 0xEF);  // MODE/VOL
  mysid.write(0x0e,0x0);    // v1 freq lo
  mysid.write(0x0f,0x3);    // v1 freq hi
  
  mysid.write(0x13,0x0F);    // ATK/DCY
  mysid.write(0x14,0xFF);    // STN/RLS
  mysid.write(0x10,0x7F);    // PULSEWIDTH
  mysid.write(0x12,0x41);    // CONTROL
  mysid.write(0x17,0x07);    // filter
  mysid.write(0x16,j--);    // Cutoff
  FD=fopen("audioout","w+");
  for (i=0;i<255;i++)
    {
      delta_t=CLOCKFREQ / (SAMPLERATE / AUDIO_BLOCK_SAMPLES);
      nbsample=mysid.clock(delta_t,out_buffer_i,AUDIO_BLOCK_SAMPLES);
      mysid.write(0x16,j--);    // Cutoff
      mysid.write(0x10,j);    // PULSEWIDTH
      printf("nbsample:%d delta_t:%d j:%d\n",nbsample,delta_t,j);
      fwrite(out_buffer_i,sizeof(int16_t),8192,FD);
    }
  fclose(FD);
}

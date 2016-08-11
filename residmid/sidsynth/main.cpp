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
  int j;
  int fd;
  int polyphony=1;
  
  FILE     * FD; 
  SID  mysid;
  cycle_count cycles;
  cycle_count delta_t = CLOCKFREQ / (SAMPLERATE / AUDIO_BLOCK_SAMPLES);

  out_buffer_f=(float*)malloc(sizeof(float)*8192);
  out_buffer_i=(int16_t*)malloc(sizeof(int16_t*)*8192);
  
  mysid.set_chip_model(MOS8580);
  mysid.set_sampling_parameters(CLOCKFREQ, SAMPLE_FAST, SAMPLERATE);
  mysid.reset();
  
  mysid.write(0x18, 0x0f);  // MODE/VOL
  mysid.write(0x00,0xF);    // v1 freq lo
  mysid.write(0x01,0x1);    // v1 freq hi
  
  mysid.write(0x05,0xE);    // ATK/DCY
  mysid.write(0x06,0xF);    // STN/RLS
  mysid.write(0x04,0x1);    // CONTROL

  FD=fopen("audioout","w+");
  mysid.clock(delta_t,out_buffer_i,AUDIO_BLOCK_SAMPLES);
  fwrite(out_buffer_i,sizeof(int16_t),8192,FD);
  fclose(FD);
}

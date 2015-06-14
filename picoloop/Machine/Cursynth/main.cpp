#include <stdio.h>
#include "cursynth_engine.h"
#include <SDL/SDL.h>
int main()
{
  mopocursynth::mopo_float 	* out_buffer_f;
  Sint16 			* out_buffer_i;
  
  int n_frames=256;
  int i;
  int j;
  int fd;
  out_buffer_f=(mopocursynth::mopo_float*)malloc(sizeof(mopocursynth::mopo_float)*8192);
  out_buffer_i=(Sint16*)malloc(sizeof(Sint16*)*8192);
  FILE     * FD;
  
  mopocursynth::CursynthEngine CS;
  //CS.setBufferSize(8192);
  
  CS.setBufferSize(256);
  CS.setSampleRate(44100);
  CS.noteOn(40,1.0);
  //CS.processAudio((mopocursynth::mopo_float*)out_buffer, n_frames);
  FD=fopen("audioout","w+");
  
  for (j=0;j<256;j++)
    {
      CS.process();
      for (i=0;i<n_frames;i++)
	{
	  printf("%f\t",CS.output()->buffer[i]);
	  out_buffer_f[i]=CS.output()->buffer[i];
	  out_buffer_f[i]=out_buffer_f[i]*16384;
	  out_buffer_i[i]=out_buffer_f[i];
	}
      fwrite(out_buffer_i,sizeof(Sint16),256,FD);
    }
  fclose(FD);
}

#include <stdio.h>
#include "cursynth_engine.h"
#include <SDL/SDL.h>
int main()
{
  mopo::mopo_float 	* out_buffer_f;
  Sint16 			* out_buffer_i;
  
  int n_frames=1024;
  int i;
  int j;
  int k;
  int fd;
  out_buffer_f=(mopo::mopo_float*)malloc(sizeof(mopo::mopo_float)*8192);
  out_buffer_i=(Sint16*)malloc(sizeof(Sint16*)*8192);
  FILE     * FD;
  
  mopo::CursynthEngine CS;
  //CS.setBufferSize(8192);
  
  CS.setBufferSize(n_frames);
  CS.setSampleRate(44100);
  CS.noteOn(50,1.0);
  CS.getControls().at("volume")->set(0.1);
  CS.getControls().at("osc 2 transpose")->set(0); 
  CS.getControls().at("osc 2 tune")->set(0); 


  CS.getControls().at("amp attack")->set(1);
  CS.getControls().at("amp decay")->set(1);
  CS.getControls().at("amp sustain")->set(0.4);
  CS.getControls().at("amp release")->set(2);


  //CS.processAudio((mopo::mopo_float*)out_buffer, n_frames);
  FD=fopen("audioout","w+");
  
  //for (k=0;k<1000;k++)
  for (j=0;j<16*1024;j++)
    {
      CS.process();
      CS.getControls().at("cutoff")->set(28+j%128);

      for (i=0;i<n_frames;i++)
	{
	  //printf("%f\t",CS.output()->buffer[i]);
	  out_buffer_f[i]=CS.output()->buffer[i];
	  out_buffer_f[i]=out_buffer_f[i]*8192;
	  out_buffer_i[i]=out_buffer_f[i];
	}
      fwrite(out_buffer_i,sizeof(Sint16),n_frames,FD);
    }
  fclose(FD);
}

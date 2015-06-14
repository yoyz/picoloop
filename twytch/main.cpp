#include <stdio.h>
#include "helm_engine.h"
#include <SDL/SDL.h>
int main()
{
  mopo::mopo_float      * out_buffer_f;
  Sint16                        * out_buffer_i;

  int n_frames=256;
  int i;
  int j;
  int fd;
  out_buffer_f=(mopo::mopo_float*)malloc(sizeof(mopo::mopo_float)*8192);
  out_buffer_i=(Sint16*)malloc(sizeof(Sint16*)*8192);
  FILE     * FD;

  mopo::HelmEngine HE;
  //CS.setBufferSize(8192);

  HE.setBufferSize(256);
  HE.setSampleRate(44100);
  HE.noteOn(40,1.0);
  //CS.processAudio((mopo::mopo_float*)out_buffer, n_frames);
  FD=fopen("audioout","w+");

  for (j=0;j<256;j++)
    {
      HE.process();
      for (i=0;i<n_frames;i++)
        {
          printf("%f\t",HE.output()->buffer[i]);
          out_buffer_f[i]=HE.output()->buffer[i];
          out_buffer_f[i]=out_buffer_f[i]*16384;
          out_buffer_i[i]=out_buffer_f[i];
        }
      fwrite(out_buffer_i,sizeof(Sint16),256,FD);
    }
  fclose(FD);
}

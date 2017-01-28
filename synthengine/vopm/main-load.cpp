#include "OPM.hpp"
#include "OPMdrv.hpp"
#include <stdlib.h>
#include <stdio.h>

int main()
{
  class Opm *pOPM;
  class OPMDRV *pOPMdrv;

  //int     nbuf=8192;
  int     nbuf=1024*128;
  //float   buffer[2][nbuf];
  float ** f_buffer;
  int    * i_buffer;
  int i=0;
  float tmp_f;

  FILE     * FD;
  
  
  f_buffer=(float**)malloc(sizeof(float*)*2);
  f_buffer[0]=(float*)malloc(sizeof(float)*nbuf);
  f_buffer[1]=(float*)malloc(sizeof(float)*nbuf);
  i_buffer=(int*)malloc(sizeof(int)*nbuf);

  pOPM= new class Opm;
  pOPMdrv=new class OPMDRV(pOPM);
  pOPM->StartPcm(44100,1,0,0);
  //pOPMdrv->Save("test.opm");
  pOPMdrv->Load("test5.opm");
  pOPMdrv->Save("test3.opm");
  //pOPMdrv->setProgNum(0);
  pOPMdrv->setProgNum(5);
  printf("%d:%s\n",pOPMdrv->getProgNum(),pOPMdrv->getProgName());
  //pOPM->pcmset22(sampleFrames,outputs,false);
  pOPMdrv->NoteOn(1,64,64);
  pOPM->pcmset22(nbuf,f_buffer,false);
  
  for(i=0;i<nbuf;i++)
    {
      //printf("%f\t",f_buffer[0][i]);
      //tmp_f=f_buffer[0][i]*32768;
      tmp_f=f_buffer[0][i]*32768*16;
      i_buffer[i]=tmp_f;
    }

  FD=fopen("audioout","w+");
  fwrite(i_buffer,sizeof(int),nbuf,FD);
  fclose(FD);

  return 0;
}

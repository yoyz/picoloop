//#include "dbopl.h"

//#include "dbopl.h"
#include "hiopl.h"
#include <stdio.h>
#include <stdint.h>
#define SAM 2048

int main()
{
  //  short buff[8192];  
  short buff[SAM];
  Hiopl ho(44100);
  int16_t s;
  int i=0;

  FILE * F;

  for (i=0;i<SAM;i++)
    buff[i]=0.0;

  
  ho.SetSampleRate(44100);
  //  ho.EnableWaveformControl();
  ho.SetWaveform(1,1,SIN);
  ho.SetWaveform(1,2,HALF_SIN);

  ho.SetFrequencyMultiple(1,1,x3);
  ho.SetFrequencyMultiple(1,2,x10);

  ho.SetEnvelopeAttack(1,1,6);
  ho.SetEnvelopeAttack(1,2,6);

  ho.SetEnvelopeDecay(1,1,6);
  ho.SetEnvelopeDecay(1,2,6);

  ho.SetEnvelopeSustain(1,1,6);
  ho.SetEnvelopeSustain(1,2,6);

  ho.SetEnvelopeRelease(1,1,6);
  ho.SetEnvelopeRelease(1,2,6);

  ho.KeyOn(1,200.0);

  ho.Generate(1024,buff);

  for (i=0;i<SAM;i++)
    printf("%d\t",(int16_t)buff[i]);
  
  F=fopen("audioout","w+");
  
  for (i=0;i<SAM;i++)
    {
      s=(int16_t)buff[i]*255;
      fwrite(&s,1,sizeof(int16_t),F);
    }
  
	//fwrite(buff,SAM,sizeof(int16_t),F);
  fclose(F);

  printf("\n");
  printf("hello\n");
}

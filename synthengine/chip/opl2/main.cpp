//#include "dbopl.h"

//#include "dbopl.h"
#include "hiopl.h"
#include <stdio.h>
#include <stdint.h>
#define SAM 1024
#define NUM 1024


int main()
{
  //  short buff[8192];  
  short buff[SAM];
  Hiopl ho(44100);
  int16_t s;
  int i=0;
  int j=0;
  int k=1;

  FILE * F;

  for (i=0;i<SAM;i++)
    buff[i]=0;

  ////  ho.EnableOpl3Mode();

  ho.SetSampleRate(44100);
  //  ho.EnableWaveformControl();
  ho.SetWaveform(1,1,SIN);
  ho.SetWaveform(1,2,QUART_SIN);

  ho.SetFrequencyMultiple(1,1,x1);

  ho.SetFrequencyMultiple(1,2,x1);

  ho.SetEnvelopeAttack(1,1,2);
  ho.SetEnvelopeDecay(1,1,3);
  ho.SetEnvelopeSustain(1,1,15);
  ho.SetEnvelopeRelease(1,1,15);

  ho.SetEnvelopeAttack(1,2,5);
  ho.SetEnvelopeDecay(1,2,2);
  ho.SetEnvelopeSustain(1,2,15);
  ho.SetEnvelopeRelease(1,2,15);

  //  ho.SetAttenuation(1, 1, 1);
  //  ho.SetAttenuation(1, 2, 1);

  //  ho.EnableSustain(1,1,false);
  //  ho.EnableSustain(1,2,true);

  ho.KeyOn(1,110.0);


  //    for (i=0;i<SAM*NUM;i++)
  //      printf("%d\t",(int16_t)buff[i]);
  
  F=fopen("audioout","w+");
  
  for (j=0;j<NUM;j++)
    {
      ho.Generate(SAM,buff);
      for (i=0;i<SAM/2;i++)
	{
	  //  printf("[%6.d]",(int16_t)buff[i]);
	}
	
      //      printf("\n{%d}\n",j);

      fwrite(buff,sizeof(int16_t),SAM/2,F);
    }
  
	//fwrite(buff,SAM,sizeof(int16_t),F);
  fclose(F);

  printf("\n");
  printf("hello\n");
}

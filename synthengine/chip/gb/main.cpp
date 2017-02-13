#include "sound.h"
#include <stdio.h>
//#include <stdint.h>

using namespace gambatte;

//#define SIZE 35100
//#define NUM  64
//#define SIZE 140448
//#define SIZEMALLOC 1024*1024*2
//#define SIZEMALLOC 140448
//#define SIZEMALLOC 35112 + 2064;
//#define SIZEMALLOC 140448
//#define SIZEMALLOC 35512

#define SIZEONE 35112
#define SIZE 140448
//#define SIZE 1024
#define NUM  2048

int main()
{
  int cycle;
  int i=0;
  int j=0;

  //  uint_least32_t buf[SIZEMALLOC];
  uint_least32_t buf[SIZE];
  FILE * F;

  for (i=0;i<SIZE;i++)
    {
      buf[i]=0;
    }

  printf("hello\n");
  PSG psg;
  psg.init(true);

  psg.setBuffer(buf);
  psg.setEnabled(true);
  psg.set_so_volume(119);
  psg.map_so(255);
  //  psg.reset();
  /*
  psg.set_nr10(0);   // NR10 FF10 -PPP NSSS    Sweep period, negate, shift
  psg.set_nr11(0);   // NR11 FF11 DDLL LLLL    Duty, Length load (64-L)
  psg.set_nr12(164); // NR12 FF12 VVVV APPP    Starting volume, Envelope add mode, period
  psg.set_nr13(44);  // NR13 FF13 FFFF FFFF    Frequency LSB
  psg.set_nr14(128); // NR14 FF14 TL-- -FFF    Trigger, Length enable, Frequency MSB
  */




  //  psg.set_nr20(0);   // NR10 FF10 -PPP NSSS    Sweep period, negate, shift
  psg.set_nr21(0);   // NR11 FF11 DDLL LLLL    Duty, Length load (64-L)
  psg.set_nr22(164); // NR12 FF12 VVVV APPP    Starting volume, Envelope add mode, period
  psg.set_nr23(44);  // NR13 FF13 FFFF FFFF    Frequency LSB
  psg.set_nr24(128); // NR14 FF14 TL-- -FFF    Trigger, Length enable, Frequency MSB
  //  psg.set_nr14(128+44); // NR14 FF14 TL-- -FFF    Trigger, Length enable, Frequency MSB



  //F=fopen("audioout","w+");
  i=0;
  for (i=0;i<NUM;i++)
    {

      for (j=0;j<SIZE;j++)
	buf[j]=0;

      //      psg.generate_samples(SIZE+(SIZE*i),0);
      psg.setBuffer(buf);
      psg.generate_samples(cycle,0);
      cycle=cycle+SIZE;
      psg.fillBuffer();  

      //      for (j=0;j<SIZE;j++)
      //      	printf("%d ",buf[j]);

      //      printf("\ni:%d\n",i);
      //      fwrite(buf,sizeof(uint_least32_t),SIZE,F);
      int k=0;                                  
      //      for (k=0;k<SIZEONE;k=k+96)              
      //	fwrite(buf+k,sizeof(  uint_least32_t ),1,F);

      if (i==40)
	{
	  psg.set_nr10(0); // NR10 FF10 -PPP NSSS    Sweep period, negate, shift
	  psg.set_nr11(190); // NR11 FF11 DDLL LLLL    Duty, Length load (64-L)
	  psg.set_nr12(164); // NR12 FF12 VVVV APPP    Starting volume, Envelope add mode, period
      	  psg.set_nr13(64); // NR13 FF13 FFFF FFFF    Frequency LSB
	  psg.set_nr14(128+16);
	}

    }


  //  for (i=0;i<SIZE;i++)
  //    {

      //printf("%.4d ",buf[i]);
  //    }
//    fclose(F);
  
}

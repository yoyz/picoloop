#include "sound.h"
#include <stdio.h>

using namespace gambatte;

#define SIZE      1024
#define NUM        128
#define BLOCKSIZE  8

int main()
{
  int i=0;
  int j=0;
  uint_least32_t buf[SIZE];
  FILE * F;
  for (i=0;i<SIZE;i++)
    {
      buf[i]=0;
    }

  printf("hello\n");
  PSG psg;
  psg.init(true);
  psg.setEnabled(true);
  psg.setBuffer(buf);
  psg.set_so_volume(0xF9);
  psg.map_so(0xFF);
  //  psg.reset();

  psg.set_nr10(0); // NR10 FF10 -PPP NSSS    Sweep period, negate, shift
  psg.set_nr11(128);        // NR11 FF11 DDLL LLLL    Duty, Length load (64-L)
  psg.set_nr12(41); // NR12 FF12 VVVV APPP    Starting volume, Envelope add mode, period
  psg.set_nr13(44); // NR13 FF13 FFFF FFFF    Frequency LSB
  psg.set_nr14(128); // NR14 FF14 TL-- -FFF    Trigger, Length enable, Frequency MSB
  //psg.set_nr14(0x7F);
  F=fopen("audioout","w+");
  for (i=0;i<NUM;i++)
    {
      psg.fillBuffer();  
      psg.generate_samples(BLOCKSIZE*1,0);


      //for (j=0;j<SIZE;j++)
      printf("[%.4x %.4x %.4x %.4x]\t",buf[i],buf[i+1],buf[i+2],buf[i+3]);
      
      //psg.set_nr14(0x7F);
      fwrite(buf+i*BLOCKSIZE,sizeof(uint_least32_t),BLOCKSIZE,F);
    }


  //  for (i=0;i<SIZE;i++)
  //    {

      //printf("%.4d ",buf[i]);
  //    }
    fclose(F);
  
}

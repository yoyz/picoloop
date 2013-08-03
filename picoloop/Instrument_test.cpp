#include "Wave.h"
#include <stdio.h>

Wave cowbell;
Instrument c;

int main()
{
  Uint32 length;
  Uint8* buf;

  cowbell.loadWave("DT-42-0.1.0/808-cowbell.wav");
  c.assignWave(cowbell);

  length=cowbell.getLength();
  printf("length:%d\n",length);


  FILE* fd=fopen("toto","w+");
  
  for (int i=0; i< length ; i++)
    {
      
      fwrite(cowbell.getBuffer(i),sizeof(Uint8),1,fd);
    }

  fclose(fd);
}

#include "rosic_Open303.h"

int main()
{

  int nbuff=16384;
  int i;
  int j;
  FILE     * FD;

  double  buffer[nbuff];
  int16_t i_buffer[nbuff];
  FD=fopen("audioout","w+");

  rosic::Open303 o303;
  o303.noteOn(30,20,0.0);

  FD=fopen("audioout","w+");



  for (j=0;j<nbuff;j++)
    {
      buffer[j]=o303.getSample()/4;
      i_buffer[j]=buffer[j]*32768;
      //buffer[i]=buffer[i]/64;
      //printf("%f\t",buffer[j]);
      printf("%d\t",i_buffer[j]);
    // 	}
    //   fwrite(buffer,sizeof(mfloat),nbuff,FD);
    //   if (j==8)
    // 	SE.releaseNote();
    // }
    }
  fwrite(i_buffer,sizeof(  int16_t),nbuff,FD);
  fclose(FD);


}

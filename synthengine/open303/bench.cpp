#include "rosic_Open303.h"

int main()
{

  int nbuff=1024;
  int i;
  int j;
  FILE     * FD;

  double  buffer[nbuff];
  int16_t i_buffer[nbuff];
  FD=fopen("audioout","w+");

  rosic::Open303 o303;
  o303.noteOn(30,20,0.0);

  //FD=fopen("audioout","w+");


for (i=0;i<16*1024;i++)
  for (j=0;j<nbuff;j++)
    {
      buffer[j]=o303.getSample()/4;
      i_buffer[j]=buffer[j]*32768;
}


}

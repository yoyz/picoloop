#include <stdio.h>
#include "drumsynth_oop5.h"
#include <string>
#include <stdlib.h>

int main(int argc,char ** argv)
{
  //printf("%d\n",ds2wav("Snare.ds","S"));
  std::string arg1;
  std::string arg2;
  std::string arg3;
  FILE    * fp;
  int16_t * buffer;
  int       len=1024;
  int       i=0;

  drumsynth dsoop;

  dsoop.init();
  
  arg1=argv[1];
  arg2=argv[2];
  arg3=argv[3];

  buffer=malloc(sizeof(int16_t)*len);


  //dsoop.setwavefile(arg2.c_str());

  //dsoop.generate(buffer,len);
  //dsoop.ds2wav(arg1.c_str(),arg2.c_str());

  fp=fopen(arg3.c_str(),"w");
  if (fp==NULL)
    exit(1);

  dsoop.load_patch(arg1.c_str());
  while (dsoop.generate(buffer,len)!=-1)
    {
      for (i=0;i<len;i++)
	printf("%d",buffer[i]);
      printf("\n");
      fwrite(buffer,sizeof(int16_t),len,fp);
    }
  dsoop.load_patch(arg2.c_str());
  while (dsoop.generate(buffer,len)!=-1)
    {
      for (i=0;i<len;i++)
	printf("%d",buffer[i]);
      printf("\n");
      fwrite(buffer,sizeof(int16_t),len,fp);
    }

  fclose(fp);
}

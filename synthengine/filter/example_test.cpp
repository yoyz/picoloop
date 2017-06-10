#include "Biquad.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024*1024*8
  
int main()
{

  FILE    * fr;
  FILE    * fw; 
  int16_t * bufin_int; 
  int16_t * bufout_int;
  float   * bufin_float; 
  float   * bufout_float;

  int       fsize;
  int16_t   v;
  float     Fc=         400.0;
  float     sampleRate=44100.0;
  int       readcount;
  int       i;


  bufin_int  = (int16_t*)malloc(sizeof(int16_t)*BUFSIZE);
  bufout_int = (int16_t*)malloc(sizeof(int16_t)*BUFSIZE);

  bufin_float  = (float*)malloc(sizeof(float)*BUFSIZE);
  bufout_float = (float*)malloc(sizeof(float)*BUFSIZE);


  fr=fopen("audioin","r");
  fw=fopen("audioout_2","w");

  readcount=1;
  i=0;
  while (readcount)
    {
      i++;
      readcount=fread(bufin_int+i,sizeof(int16_t),1,fr);
      bufin_float[i]=(float)bufin_int[i]/32768.0;
      //      printf("%d %f \n",bufin_int[i],bufin_float[i]);
    }
  fsize=i;

  i=0;



  Biquad *lpFilter = new Biquad();	// create a Biquad, lpFilter;

  lpFilter->setBiquad(bq_type_lowpass, Fc / sampleRate, 1.0, 1.0);
  lpFilter->calcBiquad();
  
  // filter a buffer of input samples, in-place
  for (int idx = 16; idx < BUFSIZE; idx++) {
    bufout_float[idx] = lpFilter->process(bufin_float[idx]);
    bufout_int[idx]=(int16_t)(bufout_float[idx]*32768.0);
    //    printf("%f %f %d %d \n",bufin_float[idx],bufout_float[idx],bufin_int[idx],bufout_int[idx]);
    fwrite(bufout_int+idx,sizeof(int16_t),1,fw);
  }

  fclose(fr);
  fclose(fw);
  
}

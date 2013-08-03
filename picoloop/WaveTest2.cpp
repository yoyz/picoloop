#include "Wave.h"
#include "AudioEngine.h"

#include "Oscillator.h"
#include "SineOscillator.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>
Wave cowbell;
AudioEngine ae;
SineOscillator s1;
SineOscillator s2;
SineOscillator s3;






void sine(int sec)
{
  int i,size;
  int debug=0;
  size = 44100*sec;
  Sint16 buffer [size]; //buffer array
  s1.setFreq(110);
  s2.setFreq(440);
  s3.setFreq(1760);

  s1.setAmplitude(127);
  s2.setAmplitude(1285);
  s3.setAmplitude(64);

  fprintf(stderr,"Generate sine 1\n");

  for(i = 0; i < size; i++)
    {
      Uint32 b;
      //buffer[i]=s1.tick()-s2.tick()-s3.tick();
      b=s1.tick()-s2.tick()-s3.tick();
      b=s1.tick()-s2.tick();
      //b=s1.tick()/2-s2.tick()/2;
      b=(s1.tick()-s2.tick()-s3.tick())/3;
      if (b < -32768) buffer[i]=-32765;
      if (b > 32768) buffer[i]=32765;
      if (b > -32786 || b < 32768 ) buffer[i]=b;
      if (debug) printf("b: %d bufi: %d\n",b,buffer[i]);
      //buffer[i]=s1.tick()+s2.tick();


      //buffer[i]=s1.tick();
      //      buffer[i]=s1.tick();
    }

  FILE* fd=fopen("sine2","w+");
  
  for (int i=0; i< size ; i++)
    {      
      Sint16 d=buffer[i];
      int f=1;
      fwrite(&d,sizeof(unsigned Sint16),1,fd);
      //fwrite(STDOUT,sizeof(unsigned Sint16),1,fd);
      //fwrite((void*)d,sizeof(unsigned Sint16),1,stdout);
  }
 
  fclose(fd);
  fprintf(stderr,"Closing sine 1\n");
  //fprintf(stderr,"closing sine 1\n");


}



int main()
{
  ae.openAudio();

  sine(4);

}

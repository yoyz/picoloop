#include "Wave.h"
#include "AudioEngine.h"

#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
#include "ADSR.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
Wave cowbell;
AudioEngine ae;
SineOscillator s1;
SawOscillator  s2;
FuzzyPulseOscillator s3;

ADSR adsr1;
ADSR adsr2;
ADSR adsr3;






void sine(int sec)
{
  int i,size,size1,size2,size3;
  int debug=1;
  //size = 44100*sec;
  s1.setFreq(440/2);
  s2.setFreq(440/4);
  s3.setFreq(440);
  //  s3.setFreq(1760);
  s1.setAmplitude(127);
  s2.setAmplitude(127);
  s3.setAmplitude(64);

  adsr1.setOscillator(&s1);
  adsr2.setOscillator(&s2);
  adsr3.setOscillator(&s3);

  adsr1.setAttack(0);
  adsr1.setDecay(127);
  adsr1.setSustain(127);
  adsr1.setRelease(64);

  adsr2.setAttack(0);
  adsr2.setDecay(127);
  adsr2.setSustain(127);
  adsr2.setRelease(64);

  adsr3.setAttack(0);
  adsr3.setDecay(127);
  adsr3.setSustain(127);
  adsr3.setRelease(64);


  adsr1.processSample();
  adsr2.processSample();
  adsr3.processSample();

  size1=adsr1.getSize();
  size2=adsr2.getSize();
  size3=adsr3.getSize();

  if (size2>=size1) { size=size2; }
  if (size1>size2)  { size=size1; }

  //  printf("size:%d\n",size);
  //  s2.setAmplitude(1285);
  //  s3.setAmplitude(64);

  Sint16 buffer [size]; //buffer array

  fprintf(stderr,"Generate sine 1\n");

  for(i = 0; i < size; i++)
    {
      Uint32 b;
      //buffer[i]=s1.tick()-s2.tick()-s3.tick();
      //b=s1.tick()-s2.tick()-s3.tick();
      //b=s1.tick()-s2.tick();
      
      //b=s1.tick()/2-s2.tick()/2;
      //b=(adsr2.tick());
      //b=adsr1.tick()+adsr2.tick();
      //b=adsr3.tick();
      //b=adsr1.tick()/2+adsr3.tick()/2;

      //b=s1.tick()/3+s2.tick()/3+s3.tick()/3;
      b=adsr1.tick()/3-adsr2.tick()/3-adsr3.tick()/3;
      
      if (b < -32768)               buffer[i]=-32765;
      if (b > 32768)                buffer[i]=32765;
      if (b > -32786 || b < 32768 ) buffer[i]=b;
      //if (debug) printf("b: %d bufi: %d\n",b,buffer[i]);
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
  //sleep(600);
  //fprintf(stderr,"closing sine 1\n");


}



int main()
{
  ae.openAudio();

  sine(4);

}

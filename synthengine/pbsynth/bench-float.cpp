#include "synthengine.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
  int nbuff=1024;
  int i;
  int j;
  FILE     * FD;

  mfloat  buffer[nbuff];
  int16_t i_buffer[nbuff];
  FD=fopen("audioout","w+");

  SynthEngine SE(nbuff,1024);
  //SE.reset();
  SE.getOscillator(0)->setWave(0);
  SE.getOscillator(1)->setWave(2);

  // SE.getEnvelope(0)->setA(-0.6f);
  // SE.getEnvelope(1)->setA(-0.5f);

  // SE.getEnvelope(0)->setR(0.4f);
  // SE.getEnvelope(1)->setR(0.5f);
  SE.getEnvelope(0)->setADSR(-1.0f, -1.0f, 1.0f, -1.0f);
  SE.getEnvelope(1)->setADSR(-1.0f, -1.0f, 1.0f, -1.0f);

  SE.getEnvelope(0)->setADSR(-0.8f, -1.0f, 1.0f, -0.2f);
  SE.getEnvelope(1)->setADSR(-0.4f, -0.5f, 0.5f, -0.8f);
  //SE.getEnvelope(1)->setADSR(-0.4f, -0.5f, 0.5f, -0.8f);
  


  // SE.getFilter(0)->setCutoff(0.5f,-0.2f);
  // SE.getFilter(1)->setCutoff(0.01f,-0.1f);

  SE.setParameter(SENGINE_FILTFREQ,0.1f);
  SE.setParameter(SENGINE_FILTRESO,0.9f);


  SE.triggerNote(24);

  for (j=0;j<16*1024;j++)
    {
      SE.process(buffer,nbuff);
    }
  printf("%f\n",buffer[1024]);
  

  return 0;
}

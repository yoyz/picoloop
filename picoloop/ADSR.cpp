using namespace std;

#include "ADSR.h"




ADSR::ADSR()
{
  attack=0;
  decay=127;
  sustain=0;
  release=2;
  sample_num=-1;
  playing=1;
}

ADSR::~ADSR()
{

}

void ADSR::setAttack(int atk)
{
  attack=atk;
}

void ADSR::setDecay(int dc)
{
  decay=dc;
}

void  ADSR::setSustain(int sust)
{
  sustain=sust;
}

void ADSR::setRelease(int rls)
{
  release=rls;
}


int ADSR::getAttack()
{
  return attack;
}

int ADSR::getDecay()
{
  return decay;
}

int ADSR::getSustain()
{
  return sustain;
}

int ADSR::getRelease()
{
  return release;
}

/*
void ADSR::setOscillator(Oscillator * osc)
{
  S=osc; 
}
*/

 //void ADSR::setOscillator(VCO * vcoosc)
void ADSR::setVCO(VCO * vcoosc)
{
  vco=vcoosc;
}

int ADSR::getPlaying()
{
  return playing;
}

void ADSR::reset()
{
  sample_num=0;
  ca=attack;
  cd=decay;
  cs=sustain;
  cr=release;
  //fseconds=(float)release/16+(float(attack/16);
  fseconds=(float)release/128;
  size=fseconds*44100;
}

int ADSR::getSize()
{
  return size;
}

Sint16 ADSR::tick()
{
  //  return S->tick();

  Sint16 s;
  float f1;
  float f2;
  int debug=1;
  int index_inverse=0;
  //if (debug) fprintf(stderr,"begin Sint16 ADSR::tick()\n");
  if (sample_num<=0)  { this->reset(); vco->reset(); }
  sample_num++;

  //s=S->tick();
  //  s=S->tick();
  s=vco->tick();

  
  //(size-sample_num)
  f1=s;
  index_inverse=size-sample_num;
  if (index_inverse<=0) { return 0; }
  f2=f1*((float)(index_inverse)/(size));
  //  printf("size:%d sample_num:%d s:%d f1:%f f2:%f\n",size,sample_num,s,f1,f2);

  s=f2;

  //if (debug) fprintf(stderr,"end Sint16 ADSR::tick()\n");
  return s;  
}

/*
Sint16 ADSR::tick()
{
  Sint16 s;
  float f1;
  float f2;
  int debug=0;
  if (debug) fprintf(stderr,"begin Sint16 ADSR::tick()\n");
  if (sample_num<0)  this->processSample();
  sample_num++;

  s=oscillator->tick();


  
  //(size-sample_num)
  f1=s;
  f2=f1*((float)(size-sample_num)/(size));
  //printf("size:%d sample_num:%d s:%d f1:%f f2:%f\n",size,sample_num,s,f1,f2);

  s=f2;

  if (debug) fprintf(stderr,"end Sint16 ADSR::tick()\n");
  return s;  
}
*/

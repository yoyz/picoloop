using namespace std;

#include "ADSR.h"




ADSR::ADSR()
{
  printf("ADSR::ADSR()\n");
  attack=0;
  decay=127;
  sustain=0;
  release=0;
  sample_num=-1;
  playing=1;

  ca=0;
  cr=0;
  ca_segment=0;
  cr_segment=0;
  ca_next_segment=0;
  cr_next_segment=0;

}

ADSR::~ADSR()
{

}

void ADSR::init()
{
  printf("ADSR::init()\n");
  attack=0;
  decay=127;
  sustain=0;
  release=0;
  sample_num=-1;
  playing=1;

  ca=0;
  cr=0;
  ca_segment=0;
  cr_segment=0;
  ca_next_segment=0;
  cr_next_segment=0;

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
void ADSR::setInput(VCO * vcoosc)
{
  //  exit(0);
  printf("ADSR::setVCO(0x%08.8X\n",vcoosc);
  vco=vcoosc;
}

int ADSR::getPlaying()
{
  return playing;
}

void ADSR::reset()
{
  printf("ADSR::reset() this=0x%08.8X\n",this);
  sample_num=0;


  cd=decay;
  cs=sustain;

  if (attack>0)
    ca=attack  << 10;
  else
    ca=0;

  if (release>0)
    cr=release << 10;
  else
    cr=0;

  car=ca+cr;

  ca_segment=ca/127;
  cr_segment=cr/127;
  
  ca_next_segment=ca_segment;
  cr_next_segment=cr_segment+ca;

  ca_div=127;
  cr_div=1;
  //fseconds=(float)release/16+(float(attack/16);
  //fseconds_release=(float)release/128;
  //fseconds_attack=(float)attack/128;
  //size_release=fseconds_release*44100;
  //size_attack=fseconds_attack*44100;
}

int ADSR::getSize()
{
  //return size_release;

  return car;
}

Sint16 ADSR::tick()
{
  //  return S->tick();

  Sint16 s=0;
  //Sint16 s_in;
  Sint32 s_in;
  //float  f1=0.0;
  //float  f2=0.0;
  int    debug=1;
  int    index_inverse=0;
  int    tmp1;
  int    tmp2;
  //if (debug) fprintf(stderr,"begin Sint16 ADSR::tick()\n");
  /*
  if (sample_num==0)  
    {      
      this->reset(); vco->reset(); 
      
    }
  */

  if (sample_num>car)
    return 0;


  sample_num++;
  

  //s=S->tick();
  //  s=S->tick();
  s_in=vco->tick();
  //return s_in;
  
  //(size-sample_num)

  //We are in the attack phase
  //if (sample_num<size_attack)

  if (sample_num < ca)    
    {
      if (sample_num>ca_next_segment)
	{
	  ca_next_segment=ca_next_segment+ca_segment;
	  ca_div=ca_div-1;
	  
	  if(ca_div<2)
	    ca_div=2;
	}
      return s_in/((ca_div/4)+1);
    }

  if (sample_num >= ca)
    {
      if (sample_num>cr_next_segment)
	{
	  cr_next_segment=cr_next_segment+cr_segment;
	  cr_div=cr_div+1;
	}

      return s_in/((cr_div/4)+1);
      
    }



  //We are in the release phase
  if (sample_num>ca)
    {
    }
       
  //s=f2;


  //if (debug) fprintf(stderr,"end Sint16 ADSR::tick()\n");
  //return s;  
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

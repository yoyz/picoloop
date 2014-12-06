using namespace std;

#include "PicosynthADSR.h"

enum  
  {
    PicosynthADSR_INIT,
    PicosynthADSR_ATTACK,
    PicosynthADSR_DECAY,
    PicosynthADSR_SUSTAIN,
    PicosynthADSR_RELEASE,
    PicosynthADSR_FINISH
  };

#define SHIFT_NOTE 8  // for synth

PicosynthADSR::PicosynthADSR() : tanh_table(new Sint16[128])
{
  float fi;
  int   ii;
  int   i;

  printf("PicosynthADSR::PicosynthADSR()\n");

  adsr_note=0;

  attack=32;
  decay=10;
  sustain=64;
  release=0;

  sample_num=-1;

  ca=0;
  cd=0;
  cr=0;

  cr_div=0;
  ca_div=0;

  ca_segment=0;
  cd_segment=0;
  cr_segment=0;


  ca_next_segment=0;
  cd_next_segment=0;
  cr_next_segment=0;

  current_segment=PicosynthADSR_INIT;
  noteOn_value=0;

  for (i=0;i<256;i++)
    {
      fi=i;
      fi=tanh(fi/128);
      fi=fi*1024;
      ii=fi;
      tanh_table[i/2]=ii;
      printf("tanh[%d]=%d\n",i,tanh_table[i/2]);
    }
}

PicosynthADSR::~PicosynthADSR()
{

}

void PicosynthADSR::init()
{
  printf("PicosynthADSR::init()\n");

  adsr_note=0;

  attack=32;
  decay=10;
  sustain=64;
  release=0;
  sample_num=-1;
  //playing=1;

  cr_div=0;
  ca_div=0;

  
  ca=0;
  cr=0;
  ca_segment=0;
  cd_segment=0;
  cr_segment=0;  
  ca_next_segment=0;
  cd_next_segment=0;
  cr_next_segment=0;

  current_segment=PicosynthADSR_INIT;
  noteOn_value=0;
}

void PicosynthADSR::setNoteADSR(int mode)
{
  adsr_note=mode;
}


int PicosynthADSR::getNoteADSR()
{
  return adsr_note;
}


void PicosynthADSR::setAttack(int atk)
{
  attack=atk;
}

void PicosynthADSR::setDecay(int dc)
{
  decay=dc;
}

void  PicosynthADSR::setSustain(int sust)
{
  sustain=sust;
}

void PicosynthADSR::setRelease(int rls)
{
  release=rls;
}


int PicosynthADSR::getAttack()
{
  return attack;
}

int PicosynthADSR::getDecay()
{
  return decay;
}

int PicosynthADSR::getSustain()
{
  return sustain;
}

int PicosynthADSR::getRelease()
{
  return release;
}


int PicosynthADSR::setSegment(int segment)
{
  if (segment==PicosynthADSR_INIT     ||
      segment==PicosynthADSR_ATTACK   ||
      segment==PicosynthADSR_DECAY    ||
      segment==PicosynthADSR_SUSTAIN  ||
      segment==PicosynthADSR_RELEASE  ||
      segment==PicosynthADSR_FINISH   
      )
    current_segment=segment;
}

void PicosynthADSR::setInput(Oscillator * vcoosc)
{
  printf("PicosynthADSR::setVCO(0x%08.8X\n",vcoosc);
  vco=(PicosynthVCO*)vcoosc;
}

void PicosynthADSR::setNoteOn()
{
  noteOn_value=1;
}

void PicosynthADSR::setNoteOff()
{
  noteOn_value=0;
}

void PicosynthADSR::setADSRRelease()
{
  current_segment=PicosynthADSR_RELEASE;
}


int PicosynthADSR::getNoteOn()
{
  return noteOn_value;
}

void PicosynthADSR::reset()
{ 
  int numsample_shift;
  printf("PicosynthADSR::reset() this=0x%08.8X\n",this);  

  numsample_shift=SHIFT_NOTE;

    
  sample_num=0;
  cs=sustain;


  if (attack>=0)
    ca=attack  << numsample_shift;
  else
    ca=0;

  if (decay>=0)
    cd=decay  << numsample_shift;
  else
    cd=0;

  if (release>=0)
    cr=release << numsample_shift;
  else
    cr=0;

  cadr=ca+cd+cr;

  ca_segment=ca/127;
  cd_segment=cd/127;
  cs_segment=cs/127;
  cr_segment=cr/127;
  
  ca_next_segment=ca_segment;
  cd_next_segment=cd_segment;
  cs_next_segment=cs_segment;
  cr_next_segment=cr_segment;

  ca_div=127;
  cd_div=127;
  cs_div=1;
  cr_div=1;
  current_segment=PicosynthADSR_INIT;
  noteOn_value=0;
}
 
 





Sint16 PicosynthADSR::tick()
{  
  return this->tick_note();
}


Sint16 PicosynthADSR::tick_note()
{
  Sint16 s=0;
  Sint32 s_in;
  Sint16 s_out=0;;
  int    debug=1;
  int    index_inverse=0;
  int    tmp1;
  int    tmp2;

  if (current_segment==PicosynthADSR_FINISH)
    {
      if (0) printf("PicosynthADSR_FINISH\n");
      s_out=0;
    }

  if (current_segment==PicosynthADSR_INIT &&
      noteOn_value==0                    
      )
    {
      if (0) printf("PicosynthADSR_INIT\n");
      current_segment=PicosynthADSR_RELEASE;
    }



  if (current_segment==PicosynthADSR_INIT &&
      noteOn_value==1                  
      )
    {
      current_segment=PicosynthADSR_ATTACK;
    }
  
      
  sample_num++;
  s_in=vco->tick();

    

  if (sample_num%8192==0)
    {
      if(current_segment==PicosynthADSR_INIT)
	  printf("***************************** INIT     noteOn:%d\n",noteOn_value);

      if(current_segment==PicosynthADSR_ATTACK)
	  printf("***************************** ATTACK   noteOn:%d\n",noteOn_value);

      if(current_segment==PicosynthADSR_DECAY)
	  printf("***************************** DECAY    noteOn:%d\n",noteOn_value);

      if(current_segment==PicosynthADSR_SUSTAIN)
	  printf("***************************** SUSTAIN  noteOn:%d\n",noteOn_value);

      if(current_segment==PicosynthADSR_RELEASE)
	  printf("***************************** RELEASE  noteOn:%d\n",noteOn_value);

      if(current_segment==PicosynthADSR_FINISH)
	  printf("***************************** FINISH  noteOn:%d\n",noteOn_value);

    }

  if (current_segment==PicosynthADSR_ATTACK &&
      sample_num > ca)
    {
      printf("***************************** DECAY noteOn:%d\n",noteOn_value);
      current_segment=PicosynthADSR_DECAY;
      cd_next_segment=sample_num+cd_segment;
    }

  if (current_segment==PicosynthADSR_DECAY &&      
      sample_num >= ca+cd)
    {
      current_segment=PicosynthADSR_SUSTAIN;
      printf("***************************** SUSTAIN  noteOn:%d\n",noteOn_value);
    }

  
  if (current_segment==PicosynthADSR_SUSTAIN &&
      noteOn_value==0)
    {
      current_segment=PicosynthADSR_RELEASE;
      cr_next_segment=sample_num+cr_segment;
      cr_div=127-cd_div;
      printf("***************************** RELEASE  noteOn:%d\n",noteOn_value);
    }

  // ATTACK
  if (current_segment==PicosynthADSR_ATTACK)    
    {
      if (sample_num>ca_next_segment)
	{
	  ca_next_segment=ca_next_segment+ca_segment;
	  ca_div=ca_div-1;
	  
	  if(ca_div<2)
	    ca_div=2;
	}
      s_out=(s_in*tanh_table[127-ca_div])/1024;

    }

  // DECAY
  if (current_segment==PicosynthADSR_DECAY)
    {

      if (sample_num>cd_next_segment)
	{
	  cd_next_segment=cd_next_segment+cd_segment;
	  cd_div=cd_div-1;
	  if (cd_div<sustain)
	    {
	      cd_div=sustain;
	    }
	}

      s_out=(s_in*tanh_table[cd_div])/1024;
    }




  // SUSTAIN
  if (current_segment==PicosynthADSR_SUSTAIN)
    {
      s_out=(s_in*tanh_table[cd_div])/1024;
    }



  // RELEASE
  if (current_segment==PicosynthADSR_RELEASE)
    {
      if (sample_num>cr_next_segment)
	{
	  cr_next_segment=cr_next_segment+cr_segment;
	  cr_div=cr_div+1;
	  if (cr_div>127)
	    {
	      cr_div=127;
	      current_segment=PicosynthADSR_FINISH;
	    }
	}

      s_out=(s_in*tanh_table[127-cr_div])/1024;
    }
       
  if (current_segment==PicosynthADSR_FINISH)
    {
      noteOn_value=0;
    } 
  return s_out;
}















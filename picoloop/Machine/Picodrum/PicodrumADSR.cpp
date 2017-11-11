using namespace std;

#include "PicodrumADSR.h"

enum  
  {
    PicodrumADSR_INIT,
    PicodrumADSR_ATTACK,
    PicodrumADSR_DECAY,
    PicodrumADSR_SUSTAIN,
    PicodrumADSR_RELEASE,
    PicodrumADSR_FINISH
  };

#define SHIFT_TRIG 8  // for drum

PicodrumADSR::PicodrumADSR()
{
  float fi;
  int   ii;
  int   i;

  DPRINTF("PicodrumADSR::PicodrumADSR()");

  adsr_note=0;

  attack=32;
  decay=10;
  sustain=64;
  release=0;

  sample_num=-1;
  //playing=1;
  cadr=0;
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

  current_segment=PicodrumADSR_INIT;
  noteOn_value=0;
  old_s_sin=0;

  old_pole=0;
  pole=0;

}

PicodrumADSR::~PicodrumADSR()
{

}

void PicodrumADSR::init()
{
  DPRINTF("PicodrumADSR::init()");

  adsr_note=0;

  attack=32;
  decay=10;
  sustain=64;
  release=0;
  sample_num=-1;


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

  current_segment=PicodrumADSR_INIT;
  noteOn_value=0;
  old_s_sin=0;
  s_in=0;
  old_pole=0;
  pole=0;
}



void PicodrumADSR::setAttack(int atk)
{
  attack=atk;
}

void PicodrumADSR::setDecay(int dc)
{
  decay=dc;
}

void  PicodrumADSR::setSustain(int sust)
{
  sustain=sust;
}

void PicodrumADSR::setRelease(int rls)
{
  release=rls;
}


int PicodrumADSR::getAttack()
{
  return attack;
}

int PicodrumADSR::getDecay()
{
  return decay;
}

int PicodrumADSR::getSustain()
{
  return sustain;
}

int PicodrumADSR::getRelease()
{
  return release;
}


int PicodrumADSR::setSegment(int segment)
{
  if (segment==PicodrumADSR_INIT     ||
      segment==PicodrumADSR_ATTACK   ||
      segment==PicodrumADSR_DECAY    ||
      segment==PicodrumADSR_SUSTAIN  ||
      segment==PicodrumADSR_RELEASE  ||
      segment==PicodrumADSR_FINISH   
      )
    current_segment=segment;
  return 0;
}

void PicodrumADSR::setInput(PicodrumVCO * vcoosc)
{
  DPRINTF("PicodrumADSR::setVCO(0x%08.8X",vcoosc);
  vco=(PicodrumVCO*)vcoosc;
}

void PicodrumADSR::setNoteOn()
{
  noteOn_value=1;
}

void PicodrumADSR::setNoteOff()
{
  noteOn_value=0;
}

void PicodrumADSR::setADSRRelease()
{
  current_segment=PicodrumADSR_RELEASE;
}


int PicodrumADSR::getNoteOn()
{
  return noteOn_value;
}

void PicodrumADSR::reset()
{ 
  int numsample_shift;
  DPRINTF("PicodrumADSR::reset() this=0x%08.8X",this);  
  
  numsample_shift=SHIFT_TRIG;

    
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
  current_segment=PicodrumADSR_INIT;
  noteOn_value=0;
}
 
 







Sint16 PicodrumADSR::tick()
{
  return this->tick_trig();
}
















Sint16 PicodrumADSR::tick_trig()
{
  Sint16 s=0;
  int    debug=1;
  int    index_inverse=0;
  int    tmp1;
  int    tmp2;

  if (sample_num>cadr)
    return 0;


  sample_num++;

  old_s_sin=s_in;
  s_in=vco->tick();
  //return s_in;
  // check if we are in the 
  //   1  32000 
  //or the 
  //  -1 -32000
  // pole allow to store this information

  if (s_in>0)
    pole=1;
  else
    pole=0;

  if (pole!=old_pole)
    {
      ca_div_woalias=ca_div;
      cr_div_woalias=cr_div;
    }

  old_pole=pole;

  if (sample_num < ca)
    {
      if (sample_num>ca_next_segment)
        {
          ca_next_segment=ca_next_segment+ca_segment;
          ca_div=ca_div-1;

          if(ca_div<2)
            ca_div=2;
        }
      return s_in/((ca_div_woalias/4)+1);
      //return s_in/((ca_div/4)+1);
    }
  
  if (sample_num >= ca)
    {
      if (sample_num>cr_next_segment)
        {
          cr_next_segment=cr_next_segment+cr_segment;
          cr_div=cr_div+1;
        }

      return s_in/((cr_div_woalias/4)+1);
      //return s_in/((cr_div/4)+1);
    }
  return 0;
}


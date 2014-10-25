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

#define SHIFT_TRIG 8  // for drum
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
  //playing=1;

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

/*
void PicosynthADSR::setOscillator(Oscillator * osc)
{
  S=osc; 
}
*/

 //void PicosynthADSR::setOscillator(VCO * vcoosc)
 //void PicosynthADSR::setInput(VCO * vcoosc)
void PicosynthADSR::setInput(Oscillator * vcoosc)
{
  //  exit(0);
  printf("PicosynthADSR::setVCO(0x%08.8X\n",vcoosc);
  vco=(PicosynthVCO*)vcoosc;
}
/*
int PicosynthADSR::getPlaying()
{
  return playing;
}
*/

void PicosynthADSR::setNoteOn()
{
  //printf("NOTEONNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n");
  noteOn_value=1;
}

void PicosynthADSR::setNoteOff()
{
  //printf("NOTEOFFFFFFFFFFFFFFFFFFFFFFF\n");
  noteOn_value=0;
}

void PicosynthADSR::setADSRRelease()
{
  //printf("NOTEOFFFFFFFFFFFFFFFFFFFFFFF\n");
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
  
  if (adsr_note==0)
    numsample_shift=SHIFT_TRIG;

  if (adsr_note==1)
    numsample_shift=SHIFT_NOTE;

    
  sample_num=0;

  //cd=decay;

  cs=sustain;


  if (attack>0)
    ca=attack  << numsample_shift;
  else
    ca=0;

  if (decay>0)
    cd=decay  << numsample_shift;
  else
    cd=0;

  if (release>0)
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
  //fseconds=(float)release/16+(float(attack/16);
  //fseconds_release=(float)release/128;
  //fseconds_attack=(float)attack/128;
  //size_release=fseconds_release*44100;
  //size_attack=fseconds_attack*44100;
  current_segment=PicosynthADSR_INIT;
  noteOn_value=0;
}
 
 


/*
int PicosynthADSR::getSize()
{
  //return size_release;

  return cadr;
}
*/





Sint16 PicosynthADSR::tick()
{
  if (adsr_note==1)
    return this->tick_note();
  else
    return this->tick_trig();
}


Sint16 PicosynthADSR::tick_note()
{
  //  return S->tick();

  Sint16 s=0;
  //Sint16 s_in;
  Sint32 s_in;
  Sint16 s_out=0;;
  //float  f1=0.0;
  //float  f2=0.0;
  int    debug=1;
  int    index_inverse=0;
  int    tmp1;
  int    tmp2;
  //if (debug) fprintf(stderr,"begin Sint16 PicosynthADSR::tick()\n");
  /*
  if (sample_num==0)  
    {      
      this->reset(); vco->reset(); 
      
    }
  */



  if (current_segment==PicosynthADSR_FINISH)
    {
      if (0) printf("PicosynthADSR_FINISH\n");
      //return 0;
      s_out=0;
    }

  if (current_segment==PicosynthADSR_INIT &&
      noteOn_value==0                    
      )
    {
      //printf("***********WHY THE HELL I AM HERE\n");
      if (0) printf("PicosynthADSR_INIT\n");
      //return(0);
      current_segment=PicosynthADSR_RELEASE;
    }



  if (current_segment==PicosynthADSR_INIT &&
      noteOn_value==1                  
      )
    {
      current_segment=PicosynthADSR_ATTACK;
    }
  
      
  sample_num++;
  

  //  s=S->tick();
  //  s=S->tick();
  s_in=vco->tick();
  //return s_in;
  
  //(size-sample_num)

  //We are in the attack phase
  //if (sample_num<size_attack)


    

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
      //printf("OK\n");
      //exit(1);
      current_segment=PicosynthADSR_RELEASE;
      cr_next_segment=sample_num+cr_segment;
      cr_div=127-cd_div;
      printf("***************************** RELEASE  noteOn:%d\n",noteOn_value);
    }
  
  /*
  if (current_segment==PicosynthADSR_SUSTAIN &&
      noteOn_value==1)
    {
      //printf("SUSTAIN!!!!!!");
    }
  */



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
      //return s_in/((ca_div)+1);
      //return (s_in*tanh_table[127-ca_div])/1024;
      s_out=(s_in*tanh_table[127-ca_div])/1024;

    }

  /*
  // DECAY
  if (current_segment==PicosynthADSR_DECAY)
    {
      //return s_in;
      s_out=s_in;
      //if (1) printf("s_in:%d s_out:%d\n",s_in,s_out);
    }
  */


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
	      //current_segment=PicosynthADSR_SUSTAIN;
	    }
	}

      //return s_in/((cr_div/4)+1);            
      //      return (s_in*tanh[cr_div])/1024;
      //return (s_in*tanh_table[127-cr_div])/1024;
      s_out=(s_in*tanh_table[cd_div])/1024;

      //return s_in;
      //s_out=s_in;
      //if (1) printf("s_in:%d s_out:%d\n",s_in,s_out);
    }




  // SUSTAIN
  if (current_segment==PicosynthADSR_SUSTAIN)
    {
      //return s_in;
      s_out=(s_in*tanh_table[cd_div])/1024;
      //s_out=s_in;
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

      //return s_in/((cr_div/4)+1);            
      //      return (s_in*tanh[cr_div])/1024;
      //return (s_in*tanh_table[127-cr_div])/1024;
      s_out=(s_in*tanh_table[127-cr_div])/1024;
    }
       
  if (current_segment==PicosynthADSR_FINISH)
    {
      noteOn_value=0;
    }

  //s=f2;

  
  return s_out;

  //if (debug) fprintf(stderr,"end Sint16 PicosynthADSR::tick()\n");
  //return s;  
}















Sint16 PicosynthADSR::tick_trig()
{
  //  return S->tick();                                                                                                                                                                                                                     

  Sint16 s=0;
  //Sint16 s_in;                                                                                                                                                                                                                            
  //Sint32 s_in;
  //float  f1=0.0;                                                                                                                                                                                                                          
  //float  f2=0.0;                                                                                                                                                                                                                          
  int    debug=1;
  int    index_inverse=0;
  int    tmp1;
  int    tmp2;

  if (sample_num>cadr)
    return 0;


  sample_num++;

  old_s_sin=s_in;
  s_in=vco->tick();

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
}

/*
Sint16 PicosynthADSR::tick()
{
  Sint16 s;
  float f1;
  float f2;
  int debug=0;
  if (debug) fprintf(stderr,"begin Sint16 PicosynthADSR::tick()\n");
  if (sample_num<0)  this->processSample();
  sample_num++;

  s=oscillator->tick();


  
  //(size-sample_num)
  f1=s;
  f2=f1*((float)(size-sample_num)/(size));
  //printf("size:%d sample_num:%d s:%d f1:%f f2:%f\n",size,sample_num,s,f1,f2);

  s=f2;

  if (debug) fprintf(stderr,"end Sint16 PicosynthADSR::tick()\n");
  return s;  
}
*/

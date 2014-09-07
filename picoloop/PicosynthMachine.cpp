#include "PicosynthMachine.h"


PicosynthMachine::PicosynthMachine() : adsr_amp(), adsr_fltr(), vco(), filter(), one_osc(), tanh_table(new Sint16[256])
{
  float fi;
  int   i;

  printf("PicosynthMachine::PicosynthMachine()\n");  
  for (i=0;i<256;i++)
    {
      fi=i;
      fi=tanh(fi/128);
      tanh_table[i]=fi*1024;
    }
  cutoff=125;
  resonance=10;
}



PicosynthMachine::~PicosynthMachine()
{
  printf("PicosynthMachine::~PicosynthMachine()\n");
}


void PicosynthMachine::init()
{

  //bq.reset();
  adsr_amp.init();
  adsr_fltr.init();

  adsr_amp.setInput(&vco); 
  adsr_fltr.setInput(&one_osc);

  one_osc.setFreq(100);
  one_osc.init();
  one_osc.reset();


  filter.init();
  sample_num=0;

  this->reset();
  this->getADSRAmp().init();
  this->getADSRFltr().init();
  this->getVCO().init();
  this->getVCO().setSynthFreq(0);      
  this->getADSRAmp().setNoteADSR(1);
  this->getADSRFltr().setNoteADSR(1);

}



int PicosynthMachine::get(int what)
{
  if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void PicosynthMachine::set(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==NOTE_ON && val==1) 
    { 
      this->getADSRAmp().reset();
      this->getADSRFltr().reset();
      this->getVCO().reset();
      this->getADSRAmp().setNoteOn(); 
      this->getADSRFltr().setNoteOn();   
    }
  if (what==NOTE_ON && val==0) 
    { 
      this->getADSRAmp().setNoteOff(); 
      this->getADSRFltr().setNoteOff(); 
    }

  if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           this->getVCO().setOscillator(0,val);
  if (what==OSC2_TYPE)           this->getVCO().setOscillator(1,val);

  if (what==OSC12_MIX)           this->getVCO().setVCOMix(val);

  if (what==LFO1_DEPTH)          this->getVCO().setLfoDepth(val);
  if (what==LFO1_FREQ)           this->getVCO().setLfoSpeed(val);

  if (what==ADSR_ENV0_ATTACK)    this->getADSRAmp().setAttack(val);
  if (what==ADSR_ENV0_DECAY)     this->getADSRAmp().setDecay(val);
  if (what==ADSR_ENV0_SUSTAIN)   this->getADSRAmp().setSustain(val);
  if (what==ADSR_ENV0_RELEASE)   this->getADSRAmp().setRelease(val);

  if (what==ADSR_ENV1_ATTACK)    this->getADSRFltr().setAttack(val);
  if (what==ADSR_ENV1_DECAY)     this->getADSRFltr().setDecay(val);
  if (what==ADSR_ENV1_SUSTAIN)   this->getADSRFltr().setSustain(val);
  if (what==ADSR_ENV1_RELEASE)   this->getADSRFltr().setRelease(val);


  if (what==FILTER1_CUTOFF)      
    { 
      cutoff=val;
      filter.setCutoff(cutoff);
      this->getADSRAmp().reset();
      this->getADSRFltr().reset();


  }
  if (what==FILTER1_RES)         
    { 
      resonance=val;
      filter.setResonance(resonance);
      this->getADSRAmp().reset();
      this->getADSRFltr().reset();
    }

  
}


ADSR & PicosynthMachine::getADSRAmp()
{
  return adsr_amp;
}


ADSR & PicosynthMachine::getADSRFltr()
{
  return adsr_fltr;
}


VCO & PicosynthMachine::getVCO()
{
  return vco;
}


void PicosynthMachine::reset()
{
  sample_num=0;
  last_sample=0;
}

int PicosynthMachine::tick()
{
  float  f_in;
  float  f_out;

  float  f_Fc;
  float  f_Q;

  Sint16 s_in;
  Sint16 s_out;

  Sint16 s_test;

  Sint32 tmp1;
  Sint32 tmp2;
  Sint32 tmp3;
  Sint16 index;

  Sint32 cutoff_tmp;
  Sint32 resonance_tmp;
  int    num=8192;
  int    i;
  
  s_in=adsr_amp.tick();
  s_in=s_in/2;
  if (sample_num==0)
    {
      for (i=0;i<num;i++)
	adsr_fltr.tick();
      
      s_test=adsr_fltr.tick();
      
      cutoff_tmp=(cutoff*s_test)/16384;
      resonance_tmp=(resonance*s_test)/16384;
      
      if (1)
      	{
	  filter.setCutoff(cutoff_tmp);
	  filter.setResonance(resonance_tmp);
      	}
    }
  

  if (sample_num>=0 &&
      sample_num < 256)
    {
      s_out=filter.process(s_in);      
    }


  if (sample_num>=256 &&
      sample_num<=num+256)
    {
      s_out=filter.process(s_in);
    }


  if (sample_num>=num+256)
    {
      s_out=filter.process(s_in);
      sample_num=-1;
    }

  
  sample_num++;
  last_sample=s_out;

  return s_out;

}

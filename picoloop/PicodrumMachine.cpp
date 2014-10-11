#include "PicodrumMachine.h"



PicodrumMachine::PicodrumMachine() : adsr_amp(), vco(), filter()
{
  float fi;
  int   i;

  printf("PicodrumMachine::PicodrumMachine()\n");  
  cutoff=125;
  resonance=10;
}



PicodrumMachine::~PicodrumMachine()
{
  printf("PicodrumMachine::~PicodrumMachine()\n");
}


void PicodrumMachine::init()
{
  adsr_amp.init();

  adsr_amp.setInput(&vco); 

  filter.init();
  sample_num=0;


  this->reset();
  this->getADSRAmp().init();

  this->getVCO().init();
  this->getVCO().setSynthFreq(0);      
  this->getADSRAmp().setNoteADSR(0);
}



int PicodrumMachine::getI(int what)
{

  if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void PicodrumMachine::setF(int what,float val)
{
  if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
}


void PicodrumMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==NOTE_ON && val==1) 
    { 

      this->getADSRAmp().reset();
      this->getVCO().reset();
      this->getADSRAmp().setNoteOn(); 
    }
  if (what==NOTE_ON && val==0) 
    { 
      this->getADSRAmp().setNoteOff(); 
    }

  if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           this->getVCO().setOscillator(0,val);
  if (what==OSC2_TYPE)           this->getVCO().setOscillator(1,val);

  if (what==OSC12_MIX)           this->getVCO().setVCOMix(val);

  if (what==LFO1_DEPTH)          this->getVCO().setLfoDepth(val);
  if (what==LFO1_FREQ)           this->getVCO().setLfoSpeed(val);


  if (what==ADSR_ENV0_ATTACK)    this->getADSRAmp().setAttack(val);
  if (what==ADSR_ENV0_RELEASE)   this->getADSRAmp().setRelease(val);



  if (what==FILTER1_CUTOFF)      
    { 
      cutoff=val;
      filter.setCutoff(cutoff);
      this->getADSRAmp().reset();

  }
  if (what==FILTER1_RES)         
    { 
      resonance=val;
      filter.setResonance(resonance);
      this->getADSRAmp().reset();
    }

  if (what==FILTER1_TYPE)     filter.setFilterType(val);
  if (what==FILTER1_ALGO)     filter.setFilterAlgo(val);

  
}


ADSR & PicodrumMachine::getADSRAmp()
{
  return adsr_amp;
}




VCO & PicodrumMachine::getVCO()
{
  //  printf("PicodrumMachine::getVCO() this=0x%08.8X\n",this);
  //  return vco_pointer;
  return vco;
}

//Biquad & PicodrumMachine::getBiquad()
//{
//  return bq;
//}

void PicodrumMachine::reset()
{
  sample_num=0;
  last_sample=0;
}


int PicodrumMachine::tick()
{
  float  f_in;
  float  f_out;
  Sint16 s_in;
  Sint16 s_out;


  s_in=adsr_amp.tick();
  s_in=s_in;


  s_out=filter.process(s_in);
  return s_out;

}

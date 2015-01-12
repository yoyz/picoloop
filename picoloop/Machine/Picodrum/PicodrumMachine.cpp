#include "PicodrumMachine.h"



PicodrumMachine::PicodrumMachine() : adsr_amp(), vco(), filter()
{
  float fi;
  int   i;

  printf("PicodrumMachine::PicodrumMachine()\n");  
  cutoff=125;
  resonance=10;
  note=0;
  detune=64;
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

  this->getPicodrumVCO().init();
  this->getPicodrumVCO().setNoteDetune(0,0);      
  //this->getPicodrumVCO().setSynthFreq(0);      
  //this->getADSRAmp().setNoteADSR(0);

  note=0;
  detune=64;

}

int PicodrumMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>4) return 4;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>4) return 4;
      return val;
      break;

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>3) return 3;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>2) return 2;
      return val;
      break;



    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      printf("WARNING: PicodrumMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int PicodrumMachine::getI(int what)
{

  if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void PicodrumMachine::setF(int what,float val)
{
  //  if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  if (what==LFO1_FREQ)           this->getPicodrumVCO().setLfoSpeed(val);
}


void PicodrumMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==NOTE_ON && val==1) 
    { 
      this->getPicodrumVCO().setNoteDetune(note,detune);
      this->getADSRAmp().reset();
      this->getPicodrumVCO().reset();
      this->getADSRAmp().setNoteOn(); 
    }
  if (what==NOTE_ON && val==0) 
    { 
      this->getADSRAmp().setNoteOff(); 
    }

  if (what==OSC1_NOTE)           note=val;

  //if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           this->getPicodrumVCO().setOscillator(0,val);
  if (what==OSC2_TYPE)           this->getPicodrumVCO().setOscillator(1,val);

  if (what==OSC12_MIX)           this->getPicodrumVCO().setPicodrumVCOMix(val);
  if (what==OSC1_PHASE)          this->getPicodrumVCO().setPicodrumVCOPhase(val);

  if (what==LFO1_DEPTH)          this->getPicodrumVCO().setLfoDepth(val);
  //if (what==LFO1_FREQ)           this->getPicodrumVCO().setLfoSpeed(val);


  if (what==ADSR_ENV0_ATTACK)    this->getADSRAmp().setAttack(val);
  if (what==ADSR_ENV0_RELEASE)   this->getADSRAmp().setRelease(val);



  if (what==FILTER1_CUTOFF)      
    { 
      cutoff=val;
      filter.setCutoff(cutoff);
      this->getADSRAmp().reset();

  }
  if (what==FILTER1_RESONANCE)         
    { 
      resonance=val;
      filter.setResonance(resonance);
      this->getADSRAmp().reset();
    }

  if (what==FILTER1_TYPE)     filter.setFilterType(val);
  if (what==FILTER1_ALGO)     filter.setFilterAlgo(val);

  
}


PicodrumADSR & PicodrumMachine::getADSRAmp()
{
  return adsr_amp;
}




PicodrumVCO & PicodrumMachine::getPicodrumVCO()
{
  //  printf("PicodrumMachine::getPicodrumVCO() this=0x%08.8X\n",this);
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

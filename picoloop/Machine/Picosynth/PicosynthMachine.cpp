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
  note=0;
  detune=64;
  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}



PicosynthMachine::~PicosynthMachine()
{
  printf("PicosynthMachine::~PicosynthMachine()\n");
}

PicosynthADSR & PicosynthMachine::getADSRAmp()
{
  return adsr_amp;
}


PicosynthADSR & PicosynthMachine::getADSRFltr()
{
  return adsr_fltr;
}


PicosynthVCO & PicosynthMachine::getVCO()
{
  return vco;
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
  //this->getVCO().setSynthFreq(0);      
  this->getVCO().setNoteDetune(0,0);      
  this->getADSRAmp().setNoteADSR(1);
  this->getADSRFltr().setNoteADSR(1);

  note=0;
  detune=64;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


int PicosynthMachine::checkI(int what,int val)
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
      printf("WARNING: PicosynthMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}

int PicosynthMachine::getI(int what)
{
  if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
}

void PicosynthMachine::setF(int what,float val)
{
  //if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==LFO1_FREQ)           this->getVCO().setLfoSpeed(val);
}

void PicosynthMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }

  if (what==NOTE_ON && val==1) 
    { 
      this->getVCO().setNoteDetune(note,detune);
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

  //if (what==OSC1_NOTE)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_NOTE)           note=val;

  //if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           this->getVCO().setOscillator(0,val);
  if (what==OSC2_TYPE)           this->getVCO().setOscillator(1,val);

  if (what==OSC12_MIX)           this->getVCO().setVCOMix(val);
  if (what==OSC1_PHASE)          this->getVCO().setVCOPhase(val);

  if (what==LFO1_DEPTH)          this->getVCO().setLfoDepth(val);
  if (what==LFO_TYPE)            this->getVCO().setLfoType(val);

  if (what==PITCHBEND_DEPTH)     this->getVCO().setPitchBendDepth(val);
  if (what==PITCHBEND_SPEED)     this->getVCO().setPitchBendSpeed(val);

  if (what==OSC1_AMP)            detune=val;
  //if (what==OSC2_AMP)            //HO->SetAttenuation(1,2,32-(val/4));



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
  if (what==FILTER1_RESONANCE)         
    { 
      resonance=val;
      filter.setResonance(resonance);
      this->getADSRAmp().reset();
      this->getADSRFltr().reset();
    }

  if (what==FILTER1_TYPE)     filter.setFilterType(val);
  if (what==FILTER1_ALGO)     filter.setFilterAlgo(val);

  
}




void PicosynthMachine::reset()
{
  sample_num=0;
  last_sample=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
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
  
  //num=1024;

  if (trig_time_mode)
    {
      if (trig_time_duration_sample<sample_num)
	{
	  this->setI(NOTE_ON,0);
	  trig_time_mode=0;
	  //printf("\t\t\t\t\t\tDONE\n");
	}

    }
      

  s_in=adsr_amp.tick();
  s_in=s_in/6;
 
  s_out=filter.process(s_in);      
    
  sample_num++;
  last_sample=s_out;

  return s_out;

}

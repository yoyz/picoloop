#include "Master.h"
#include "PicosynthMachine.h"

#define SAM 64

//PicosynthMachine::PicosynthMachine() : adsr_amp(), adsr_fltr(), vco(), filter(), one_osc()
PicosynthMachine::PicosynthMachine() 
{
  float fi;
  int   i;

  DPRINTF("PicosynthMachine::PicosynthMachine()");  
  cutoff=125;
  resonance=10;
  note=0;
  detune=64;
  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
  buffer=0;
}



PicosynthMachine::~PicosynthMachine()
{
  DPRINTF("PicosynthMachine::~PicosynthMachine()");
  if (buffer)
    free(buffer);
  //delete(one_osc);
}
/*
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
*/


void PicosynthMachine::init()
{
  //: adsr_amp(), adsr_fltr(), vco(), filter(), one_osc()
  DPRINTF("PicosynthMachine::init()");
  vco=new PicosynthVCO();
  adsr_amp=new PicosynthADSR();
  adsr_fltr=new PicosynthADSR();
  filter=new Filter();
  one_osc=new OneOscillator();
  
  if (buffer==0)
    {     
      //buffer = (Sint16*)malloc(sizeof(Sint16)*SAM);
      buffer = (int32_t*)malloc(sizeof(int32_t)*SAM);
    }
  index=0; // use by this->tick()
  
  //bq.reset();
  vco->init();
  adsr_amp->init();
  adsr_fltr->init();

  adsr_amp->setInput(vco); 
  //adsr_fltr->setInput(one_osc);

  one_osc->setFreq(100);
  one_osc->init();
  one_osc->reset();


  filter->init();
  sample_num=0;

  this->reset();
  //this->getADSRAmp().init();
  //this->getADSRFltr().init();
  //this->getVCO().init();
  //this->getVCO().setSynthFreq(0);      
  //this->getVCO().setNoteDetune(0,0,0);      
  //this->getADSRAmp().setNoteADSR(1);
  //this->getADSRFltr().setNoteADSR(1);
  vco->setNoteDetune(0,0,0);
  adsr_amp->setNoteADSR(1);
  adsr_fltr->setNoteADSR(1);
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

    // case ADSR_ENV0_ATTACK:
    //   if (val<=1) return 1;
    //   if (val>=127) return 127;
    //   return val;
    //   break;

    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>=PICO_WAVETABLE_SIZE-1) return PICO_WAVETABLE_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>=PICO_WAVETABLE_SIZE-1) return PICO_WAVETABLE_SIZE-1;
      return val;
      break;

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>=FILTER_TYPE_SIZE-1) return FILTER_TYPE_SIZE-1;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>=FILTER_ALGO_SIZE-1) return FILTER_ALGO_SIZE-1;
      return val;
      break;

    case LFO_TYPE:
      if (val<0) return 0;
      if (val>=LFO_TYPE_SIZE-1) return LFO_TYPE_SIZE-1;
      return val;
      break;


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: PicosynthMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
  return val;
}

int PicosynthMachine::getI(int what)
{
  if (what==NOTE_ON) return adsr_amp->getNoteOn();
  return 0;
}

void PicosynthMachine::setF(int what,float val)
{
  //if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  //if (what==LFO1_FREQ)           this->getVCO().setLfoSpeed(val);
  if (what==LFO1_FREQ)           vco->setLfoSpeed(val);
}

void PicosynthMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }

  if (what==NOTE_ON && val==1) 
    { 
      vco->setNoteDetune(note+osc1_scale,note+osc2_scale,detune);
      adsr_amp->reset();
      //this->getADSRFltr().reset();
      vco->reset();
      adsr_amp->setNoteOn(); 
      //this->getADSRFltr().setNoteOn();
    }
  if (what==NOTE_ON && val==0) 
    { 
      adsr_amp->setNoteOff();
      //this->getADSRFltr().setNoteOff();
    }

  //if (what==OSC1_NOTE)           this->getVCO().setSynthFreq(val);
  if (what==NOTE1)                 note=val;

  //if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           { vco->setOscillator(0,val); osc1_type=val; }
  if (what==OSC2_TYPE)           { vco->setOscillator(1,val); osc2_type=val; }

  if (what==OSC12_MIX)           vco->setVCOMix(val);
  //if (what==OSC1_PHASE)          //this->getVCO().setVCOPhase(val);
  if (what==OSC1_DETUNE)         detune=val;//this->getVCO().setVCOPhase(val);

  if (what==LFO1_DEPTH)          vco->setLfoDepth(val);
  if (what==LFO_TYPE)            vco->setLfoType(val);

  if (what==PITCHBEND_DEPTH)     vco->setPitchBendDepth(val);
  if (what==PITCHBEND_SPEED)     vco->setPitchBendSpeed(val);

  //if (what==OSC1_AMP)            detune=val;
  //if (what==OSC2_AMP)            //HO->SetAttenuation(1,2,32-(val/4));
  if (what==OSC1_SCALE)    osc1_scale=val;
  if (what==OSC2_SCALE)    osc2_scale=val;



  if (what==ADSR_ENV0_ATTACK)    adsr_amp->setAttack(val);
  if (what==ADSR_ENV0_DECAY)     adsr_amp->setDecay(val);
  if (what==ADSR_ENV0_SUSTAIN)   adsr_amp->setSustain(val);
  if (what==ADSR_ENV0_RELEASE)   adsr_amp->setRelease(val);
  /*
  if (what==ADSR_ENV1_ATTACK)    this->getADSRFltr().setAttack(val);
  if (what==ADSR_ENV1_DECAY)     this->getADSRFltr().setDecay(val);
  if (what==ADSR_ENV1_SUSTAIN)   this->getADSRFltr().setSustain(val);
  if (what==ADSR_ENV1_RELEASE)   this->getADSRFltr().setRelease(val);
  */

  if (what==FILTER1_CUTOFF)      
    { 
      cutoff=val;
      filter->setCutoff(cutoff);
      adsr_amp->reset();
      
      //this->getADSRAmp().reset();
      //this->getADSRFltr().reset();


  }
  if (what==FILTER1_RESONANCE)         
    { 
      resonance=val;
      filter->setResonance(resonance);
      adsr_amp->reset();
      //this->getADSRAmp().reset();
      //this->getADSRFltr().reset();
    }

  if (what==FILTER1_TYPE)     filter->setFilterType(val);
  if (what==FILTER1_ALGO)     filter->setFilterAlgo(val);

  
}

const char * PicosynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_null       = "NULL ";

  static const char * str_sine       = "SINE ";
  static const char * str_saw        = "SAW  ";
  static const char * str_pulse      = "PULSE";
  static const char * str_trgl       = "TRGL ";

  static const char * str_smsine     = "SSIN ";
  static const char * str_smsaw      = "SSAW ";
  static const char * str_smpulse    = "SPULS";
  static const char * str_smtrgl     = "STRGL";

  static const char * str_lfsr_noise = "LFSR ";
  static const char * str_noise      = "NOISE";

  const        char * str_osc[PICO_WAVETABLE_SIZE];


  static const char * str_fltr_algo_nofilter = "NOFL";
  static const char * str_fltr_algo_biquad   = "BIQU";
  static const char * str_fltr_algo_amsynth  = "AMST";
  
  const        char * str_fltr_algo[FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp   = "LP";
  static const char * str_fltr_type_bp   = "BP";
  static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[FILTER_TYPE_SIZE];


  static const char * str_lfo_type_lfo   = "PLFO";
  static const char * str_lfo_type_pb    = "PB";

  const        char * str_lfo_type[LFO_TYPE_SIZE];





  str_osc[PICO_WAVETABLE_SINE]        = str_sine;
  str_osc[PICO_WAVETABLE_SAW]         = str_saw;
  str_osc[PICO_WAVETABLE_PULSE]       = str_pulse;
  str_osc[PICO_WAVETABLE_TRGL]        = str_trgl;

  str_osc[PICO_WAVETABLE_SMSINE]      = str_smsine;
  str_osc[PICO_WAVETABLE_SMSAW]       = str_smsaw;
  str_osc[PICO_WAVETABLE_SMPULSE]     = str_smpulse;
  str_osc[PICO_WAVETABLE_SMTRGL]      = str_smtrgl;

  str_osc[PICO_WAVETABLE_NOISE]       = str_noise;
  str_osc[PICO_WAVETABLE_LFSRNOISE]   = str_lfsr_noise;

  str_fltr_algo[FILTER_ALGO_NOFILTER] = str_fltr_algo_nofilter;
  str_fltr_algo[FILTER_ALGO_BIQUAD]   = str_fltr_algo_biquad;
  str_fltr_algo[FILTER_ALGO_AMSYNTH]  = str_fltr_algo_amsynth;

  str_fltr_type[FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[FILTER_TYPE_BP]       = str_fltr_type_bp;
  str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  str_lfo_type[LFO_TYPE_PITCHLFO]     = str_lfo_type_lfo;
  str_lfo_type[LFO_TYPE_PITCHBEND]    = str_lfo_type_pb;

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
     
    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    case LFO_TYPE:
      return str_lfo_type[this->checkI(LFO_TYPE,paramValue)];

    }
  return str_null;
}



void PicosynthMachine::reset()
{
  sample_num=0;
  last_sample=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


Sint32 PicosynthMachine::tick()
{
  int i=0;
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


  Sint32 cutoff_tmp;
  Sint32 resonance_tmp;
  int    num=8192;
  
//printf("index:%d buffer:%d\n",index,buffer);
  if (index>=SAM | index<0)
    index=0;
  
  if (index==0)
    {
      i=0;
      while (i<SAM)
	{
	  if (trig_time_mode)
	    {
	      if (trig_time_duration_sample<sample_num)
		{
		  this->setI(NOTE_ON,0);
		  trig_time_mode=0;
		}
	    }
	      
	  s_in=adsr_amp->tick();
	  s_in=s_in/6;
	  buffer[i]=s_in;
	  i++;
	}
      /*
      i=0;
      while (i<SAM)
	{
	  buffer[i]=filter.process_one_sample(buffer[i]);
	  i++;
	}    
      */
      filter->process_samples(buffer,SAM);
    }
  
  s_out=buffer[index];
  
  index++;
  sample_num++;
  
  return s_out;
  
}
  

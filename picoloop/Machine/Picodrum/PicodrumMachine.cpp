#include "PicodrumMachine.h"

#define SAM 64

PicodrumMachine::PicodrumMachine() //: adsr_amp(), vco(), filter()
{
  //float fi;
  //int   i;

  DPRINTF("PicodrumMachine::PicodrumMachine()");  
  cutoff=125;
  resonance=10;
  note=0;
  detune=0;
  buffer_picodrum=0;
  index=0;
}



PicodrumMachine::~PicodrumMachine()
{
  DPRINTF("PicodrumMachine::~PicodrumMachine()");
  if (buffer_picodrum)
    free(buffer_picodrum);
}



void PicodrumMachine::init()
{
  vco=new PicodrumVCO();
  adsr_amp=new PicodrumADSR();
  filter=new Filter(); 

  if (buffer_picodrum==0)
    buffer_picodrum = (int32_t*)malloc(sizeof(int32_t)*SAM);
  index=0; // use by this->tick()

  vco->init();
  vco->setNoteDetune(0,0);      
    
  adsr_amp->init();
  adsr_amp->setInput(vco); 

  filter->init();
  sample_num=0;

  //adsr_amp->init();

  note=0;
  detune=0;

}

int PicodrumMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>PICO_WAVETABLE_SIZE-2) return PICO_WAVETABLE_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>PICO_WAVETABLE_SIZE-2) return PICO_WAVETABLE_SIZE-1;
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

    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: PicodrumMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
  return 0;
}


int PicodrumMachine::getI(int what)
{

  if (what==NOTE_ON) return adsr_amp->getNoteOn();
  return 0;
}

void PicodrumMachine::setF(int what,float val)
{
  //  if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  if (what==LFO1_FREQ)           vco->setLfoSpeed(val);
}


void PicodrumMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;
  int noteShift=0;

  if (what==NOTE_ON && val==1) 
    {       
      adsr_amp->reset();
      vco->reset();
      vco->setPicodrumVCOPhase(phase);
      vco->setNoteDetune(note+noteShift,detune);
      adsr_amp->setNoteOn(); 
    }
  if (what==NOTE_ON && val==0) 
    { 
      adsr_amp->setNoteOff(); 
    }

  if (what==NOTE1)               note=val;
  if (what==OSC1_TYPE)           { vco->setOscillator(0,val); osc1_type=val; }
  if (what==OSC2_TYPE)           { vco->setOscillator(1,val); osc2_type=val; }

  if (what==OSC12_MIX)           vco->setPicodrumVCOMix(val);

  if (what==LFO1_DEPTH)          vco->setLfoDepth(val);
  if (what==LFO_TYPE)            vco->setLfoType(val);

  if (what==PITCHBEND_DEPTH)     vco->setPitchBendDepth(val);
  if (what==PITCHBEND_SPEED)     vco->setPitchBendSpeed(val);

  if (what==OSC1_PHASE)          { phase=val; vco->setPicodrumVCOPhase(val); }


  //if (what==LFO1_FREQ)           this->getPicodrumVCO().setLfoSpeed(val);


  if (what==ADSR_ENV0_ATTACK)    adsr_amp->setAttack(val);
  if (what==ADSR_ENV0_RELEASE)   adsr_amp->setRelease(val);



  if (what==FILTER1_CUTOFF)      
    { 
      cutoff=val;
      filter->setCutoff(cutoff);
      adsr_amp->reset();
  }
  
  if (what==FILTER1_RESONANCE)         
    { 
      resonance=val;
      filter->setResonance(resonance);
      adsr_amp->reset();
    }

  if (what==FILTER1_TYPE)     filter->setFilterType(val);
  if (what==FILTER1_ALGO)     filter->setFilterAlgo(val);

  
}



const char * PicodrumMachine::getMachineParamCharStar(int machineParam,int paramValue)
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

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];;
     
    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    }
  return str_null;
}




void PicodrumMachine::reset()
{
  sample_num=0;
  last_sample=0;
  adsr_amp->reset();
  vco->reset();
}


Sint32 PicodrumMachine::tick()
{
  float  f_in;
  float  f_out;
  Sint16 s_in;
  Sint16 s_out;
  int        i;

  // there is something wrong here, need to debug it
  // return filter.process_one_sample(adsr_amp.tick());
  //return adsr_amp.tick();
  if (index>=SAM | index<0)
    index=0;

  if (index==0)
    {
      i=0;
      while (i<SAM)
	{	  
	  buffer_picodrum[i]=adsr_amp->tick();
	  //buffer_picodrum[i]=vco->tick();
	  i++;
	  //printf("i:%d\n");
	}
      //printf("%d %d %d %d ",buffer_picodrum[0],buffer_picodrum[1],buffer_picodrum[2],buffer_picodrum[3]);
      /*
      i=0;      
      while (i<SAM)
	{
	  buffer_picodrum[i]=filter.process_one_sample(buffer_picodrum[i]);
	  i++;
	}
      */
      filter->process_samples(buffer_picodrum,SAM);
    }
    
  s_out=buffer_picodrum[index];
  
  index++;
  sample_num++;
  return s_out;
}

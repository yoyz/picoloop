#include "Master.h"
#include "CursynthMachine.h"

#define SAM 256

CursynthMachine::CursynthMachine()
{
  float fi;
  int   i;
  printf("CursynthMachine::CursynthMachine()\n");  
  note_on=0;
  buffer_i=0;
  buffer_f=0;

}



CursynthMachine::~CursynthMachine()
{
  printf("CursynthMachine::~CursynthMachine()\n");
}




void CursynthMachine::init()
{
  int i;
  int j;
  if (buffer_i==0 ||
      buffer_f==0)
    {
      CSE=new mopo::CursynthEngine();
      //CS->setBufferSize(SAM);     
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
      buffer_f = (mopo::mopo_float*)malloc(sizeof(mopo::mopo_float)*SAM*8);

      CSE->setBufferSize(SAM);
      CSE->setSampleRate(44100);

    }
  for (i=0;i<SAM;i++)
    buffer_i[i]=0;
  for (i=0;i<SAM;i++)
    buffer_f[i]=0.0;


  //bq.reset();
  note_on=0;

  sample_num=0;


  note=0;
  detune=64;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


int CursynthMachine::checkI(int what,int val)
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
      if (val>FILTER_TYPE_SIZE-1) return FILTER_TYPE_SIZE-1;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>FILTER_ALGO_SIZE-1) return FILTER_ALGO_SIZE-1;
      return val;
      break;


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      printf("WARNING: CursynthMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
  return val;
}

int CursynthMachine::getI(int what)
{
  if (what==NOTE_ON) return note_on;
}

void CursynthMachine::setF(int what,float val)
{
  //if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  //if (what==LFO1_FREQ)           this->getVCO().setLfoSpeed(val);
}

void CursynthMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }

  if (what==NOTE_ON && val==1) 
    { 
      NoteFreq & NF = NoteFreq::getInstance(); 
      note_on=1;
      //CS->noteOn(NF.getINoteFreq(note),1.0);
      CSE->noteOn(note,1.0);
      CSE->getControls();
    }
  if (what==NOTE_ON && val==0) 
    { 
      note_on=0;
      CSE->noteOff(val);
    }

  if (what==OSC1_NOTE)           note=val;

  if (what==ADSR_ENV0_ATTACK)    CSE->getControls().at("amp attack")->set(val);

  
}

const char * CursynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_null       = "NULL ";

  static const char * str_sine       = "SINE ";
  static const char * str_saw        = "SAW  ";
  static const char * str_pulse      = "PULSE";
  static const char * str_trgl       = "TRGL ";
  static const char * str_noise      = "NOISE";
  static const char * str_wtbl       = "WTBL ";

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
  str_osc[PICO_WAVETABLE_NOISE]       = str_noise;

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
      return str_osc[paramValue];
    case OSC2_TYPE:
      return str_osc[paramValue];
     
    case FILTER1_ALGO:
      return str_fltr_algo[paramValue];

    case FILTER1_TYPE:
      return str_fltr_type[paramValue];

    case LFO_TYPE:
      return str_lfo_type[paramValue];

    }
  return str_null;
}



void CursynthMachine::reset()
{
  sample_num=0;
  last_sample=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


int CursynthMachine::tick()
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
  //Sint16 index;

  Sint32 cutoff_tmp;
  Sint32 resonance_tmp;
  int    num=8192;
  int    i;


  if (index>=SAM | 
      index<0)
    index=0;
  

  if (trig_time_mode)
    {
      if (trig_time_duration_sample<sample_num)
	{
	  this->setI(NOTE_ON,0);
	  trig_time_mode=0;
	  //printf("\t\t\t\t\t\tDONE\n");
	}

    }
      
  if (sample_num==0 || 
      index==0 )
    {
      CSE->process();
    }
 
  buffer_f[index]=CSE->output()->buffer[index];
  buffer_f[index]=buffer_f[index]*16384;
  buffer_i[index]=buffer_f[index];
    
  s_out=buffer_i[index];

  index++;
  sample_num++;
  last_sample=s_out;

  return s_out;

}

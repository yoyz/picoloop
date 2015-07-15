#include "PBSynthMachine.h"
//#define SAM 512
#define SAM 64

PBSynthMachine::PBSynthMachine()
{
  DPRINTF("PBSynthMachine::PBSynthMachine()\n");  
  buffer_f=0;
  buffer_i=0;
  cutoff=125;
  resonance=10;
  index=0;

  lfo_depth=0;
  lfo_depth_shift=20;
  lfo_speed=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;

}


PBSynthMachine::~PBSynthMachine()
{
  DPRINTF("PBSynthMachine::~PBSynthMachine()\n");  
  if (buffer_f)
    free(buffer_f);
  if (buffer_i)
    free(buffer_i);

}


void PBSynthMachine::init()
{
  DPRINTF("PBSynthMachine::init()\n");  
  int i;

  //HO(44100);
  if (buffer_f==0)
    {
      SE=new SynthEngine(SAM,100);
      buffer_f = (mfloat*)malloc(sizeof(mfloat)*SAM);
    }
  if (buffer_i==0)
    {
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM);
    }

  DPRINTF("buffer_f:0x%08.8X\n",buffer_f);
  DPRINTF("buffer_i:0x%08.8X\n",buffer_i);
  for (i=0;i<SAM;i++)
    {
      buffer_f[i]=0;
      buffer_i[i]=0;
    }
  sample_num=0;
  index=0;
  freq=110.0;
  keyon=0;

  SE->init();
  SE->setPBSynthFilter24dB(1);
  //SE.getPBSynthOscillator(0)->setWave(0);
  //SE.getPBSynthOscillator(1)->setWave(0);
  //SE.reset();
}

const char * PBSynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_pbsynth_null       = " NULL";
  static const char * str_pbsynth_sqr        = "  SQR";
  static const char * str_pbsynth_trgl       = " TRGL";
  static const char * str_pbsynth_saw        = "  SAW";

  const char * str_osc[PICO_PBSYNTH_SIZE];

  static const char * str_fltr_algo_nofilter = "NOFL";
  static const char * str_fltr_algo_biquad   = "BIQU";
  static const char * str_fltr_algo_amsynth  = "AMST";
  
  const        char * str_fltr_algo[FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp   = "LP";
  static const char * str_fltr_type_bp   = "BP";
  static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[FILTER_TYPE_SIZE];

  static const char * str_fm_type_am      = "2OP_AM ";
  static const char * str_fm_type_fm      = "2OP_FM ";

  const        char * str_fm_type[FM_TYPE_SIZE];


  str_osc[PICO_PBSYNTH_SQUARE]        = str_pbsynth_sqr;
  str_osc[PICO_PBSYNTH_SAW]           = str_pbsynth_saw;
  str_osc[PICO_PBSYNTH_TRIANGLE]      = str_pbsynth_trgl;

  str_fltr_algo[FILTER_ALGO_NOFILTER] = str_fltr_algo_nofilter;
  str_fltr_algo[FILTER_ALGO_BIQUAD]   = str_fltr_algo_biquad;
  str_fltr_algo[FILTER_ALGO_AMSYNTH]  = str_fltr_algo_amsynth;

  str_fltr_type[FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[FILTER_TYPE_BP]       = str_fltr_type_bp;
  str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  str_fm_type[FM_TYPE_AM]             = str_fm_type_am;
  str_fm_type[FM_TYPE_FM]             = str_fm_type_fm;



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

    case FM_TYPE:
      return str_fm_type[paramValue];

    }
  return str_pbsynth_null;
}



void PBSynthMachine::reset()
{
 sample_num=0;
 keyon=0;

 trig_time_mode=0;
 trig_time_duration=0;
 trig_time_duration_sample=0;
}

int PBSynthMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0)                  return 0;
      if (val>=PICO_PBSYNTH_SIZE) return PICO_PBSYNTH_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0)                  return 0;
      if (val>=PICO_PBSYNTH_SIZE) return PICO_PBSYNTH_SIZE-1;
      return val;
      break;

    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: PBSynthMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int PBSynthMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
}

void PBSynthMachine::setF(int what,float val)
{
  float f_val=val;
  f_val=f_val/128;

  //if (what==OSC1_FREQ)           { freq=val; }
  //  if (what==LFO1_FREQ)           { lfo_speed=val/4.0; sineLfoOsc1.setFreq(lfo_speed); }
  if (what==LFO1_FREQ)           SE->getLFO(0)->setRate(f_val*100);
  if (what==LFO2_FREQ)           SE->getLFO(1)->setRate(f_val*100);

}



void PBSynthMachine::setI(int what,int val)
{
  float        f_val_cutoff;
  float        f_val_resonance;
  float        f_val;

  f_val=val;
  f_val=f_val/128;
  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }


    if (what==NOTE_ON && val==1) 
    { 
      keyon=1;
      SE->releaseNote();
      SE->triggerNote(note);
      // sineLfoOsc1.reset();
      // NoteFreq & NF = NoteFreq::getInstance();
      // HO->KeyOn(1,NF.getINoteFreq(note));
      // freq=NF.getINoteFreq(note);
    }

    if (what==NOTE_ON && val==0) 
    { 
      SE->releaseNote();
      keyon=0;
    }

    if (what==OSC1_TYPE)           
      { 
	osc1_type=val;
	SE->getPBSynthOscillator(0)->setWave(this->checkI(OSC1_TYPE,val));
      }
    if (what==OSC2_TYPE)           
      {
	osc2_type=val;
	//SE.getPBSynthOscillator(1)->setWave(val%2);
	SE->getPBSynthOscillator(1)->setWave(this->checkI(OSC2_TYPE,val));
      }

    if (what==ADSR_ENV0_ATTACK)    SE->getEnvelope(0)->setA(-1+f_val);
    if (what==ADSR_ENV0_DECAY)     SE->getEnvelope(0)->setD(-1+f_val);
    if (what==ADSR_ENV0_SUSTAIN)   SE->getEnvelope(0)->setS( f_val);
    if (what==ADSR_ENV0_RELEASE)   SE->getEnvelope(0)->setR(-1+f_val);


    if (what==ADSR_ENV1_ATTACK)    SE->getEnvelope(1)->setA(-1+f_val);
    if (what==ADSR_ENV1_DECAY)     SE->getEnvelope(1)->setD(-1+f_val);
    if (what==ADSR_ENV1_SUSTAIN)   SE->getEnvelope(1)->setS( 1+f_val);
    if (what==ADSR_ENV1_RELEASE)   SE->getEnvelope(1)->setR(-1+f_val);

    //if (what==VCO_MIX)             SE.setParameter(SENGINE_OSCMIX,1.0f/val);
    //if (what==VCO_MIX)             SE.setParameter(SENGINE_ENV2_TO_CUTOFF,1.0f/(val+1));
    //if (what==VCO_MIX)             SE.setParameter(SENGINE_ENV2_TO_CUTOFF,(f_val*2)-1);
    //if (what==OSC1_PHASE)          SE->setParameter(SENGINE_ENV2_TO_CUTOFF,(f_val*2)-1);
    if (what==OSC1_PHASE)          SE->setParameter(SENGINE_ENV2_TO_CUTOFF,(-f_val*2)+1);
    if (what==VCO_MIX)             SE->setParameter(SENGINE_OSCMIX,(f_val));

    if (what==LFO1_DEPTH)          SE->setParameter(SENGINE_LFO1_TO_AMP,(f_val));
    //if (what==LFO1_FREQ)           SE->setParameter(SENGINE_LFO1_TO_AMP,(f_val));


    if (what==LFO2_DEPTH)          SE->setParameter(SENGINE_LFO2_TO_CUTOFF,(f_val));

  if (what==OSC1_NOTE)                note=val;
    //if (what==OSC1_FREQ)           freq=val;

  // if (what==FILTER1_CUTOFF)         SE.setParameter(SENGINE_FILTFREQ,(2.0f/(val)-1));
  // if (what==FILTER1_RESONANCE)      SE.setParameter(SENGINE_FILTRESO,(2.0f/(val)-1));

  // if (what==FILTER1_CUTOFF)         SE.setParameter(SENGINE_FILTFREQ,1.0f/(val+1));
  // if (what==FILTER1_RESONANCE)      SE.setParameter(SENGINE_FILTRESO,1.0f/(val+1));
  if (what==FILTER1_CUTOFF)         SE->setParameter(SENGINE_FILTFREQ,(f_val*2)-1);
  if (what==FILTER1_RESONANCE)      SE->setParameter(SENGINE_FILTRESO,(f_val*2)-1);

  if (what==OSC1_AMP)               SE->setParameter(SENGINE_ENV1_TO_OSC1PW,(f_val*2)-1);
  if (what==OSC2_AMP)               SE->setParameter(SENGINE_ENV1_TO_OSC2PW,(f_val*2)-1);

}

int PBSynthMachine::tick()
{
  Sint16 s_in;
  Sint32 s_in32;
  Sint16 s_out;
  int    modulated_freq;
  int i;
  float buf_f;

  if (index>=SAM | 
      index<0)
    index=0;


  if ( index==0 )
    {
      SE->process(buffer_f,SAM);
      for(i=0;i<SAM;i++)
       	{
       	  //buffer[i]=buffer[i]*2048;
	  buffer_i[i]=buffer_f[i]*2048;
       	}
    }


  if (trig_time_mode)
    {
      if (trig_time_duration_sample<sample_num)
	{
	  this->setI(NOTE_ON,0);
	  trig_time_mode=0;
	  //DPRINTF("\t\t\t\t\t\tDONE\n");
	}

    }
  s_in32=buffer_i[index];
  if (s_in32>32000)  s_in32=32000;
  if (s_in32<-32000) s_in32=-32000;
  s_out=s_in32;

  index++;
  sample_num++;

  return s_out;
}


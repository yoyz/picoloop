#include "PBSynthMachine.h"
//#define SAM 512
#define SAM 512

//enum {
//  FMTYPE_2_OP_AM,
//  FMTYPE_2_OP_FM
//};


int arpMode = 0;
int arpSpeed = 200;
int arpOctaveMax = 4;
int arpInc = 1;
int oscBase[2];
int note[2];


PBSynthMachine::PBSynthMachine() : SE(SAM,100)
{
  printf("PBSynthMachine::PBSynthMachine()\n");  
  buffer=0;
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
  printf("PBSynthMachine::~PBSynthMachine()\n");  
  if (buffer)
    free(buffer);
}


void PBSynthMachine::init()
{
  printf("PBSynthMachine::init()\n");  
  int i;

  //HO(44100);
  if (buffer==0)
    {
      buffer = (mfloat*)malloc(sizeof(mfloat)*SAM*8);
    }

  printf("buffer:0x%08.8X\n",buffer);
  for (i=0;i<SAM;i++)
    buffer[i]=0;

  sample_num=0;
  index=0;
  freq=110.0;
  keyon=0;

  SE.init();
  SE.setPBSynthFilter24dB(1);
  //SE.getPBSynthOscillator(0)->setWave(0);
  //SE.getPBSynthOscillator(1)->setWave(0);
  //SE.reset();
}

const char * PBSynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_null       = "NULL ";

  static const char * str_opl2_sine  = "  SIN";
  static const char * str_opl2_hsin  = " HSIN";
  static const char * str_opl2_absin = "ABSIN";
  static const char * str_opl2_qsin  = " PSIN";

  static const char * str_opl2_epsin = "EPSIN";
  static const char * str_opl2_asin  = " ASIN";
  static const char * str_opl2_sqr   = "  SQR";
  static const char * str_opl2_dsqr  = " DSQR";

  const char * str_osc[PICO_DBOPL_SIZE];

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


  str_osc[PICO_DBOPL_SINE]            = str_opl2_sine;
  str_osc[PICO_DBOPL_HSIN]            = str_opl2_hsin;
  str_osc[PICO_DBOPL_ABSIN]           = str_opl2_absin;
  str_osc[PICO_DBOPL_QSIN]            = str_opl2_qsin;

  str_osc[PICO_DBOPL_EPSIN]           = str_opl2_epsin;
  str_osc[PICO_DBOPL_ASIN]            = str_opl2_asin;
  str_osc[PICO_DBOPL_SQR]             = str_opl2_sqr;
  str_osc[PICO_DBOPL_DSQR]            = str_opl2_dsqr;

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
  return str_null;
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
      if (val<0) return 0;
      if (val>2) return 2;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>2) return 2;
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
      printf("WARNING: Dboplmachine::checkI(%d,%d)\n",what,val);
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
  //if (what==OSC1_FREQ)           { freq=val; }
  //  if (what==LFO1_FREQ)           { lfo_speed=val/4.0; sineLfoOsc1.setFreq(lfo_speed); }
}



void PBSynthMachine::setI(int what,int val)
{
  float        f_val_cutoff;
  float        f_val_resonance;
  //FreqMultiple freqM[15];
  //Waveform     w[4];
  // w[0]=SIN;
  // w[1]=HALF_SIN;
  // w[2]=ABS_SIN;
  // w[3]=QUART_SIN;

  // freqM[0]=xHALF;
  // freqM[1]=x1;
  // freqM[2]=x2;
  // freqM[3]=x3;
  // freqM[4]=x4;
  // freqM[5]=x5;
  // freqM[6]=x6;
  // freqM[7]=x7;
  // freqM[8]=x8;
  // freqM[9]=x9;
  // freqM[10]=x10;
  // freqM[11]=x12;
  // freqM[12]=x15;

  //HO->EnableAdditiveSynthesis(1,true);

  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }


    if (what==NOTE_ON && val==1) 
    { 
      keyon=1;
      SE.triggerNote(note);
      // sineLfoOsc1.reset();
      // NoteFreq & NF = NoteFreq::getInstance();
      // HO->KeyOn(1,NF.getINoteFreq(note));
      // freq=NF.getINoteFreq(note);
    }

    if (what==NOTE_ON && val==0) 
    { 
      SE.releaseNote();
      keyon=0;
    }

    if (what==OSC1_TYPE)           
      { 
	osc1_type=val;
	//SE.getPBSynthOscillator(0)->setWave(val%2);
	SE.getPBSynthOscillator(0)->setWave(val);
	//HO->SetWaveform(1,1,w[val]);
	//HO->EnableWaveformControl();
      }
    if (what==OSC2_TYPE)           
      {
	osc2_type=val;
	//SE.getPBSynthOscillator(1)->setWave(val%2);
	SE.getPBSynthOscillator(1)->setWave(val);
	// HO->SetWaveform(1,2,w[val]);
	// HO->EnableWaveformControl();
      }


    if (what==ADSR_ENV0_ATTACK)    SE.getEnvelope(0)->setA(-1.0f/val);
    if (what==ADSR_ENV0_DECAY)     SE.getEnvelope(0)->setD(-1.0f/val);
    if (what==ADSR_ENV0_SUSTAIN)   SE.getEnvelope(0)->setS( 1.0f/val);
    if (what==ADSR_ENV0_RELEASE)   SE.getEnvelope(0)->setR(-1.0f/val);


    if (what==ADSR_ENV1_ATTACK)    SE.getEnvelope(1)->setA(-1.0f/val);
    if (what==ADSR_ENV1_DECAY)     SE.getEnvelope(1)->setD(-1.0f/val);
    if (what==ADSR_ENV1_SUSTAIN)   SE.getEnvelope(1)->setS( 1.0f/val);
    if (what==ADSR_ENV1_RELEASE)   SE.getEnvelope(1)->setR(-1.0f/val);

    //if (what==VCO_MIX)             SE.setParameter(SENGINE_OSCMIX,1.0f/val);
    if (what==VCO_MIX)             SE.setParameter(SENGINE_ENV2_TO_CUTOFF,1.0f/val);


  if (what==OSC1_NOTE)                note=val;
    //if (what==OSC1_FREQ)           freq=val;

  // if (what==FILTER1_CUTOFF)         SE.setParameter(SENGINE_FILTFREQ,(2.0f/(val)-1));
  // if (what==FILTER1_RESONANCE)      SE.setParameter(SENGINE_FILTRESO,(2.0f/(val)-1));

  if (what==FILTER1_CUTOFF)         SE.setParameter(SENGINE_FILTFREQ,1.0f/val);
  if (what==FILTER1_RESONANCE)      SE.setParameter(SENGINE_FILTRESO,1.0f/val);

    // if (what==FILTER1_CUTOFF)      
    //   { 
    // 	cutoff=val;
    // 	filter.setCutoff(cutoff);
    //   }
    
    // if (what==FILTER1_RESONANCE)         
    //   { 
    // 	resonance=val;
    // 	filter.setResonance(resonance);
    //   }
    
    // if (what==FILTER1_TYPE)     filter.setPBSynthFilterType(val);
    // if (what==FILTER1_ALGO)     filter.setPBSynthFilterAlgo(val);
    
    
  
}

int PBSynthMachine::tick()
{
  Sint16 s_in;
  Sint32 s_in32;
  Sint16 s_out;
  int    modulated_freq;
  int i;

  if (index>=SAM | 
      index<0)
    index=0;

  if (trig_time_mode)
    {
      if (trig_time_duration_sample<sample_num)
	{
	  this->setI(NOTE_ON,0);
	  trig_time_mode=0;
	  //[5~printf("\t\t\t\t\t\tDONE\n");
	}

    }




  if (sample_num==0 || 
      index==0 )
    {
      //modulated_freq=(sineLfoOsc1.tick()>>lfo_depth_shift);
      //modulated_freq=((sineLfoOsc1.tick()>>5)/(128-lfo_depth));
      
      //HO->Generate(SAM,buffer);
      SE.process(buffer,SAM);
      for(i=0;i<SAM;i++)
       	{
       	  //buffer[i]=buffer[i]/64;
       	  buffer[i]=buffer[i]*2048;
       	}
       // for(i=0;i<32;i++)
       // 	 printf("%d\t",buffer[i]);
       // printf("\n");
      
      // for(i=0;i<SAM;i++)
      // 	{
      // 	  printf("%d\t",buffer[i]);
      // 	}

    }



  //s_in=(mfloat)buffer[index];
  s_in32=buffer[index];
  //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %d %d %d %d\n",index,s_in32,buffer[index],sizeof(mfloat));
  //s_out=filter.process(s_in);
  s_out=s_in32;

  index++;
  sample_num++;

  return s_out;
}


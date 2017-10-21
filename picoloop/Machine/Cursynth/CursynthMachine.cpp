#include "Master.h"
#include "CursynthMachine.h"

//#define SAM 256
#define SAM 64

CursynthMachine::CursynthMachine()
{
  float fi;
  int   i;
  DPRINTF("CursynthMachine::CursynthMachine()");  
  note_on=0;
  buffer_i=0;
  buffer_f=0;
  index=0;
}



CursynthMachine::~CursynthMachine()
{
  DPRINTF("CursynthMachine::~CursynthMachine()");
}




void CursynthMachine::init()
{
  int i;
  int j;
  if (buffer_i==0 ||
      buffer_f==0)
    {
      CSE=new mopocursynth::CursynthEngine();
      //CS->setBufferSize(SAM);     
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
      buffer_f = (mopocursynth::mopo_float*)malloc(sizeof(mopocursynth::mopo_float)*SAM*8);

      CSE->setBufferSize(SAM);
      //CSE->setSampleRate(48000);
      //CSE->setSampleRate(44100);
      //CSE->setSampleRate(44100);
      CSE->setSampleRate(DEFAULTFREQ);

    }
  for (i=0;i<SAM;i++)
    buffer_i[i]=0;
  for (i=0;i<SAM;i++)
    buffer_f[i]=0.0;

  //CSE->getControls().at("delay time")->set(0.1);      
  //CSE->getControls().at("delay dry/wet")->set(0.2);   
  CSE->getControls().at("delay time")->set(0.0);      
  CSE->getControls().at("delay dry/wet")->set(0.0);   

  //CSE->getControls().at("delay feedback")->set(0.4);
  CSE->getControls().at("delay feedback")->set(0);
  CSE->getControls().at("cross modulation")->set(0);  

  CSE->getControls().at("osc 2 transpose")->set(0); 
  CSE->getControls().at("osc 2 tune")->set(0); 

  CSE->getControls().at("volume")->set(0.1);

  CSE->getControls().at("lfo 1 waveform")->set(2); 
  CSE->getControls().at("lfo 2 waveform")->set(2); 


  CSE->getControls().at("mod source 1")->set(3);     
  CSE->getControls().at("mod source 2")->set(4);     
  CSE->getControls().at("mod destination 1")->set(4);     
  CSE->getControls().at("mod destination 2")->set(2);     
  CSE->getControls().at("mod scale 1")->set(0);     
  CSE->getControls().at("mod scale 2")->set(0);     
  //CSE->getControls().at("lfo 1 frequency")->set(2.2);     
  CSE->getControls().at("lfo 1 frequency")->set(0);     

  CSE->getControls().at("polyphony")->set(1);     

  //CSE->getControls().at("pitch wheel")->set();

  //bq.reset();
  note_on=0;

  sample_num=0;
  index=0;


  note=0;
  //detune=64;
  detune=0;

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
      if (val>=PICO_CURSYNTH_SIZE-1) return PICO_CURSYNTH_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>=PICO_CURSYNTH_SIZE-1) return PICO_CURSYNTH_SIZE-1;
      return val;
      break;

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>=CURSYNTH_FILTER_TYPE_SIZE-1) return CURSYNTH_FILTER_TYPE_SIZE-1;
      return val;
      break;

    case LFO1_WAVEFORM:
      if (val<0) return 0;
      if (val>=PICO_CURSYNTH_SIZE-2) return PICO_CURSYNTH_SIZE-2;
      return val;
      break;


    case LFO2_WAVEFORM:
      if (val<0) return 0;
      if (val>=PICO_CURSYNTH_SIZE-2) return PICO_CURSYNTH_SIZE-2;
      return val;
      break;


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: CursynthMachine::checkI(%d,%d)\n",what,val);
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
  float f_val_cutoff;
  float f_val_resonance;

  float f_val=val;
  f_val=f_val/128;

  //if (what==LFO1_FREQ)             CSE->getControls().at("lfo 1 frequency")->set(f_val*10); 
  if (what==LFO1_FREQ)             CSE->getControls().at("lfo 1 frequency")->set(f_val*10); 
  if (what==LFO2_FREQ)             CSE->getControls().at("lfo 2 frequency")->set(f_val*10); 
}

void CursynthMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;
  int   noteShift=11;

  float f_val=val;
  f_val=f_val/128;

  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }

  if (what==NOTE_ON && val==1) 
    { 
      //NoteFreq & NF = NoteFreq::getInstance(); 
      note_on=1;

      if (old_note!=note)
	{
	  CSE->noteOn(note+noteShift,velocity/32);
	  CSE->noteOff(old_note+noteShift);
	}
      else
	{
	  if (note_on==0)
	    CSE->noteOn(note+noteShift,velocity/32);
	  else
	    {
	      CSE->noteOff(old_note+noteShift);	  
	      CSE->noteOn(note+noteShift,velocity/32);
	    }
	}

      //CS->noteOn(NF.getINoteFreq(note),1.0);
      //CSE->noteOn(note+11,0.8);
      //CSE->noteOn(note+11,velocity/32);
      //CSE->getControls();
      //f_val=trig_time_duration;
      //CSE->getControls().at("velocity track")->set(trig_time_duration);
    }
  if (what==NOTE_ON && val==0) 
    { 
      note_on=0;
      CSE->noteOff(note+noteShift);
    }

  //if (what==OSC1_NOTE)           { if (note_on) { this->setI(NOTE_ON,0); note=val; } }
  if (what==NOTE1)              {  old_note=note; note=val; }

  //if (what==ADSR_ENV0_ATTACK)    CSE->getControls().at("amp attack")->set(val);
  //delay time"
  //if (what==ADSR_ENV0_ATTACK)    CSE->getControls().at("delay time")->set(f_val);
  //if (what==ADSR_ENV0_RELEASE)   CSE->getControls().at("delay dry/wet")->set(f_val);
  //if (what==ADSR_ENV0_DECAY)     CSE->getControls().at("delay feedback")->set(f_val);

  //  if (what==OSC1_SCALE)          { CSE->getControls().at("osc 1 transpose")->set(f_val*128); }
  if (what==OSC2_SCALE)          { CSE->getControls().at("osc 2 transpose")->set(f_val*128); }


  if (what==OSC1_TYPE)           { CSE->getControls().at("osc 1 waveform")->set(f_val*128); }
  if (what==OSC2_TYPE)           { CSE->getControls().at("osc 2 waveform")->set(f_val*128); }

  if (what==LFO1_WAVEFORM)       { CSE->getControls().at("lfo 1 waveform")->set(f_val*128); }
  if (what==LFO2_WAVEFORM)       { CSE->getControls().at("lfo 2 waveform")->set(f_val*128); }


  if (what==ADSR_ENV0_ATTACK)    CSE->getControls().at("amp attack")->set(f_val*3);
  if (what==ADSR_ENV0_DECAY)     CSE->getControls().at("amp decay")->set(f_val*3);
  if (what==ADSR_ENV0_SUSTAIN)   CSE->getControls().at("amp sustain")->set(f_val);
  if (what==ADSR_ENV0_RELEASE)   CSE->getControls().at("amp release")->set(f_val*3);

  if (what==ADSR_ENV1_ATTACK)    CSE->getControls().at("fil attack")->set(f_val*3);
  if (what==ADSR_ENV1_DECAY)     CSE->getControls().at("fil decay")->set(f_val*3);
  if (what==ADSR_ENV1_SUSTAIN)   CSE->getControls().at("fil sustain")->set(f_val);
  if (what==ADSR_ENV1_RELEASE)   CSE->getControls().at("fil release")->set(f_val*3);


  if (what==LFO1_ENV_AMOUNT)     CSE->getControls().at("mod scale 1")->set(f_val);
  if (what==LFO2_ENV_AMOUNT)     CSE->getControls().at("mod scale 2")->set(f_val);


  if (what==VELOCITY)            velocity=val;
  if (what==OSC12_MIX)           CSE->getControls().at("osc mix")->set(f_val);
  //if (what==OSC12_MIX)           CSE->getControls().at("amp env depth")->set(f_val*12);


  //if (what==OSC12_MIX)           CSE->getControls().at("osc mix")->set(f_val);

  //if (what==OSC1_AMP)            CSE->getControls().at("velocity track")->set(f_val);

  //if (what==OSC1_DETUNE)         CSE->getControls().at("osc 2 tune"   )->set((f_val*2)-1);
  //if (what==OSC1_DETUNE)         CSE->getControls().at("osc 2 tune"   )->set(((f_val*2)-1)*128);

  if (what==OSC1_DETUNE)         CSE->getControls().at("osc 2 tune"   )->set(((f_val*2)-1));
  if (what==OSC1_MOD)            CSE->getControls().at("cross modulation")->set(f_val);

  if (what==KEYTRACK)            CSE->getControls().at("keytrack"     )->set(((f_val*2)-1)*128);
  if (what==ENV1_DEPTH)            CSE->getControls().at("fil env depth")->set(((f_val*2)-1)*128);
  //if (what==OSC1_AMP)            CSE->getControls().at("velocity track")->set(f_val);
  
  if (what==FILTER1_TYPE)        CSE->getControls().at("filter type")->set(f_val*128);
    //filter.setFilterType(val);
  //"filter type"


  if (what==FILTER1_CUTOFF)      
    { 
      CSE->getControls().at("cutoff")->set(28+f_val*100);
    }
  
  if (what==FILTER1_RESONANCE)         
    { 
      //CSE->getControls().at("resonance")->set(0.5+(f_val*10));
      CSE->getControls().at("resonance")->set(0+(f_val*10));
    }

  if (what==FX1_DEPTH)          CSE->getControls().at("delay dry/wet")->set(f_val*128);   
  if (what==FX1_SPEED)          CSE->getControls().at("delay time"   )->set(f_val*128);      
  

}

const char * CursynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_null       = "NULL ";

  static const char * str_sin           = "SIN  ";
  static const char * str_triangle      = "TRGL ";
  static const char * str_square        = "SQR  ";
  static const char * str_downsaw       = "DSAW ";
  static const char * str_upsaw         = "USAW ";

  static const char * str_threestep     = "TSTEP";
  static const char * str_fourstep      = "FSTEP";
  static const char * str_heightstep    = "HSTEP";

  static const char * str_threepyramid  = "TPYR ";
  static const char * str_fivepyramid   = "FPYR ";
  static const char * str_ninepyramid   = "NPYR ";
  static const char * str_whitenoise    = "NOISE";

  //static const char * str_wtbl          = "WTBL ";

  const        char * str_osc[PICO_CURSYNTH_SIZE];


  static const char * str_fltr_algo_nofilter = "NOFL";
  static const char * str_fltr_algo_biquad   = "BIQU";
  static const char * str_fltr_algo_amsynth  = "AMST";
  
  const        char * str_fltr_algo[FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp   = "LP";
  static const char * str_fltr_type_hp   = "HP";
  //static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[CURSYNTH_FILTER_TYPE_SIZE];


  static const char * str_lfo_type_lfo   = "PLFO";
  static const char * str_lfo_type_pb    = "PB";

  const        char * str_lfo_type[LFO_TYPE_SIZE];





  str_osc[PICO_CURSYNTH_SIN]              = str_sin;
  str_osc[PICO_CURSYNTH_TRIANGLE]         = str_triangle;
  str_osc[PICO_CURSYNTH_SQUARE]           = str_square;
  str_osc[PICO_CURSYNTH_DOWNSAW]          = str_downsaw;
  str_osc[PICO_CURSYNTH_UPSAW]            = str_upsaw;

  str_osc[PICO_CURSYNTH_THREESTEP]        = str_threestep;
  str_osc[PICO_CURSYNTH_FOURSTEP]         = str_fourstep;
  str_osc[PICO_CURSYNTH_HEIGHTSTEP]       = str_heightstep;

  str_osc[PICO_CURSYNTH_THREEPYRAMID]     = str_threepyramid;
  str_osc[PICO_CURSYNTH_FIVEPYRAMID]      = str_fivepyramid;
  str_osc[PICO_CURSYNTH_NINEPYRAMID]      = str_ninepyramid;

  str_osc[PICO_CURSYNTH_WHITENOISE]       = str_whitenoise;

  // str_fltr_algo[FILTER_ALGO_NOFILTER] = str_fltr_algo_nofilter;
  // str_fltr_algo[FILTER_ALGO_BIQUAD]   = str_fltr_algo_biquad;
  // str_fltr_algo[FILTER_ALGO_AMSYNTH]  = str_fltr_algo_amsynth;

  str_fltr_type[CURSYNTH_FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[CURSYNTH_FILTER_TYPE_HP]       = str_fltr_type_hp;
  //str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  str_lfo_type[LFO_TYPE_PITCHLFO]     = str_lfo_type_lfo;
  str_lfo_type[LFO_TYPE_PITCHBEND]    = str_lfo_type_pb;

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];
     
    // case FILTER1_ALGO:
    //   return str_fltr_algo[paramValue];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    case LFO_TYPE:
      return str_lfo_type[this->checkI(LFO_TYPE,paramValue)];

    case LFO1_WAVEFORM:
      return str_osc[this->checkI(LFO1_WAVEFORM,paramValue)];
    case LFO2_WAVEFORM:
      return str_osc[this->checkI(LFO2_WAVEFORM,paramValue)];


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


Sint32 CursynthMachine::tick()
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
  

      
  if (//sample_num==0 || 
      index==0 )
    {
      CSE->process();
      for (index==0;index<SAM;index++)
	{
	  buffer_f[index]=CSE->output()->buffer[index];
	  buffer_f[index]=buffer_f[index]*8192;
	  buffer_i[index]=buffer_f[index];
	}
      index=0;
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

 
  //buffer_f[index]=buffer_f[index]*14336;
  //buffer_f[index]=buffer_f[index]*2048;
  //buffer_f[index]=buffer_f[index]*1024;

    
  s_out=buffer_i[index];

  index++;
  sample_num++;
  last_sample=s_out;

  return s_out;

}

#include "Master.h"
#include "TwytchsynthMachine.h"

//#define SAM 256
#define SAM 64

TwytchsynthMachine::TwytchsynthMachine()
{
  float fi;
  int   i;
  printf("TwytchsynthMachine::TwytchsynthMachine()\n");  
  note_on=0;
  buffer_i=0;
  buffer_f=0;

}



TwytchsynthMachine::~TwytchsynthMachine()
{
  printf("TwytchsynthMachine::~TwytchsynthMachine()\n");
}




void TwytchsynthMachine::init()
{
  int i;
  int j;
  if (buffer_i==0 ||
      buffer_f==0)
    {
      TWE=new mopotwytchsynth::HelmEngine();
      //CS->setBufferSize(SAM);     
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
      buffer_f = (mopotwytchsynth::mopo_float*)malloc(sizeof(mopotwytchsynth::mopo_float)*SAM*8);

      TWE->setBufferSize(SAM);
      TWE->setSampleRate(44100);

    }
  for (i=0;i<SAM;i++)
    buffer_i[i]=0;
  for (i=0;i<SAM;i++)
    buffer_f[i]=0.0;

  //TWE->getControls().at("delay time")->set(0.1);      
  //TWE->getControls().at("delay dry/wet")->set(0.2);   
  // TWE->getControls().at("delay time")->set(0.0);      
  // TWE->getControls().at("delay dry/wet")->set(0.0);   

  // TWE->getControls().at("delay feedback")->set(0.4);
  // TWE->getControls().at("cross modulation")->set(0);  

  // TWE->getControls().at("osc 2 transpose")->set(0); 
  // TWE->getControls().at("osc 2 tune")->set(0); 
  // TWE->getControls().at("volume")->set(0.1);

  // TWE->getControls().at("lfo 1 waveform")->set(2); 
  // TWE->getControls().at("lfo 2 waveform")->set(2); 


  // TWE->getControls().at("mod source 1")->set(3);     
  // TWE->getControls().at("mod source 2")->set(4);     
  // TWE->getControls().at("mod destination 1")->set(4);     
  // TWE->getControls().at("mod destination 2")->set(2);     
  // TWE->getControls().at("mod scale 1")->set(0.9);     
  // TWE->getControls().at("mod scale 2")->set(0.9);     
  // TWE->getControls().at("lfo 1 frequency")->set(2.2);     
  // TWE->getControls().at("lfo 1 frequency")->set(3.2);     

  TWE->getControls().at("filter_type")->set(0);
  TWE->getControls().at("beats_per_minute")->set(120);
  //bq.reset();
  note_on=0;

  sample_num=0;



  note=0;
  detune=64;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
  f_env1_amount=0.5; // 0 < 0.5 < 1
  f_env2_amount=0.5; // 0 < 0.5 < 1
}


int TwytchsynthMachine::checkI(int what,int val)
{
  switch (what)
    {

    case BPM:
      if (val<=0) return 1;
      if (val>=300) return 300;
      return val;
      break;



    case TRIG_TIME_DURATION:
      if (val<=0) return 1;
      if (val>=127) return 127;
      return val;
      break;

    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>PICO_TWYTCHSYNTH_SIZE-1) return PICO_TWYTCHSYNTH_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>PICO_TWYTCHSYNTH_SIZE-1) return PICO_TWYTCHSYNTH_SIZE-1;
      return val;
      break;

    case OSC3_TYPE:
      if (val<0) return 0;
      if (val>PICO_TWYTCHSYNTH_SIZE-1) return PICO_TWYTCHSYNTH_SIZE-1;
      return val;
      break;

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>TWYTCHSYNTH_FILTER_TYPE_SIZE-1) return TWYTCHSYNTH_FILTER_TYPE_SIZE-1;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>FILTER_ALGO_SIZE-1) return FILTER_ALGO_SIZE-1;
      return val;
      break;


    case LFO1_WAVEFORM:
      if (val<0) return 0;
      if (val>PICO_TWYTCHSYNTH_SIZE-2) return PICO_TWYTCHSYNTH_SIZE-2;
      return val;
      break;


    case LFO2_WAVEFORM:
      if (val<0) return 0;
      if (val>PICO_TWYTCHSYNTH_SIZE-2) return PICO_TWYTCHSYNTH_SIZE-2;
      return val;
      break;


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      printf("WARNING: TwytchsynthMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
  return val;
}

int TwytchsynthMachine::getI(int what)
{
  if (what==NOTE_ON) return note_on;
}

void TwytchsynthMachine::setF(int what,float val)
{
  float f_val_cutoff;
  float f_val_resonance;

  float f_val=val;
  f_val=f_val/128;

  //if (what==LFO1_FREQ)             TWE->getControls().at("lfo 1 frequency")->set(f_val*10); 
  if (what==LFO1_FREQ)             TWE->getControls().at("mono_lfo_1_tempo"    )->set(f_val*11);
  //if (what==LFO1_FREQ)             TWE->getControls().at("mono_lfo_1_frequency")->set(((f_val*2)-1)*6); 
  //if (what==LFO1_FREQ)             TWE->getControls().at("mono_lfo_1_sync"    )->set(2);

  
  if (what==LFO2_FREQ)             TWE->getControls().at("mono_lfo_2_tempo"    )->set(f_val*11); 
  //if (what==LFO2_FREQ)             TWE->getControls().at("mono_lfo_2_frequency")->set(((f_val*2)-1)*6); 
  //if (what==LFO1_FREQ)             TWE->getControls().at("mono_lfo_2_sync"    )->set(2);
}

void TwytchsynthMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  float f_val=val;
  f_val=f_val/128;

  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }

  if (what==NOTE_ON && val==1) 
    { 
      NoteFreq & NF = NoteFreq::getInstance(); 
      note_on=1;

      if (old_note!=note)
	{
	  TWE->noteOn(note-1,velocity/32);
	  TWE->noteOff(old_note-1);
	}
      else
	{
	  if (note_on==0)
	    TWE->noteOn(note-1,velocity/32);
	  else
	    {
	      TWE->noteOff(old_note-1);	  
	      TWE->noteOn(note-1,velocity/32);
	    }
	}

      //CS->noteOn(NF.getINoteFreq(note),1.0);
      //TWE->noteOn(note+11,0.8);
      //TWE->noteOn(note+11,velocity/32);
      //TWE->getControls();
      //f_val=trig_time_duration;
      //TWE->getControls().at("velocity track")->set(trig_time_duration);
    }
  if (what==NOTE_ON && val==0) 
    { 
      note_on=0;
      TWE->noteOff(note-1);
    }

  //if (what==OSC1_NOTE)           { if (note_on) { this->setI(NOTE_ON,0); note=val; } }
  if (what==OSC1_NOTE)              {  old_note=note; note=val; }

  //if (what==ADSR_ENV0_ATTACK)    TWE->getControls().at("amp_attack")->set(val);
  // //delay time"
  // //if (what==ADSR_ENV0_ATTACK)    TWE->getControls().at("delay time")->set(f_val);
  // //if (what==ADSR_ENV0_RELEASE)   TWE->getControls().at("delay dry/wet")->set(f_val);
  // //if (what==ADSR_ENV0_DECAY)     TWE->getControls().at("delay feedback")->set(f_val);

  if (what==BPM)                 { TWE->getControls().at("beats_per_minute")->set(val);     }

  if (what==OSC1_TYPE)           { TWE->getControls().at("osc_1_waveform")->set(f_val*128); }
  if (what==OSC2_TYPE)           { TWE->getControls().at("osc_2_waveform")->set(f_val*128); }

  if (what==OSC3_TYPE)           { TWE->getControls().at("sub_waveform"  )->set(f_val*128); }

  if (what==LFO1_WAVEFORM)       { TWE->getControls().at("mono_lfo_1_waveform")->set(f_val*128); }
  if (what==LFO2_WAVEFORM)       { TWE->getControls().at("mono_lfo_2_waveform")->set(f_val*128); }


   if (what==ADSR_ENV0_ATTACK)    TWE->getControls().at("amp_attack")->set(f_val*4);
   if (what==ADSR_ENV0_DECAY)     TWE->getControls().at("amp_decay")->set(f_val*4);
   if (what==ADSR_ENV0_SUSTAIN)   TWE->getControls().at("amp_sustain")->set(f_val);
   if (what==ADSR_ENV0_RELEASE)   TWE->getControls().at("amp_release")->set(f_val*4);

   if (what==ADSR_ENV1_ATTACK)    TWE->getControls().at("fil_attack")->set(f_val*4);
   if (what==ADSR_ENV1_DECAY)     TWE->getControls().at("fil_decay")->set(f_val*4);
   if (what==ADSR_ENV1_SUSTAIN)   TWE->getControls().at("fil_sustain")->set(f_val);
   if (what==ADSR_ENV1_RELEASE)   TWE->getControls().at("fil_release")->set(f_val*4);


   if (what==LFO1_ENV_AMOUNT)     
     { 
       f_env1_amount=abs(abs(f_val-0.5)*2); 
       TWE->getControls().at("mono_lfo_1_amplitude")->set(((f_val*2)-1)*4);
     }

   if (what==LFO2_ENV_AMOUNT)     
     { 
       f_env2_amount=abs(abs(f_val-0.5)*2); 
       TWE->getControls().at("mono_lfo_2_amplitude")->set(((f_val*2)-1)*4); 

       TWE->clearModulations();
       mopotwytchsynth::ModulationConnection * connection =
	 new mopotwytchsynth::ModulationConnection("mono_lfo_1","keytrack");
       connection->amount.set(f_env1_amount);
       TWE->connectModulation(connection);

     }



   if (what==OSC12_MIX)           TWE->getControls().at("osc_mix")->set(f_val);

   //if (what==OSC1_AMP)            TWE->getControls().at("velocity_track")->set((f_val*2)-1);
   //if (what==OSC1_AMP)            TWE->getControls().at("")->set(f_val);

   if (what==VELOCITY)            TWE->getControls().at("velocity_track"     )->set(abs(f_val-1));


   if (what==OSC1_DETUNE)         TWE->getControls().at("osc_2_tune"   )->set((f_val*2)-1);
   if (what==OSC1_MOD)            TWE->getControls().at("cross_modulation")->set(f_val/2);

   //if (what==OSC1_AMP)            TWE->getControls().at("keytrack"     )->set(((f_val*2)-1)*128);
   if (what==OSC1_AMP)            TWE->getControls().at("keytrack"     )->set(((f_val*2)-1));
   if (what==OSC2_AMP)            TWE->getControls().at("fil_env_depth")->set(((f_val*2)-1)*128);


   if (what==OSC3_AMP)            TWE->getControls().at("sub_volume"     )->set(f_val);
   if (what==OSC4_AMP)            TWE->getControls().at("noise_volume"   )->set(f_val);


   if (what==OSC1_UNISON)         TWE->getControls().at("osc_1_unison_voices")->set(f_val*15);
   if (what==OSC2_UNISON)         TWE->getControls().at("osc_2_unison_voices")->set(f_val*15);


   if (what==OSC1_UNISONDT)       TWE->getControls().at("osc_1_unison_detune")->set(f_val*100);
   if (what==OSC2_UNISONDT)       TWE->getControls().at("osc_2_unison_detune")->set(f_val*100);

  // //if (what==OSC1_AMP)            TWE->getControls().at("velocity track")->set(f_val);
  
   if (what==FILTER1_TYPE)        TWE->getControls().at("filter_type")->set(f_val*128);

   if (what==FILTER1_SATURATION)  TWE->getControls().at("filter_saturation")->set(((f_val*2)-1)*20);
   if (what==FILTER1_FEEDBACK)    TWE->getControls().at("osc_feedback_amount")->set(((f_val*2)-1)*1);
   //filter.setFilterType(val);
  // //"filter type"


   if (what==FILTER1_CUTOFF)      
     { 
       //TWE->getControls().at("cutoff")->set(28+f_val*100);
       TWE->getControls().at("cutoff")->set(28+f_val*99);
     }
   
   if (what==FILTER1_RESONANCE)         
     { 
       //TWE->getControls().at("resonance")->set(0.5+(f_val*10));
       TWE->getControls().at("resonance")->set(f_val);
     }

  // if (what==FX1_DEPTH)          TWE->getControls().at("delay dry/wet")->set(f_val*128);   
  // if (what==FX1_SPEED)          TWE->getControls().at("delay time"   )->set(f_val*128);      
  

}

const char * TwytchsynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
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

  const        char * str_osc[PICO_TWYTCHSYNTH_SIZE];


  static const char * str_fltr_algo_nofilter = "NOFL";
  static const char * str_fltr_algo_biquad   = "BIQU";
  static const char * str_fltr_algo_amsynth  = "AMST";
  
  const        char * str_fltr_algo[FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp   = "LP";
  static const char * str_fltr_type_hp   = "HP";
  static const char * str_fltr_type_bp   = "BP";
  static const char * str_fltr_type_ls   = "LS";
  static const char * str_fltr_type_hs   = "HS";
  static const char * str_fltr_type_bs   = "BS";
  static const char * str_fltr_type_ap   = "AP";
  static const char * str_fltr_type_nt   = "NT";
  //static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_SIZE];


  static const char * str_lfo_type_lfo   = "PLFO";
  static const char * str_lfo_type_pb    = "PB";

  const        char * str_lfo_type[LFO_TYPE_SIZE];





  str_osc[PICO_TWYTCHSYNTH_SIN]              = str_sin;
  str_osc[PICO_TWYTCHSYNTH_TRIANGLE]         = str_triangle;
  str_osc[PICO_TWYTCHSYNTH_SQUARE]           = str_square;
  str_osc[PICO_TWYTCHSYNTH_DOWNSAW]          = str_downsaw;
  str_osc[PICO_TWYTCHSYNTH_UPSAW]            = str_upsaw;

  str_osc[PICO_TWYTCHSYNTH_THREESTEP]        = str_threestep;
  str_osc[PICO_TWYTCHSYNTH_FOURSTEP]         = str_fourstep;
  str_osc[PICO_TWYTCHSYNTH_HEIGHTSTEP]       = str_heightstep;

  str_osc[PICO_TWYTCHSYNTH_THREEPYRAMID]     = str_threepyramid;
  str_osc[PICO_TWYTCHSYNTH_FIVEPYRAMID]      = str_fivepyramid;
  str_osc[PICO_TWYTCHSYNTH_NINEPYRAMID]      = str_ninepyramid;

  //str_osc[PICO_TWYTCHSYNTH_WHITENOISE]       = str_whitenoise;

  // str_fltr_algo[FILTER_ALGO_NOFILTER] = str_fltr_algo_nofilter;
  // str_fltr_algo[FILTER_ALGO_BIQUAD]   = str_fltr_algo_biquad;
  // str_fltr_algo[FILTER_ALGO_AMSYNTH]  = str_fltr_algo_amsynth;

  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_HP]       = str_fltr_type_hp;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_BP]       = str_fltr_type_bp;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_LS]       = str_fltr_type_ls;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_HS]       = str_fltr_type_hs;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_BS]       = str_fltr_type_bs;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_AP]       = str_fltr_type_ap;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_NT]       = str_fltr_type_nt;
  //str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  str_lfo_type[LFO_TYPE_PITCHLFO]     = str_lfo_type_lfo;
  str_lfo_type[LFO_TYPE_PITCHBEND]    = str_lfo_type_pb;

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_osc[paramValue];
    case OSC2_TYPE:
      return str_osc[paramValue];

    case OSC3_TYPE:
      return str_osc[paramValue];
     
    // case FILTER1_ALGO:
    //   return str_fltr_algo[paramValue];

    case FILTER1_TYPE:
      return str_fltr_type[paramValue];

    case LFO_TYPE:
      return str_lfo_type[paramValue];

    case LFO1_WAVEFORM:
      return str_osc[paramValue];
    case LFO2_WAVEFORM:
      return str_osc[paramValue];


    }
  return str_null;
}



void TwytchsynthMachine::reset()
{
  sample_num=0;
  last_sample=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


int TwytchsynthMachine::tick()
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
  

      
  if (sample_num==0 || 
      index==0 )
    {
      TWE->process();
    }

  if (trig_time_mode)
    {
      if (trig_time_duration_sample<sample_num)
	{
	  this->setI(NOTE_ON,0);
	  trig_time_mode=0;
	  //printf("\t\t\t\t\t\tDONE\n");
	}

    }

 
  buffer_f[index]=TWE->output()->buffer[index];
  buffer_f[index]=buffer_f[index]*8192;
  //buffer_f[index]=buffer_f[index]*14336;
  //buffer_f[index]=buffer_f[index]*2048;
  //buffer_f[index]=buffer_f[index]*1024;
  buffer_i[index]=buffer_f[index];
    
  s_out=buffer_i[index];

  index++;
  sample_num++;
  last_sample=s_out;

  return s_out;

}

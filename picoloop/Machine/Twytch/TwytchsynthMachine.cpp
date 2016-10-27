#include "Master.h"
#include "TwytchsynthMachine.h"


#define SAM 64


TwytchsynthMachine::TwytchsynthMachine()
{
  float fi;
  int   i;
  DPRINTF("TwytchsynthMachine::TwytchsynthMachine()\n");  
  note_on=0;
  buffer_i=0;
  buffer_f=0;
  pole=0;
  need_note_on=0;

  bpm=-1;
  osc1_type=-1;
  osc2_type=-1;
  osc3_type=-1;
  lfo1_waveform=-1;
  lfo2_waveform=-1;
  
  adsr_env0_attack=-1;
  adsr_env0_decay=-1;
  adsr_env0_sustain=-1;
  adsr_env0_release=-1;
  
  
  adsr_env1_attack=-1;
  adsr_env1_decay=-1;
  adsr_env1_sustain=-1;
  adsr_env1_release=-1;
  
  lfo1_env_amount=-1;
  lfo2_env_amount=-1;
  
  osc12_mix=-1;
  velocity=-1;
  osc1_detune=-1;
  osc1_mod=-1;
  
  osc1_amp=-1;
  osc2_amp=-1;
  osc3_amp=-1;
  osc4_amp=-1;
  
  osc1_unison=-1;
  osc2_unison=-1;
  osc1_unisondt=-1;
  osc2_unisondt=-1;
  
  filter1_type=-1;
  filter1_saturation=-1;
  filter1_feedback=-1;
  filter1_cutoff=-1;
  filter1_resonance=-1;
  
}



TwytchsynthMachine::~TwytchsynthMachine()
{
  DPRINTF("TwytchsynthMachine::~TwytchsynthMachine()\n");
}




void TwytchsynthMachine::init()
{
  int i;
  int j;
  if (buffer_i==0 ||
      buffer_f==0)
    {
      TWE=new mopotwytchsynth::HelmEngine();
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM);
      buffer_f = (mopotwytchsynth::mopo_float*)malloc(sizeof(mopotwytchsynth::mopo_float)*SAM);

      TWE->setBufferSize(SAM);
      //TWE->setSampleRate(44100);
      TWE->setSampleRate(DEFAULTFREQ);

    }
  for (i=0;i<SAM;i++)
    buffer_i[i]=0;
  for (i=0;i<SAM;i++)
    buffer_f[i]=0.0;

  TWE->getControls().at("polyphony")->set(1);
  TWE->getControls().at("filter_type")->set(0);
  TWE->getControls().at("beats_per_minute")->set(120);
  TWE->getControls().at("legato")->set(0);
  TWE->getControls().at("arp_on")->set(0);
  TWE->getControls().at("arp_pattern")->set(0);
  TWE->getControls().at("arp_sync")->set(1);
  TWE->getControls().at("formant_on")->set(0);
  TWE->getControls().at("osc_feedback_transpose")->set(-12);
  TWE->getControls().at("osc_feedback_tune")->set(0);
  TWE->getControls().at("step_frequency")->set(3);
  TWE->getControls().at("stutter_on")->set(0);
  TWE->getControls().at("volume")->set(0.5);
  TWE->getControls().at("delay_on")->set(0);
  TWE->getControls().at("delay_sync")->set(1);
  TWE->getControls().at("filter_saturation")->set(0);
  TWE->getControls().at("portamento_type")->set(0);
  TWE->getControls().at("portamento")->set(-9);
  TWE->getControls().at("reverb_on")->set(0);

  TWE->getControls().at("osc_1_tune"   )->set(0.3);
  TWE->getControls().at("osc_2_tune"   )->set(0.3);

  TWE->getControls().at("osc_1_tune"   )->set(0.28);
  TWE->getControls().at("osc_2_tune"   )->set(0.28);

  TWE->getControls().at("osc_1_tune"   )->set(0.29);
  TWE->getControls().at("osc_2_tune"   )->set(0.29);

  TWE->getControls().at("osc_1_tune"   )->set(0.285);
  TWE->getControls().at("osc_2_tune"   )->set(0.285);

  TWE->getControls().at("osc_1_tune"   )->set(0.287);
  TWE->getControls().at("osc_2_tune"   )->set(0.287);

  TWE->getControls().at("osc_1_tune"   )->set(0.295);
  TWE->getControls().at("osc_2_tune"   )->set(0.295);

  TWE->getControls().at("osc_1_tune"   )->set(0);
  TWE->getControls().at("osc_2_tune"   )->set(0);

  note_on=0;
  sample_num=0;

  note=0;
  detune=0;

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



    case ADSR_ENV0_ATTACK:
      if (val<=1) return 1;
      if (val>=127) return 127;
      return val;
      break;

    case ADSR_ENV0_RELEASE:
      if (val<=1) return 1;
      if (val>=127) return 127;
      return val;
      break;


    case TRIG_TIME_DURATION:
      if (val<=1) return 1;
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
      DPRINTF("WARNING: TwytchsynthMachine::checkI(%d,%d)\n",what,val);
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

  if (what==LFO1_FREQ && lfo1_freq!=val)  { lfo1_freq=val; TWE->getControls().at("mono_lfo_1_tempo"    )->set(f_val*11); }
  if (what==LFO2_FREQ && lfo2_freq!=val)  { lfo2_freq=val; TWE->getControls().at("mono_lfo_2_tempo"    )->set(f_val*11); }
}

void TwytchsynthMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;

  int   noteShift=12;
  float f_val=val;
  f_val=f_val/128;

  if (what==TRIG_TIME_MODE)     {  trig_time_mode=val; }
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }

  if (what==NOTE_ON && val==1) 
    { 
      note_on=1;

      if (old_note!=note)
	{
	  TWE->noteOn(note-1+noteShift);
	  TWE->noteOff(old_note-1+noteShift);
	  //need_note_on=1;
	}
      else
	{
	  if (note_on==0)
	    {
	      TWE->noteOn(note-1+noteShift);
	    }
	  else
	    {
	      TWE->noteOn(note-1+noteShift);
	    }
	}
    }
  if (what==NOTE_ON && val==0) 
    { 
      note_on=0;
      TWE->noteOff(note-1+noteShift);
    }

  if (what==NOTE1)                                    {  old_note=note; note=val; }

  if (what==BPM && bpm!=val)                              { bpm=val; TWE->getControls().at("beats_per_minute")->set(val);     }

  if (what==OSC1_TYPE && osc1_type!=val )                 { osc1_type=val; TWE->getControls().at("osc_1_waveform")->set(f_val*128); }
  if (what==OSC2_TYPE && osc2_type!=val )                 { osc2_type=val; TWE->getControls().at("osc_2_waveform")->set(f_val*128); }

  if (what==OSC1_SCALE)                                   {                TWE->getControls().at("osc_1_transpose")->set(f_val*128); }

  if (what==OSC2_SCALE)                                   {                TWE->getControls().at("osc_2_transpose")->set(f_val*128); }

  if (what==OSC3_TYPE && osc3_type!=val )                 { osc3_type=val; TWE->getControls().at("sub_waveform"  )->set(f_val*128); }

  if (what==LFO1_WAVEFORM && lfo1_waveform!=val)          { lfo1_waveform=val; TWE->getControls().at("mono_lfo_1_waveform")->set(f_val*128); }
  if (what==LFO2_WAVEFORM && lfo2_waveform!=val)          { lfo2_waveform=val; TWE->getControls().at("mono_lfo_2_waveform")->set(f_val*128); }

  if (what==ADSR_ENV0_ATTACK  && adsr_env0_attack!=val)   { adsr_env0_attack=val;  TWE->getControls().at("amp_attack")->set(f_val*4);  }
  if (what==ADSR_ENV0_DECAY   && adsr_env0_decay!=val)    { adsr_env0_decay=val;   TWE->getControls().at("amp_decay")->set(f_val*4);   }
  if (what==ADSR_ENV0_SUSTAIN && adsr_env0_sustain!=val)  { adsr_env0_sustain=val; TWE->getControls().at("amp_sustain")->set(f_val);   }
  if (what==ADSR_ENV0_RELEASE && adsr_env0_release!=val)  { adsr_env0_release=val; TWE->getControls().at("amp_release")->set(f_val*4); }

  if (what==ADSR_ENV1_ATTACK  && adsr_env1_attack!=val)   { adsr_env1_attack=val;  TWE->getControls().at("fil_attack")->set(f_val*4);  }
  if (what==ADSR_ENV1_DECAY   && adsr_env1_decay!=val)    { adsr_env1_decay=val;   TWE->getControls().at("fil_decay")->set(f_val*4);   }
  if (what==ADSR_ENV1_SUSTAIN && adsr_env1_sustain!=val)  { adsr_env1_sustain=val; TWE->getControls().at("fil_sustain")->set(f_val);   }
  if (what==ADSR_ENV1_RELEASE && adsr_env1_release!=val)  { adsr_env1_release=val; TWE->getControls().at("fil_release")->set(f_val*4); }



   if (what==FILTER1_CUTOFF && filter1_cutoff!=val)      
     { 
       filter1_cutoff=val;
       TWE->getControls().at("cutoff")->set(28+f_val*99);
     }
   
   if (what==FILTER1_RESONANCE && filter1_resonance!=val)         
     { 
       filter1_resonance=val;
       TWE->getControls().at("resonance")->set(f_val);
     }  


   //if (what==OSC12_MIX && osc12_mix!=val)             { osc12_mix=val;  TWE->getControls().at("osc_mix")->set(f_val); }
   //if (what==OSC12_MIX && osc12_mix!=val)             { osc12_mix=val;  TWE->getControls().at("osc_mix")->set(0); }

   if (what==VELOCITY && velocity!=val)               { velocity=val;   TWE->getControls().at("velocity_track"     )->set(abs(f_val-1)); }


   //if (what==OSC1_DETUNE )                            { TWE->getControls().at("osc_2_tune"   )->set((f_val*2)-1); }
   if (what==OSC1_MOD    )                            { TWE->getControls().at("cross_modulation")->set(f_val/2); }

   if (what==KEYTRACK   )                             { TWE->getControls().at("keytrack"     )->set(((f_val*2)-1)); }
   if (what==ENV1_DEPTH )                             { TWE->getControls().at("fil_env_depth")->set(((f_val*2)-1)*128); }

   if (what==OSC3_AMP )                               { TWE->getControls().at("sub_volume"     )->set(f_val);}
   if (what==OSC4_AMP )                               { TWE->getControls().at("noise_volume"   )->set(f_val);}


   if (what==OSC1_UNISON && osc1_unison!=val)         { osc1_unison=val; TWE->getControls().at("osc_1_unison_voices")->set(f_val*15); }
   if (what==OSC2_UNISON && osc2_unison!=val)         { osc2_unison=val; TWE->getControls().at("osc_2_unison_voices")->set(f_val*15); }


   if (what==OSC1_UNISONDT && osc1_unisondt!=val)     { osc1_unisondt=val; TWE->getControls().at("osc_1_unison_detune")->set(f_val*100); }
   if (what==OSC2_UNISONDT &&osc2_unisondt!=val)      { osc2_unisondt=val; TWE->getControls().at("osc_2_unison_detune")->set(f_val*100); }


   if (what==FILTER1_TYPE && filter1_type!=val)             { filter1_type=val; TWE->getControls().at("filter_type")->set(f_val*128); }

   if (what==FILTER1_SATURATION && filter1_saturation!=val) { filter1_saturation=val; TWE->getControls().at("filter_saturation")->set(((f_val*2)-1)*20); }
   if (what==FILTER1_FEEDBACK   && filter1_feedback!=val)   { filter1_feedback=val; TWE->getControls().at("osc_feedback_amount")->set(((f_val*2)-1)*1); }


   if (what==LFO1_ENV_AMOUNT && lfo1_env_amount!=val)
     {
       lfo1_env_amount=val;
       f_env1_amount=abs(abs(f_val-0.5)*2);
       TWE->getControls().at("mono_lfo_1_amplitude")->set(((f_val*2)-1)*4);
     }

   if (what==LFO2_ENV_AMOUNT && lfo2_env_amount!=val)
     {
       lfo2_env_amount=val;
       f_env2_amount=abs(abs(f_val-0.5)*2);
       TWE->getControls().at("mono_lfo_2_amplitude")->set(((f_val*2)-1)*4);

	// clearModulations disapear from 0.4 to 0.6
        // TWE->clearModulations();
       mopotwytchsynth::ModulationConnection * connection1;
       mopotwytchsynth::ModulationConnection * connection2;

       connection1 =
	 new mopotwytchsynth::ModulationConnection("mono_lfo_1","keytrack");

       connection1->amount.set(f_env1_amount);
       TWE->connectModulation(connection1);

       //mopotwytchsynth::ModulationConnection * connection; 
       connection2 =
	 new mopotwytchsynth::ModulationConnection("mono_lfo_2","osc_1_waveform");

       connection2->amount.set(f_env2_amount);
       TWE->connectModulation(connection2);

       
       filter1_cutoff=val;
       TWE->getControls().at("cutoff")->set(28+f_val*99);
     }


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


  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_HP]       = str_fltr_type_hp;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_BP]       = str_fltr_type_bp;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_LS]       = str_fltr_type_ls;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_HS]       = str_fltr_type_hs;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_BS]       = str_fltr_type_bs;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_AP]       = str_fltr_type_ap;
  str_fltr_type[TWYTCHSYNTH_FILTER_TYPE_NT]       = str_fltr_type_nt;


  str_lfo_type[LFO_TYPE_PITCHLFO]     = str_lfo_type_lfo;
  str_lfo_type[LFO_TYPE_PITCHBEND]    = str_lfo_type_pb;

  switch (machineParam)
    {
    case OSC1_TYPE:
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];

    case OSC3_TYPE:
      return str_osc[this->checkI(OSC3_TYPE,paramValue)];
     

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



void TwytchsynthMachine::reset()
{
  sample_num=0;
  last_sample=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


Sint16 TwytchsynthMachine::tick()
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

  Sint32 cutoff_tmp;
  Sint32 resonance_tmp;
  int    num=8192;
  int    i;

  float  buf_f;
  Sint16 buf_i;


  if (index>=SAM | 
      index<0)
    index=0;
  

      
  if (index==0 )
    {
      TWE->process();
      for (i=0;i<SAM;i++)
	{
	  buf_f=TWE->output(0)->buffer[i];
	  buf_f=buf_f*8192;
	  buffer_i[i]=buf_f;
	}
    }

  if (trig_time_mode)
    {
      if (trig_time_duration_sample<sample_num)
	{
	  this->setI(NOTE_ON,0);
	  trig_time_mode=0;
	}

    }

  
  s_out=buffer_i[index];
  
  index++;
  sample_num++;
  last_sample=s_out;

  return s_out;

}

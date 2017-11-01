#include "Master.h"
#include "MidiOutMachine.h"

// #if defined(__RTMIDI__)
// MidiOutSystem & MOS=MidiOutSystem::getInstance();
// #endif

MidiOutMachine::MidiOutMachine() 
{
  float fi;
  int   i;

  DPRINTF("MidiOutMachine::MidiOutMachine()\n");  
  // for (i=0;i<256;i++)
  //   {
  //     fi=i;
  //     fi=tanh(fi/128);
  //     tanh_table[i]=fi*1024;
  //   }
  cutoff=125;
  resonance=10;
  note=0;
  detune=64;
  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
  need_to_noteOn=0;
  //delay_to_noteOn=40000;
  //delay_to_noteOn=000;
  //delay_to_noteOn=32768;
  //delay_to_noteOn=2048;
  delay_to_noteOn=MIDI_DELAY_IN_SAMPLE;
  //delay_to_noteOn=512;
  //delay_to_noteOn=0;
  midi_machine_type=0; // korg volca key
  //midi_machine_type=1; // korg volca bass
}



MidiOutMachine::~MidiOutMachine()
{
  DPRINTF("MidiOutMachine::~MidiOutMachine()\n");
}


int MidiOutMachine::getCC(int midiMachineType,int cc)
{  
  int return_cc=0;
  if (midiMachineType==0) // korg volca key
    {
      DPRINTF("VOLCA KEY\n");
      if (cc==OSC1_DETUNE)      cc=42;
      if (cc==FILTER1_CUTOFF)   cc=44;
      if (cc==OSC2_AMP)         cc=43;  // VCO_EG_INT on volca keys, but OSC1_AMP is used for EG in twytch and other
      if (cc==OSC1_AMP)         cc=45;  // VCF_EG_INT on volca keys, but OSC2_AMP is used for EG in twytch and other
      if (cc==OSC1_DETUNE)      cc=42;
      if (cc==ADSR_AMP_ATTACK)  cc=49;
      if (cc==ADSR_AMP_RELEASE) cc=50;
      if (cc==ADSR_AMP_SUSTAIN) cc=51;
    }

  if (midiMachineType==1) // korg volca BASS
    {
      DPRINTF("VOLCA BASS\n");
      // if (cc==OSC1_DETUNE)      cc=42;

      // if (cc==OSC2_AMP)         cc=43;  // VCO_EG_INT on volca keys, but OSC1_AMP is used for EG in twytch and other
      // if (cc==OSC1_AMP)         cc=45;  // VCF_EG_INT on volca keys, but OSC2_AMP is used for EG in twytch and other
      // if (cc==OSC1_DETUNE)      cc=42;

      //if (cc==FILTER1_CUTOFF)   cc=48;
      if (cc==ADSR_AMP_ATTACK)  cc=46;
      if (cc==ADSR_AMP_RELEASE) cc=47;

      if (cc==OSC1_AMP)         cc=48;
    }


  return cc;
}



void MidiOutMachine::init()
{

  sample_num=0;
  note=0;
  detune=64;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=512;
}


int MidiOutMachine::checkI(int what,int val)
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
      DPRINTF("WARNING: MidiOutMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
  return val;
}

int MidiOutMachine::getI(int what)
{
  // if (what==NOTE_ON) return this->getADSRAmp().getNoteOn();
  return 0;
}

void MidiOutMachine::setF(int what,float val)
{
  //if (what==OSC1_FREQ)           this->getVCO().setSynthFreq(val);
  // if (what==LFO1_FREQ)           this->getVCO().setLfoSpeed(val);
}

void MidiOutMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;


#ifdef __RTMIDI__
  MidiOutSystem & MOS=MidiOutSystem::getInstance();

  if (what==TRIG_TIME_MODE)     {  trig_time_mode=val; }
  if (what==TRIG_TIME_DURATION) 
    { 
      trig_time_duration=val; 
      trig_time_duration_sample=val*512; 
      DPRINTF("                                                SETTRIG TIME : %d\n",trig_time_duration_sample);
  }
  
  if (what==NOTE_ON && val==1) 
    { 
      if (need_to_noteOn)
	{
	  DPRINTF("                                                   NOTEOFF %d sample_num %d\n",old_note,sample_num);
	  MOS.noteOff(0,old_note-1+12);
	  MOS.flushMsg();
	}
      need_to_noteOn=1;
      //sample_num=0;
    }


  if (what==NOTE_ON && val==0) 
    { 
      //need_to_noteOn=0;
      need_to_noteOff=1;
      // MOS.noteOff(0,note-1+12);
      // MOS.flushMsg();
      //MOS.noteOff(0,note-1+12);
      //MOS.flushMsg();
    }

  
  if (what==FILTER1_CUTOFF)   { MOS.cc(0,this->getCC(midi_machine_type,FILTER1_CUTOFF),val);   MOS.flushMsg(); } 
  if (what==ADSR_AMP_ATTACK)  { MOS.cc(0,this->getCC(midi_machine_type,ADSR_AMP_ATTACK),val);  MOS.flushMsg(); }  
  if (what==ADSR_AMP_RELEASE) { MOS.cc(0,this->getCC(midi_machine_type,ADSR_AMP_RELEASE),val); MOS.flushMsg(); } 

  // if (what==ADSR_AMP_SUSTAIN) { MOS.cc(0,this->getCC(midi_machine_type,ADSR_AMP_SUSTAIN),val); MOS.flushMsg(); } 

  // if (what==OSC1_DETUNE)      { MOS.cc(0,this->getCC(midi_machine_type,OSC1_DETUNE),val);      MOS.flushMsg(); } 
  if (what==OSC1_AMP)         { MOS.cc(0,this->getCC(midi_machine_type,OSC1_AMP),val);         MOS.flushMsg(); } 
  // if (what==OSC2_AMP)         { MOS.cc(0,this->getCC(midi_machine_type,OSC2_AMP),val);         MOS.flushMsg(); } 




  
  if (what==NOTE1)    {old_note=note;       note=val; }


#endif
}

const char * MidiOutMachine::getMachineParamCharStar(int machineParam,int paramValue)
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
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];
     
    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    case LFO_TYPE:
      return str_lfo_type[this->checkI(LFO_TYPE,paramValue)];

    }
  return str_null;
}



void MidiOutMachine::reset()
{
  // sample_num=0;
  // last_sample=0;
  DPRINTF("                                                                 RESET\n");
  // trig_time_mode=0;
  sample_num=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}


Sint32 MidiOutMachine::tick()
{
  float  f_in;
  float  f_out;

  float  f_Fc;
  float  f_Q;

  Sint16 s_in;
  Sint16 s_out=0;

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
#ifdef __RTMIDI__
  MidiOutSystem & MOS=MidiOutSystem::getInstance();  

  // if (need_to_noteOn==1 && 
  //     sample_num>delay_to_noteOn)
  if (sample_num>delay_to_noteOn &&
      need_to_noteOn)
    {
      DPRINTF("                                                       NOTEON  %d sample_num:%d\n",note,sample_num);
      MOS.noteOn(0,note-1+12,50);
      MOS.flushMsg();
      //this->setI(NOTE_ON,1);
      need_to_noteOn=0;
      //need_to_noteOff=1;
      //sample_num=0;
      need_to_noteOff=1;

    }

  //if (need_to_noteOff && trig_time_mode)
  if (need_to_noteOff && trig_time_mode)
    {
      if (sample_num>trig_time_duration_sample+delay_to_noteOn &&
	  need_to_noteOff)
	{
	  DPRINTF("                                                   NOTEOFF %d sample_num %d\n",note,sample_num);
	  MOS.noteOff(0,note-1+12);
	  MOS.flushMsg();
	  //trig_time_mode=0;
	  //sample_num=0;
	  need_to_noteOff=0;
	}
    }
#endif      

  sample_num++;

  return s_out;

}

#include "Master.h"
#include "Open303Machine.h"

/*
  Open303 adapter notes
  ---------------------
  - noteShift = 11 converts picoloop note numbers (NoteFreq: 1 = C-1) to MIDI
    note numbers for rosic::Open303.
  - TeeBeeFilter::setResonance() and Open303::setEnvMod() take PERCENT (0..100).
    The 0..127 pattern values are mapped; passing them raw (and especially a
    negative EnvMod) pushes the filter out of its stable range.
  - All member state is initialised in the constructor. The cutoff / resonance
    / env-depth servos (current_*) and index used to start from uninitialised
    memory, so the filter could remain stuck far away from the UI value.
  - NOTE_ON=1 releases the note that is actually sounding (playing_note) and
    then triggers the new one, so a note change always retriggers the amp
    envelope instead of sliding into it.
  - tick() clamps before the Sint16 conversion; the resonant filter can exceed
    1.0 and the old narrowing wrapped around.
*/

//#define SAM 256
#define SAM 64

Open303Machine::Open303Machine()
{
  float fi;
  int   i;
  DPRINTF("Open303Machine::Open303Machine()");  
  note_on=0;
  velocity=32;
  parameter_portamento=32;

  // Everything below used to be uninitialised: the portamento servos
  // (current_*) started at whatever the allocator left there and the filter
  // could stay stuck far away from the UI value ("going too far and never
  // coming back"). Keep the initial state consistent with the PatternElement
  // defaults pushed by seq_update_track().
  note=60;
  old_note=note;
  playing_note=note;

  cutoff=112;
  resonance=16;
  env_depth=0;

  old_cutoff=cutoff;
  old_resonance=resonance;
  old_envdepth=env_depth;

  current_cuttoff=cutoff;
  current_resonance=resonance;
  current_envdepth=env_depth;

  index=parameter_portamento+1;   // apply the parameters on the first tick

  sample_num=0;
  last_sample=0;
  detune=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;

  osc1_type=0;
  osc2_type=0;

  buffer_i=NULL;
  tanh_table=NULL;

  //buffer_i=0;
  //buffer_f=0;

}



Open303Machine::~Open303Machine()
{
  DPRINTF("Open303Machine::~Open303Machine()");
}




void Open303Machine::init()
{
  int i;
  int j;

  O303E=new rosic::Open303();
  //O303E->setSampleRate(44100);
  O303E->setSampleRate(DEFAULTFREQ);

  // Make the engine agree with the (now initialised) adapter state before the
  // first tick, instead of waiting for the first portamento update.
  O303E->setCutoff(100+current_cuttoff*17);
  O303E->setResonance(resonance*100.0/127.0);
  O303E->setEnvMod(env_depth*100.0/127.0);

  //  O303E->setAccent(127);

  // O303E->noteOn(1,velocity,0.0);
  // O303E->getSample();
  // O303E->noteOn(1,0,0.0);
  // O303E->getSample();

  //O303E->setDecay(0.4);
  //O303E->setEnvMod(0.4);

  //O303E->setAmpDecay(0.4);
  //O303E->setAmpRelease(0.4);
  //O303E->setAmpSustain(0.5);
  //O303E->setDecay(300);
  //O303E->setDecay(0.4);

}


int Open303Machine::checkI(int what,int val)
{
  switch (what)
    {

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>=OPEN303_FILTER_TYPE_SIZE-1) return OPEN303_FILTER_TYPE_SIZE-1;
      return val;
      break;

    // case FILTER1_ALGO:
    //   if (val<0) return 0;
    //   if (val>OPEN303_FILTER_TYPE_SIZE-1) return OPEN303_FILTER_TYPE_SIZE-1;
    //   return val;
    //   break;


    case LFO1_WAVEFORM:
      if (val<0) return 0;
      if (val>PICO_CURSYNTH_SIZE-2) return PICO_CURSYNTH_SIZE-2;
      return val;
      break;


    case LFO2_WAVEFORM:
      if (val<0) return 0;
      if (val>PICO_CURSYNTH_SIZE-2) return PICO_CURSYNTH_SIZE-2;
      return val;
      break;


    case OSC1_AMP: // linked to velocity
      if (val<1) return 1;
      if (val>127) return 127;
      return val;
      break;



    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: Open303Machine::checkI(%d,%d)",what,val);
      return val;
      break;      
    }
  return val;
}

int Open303Machine::getI(int what)
{
  if (what==NOTE_ON) return note_on;
  return 0;
}

void Open303Machine::setF(int what,float val)
{
}

void Open303Machine::setI(int what,int val)
{
  int noteShift=11;
  float f_val=val;
  f_val=f_val/128;

   if (what==TRIG_TIME_MODE)       trig_time_mode=val;
   if (what==TRIG_TIME_DURATION) 
     { 
       if (val<3) val=3;
       trig_time_duration=val;
       trig_time_duration_sample=val*512; 
     }

  if (what==NOTE_ON && val==1) 
    { 
       // Release whatever is currently sounding before triggering the new
       // note. The old code sent note-on first and note-off second, so when a
       // new note arrived while the previous one was still gated the engine
       // slid to it instead of retriggering the amp envelope - which sounds
       // like a much quieter (or missing) note.
       if (note_on)
         O303E->noteOn(playing_note+noteShift,0,0.0);

       O303E->noteOn(note+noteShift,velocity,0.0);
       playing_note=note;
       note_on=1;
    }
  if (what==NOTE_ON && val==0) 
    { 
       // Release the note that was actually triggered. NOTE1 may already have
       // been changed for the next step, so using note here would release the
       // wrong key and leave the previous one hanging.
       if (note_on)
         O303E->noteOn(playing_note+noteShift,0,0.0);

       note_on=0;
       //CSE->noteOff(note);
       //O303E->releaseNote(note);
       //O303E->allNotesOff();
    }

  //if (what==OSC1_NOTE)              { O303E->noteOn(note+11,0,0.0); note=val; }
  if (what==NOTE1)              {  old_note=note; note=val; }



  if (what==ADSR_ENV0_ATTACK)       O303E->setAccentDecay(val*4);      // <= it works
  if (what==ADSR_ENV0_RELEASE)      O303E->setAmpRelease(val);         // <= it works
  if (what==ADSR_ENV0_DECAY)        O303E->setSlideTime(val*4);        // <= it works
  if (what==ADSR_ENV0_SUSTAIN)      O303E->setSquarePhaseShift(val*4);


  if (what==OSC12_MIX)              O303E->setWaveform(f_val); 

  if (what==OSC1_AMP)               velocity=val;
  if (what==OSC2_AMP)               O303E->setAccent(val);
  
  if (what==FILTER1_TYPE)            O303E->filter.setMode(val);

  if (what==OSC1_PHASE)             
    {
      //O303E->setEnvMod(1.5-(f_val*192));    
      old_envdepth=env_depth;
      env_depth=val;
    }

  if (what==FILTER1_CUTOFF)      
     { 
       //O303E->setCutoff(100+val*17);
       old_cutoff=cutoff;
       cutoff=val;      // formula : 100+val*17

     }
  
   if (what==FILTER1_RESONANCE)         
     {   
       //O303E->setResonance(f_val*128);
       old_resonance=resonance;
       resonance=val;
     }
}

const char * Open303Machine::getMachineParamCharStar(int machineParam,int paramValue)
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


  static const char * str_fltr_type_flat   = "FLAT  ";
  static const char * str_fltr_type_lp6    = "LP6   ";
  static const char * str_fltr_type_lp12   = "LP12  ";
  static const char * str_fltr_type_lp18   = "LP18  ";
  static const char * str_fltr_type_lp24   = "LP24  ";

  static const char * str_fltr_type_hp6    = "HP6   ";
  static const char * str_fltr_type_hp12   = "HP12  ";
  static const char * str_fltr_type_hp18   = "HP18  ";
  static const char * str_fltr_type_hp24   = "HP24  ";

  static const char * str_fltr_type_bp1212 = "BP1212";
  static const char * str_fltr_type_bp6_18 = "BP6_18";
  static const char * str_fltr_type_bp18_6 = "BP18_6";
  static const char * str_fltr_type_bp6_12 = "BP6_12";
  static const char * str_fltr_type_bp12_6 = "BP12_6";
  static const char * str_fltr_type_bp6__6 = "BP6__6";

  static const char * str_fltr_type_tb303  = "TB303 ";

  //static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[OPEN303_FILTER_TYPE_SIZE];


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

  str_fltr_type[OPEN303_FILTER_TYPE_FLAT]      = str_fltr_type_flat;

  str_fltr_type[OPEN303_FILTER_TYPE_LP_6]      = str_fltr_type_lp6;
  str_fltr_type[OPEN303_FILTER_TYPE_LP_12]     = str_fltr_type_lp12;
  str_fltr_type[OPEN303_FILTER_TYPE_LP_18]     = str_fltr_type_lp18;
  str_fltr_type[OPEN303_FILTER_TYPE_LP_24]     = str_fltr_type_lp24;

  str_fltr_type[OPEN303_FILTER_TYPE_HP_6]      = str_fltr_type_hp6;
  str_fltr_type[OPEN303_FILTER_TYPE_HP_12]     = str_fltr_type_hp12;
  str_fltr_type[OPEN303_FILTER_TYPE_HP_18]     = str_fltr_type_hp18;
  str_fltr_type[OPEN303_FILTER_TYPE_HP_24]     = str_fltr_type_hp24;

  str_fltr_type[OPEN303_FILTER_TYPE_BP_12_12]  = str_fltr_type_bp1212;
  str_fltr_type[OPEN303_FILTER_TYPE_BP_6_18]   = str_fltr_type_bp6_18;
  str_fltr_type[OPEN303_FILTER_TYPE_BP_18_6]   = str_fltr_type_bp18_6;
  str_fltr_type[OPEN303_FILTER_TYPE_BP_6_12]   = str_fltr_type_bp6_12;
  str_fltr_type[OPEN303_FILTER_TYPE_BP_12_6]   = str_fltr_type_bp12_6;
  str_fltr_type[OPEN303_FILTER_TYPE_BP_6_6]    = str_fltr_type_bp6__6;

  str_fltr_type[OPEN303_FILTER_TYPE_TB_303]    = str_fltr_type_tb303;


  //str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  str_lfo_type[LFO_TYPE_PITCHLFO]     = str_lfo_type_lfo;
  str_lfo_type[LFO_TYPE_PITCHBEND]    = str_lfo_type_pb;

  switch (machineParam)
    {
     
    // case FILTER1_ALGO:
    //   return str_fltr_algo[paramValue];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    case LFO_TYPE:
      return str_lfo_type[paramValue];

    case LFO1_WAVEFORM:
      return str_osc[paramValue];

    case LFO2_WAVEFORM:
      return str_osc[paramValue];


    }
  return str_null;
}



void Open303Machine::reset()
{
  sample_num=0;
  last_sample=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;
}



Sint32 Open303Machine::tick()
{
  double    f_in;
  Sint16 s_out;
  float  f_val;
  

   if (trig_time_mode)
     {
       if (trig_time_duration_sample<sample_num)
	 {
	   this->setI(NOTE_ON,0);
	   trig_time_mode=0;
	 }       
     }

   if (index>parameter_portamento)
     {
       index=0;

       if (current_cuttoff<cutoff)
	 current_cuttoff++;
       if (current_cuttoff>cutoff)
	   current_cuttoff--;
       O303E->setCutoff(100+current_cuttoff*17);

       if (current_resonance<resonance)
	 current_resonance++;
       if (current_resonance>resonance)
	   current_resonance--;
       // TeeBeeFilter::setResonance() expects percent (100 % = self
       // oscillation). The 0..127 knob has to be mapped, otherwise 101..127
       // pushes the filter past self-oscillation and it can stay stuck.
       f_val=(double)current_resonance*100.0/127.0;
       if (f_val>100.0) f_val=100.0;
       O303E->setResonance(f_val);

       if (current_envdepth<env_depth)
	 current_envdepth++;
       if (current_envdepth>env_depth)
	 current_envdepth--;
       // Open303::setEnvMod() also expects percent (0..100). The previous
       // formula produced 1.5 at 0 and -189 at 127, i.e. negative cutoff
       // modulation at the top of the range.
       O303E->setEnvMod((double)current_envdepth*100.0/127.0);
       //O303E->setResonance(current_resonance*128);




     }
      
   
   f_in=O303E->getSample();

   // Clamp before the Sint16 conversion: a resonant filter can overshoot 1.0
   // and the old implicit double->Sint16 narrowing wrapped around, which
   // turned a loud note into full-scale noise of the wrong sign.
   double d_out=f_in*2048.0;
   if (d_out!=d_out)   d_out=0.0;          // NaN guard
   if (d_out> 32767.0) d_out= 32767.0;
   if (d_out<-32768.0) d_out=-32768.0;
   s_out=(Sint16)d_out;

   sample_num++;
   index++;
  return s_out;
}

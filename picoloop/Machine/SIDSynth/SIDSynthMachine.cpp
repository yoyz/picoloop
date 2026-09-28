#include "SIDSynthMachine.h"
#include "math.h"
#include <stdlib.h>

//#define SAM 512
#define SAM 64
#define SID_SAMPLERATE 44118
#define SID_CLOCKFREQ (22.5 * SID_SAMPLERATE) //nearest int to 985248


// Voice 1
#define FREQUENCY_VOICE_1_LOW_BYTE              0x00  // 8 bit full 0xXX
#define FREQUENCY_VOICE_1_HIGH_BYTE             0x01  // 8 bit full 0xXX
#define PULSE_WAVE_DUTY_CYCLE_VOICE_1_LOW_BYTE  0x02  // 8 bit full 0xXX
#define PULSE_WAVE_DUTY_CYCLE_VOICE_1_HIGH_BYTE 0x03  // 4 bit low  0x0X
#define CONTROL_REGISTER_VOICE_1                0x04  // 0b[NPST][TRSG]
                                                      // [Noise Pulse Saw Triange][Test Ring Sync Gate]
#define AD_VOICE1                               0x05  // 8 bit full 0xAD
#define SR_VOICE1                               0x06  // 8 bit full 0xSR

// Voice 2
#define FREQUENCY_VOICE_2_LOW_BYTE              0x07  // 8 bit full 0xXX
#define FREQUENCY_VOICE_2_HIGH_BYTE             0x08  // 8 bit full 0xXX
#define PULSE_WAVE_DUTY_CYCLE_VOICE_2_LOW_BYTE  0x09  // 8 bit full 0xXX
#define PULSE_WAVE_DUTY_CYCLE_VOICE_2_HIGH_BYTE 0x0a  // 4 bit low  0x0X
#define CONTROL_REGISTER_VOICE_2                0x0b  // 0b[NPST][TRSG]

#define AD_VOICE2                               0x0c  // 8 bit full 0xAD
#define SR_VOICE2                               0x0d  // 8 bit full 0xSR

// Voice 3
#define FREQUENCY_VOICE_3_LOW_BYTE              0x0e  // 8 bit full 0xXX
#define FREQUENCY_VOICE_3_HIGH_BYTE             0x0f  // 8 bit full 0xXX
#define PULSE_WAVE_DUTY_CYCLE_VOICE_3_LOW_BYTE  0x10  // 8 bit full 0xXX
#define PULSE_WAVE_DUTY_CYCLE_VOICE_3_HIGH_BYTE 0x11  // 4 bit low  0x0X
#define CONTROL_REGISTER_VOICE_3                0x12  // 0b[NPST][TRSG]
                                                      // [Noise Pulse Saw Triange][Test Ring Sync Gate]

#define AD_VOICE3                               0x13  // 8 bit full 0xAD
#define SR_VOICE3                               0x14  // 8 bit full 0xSR



// Filter & Volume & Routing
#define CUTOFF_LOW                              0x15  // 4 bit low  0x0C
#define CUTOFF_HIGH                             0x16  // 8 bit full 0xXX
#define RES_ROUTE                               0x17  // 8 bit full 0b[RRRR][E321]
                                                      // [Resonance][External Voice3 Voice2 Voice1]
#define FILTER_MAINVOL                          0x18  // 8 bit full 0b[MHBL][VVVV]
#define FREQ_TABLE_SIZE                           96

int freq_low[96]={
0x17,0x27,0x39,0x4b,0x5f,0x74,0x8a,0xa1,0xba,0xd4,0xf0,0x0e,
0x2d,0x4e,0x71,0x96,0xbe,0xe8,0x14,0x43,0x74,0xa9,0xe1,0x1c,
0x5a,0x9c,0xe2,0x2d,0x7c,0xcf,0x28,0x85,0xe8,0x52,0xc1,0x37,
0xb4,0x39,0xc5,0x5a,0xf7,0x9e,0x4f,0x0a,0xd1,0xa3,0x82,0x6e,
0x68,0x71,0x8a,0xb3,0xee,0x3c,0x9e,0x15,0xa2,0x46,0x04,0xdc,
0xd0,0xe2,0x14,0x67,0xdd,0x79,0x3c,0x29,0x44,0x8d,0x08,0xb8,
0xa1,0xc5,0x28,0xcd,0xba,0xf1,0x78,0x53,0x87,0x1a,0x10,0x71,
0x42,0x89,0x4f,0x9b,0x74,0xe2,0xf0,0xa6,0x0e,0x33,0x20,0xff};
// C   C#    D   D#    E    F   F#    G   G#    A   A#    B
int freq_high[96]={
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x04,
0x04,0x04,0x04,0x05,0x05,0x05,0x06,0x06,0x06,0x07,0x07,0x08,
0x08,0x09,0x09,0x0a,0x0a,0x0b,0x0c,0x0d,0x0d,0x0e,0x0f,0x10,
0x11,0x12,0x13,0x14,0x15,0x17,0x18,0x1a,0x1b,0x1d,0x1f,0x20,
0x22,0x24,0x27,0x29,0x2b,0x2e,0x31,0x34,0x37,0x3a,0x3e,0x41,
0x45,0x49,0x4e,0x52,0x57,0x5c,0x62,0x68,0x6e,0x75,0x7c,0x83,
0x8b,0x93,0x9c,0xa5,0xaf,0xb9,0xc4,0xd0,0xdd,0xea,0xf8,0xff};


// Map a 0..127 cutoff knob value to the 11-bit SID filter cutoff register
// (FC, 0..2047). The MOS8580 filter frequency is roughly linear in FC
// (~30Hz at FC 0 to ~12.5kHz at FC 2047), so writing the knob value straight
// to FC_HI yields a linear-in-frequency sweep that crams the low octaves into
// the first few knob steps. Use an exponential (per-octave) curve instead so
// every knob step moves through the octaves evenly.
static int sid_cutoff_to_fc(int cutoff)
{
  const double f_lo    = 30.0;     // Hz, lowest usable cutoff (FC ~0)
  const double f_hi    = 12500.0;  // Hz, cutoff at FC 2047
  const double octaves = log(f_hi / f_lo) / log(2.0);  // ~8.7 octaves

  if (cutoff < 0)   cutoff = 0;
  if (cutoff > 127) cutoff = 127;

  double f = f_lo * pow(2.0, octaves * cutoff / 127.0);
  int fc = (int)(f * 2047.0 / f_hi + 0.5);
  if (fc < 0)    fc = 0;
  if (fc > 2047) fc = 2047;
  return fc;
}


SIDSynthMachine::SIDSynthMachine()
{
  DPRINTF("SIDSynthMachine::SIDSynthMachine()");  
  buffer_f=0;
  buffer_i=0;
  cutoff=125;
  resonance=10;
  index=0;
  sid=0;   // allocated in init(); keep it deterministic so a use before
           // init() hits a clean NULL deref instead of garbage memory

  lfo_depth=0;
  lfo_depth_shift=20;
  lfo_speed=0;
  lfo1_depth=0;
  lfo1_freq=0.0f;
  lfo1_phase=0.0;
  lfo2_depth=0;
  lfo2_freq=0.0f;
  lfo2_phase=0.0;
  base_reg1=0;
  base_reg2=0;

  attack=64;
  decay=64;
  sustain=64;
  release=64;
  attack2=64;
  decay2=64;
  sustain2=64;
  release2=64;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;

}


SIDSynthMachine::~SIDSynthMachine()
{
  DPRINTF("SIDSynthMachine::~SIDSynthMachine()");  
  if (buffer_f)
    free(buffer_f);
  if (buffer_i)
    free(buffer_i);

}


void SIDSynthMachine::init()
{
  DPRINTF("SIDSynthMachine::init()");  
  int i;

  //HO(44100);
  if (sid==0)
    {
      sid=new SIDCHIP();
    }
  if (buffer_f==0)
    {
      buffer_f = (float*)malloc(sizeof(float)*SAM);
    }
  if (buffer_i==0)
    {
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM);
    }

  DPRINTF("buffer_f:0x%08.8X",buffer_f);
  DPRINTF("buffer_i:0x%08.8X",buffer_i);

  sid_note_frqs=(double*)malloc(sizeof(double)*128);
  for(i=0; i<128; i++) 
    {
      sid_note_frqs[i]=440.0*pow(2,((double)i-69.0)/12.0);
      //printf("i:%d %f\n",i,sid_note_frqs[i]);
      // i:0 8.175799
      // i:29 43.653529
      // i:127 12543.853951
    }

  for (i=0;i<SAM;i++)
    {
      buffer_f[i]=0;
      buffer_i[i]=0;
    }
  sample_num=0;
  index=0;
  freq=110.0;
  keyon=0;

  sid->set_chip_model(MOS8580);
  sid->set_sampling_parameters(SID_CLOCKFREQ, SAMPLE_FAST, SID_SAMPLERATE);
  sid->reset();

  //sid->write(0x04,0x20);    // CONTROL
  //sid->write(0x12,0x40);    // CONTROL
  //sid->write(FILTER_MAINVOL,0x1F);  // MODE/VOL
  sid->write(FILTER_MAINVOL,0x15);  // MODE/VOL
  sid->write(PULSE_WAVE_DUTY_CYCLE_VOICE_1_HIGH_BYTE,0x7F); // set pulse width to middle
  sid->write(PULSE_WAVE_DUTY_CYCLE_VOICE_2_HIGH_BYTE,0x7F); // for the three voice
  sid->write(PULSE_WAVE_DUTY_CYCLE_VOICE_3_HIGH_BYTE,0x7F); // 1 2 3
}

const char * SIDSynthMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * str_sidsynth_null       = " NULL";
  static const char * str_sidsynth_sqr        = "  SQR";
  static const char * str_sidsynth_trgl       = " TRGL";
  static const char * str_sidsynth_saw        = "  SAW";
  static const char * str_sidsynth_noise      = "NOISE";


  const char * str_osc[PICO_SIDSYNTH_SIZE];

  static const char * str_fltr_algo_pblp = "PBLP";
  
  const        char * str_fltr_algo[PBSYNTH_FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp12   = "LP12";
  static const char * str_fltr_type_lp24   = "LP24";

  const        char * str_fltr_type[FILTER_TYPE_SIZE];

  static const char * str_fm_type_am      = "2OP_AM ";
  static const char * str_fm_type_fm      = "2OP_FM ";

  const        char * str_fm_type[FM_TYPE_SIZE];


  str_osc[PICO_SIDSYNTH_SQUARE]        = str_sidsynth_sqr;
  str_osc[PICO_SIDSYNTH_SAW]           = str_sidsynth_saw;
  str_osc[PICO_SIDSYNTH_TRIANGE]       = str_sidsynth_trgl;
  str_osc[PICO_SIDSYNTH_NOISE]         = str_sidsynth_noise;

  str_fltr_algo[PBSYNTH_FILTER_ALGO_PBLP]       = str_fltr_algo_pblp;

  str_fltr_type[PBSYNTH_FILTER_TYPE_LP12]       = str_fltr_type_lp12;

  str_fltr_type[PBSYNTH_FILTER_TYPE_LP12]       = str_fltr_type_lp12;
  str_fltr_type[PBSYNTH_FILTER_TYPE_LP24]       = str_fltr_type_lp24;

  str_fm_type[FM_TYPE_AM]             = str_fm_type_am;
  str_fm_type[FM_TYPE_FM]             = str_fm_type_fm;



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

    case FM_TYPE:
      return str_fm_type[paramValue];

    case OSC1_PHASE:
      return (paramValue>0) ? "  ON" : " OFF";
    case OSC2_PHASE:
      return (paramValue>0) ? "  ON" : " OFF";

    }
  return str_sidsynth_null;
}



void SIDSynthMachine::reset()
{
 sample_num=0;
 keyon=0;

 trig_time_mode=0;
 trig_time_duration=0;
 trig_time_duration_sample=0;
}

int SIDSynthMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0)                   return 0;
      if (val>=PICO_SIDSYNTH_SIZE) return PICO_SIDSYNTH_SIZE-1;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0)                   return 0;
      if (val>=PICO_SIDSYNTH_SIZE) return PICO_SIDSYNTH_SIZE-1;
      return val;
      break;


    case FILTER1_TYPE:
      if (val<0)                  return 0;
      if (val>=PBSYNTH_FILTER_TYPE_SIZE) return PBSYNTH_FILTER_TYPE_SIZE-1;
      return val;
      break;


    case FILTER1_ALGO:
      if (val<0)                  return 0;
      if (val>=PBSYNTH_FILTER_ALGO_SIZE) return PBSYNTH_FILTER_ALGO_SIZE-1;
      return val;
      break;

    // SID hard sync / ring mod are single-bit toggles: clamp 0/1 so the
    // knob behaves as on/off instead of an unbounded 0-127 value.
    case OSC1_PHASE:
      return (val>0) ? 1 : 0;
      break;
    case OSC2_PHASE:
      return (val>0) ? 1 : 0;
      break;


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: SIDSynthMachine::checkI(%d,%d)",what,val);
      return val;
      break;      
    }
}


int SIDSynthMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
  return 0;
}

void SIDSynthMachine::setF(int what,float val)
{
  float f_val=val;
  f_val=f_val/128;

  // Software LFO rate (0..1 -> 0..20Hz). LFO1 bends osc1, LFO2 bends osc2.
  if (what==LFO1_FREQ)  lfo1_freq=f_val;
  if (what==LFO2_FREQ)  lfo2_freq=f_val;
}



void SIDSynthMachine::setI(int what,int val)
{
  float        f_val_cutoff;
  float        f_val_resonance;
  float        f_val;
  int          noteShift=4;
  int          low1=0;
  int          high1=0;
  int          low2=0;
  int          high2=0;
  int          tmp;
  
  f_val=val;
  f_val=f_val/128.0;
  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }


    if (what==NOTE_ON && val==1) 
    { 
      keyon=1;

      //sid->write(0x04,0x40);    // CONTROL
      sid->write(CONTROL_REGISTER_VOICE_1,osc1_type*16);    // CONTROL
      sid->write(CONTROL_REGISTER_VOICE_2,osc2_type*16
		 + (osc2_sync?0x02:0)    // hard sync to osc1
		 + (osc2_ring?0x04:0));  // ring modulation by osc1

      // Retrigger from silence: zero the envelope counters so the gate-on
      // below attacks from 0 even if the previous note's envelope was still
      // in release/sustain.
      sid->reset_envelopes();

      sid->write(PULSE_WAVE_DUTY_CYCLE_VOICE_1_HIGH_BYTE,(255-osc1_mod*2));    // PWM voice 1
      sid->write(PULSE_WAVE_DUTY_CYCLE_VOICE_2_HIGH_BYTE,(255-osc2_mod*2));    // PWM voice 2

      //sid->write(0x12,0x40);    // CONTROL

      //sid->write(0x04,0x20);    // CONTROL
      //sid->write(0x12,0x40);    // CONTROL

      //sid->write(0x18, 0x08);  // MODE/VOL
      //sid->write(0x18,0x1F);  // MODE/VOL
  
      //sid->write(0x00,0x0);    // v1 freq lo
      //sid->write(0x01,0x4);    // v1 freq hi voice 1
      /*
      int tmp=sid_note_frqs[note];
      sid->write(FREQUENCY_VOICE_1_HIGH_BYTE,tmp/12);    // v1 freq hi voice 1
      sid->write(FREQUENCY_VOICE_2_HIGH_BYTE,tmp/12);    // v1 freq hi voice 2
      */
      // Oscillator 1 and 2 pitch: MIDI note + per-oscillator scale
      // (OSC1_SCALE / OSC2_SCALE, in semitones), looked up in the
      // freq_low/freq_high tables. The table is indexed by
      // note+noteShift+scale; the index is clamped so scale never pushes
      // the lookup out of range (which used to zero the frequency and
      // silence/crack the voice).
      {
	int idx1 = note + noteShift + osc1_scale;
	int idx2 = note + noteShift + osc2_scale;
	if (idx1 < 0)            idx1 = 0;
	if (idx1 > FREQ_TABLE_SIZE-1) idx1 = FREQ_TABLE_SIZE-1;
	if (idx2 < 0)            idx2 = 0;
	if (idx2 > FREQ_TABLE_SIZE-1) idx2 = FREQ_TABLE_SIZE-1;
	low1=freq_low[idx1];
	high1=freq_high[idx1];
	low2=freq_low[idx2];
	high2=freq_high[idx2];

	// Save the unmodulated FREQ registers so the LFO pitch bend can
	// shift them in tick(); restart the LFO phase at the note center.
	base_reg1=(high1<<8)|low1;
	base_reg2=(high2<<8)|low2;
	lfo1_phase=0.0;
	lfo2_phase=0.0;

	sid->write(FREQUENCY_VOICE_1_HIGH_BYTE,high1);    // v1 freq hi voice 1
	sid->write(FREQUENCY_VOICE_2_HIGH_BYTE,high2);    // v1 freq hi voice 2
	sid->write(FREQUENCY_VOICE_1_LOW_BYTE,low1);      // v1 freq lo voice 1
	sid->write(FREQUENCY_VOICE_2_LOW_BYTE,low2);      // v1 freq lo voice 2
      }

      //printf("*********************************************************************************** %d\n",tmp);
      //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %d\n",note);


      //sid->write(0x0f,0x4);    // v1 freq hi voice 2
      //sid->write(0x01,note);    // v1 freq hi
      
      //sid->write(0x05,0x0F);    // ATK/DCY
      //sid->write(0x06,0xF6);    // STN/RLS
      

      
      
      //sid->write(0x18, 0x08);  // MODE/VOL
      //sid->write(0x17,0xF7);    // filter
      
      
      //sid->write(0x0e,0x1);    // v1 freq lo

      //sid->write(0x0f,note);
      //sid->write(0x13,0x0F);    // ATK/DCY
      //sid->write(0x14,0xF6);    // STN/RLS
      //sid->write(0x13,0x00);    // ATK/DCY
      //sid->write(0x14,0x00);    // STN/RL



      // Oscillator 1 (voice 1) envelope: ADSR_ENV0
      sid->write(AD_VOICE1,((attack/8)*16)+(decay/8));     // ATK/DCY voice 1
      sid->write(SR_VOICE1,((sustain/8)*16)+release/8);    // STN/RLS voice 1
      // Oscillator 2 (voice 2) envelope: ADSR_ENV1
      sid->write(AD_VOICE2,((attack2/8)*16)+(decay2/8));   // ATK/DCY voice 2
      sid->write(SR_VOICE2,((sustain2/8)*16)+release2/8);  // STN/RLS voice 2


      //sid->write(0x13,((attack/8)*16)+(decay/8));     // ATK/DCY voice 2
      //sid->write(0x14,((sustain/8)*16)+release/8);    // STN/RLS voice 2



      //sid->write(0x10,0x3F);    // PULSEWIDTH
      sid->write(CONTROL_REGISTER_VOICE_1,osc1_type*16+1);    // CONTROL voice 1 + GateOn
      sid->write(CONTROL_REGISTER_VOICE_2,osc2_type*16+1
		 + (osc2_sync?0x02:0)    // hard sync to osc1
		 + (osc2_ring?0x04:0));  // ring modulation by osc1
      //sid->write(0x12,0x41);    // CONTROL voice 2
      
      //sid->write(0x16,j--);    // Cutoff
      //sid->write(0x17,0xFF);    // filter
      //sid->write(0x17,0x07);    // filter
      sid->write(RES_ROUTE,((resonance/8)*16)+7);    // filter
      //sid->write(0x16,0xFF);    // Cutoff
      {
	// Per-octave cutoff curve (see sid_cutoff_to_fc above).
	int fc = sid_cutoff_to_fc(cutoff);
	sid->write(CUTOFF_LOW,  fc & 0x07);          // 3 low bits
	sid->write(CUTOFF_HIGH, (fc >> 3) & 0xFF);   // 8 high bits
      }
      //sid->write(0x16,cutoff);    // Cutoff

      
    }

    if (what==NOTE_ON && val==0) 
    { 
      keyon=0;
      sid->write(CONTROL_REGISTER_VOICE_1,osc1_type*16);    // CONTROL
      sid->write(CONTROL_REGISTER_VOICE_2,osc2_type*16);    // CONTROL
      //sid->write(0x12,0x40);          // CONTROL

    }

    if (what==OSC1_TYPE)           
      { 
	if (val==PICO_SIDSYNTH_SQUARE)  osc1_type=4;
	if (val==PICO_SIDSYNTH_TRIANGE) osc1_type=1;
	if (val==PICO_SIDSYNTH_SAW)     osc1_type=2;
	if (val==PICO_SIDSYNTH_NOISE)   osc1_type=8;
      }
    if (what==OSC2_TYPE)           
      { 
	if (val==PICO_SIDSYNTH_SQUARE)  osc2_type=4;
	if (val==PICO_SIDSYNTH_TRIANGE) osc2_type=1;
	if (val==PICO_SIDSYNTH_SAW)     osc2_type=2;
	if (val==PICO_SIDSYNTH_NOISE)   osc2_type=8;
      }

    if (what==OSC1_DETUNE)           
      {
	osc1_detune=val;
      }


    if (what==OSC2_DETUNE)           
      {
	osc2_detune=val;
      }


    if (what==OSC1_SCALE)    osc1_scale=val;
    if (what==OSC2_SCALE)    osc2_scale=val;

    if (what==OSC1_MOD)      osc1_mod=val;
    if (what==OSC2_MOD)      osc2_mod=val;

    // Software LFO depth for pitch bend/vibrato.
    if (what==LFO1_DEPTH)    lfo1_depth=val;
    if (what==LFO2_DEPTH)    lfo2_depth=val;

    // SID hard sync (OSC1_PHASE) and ring mod (OSC2_PHASE) for osc2.
    if (what==OSC1_PHASE)    osc2_sync=(val>0)?1:0;
    if (what==OSC2_PHASE)    osc2_ring=(val>0)?1:0;


    if (what==ADSR_ENV0_ATTACK)    attack=val;
    if (what==ADSR_ENV0_DECAY)     decay=val;
    if (what==ADSR_ENV0_SUSTAIN)   sustain=val;
    if (what==ADSR_ENV0_RELEASE)   release=val;

    // Second SID envelope generator -> oscillator 2 (voice 2).
    if (what==ADSR_ENV1_ATTACK)    attack2=val;
    if (what==ADSR_ENV1_DECAY)     decay2=val;
    if (what==ADSR_ENV1_SUSTAIN)   sustain2=val;
    if (what==ADSR_ENV1_RELEASE)   release2=val;

    if (what==NOTE1)                note=val;


    if (what==FILTER1_CUTOFF)            cutoff=val;
    if (what==FILTER1_RESONANCE)         resonance=val;
}

Sint32 SIDSynthMachine::tick()
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
      delta_t=SID_CLOCKFREQ / (SID_SAMPLERATE / SAM);
      //nbsample=mysid.clock(delta_t,out_buffer_i,SAM);

      // Software LFO -> pitch bend/vibrato. Every buffer (SAM samples) the
      // frequency registers are rewritten pitch-shifted from base_reg1/2.
      // Depth 0..127 -> 0..24 semitones, rate 0..1 -> 0..20Hz.
      {
	const double two_pi  = 6.2831853071795865;
	const double samples_per_buf = (double)SAM;
	double phase_inc1 = lfo1_freq * two_pi * samples_per_buf / 44100.0;
	double phase_inc2 = lfo2_freq * two_pi * samples_per_buf / 44100.0;
	lfo1_phase += phase_inc1;
	lfo2_phase += phase_inc2;
	if (lfo1_phase > two_pi) lfo1_phase -= two_pi;
	if (lfo2_phase > two_pi) lfo2_phase -= two_pi;

	double semis1 = sin(lfo1_phase) * (lfo1_depth/127.0) * 24.0;
	double semis2 = sin(lfo2_phase) * (lfo2_depth/127.0) * 24.0;
	int r1 = (int)(base_reg1 * pow(2.0, semis1/12.0));
	int r2 = (int)(base_reg2 * pow(2.0, semis2/12.0));
	if (r1 < 0) r1 = 0;
	if (r1 > 65535) r1 = 65535;
	if (r2 < 0) r2 = 0;
	if (r2 > 65535) r2 = 65535;
	sid->write(FREQUENCY_VOICE_1_HIGH_BYTE,(r1>>8)&0xFF);
	sid->write(FREQUENCY_VOICE_1_LOW_BYTE, r1&0xFF);
	sid->write(FREQUENCY_VOICE_2_HIGH_BYTE,(r2>>8)&0xFF);
	sid->write(FREQUENCY_VOICE_2_LOW_BYTE, r2&0xFF);
      }

      sid->clock(delta_t,buffer_i,SAM);      
      //SE->process(buffer_f,SAM);
      //for(i=0;i<SAM;i++)
      // 	{
       	  //buffer[i]=buffer[i]*2048;
	  //buffer_i[i]=buffer_f[i]*1536;
      //buffer_i[i]=buffer_f[i]*1280;
	  // 	}
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


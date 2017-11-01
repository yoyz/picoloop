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


SIDSynthMachine::SIDSynthMachine()
{
  DPRINTF("SIDSynthMachine::SIDSynthMachine()\n");  
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


SIDSynthMachine::~SIDSynthMachine()
{
  DPRINTF("SIDSynthMachine::~SIDSynthMachine()\n");  
  if (buffer_f)
    free(buffer_f);
  if (buffer_i)
    free(buffer_i);

}


void SIDSynthMachine::init()
{
  DPRINTF("SIDSynthMachine::init()\n");  
  int i;

  //HO(44100);
  if (buffer_f==0)
    {
      sid=new SIDCHIP();
      //SE=new SynthEngine(SAM,100);
      buffer_f = (float*)malloc(sizeof(float)*SAM);
    }
  if (buffer_i==0)
    {
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM);
    }

  DPRINTF("buffer_f:0x%08.8X\n",buffer_f);
  DPRINTF("buffer_i:0x%08.8X\n",buffer_i);

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


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: SIDSynthMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int SIDSynthMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
}

void SIDSynthMachine::setF(int what,float val)
{
  float f_val=val;
  f_val=f_val/128;

  //if (what==OSC1_FREQ)           { freq=val; }
  //  if (what==LFO1_FREQ)           { lfo_speed=val/4.0; sineLfoOsc1.setFreq(lfo_speed); }

  /*
  if (what==LFO1_FREQ)           SE->getLFO(0)->setRate(f_val);
  if (what==LFO2_FREQ)           SE->getLFO(1)->setRate(f_val);
  */
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
  int          low3=0;
  int          high3=0;

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
      sid->write(CONTROL_REGISTER_VOICE_2,osc2_type*16);    // CONTROL

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
      if (note>=0-noteShift && note < FREQ_TABLE_SIZE-noteShift)
	{
	  if (note+noteShift+osc1_scale<FREQ_TABLE_SIZE)
	    {
	      low1=freq_low[note+noteShift+osc1_scale];
	      high1=freq_high[note+noteShift+osc1_scale];
	    }
	  if (note+noteShift+osc2_scale<FREQ_TABLE_SIZE)
	    {
	      low2=freq_low[note+noteShift+osc2_scale];
	      high2=freq_high[note+noteShift+osc2_scale];
	    }

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



      sid->write(AD_VOICE1,((attack/8)*16)+(decay/8));     // ATK/DCY voice 1
      sid->write(SR_VOICE1,((sustain/8)*16)+release/8);    // STN/RLS voice 1

      sid->write(AD_VOICE1,((attack/8)*16)+(decay/8));     // ATK/DCY voice 1
      sid->write(SR_VOICE2,((sustain/8)*16)+release/8);    // STN/RLS voice 1


      //sid->write(0x13,((attack/8)*16)+(decay/8));     // ATK/DCY voice 2
      //sid->write(0x14,((sustain/8)*16)+release/8);    // STN/RLS voice 2



      //sid->write(0x10,0x3F);    // PULSEWIDTH
      sid->write(CONTROL_REGISTER_VOICE_1,osc1_type*16+1);    // CONTROL voice 1 + GateOn
      sid->write(CONTROL_REGISTER_VOICE_2,osc2_type*16+1);    // CONTROL voice 2 + GateOn
      //sid->write(0x12,0x41);    // CONTROL voice 2
      
      //sid->write(0x16,j--);    // Cutoff
      //sid->write(0x17,0xFF);    // filter
      //sid->write(0x17,0x07);    // filter
      sid->write(RES_ROUTE,((resonance/8)*16)+7);    // filter
      //sid->write(0x16,0xFF);    // Cutoff
      sid->write(CUTOFF_HIGH,cutoff);    // Cutoff
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


    if (what==ADSR_ENV0_ATTACK)    attack=val;
    if (what==ADSR_ENV0_DECAY)     decay=val;
    if (what==ADSR_ENV0_SUSTAIN)   sustain=val;
    if (what==ADSR_ENV0_RELEASE)   release=val;

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


#include "SIDSynthMachine.h"
#include "math.h"
#include <stdlib.h>

//#define SAM 512
#define SAM 64
#define SID_SAMPLERATE 44118
#define SID_CLOCKFREQ (22.5 * SID_SAMPLERATE) //nearest int to 985248


//#      sid->write(0x18,0x1F);  // MODE/VOL
//#for(i=0; i<128; i++) note_frqs[i]=440.0*pow(2,((double)i-69.0)/12.0);

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
      sid=new SID();
      //SE=new SynthEngine(SAM,100);
      buffer_f = (float*)malloc(sizeof(float)*SAM);
    }
  if (buffer_i==0)
    {
      buffer_i = (Sint16*)malloc(sizeof(Sint16)*SAM);
    }

  DPRINTF("buffer_f:0x%08.8X\n",buffer_f);
  DPRINTF("buffer_i:0x%08.8X\n",buffer_i);

  sid_note_frqs=malloc(sizeof(double)*128);
  for(i=0; i<128; i++) 
    {
      sid_note_frqs[i]=440.0*pow(2,((double)i-69.0)/12.0);
      printf("i:%d %f\n",i,sid_note_frqs[i]);
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

  sid->write(0x04,0x20);    // CONTROL
  sid->write(0x12,0x40);    // CONTROL
  sid->write(0x18,0x1F);  // MODE/VOL
  
  /*
  SE->init();
  SE->setSIDSynthFilter24dB(1);
  SE->process(buffer_f,SAM);
  SE->process(buffer_f,SAM);
  SE->process(buffer_f,SAM);
  SE->process(buffer_f,SAM);
  */
  //SE.getSIDSynthOscillator(0)->setWave(0);
  //SE.getSIDSynthOscillator(1)->setWave(0);
  //SE.reset();
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
  int          noteShift=2;
  f_val=val;
  f_val=f_val/128.0;
  if (what==TRIG_TIME_MODE)       trig_time_mode=val;
  if (what==TRIG_TIME_DURATION) { trig_time_duration=val; trig_time_duration_sample=val*512; }


    if (what==NOTE_ON && val==1) 
    { 
      keyon=1;

      //sid->write(0x04,0x40);    // CONTROL
      sid->write(0x04,osc1_type*16);    // CONTROL
      //sid->write(0x12,0x40);    // CONTROL

      //sid->write(0x04,0x20);    // CONTROL
      //sid->write(0x12,0x40);    // CONTROL

      //sid->write(0x18, 0x08);  // MODE/VOL
      //sid->write(0x18,0x1F);  // MODE/VOL
  
      //sid->write(0x00,0x0);    // v1 freq lo
      //sid->write(0x01,0x4);    // v1 freq hi voice 1
      int tmp=sid_note_frqs[note];
      sid->write(0x01,tmp/12);    // v1 freq hi voice 1
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



      sid->write(0x05,((attack/8)*16)+(decay/8));     // ATK/DCY voice 1
      sid->write(0x06,((sustain/8)*16)+release/8);    // STN/RLS voice 1
      //sid->write(0x13,((attack/8)*16)+(decay/8));     // ATK/DCY voice 2
      //sid->write(0x14,((sustain/8)*16)+release/8);    // STN/RLS voice 2



      //sid->write(0x10,0x3F);    // PULSEWIDTH
      sid->write(0x04,osc1_type*16+1);    // CONTROL voice 1
      //sid->write(0x12,0x41);    // CONTROL voice 2
      
      //sid->write(0x16,j--);    // Cutoff
      //sid->write(0x17,0xFF);    // filter
      //sid->write(0x17,0x07);    // filter
      sid->write(0x17,((resonance/8)*16)+7);    // filter
      //sid->write(0x16,0xFF);    // Cutoff
      sid->write(0x16,cutoff);    // Cutoff
      //sid->write(0x16,cutoff);    // Cutoff

      
    }

    if (what==NOTE_ON && val==0) 
    { 
      //SE->releaseNote();
      keyon=0;
      sid->write(0x04,osc1_type*16);    // CONTROL
      //sid->write(0x12,0x40);          // CONTROL

    }

    if (what==OSC1_TYPE)           
      { 
	osc1_type=val;
	if (val==PICO_SIDSYNTH_SQUARE)  osc1_type=4;
	if (val==PICO_SIDSYNTH_TRIANGE) osc1_type=1;
	if (val==PICO_SIDSYNTH_SAW)     osc1_type=2;
	if (val==PICO_SIDSYNTH_NOISE)   osc1_type=8;
      }

    if (what==OSC1_DETUNE)           
      {
	osc1_detune=val;
	//SE->getSIDSynthOscillator(0)->setDetune(this->checkI(OSC1_DETUNE,val)-192);
	//SE->getSIDSynthOscillator(0)->setDetune(this->checkI(OSC1_DETUNE,val)+64);
      }


    if (what==OSC2_DETUNE)           
      {
	osc2_detune=val;
	//SE->getSIDSynthOscillator(1)->setDetune(this->checkI(OSC2_DETUNE,val)-192);
	//SE->getSIDSynthOscillator(1)->setDetune(this->checkI(OSC2_DETUNE,val)+64);
      }

    if (what==OSC2_TYPE)           
      {
	osc2_type=val;
	//SE.getSIDSynthOscillator(1)->setWave(val%2);
	//SE->getSIDSynthOscillator(1)->setWave(this->checkI(OSC2_TYPE,val));
      }


    if (what==OSC1_SCALE)    osc1_scale=val;
    if (what==OSC2_SCALE)    osc2_scale=val;


    if (what==ADSR_ENV0_ATTACK)    attack=val;
    if (what==ADSR_ENV0_DECAY)     decay=val;
    if (what==ADSR_ENV0_SUSTAIN)   sustain=val;
    if (what==ADSR_ENV0_RELEASE)   release=val;

    if (what==NOTE1)                note=val;


    if (what==FILTER1_CUTOFF)            cutoff=val;
    if (what==FILTER1_RESONANCE)         resonance=val;
}

Sint16 SIDSynthMachine::tick()
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


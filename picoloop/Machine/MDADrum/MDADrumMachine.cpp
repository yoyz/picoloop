#include "MDADrumMachine.h"

//#define SAM 512
#define SAM 64

//enum {
//  FMTYPE_2_OP_AM,
//  FMTYPE_2_OP_FM
//};


MDADrumMachine::MDADrumMachine() 
{
  DPRINTF("MDADrumMachine::MDADrumMachine()\n");  
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

  need_note_on=0;
  buffer_size=0;
  note_on=0;
}


MDADrumMachine::~MDADrumMachine()
{
  DPRINTF("MDADrumMachine::~MDADrumMachine()\n");  
}


void MDADrumMachine::init()
{
  DPRINTF("MDADrumMachine::init()\n");  
  int i;

  //HO(44100);
  // if (buffer==0)
  //   {
  //     //HO=new Hiopl(44100);
  //     HO=new Hiopl(4096);
  //     buffer = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
  //   }

  // DPRINTF("buffer:0x%08.8X\n",buffer);
  // for (i=0;i<SAM;i++)
  //   buffer[i]=0;


}

const char * MDADrumMachine::getMachineParamCharStar(int machineParam,int paramValue)
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


  static const char * str_drm_1       =   "patch/MDADrum/tr808/Kick.ds";
  static const char * str_drm_2       =   "patch/MDADrum/tr808/Snare.ds";
  static const char * str_drm_3       =   "patch/MDADrum/tr808/Rimshot.ds";
  static const char * str_drm_4       =   "patch/MDADrum/tr808/Ride.ds";

  static const char * str_drm_5       =   "patch/MDADrum/tr808/Hat_c.ds";
  static const char * str_drm_6       =   "patch/MDADrum/tr808/Hat_o.ds";
  static const char * str_drm_7       =   "patch/MDADrum/tr808/Cowbell.ds";
  static const char * str_drm_8       =   "patch/MDADrum/tr808/Clave.ds";


  const char * str_drm[PICO_DBOPL_SIZE];

  str_drm[0]                          = str_drm_1;
  str_drm[1]                          = str_drm_2;
  str_drm[2]                          = str_drm_3;
  str_drm[3]                          = str_drm_4;
  str_drm[4]                          = str_drm_5;
  str_drm[5]                          = str_drm_6;
  str_drm[6]                          = str_drm_7;
  str_drm[7]                          = str_drm_8;


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
      return str_osc[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];

    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    case FM_TYPE:
      return str_fm_type[this->checkI(FM_TYPE,paramValue)];

    case 255:
      return str_drm[paramValue];
    }
  return str_null;
}



void MDADrumMachine::reset()
{
 sample_num=0;
}

int MDADrumMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>7) return 7;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>7) return 7;
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
      DPRINTF("WARNING: Dboplmachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int MDADrumMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
}

void MDADrumMachine::setF(int what,float val)
{
  //if (what==OSC1_FREQ)           { freq=val; }
}



void MDADrumMachine::setI(int what,int val)
{
  float        f_val_cutoff;
  float        f_val_resonance;

  if (what==NOTE_ON && val==1) 
    { 
      need_note_on=1;
    }
  if (what==OSC1_TYPE)           { osc1_type=val; }
}


int MDADrumMachine::tick()
{
  if (need_note_on==1)
    {
      if (buffer!=0)
	{
	  free(buffer);
	  buffer=0;
	}
      //buffer_size=ds2mem("patch/MDADrum/tr808/Kick.ds",&buffer,1.0,1.0,1.0,1.0,0.0,1.0);
      buffer_size=ds2mem(this->getMachineParamCharStar(255,osc1_type),&buffer,1.0,1.0,1.0,1.0,0.0,1.0);
      DPRINTF("MDA buffer_size: %d buffer:0x%08.8X\n",buffer_size,buffer);
      index=0;
      need_note_on=0;
      note_on=1;
    }
  if (note_on)
    {
      if (index<buffer_size)
       	return buffer[++index]/4;
      else
       	note_on=0;
    }
  return 0;
}

// int MDADrumMachine::tick()
// {
//   Sint16 s_in;
//   Sint16 s_out;
//   int    modulated_freq;

//   if (index>=SAM | 
//       index<0)
//     index=0;


//   if (index==0 )
//     {
//       //modulated_freq=(sineLfoOsc1.tick()>>lfo_depth_shift);
//       modulated_freq=((sineLfoOsc1.tick()>>5)/(128-lfo_depth));
      
//       if (keyon)
// 	{
// 	  //HO->KeyOn(1,freq+modulated_freq);
// 	  HO->SetFrequency(1,freq+modulated_freq,true);
// 	  //HO->KeyOn(1,freq);
// 	}
//       else
// 	{
// 	  HO->SetFrequency(1,freq+modulated_freq,false);
// 	  //HO->SetFrequency(1,freq,false);
// 	}
//       HO->Generate(SAM,buffer);
//     }



//   if (trig_time_mode)
//     {
//       if (trig_time_duration_sample<sample_num)
// 	{
// 	  this->setI(NOTE_ON,0);
// 	  trig_time_mode=0;
// 	  //DPRINTF("\t\t\t\t\t\tDONE\n");
// 	}

//     }







//   s_in=buffer[index];
//   s_out=filter.process(s_in);

//   index++;
//   sample_num++;

//   return s_out;
// }


#include "LgptsamplerMachine.h"

#define SAM 256

LgptsamplerMachine::LgptsamplerMachine()
{
  float fi;
  int   i;

  DPRINTF("LgptsamplerMachine::LgptsamplerMachine()");  
  cutoff=125;
  resonance=10;
  note=0;
  detune=0;
  buffer=0;
  buffer16=0;
  index=0;

  channel=-1;
}



LgptsamplerMachine::~LgptsamplerMachine()
{
  DPRINTF("LgptsamplerMachine::~LgptsamplerMachine()");
  if (buffer)
    free(buffer);
  if (buffer16)
    free(buffer16);
  if (SI)
    delete(SI);
}



void LgptsamplerMachine::init()
{
  //return 0;
  //return;
  DPRINTF("LgptsamplerMachine::init() : 0x%08.8X",this);
  if (buffer==0)
    buffer=(lgptfixed*)malloc(sizeof(lgptfixed)*1024);
  if (buffer16==0)
    buffer16=(int16_t*)malloc(sizeof(int16_t)*1024);
  index=0; // use by this->tick()


  
  sample_num=0;

  note=0;
  detune=0;

  //DEBSystem DEB;
  //SampleInstrument SI;
  SamplePool * SP=SamplePool::GetInstance();
  SyncMaster * SM=SyncMaster::GetInstance();
  int argc;
  char ** argv;
  //DEB.Boot(argc,argv);

  //Path::SetAlias("samples","samples") ;
  //SP->Load();
  //SP->loadSample("samples/70_SUREG_B.wav");
  //SP->loadSample("samples/808-cowbell.wav");
  /*
  SP->loadSample("samples/SwaneeC4.wav");
  SP->loadSample("samples/Steel1C4.wav");
  SP->loadSample("samples/OohhC5.wav");
  SP->loadSample("samples/SymTomHi.wav");
  SP->loadSample("samples/Fishs.wav");
  SP->loadSample("samples/BrsSect4C3.wav");
  SP->loadSample("samples/DirtGtrC4.wav");
  SP->loadSample("samples/FlteC4.wav");
  SP->loadSample("samples/Snare.wav");
  SP->loadSample("samples/Orch5C4.wav");
  SP->loadSample("samples/BD1.wav");
  SP->loadSample("samples/WhistC3.wav");
  SP->loadSample("samples/PianoHiA4.wav");
  SP->loadSample("samples/Ssnre.wav");
  SP->loadSample("samples/MufBassC2.wav");
  SP->loadSample("samples/VoxC5.wav");
  SP->loadSample("samples/OpClHat.wav");
  SP->loadSample("samples/Pang.wav");
  SP->loadSample("samples/SaxzC4.wav");
  SP->loadSample("samples/VlnTrem2C4.wav");
  SP->loadSample("samples/MDWeeeeeC4.wav");
  SP->loadSample("samples/Eerr1C4.wav");
  SP->loadSample("samples/HHClosed.wav");
  */
  /*
  SP->loadSample(0,"samples/SwaneeC4.wav");
  SP->loadSample(1,"samples/Steel1C4.wav");
  SP->loadSample(2,"samples/OohhC5.wav");
  SP->loadSample(3,"samples/SymTomHi.wav");
  SP->loadSample(4,"samples/Fishs.wav");
  SP->loadSample(5,"samples/BrsSect4C3.wav");
  SP->loadSample(6,"samples/DirtGtrC4.wav");
  SP->loadSample(7,"samples/FlteC4.wav");
  SP->loadSample(8,"samples/Snare.wav");
  SP->loadSample(9,"samples/Orch5C4.wav");
  SP->loadSample(10,"samples/BD1.wav");
  SP->loadSample(11,"samples/WhistC3.wav");
  SP->loadSample(12,"samples/PianoHiA4.wav");
  SP->loadSample(13,"samples/Ssnre.wav");
  SP->loadSample(14,"samples/MufBassC2.wav");
  SP->loadSample(15,"samples/VoxC5.wav");
  SP->loadSample(16,"samples/OpClHat.wav");
  SP->loadSample(17,"samples/Pang.wav");
  SP->loadSample(18,"samples/SaxzC4.wav");
  SP->loadSample(19,"samples/VlnTrem2C4.wav");
  SP->loadSample(20,"samples/MDWeeeeeC4.wav");
  SP->loadSample(21,"samples/Eerr1C4.wav");
  SP->loadSample(22,"samples/HHClosed.wav");
  */
  SM->SetTempo(120);
  SI=new SampleInstrument();
  SI->Init();
  SI->AssignSample(0);
  //SI.Init();
  //SI.AssignSample(0);
  channel=-1;
  afterinit=1;  
}

void LgptsamplerMachine::setChannelNumber(int c)
{
  channel=c;
}

int LgptsamplerMachine::getChannelNumber()
{
  return channel;
}



int LgptsamplerMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>50) return 50;
      return val;
      break;

    case OSC2_TYPE:
      if (val<0) return 0;
      if (val>PICO_WAVETABLE_SIZE-2) return PICO_WAVETABLE_SIZE-1;
      return val;
      break;

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>=FILTER_TYPE_SIZE-1) return FILTER_TYPE_SIZE-1;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>=FILTER_ALGO_SIZE-1) return FILTER_ALGO_SIZE-1;
      return val;
      break;

    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: LgptsamplerMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int LgptsamplerMachine::getI(int what)
{

}

void LgptsamplerMachine::setF(int what,float val)
{
  //  if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  //if (what==LFO1_FREQ)           this->getPicodrumVCO().setLfoSpeed(val);
}


void LgptsamplerMachine::setI(int what,int val)
{
  float f_val_cutoff;
  float f_val_resonance;
  int noteShift=0;

  if (what==NOTE_ON && val==1) 
    {
      //SI.Start(0,72,true);
      /*
      SI.AssignSample(osc1_type%22);
      SI.Start(channel,note,true);
      
      SI.ProcessCommand(channel,SIP_FILTCUTOFF,0x0000+cutoff);
      SI.ProcessCommand(channel,MAKE_FOURCC('F','R','E','S'),0x0000+resonance);
      */
      SI->AssignSample(osc1_type%22);
      SI->Start(channel,note,true);
      
      SI->ProcessCommand(channel,SIP_FILTCUTOFF,0x0000+cutoff);
      SI->ProcessCommand(channel,MAKE_FOURCC('F','R','E','S'),0x0000+resonance);

      // SI.ProcessCommand(0,MAKE_FOURCC('L','S','T','A'),lfo1_freq);
      // SI.ProcessCommand(0,MAKE_FOURCC('L','L','E','N'),lfo1_depth);
      // SI.ProcessCommand(0,MAKE_FOURCC('L','M','O','D'),osc1_type%4);
      // SI.ProcessCommand(0,MAKE_FOURCC('S','T','R','T'),pb_freq);
      // SI.ProcessCommand(0,MAKE_FOURCC('E','N','D','_'),pb_depth);

      // this->getPicodrumVCO().setNoteDetune(note+noteShift,detune);
      // this->getADSRAmp().reset();
      // this->getPicodrumVCO().reset();
      // this->getPicodrumVCO().setPicodrumVCOPhase(phase);
      // this->getADSRAmp().setNoteOn(); 
    }
  if (what==NOTE_ON && val==0) 
    { 
      //this->getADSRAmp().setNoteOff(); 
    }

  if (what==NOTE1)           note=val;

  //if (what==OSC1_FREQ)           this->getPicodrumVCO().setSynthFreq(val);
  if (what==OSC1_TYPE)           osc1_type=val;
  if (what==OSC2_TYPE)           osc2_type=val;
  if (what==LFO1_FREQ)           lfo1_freq=val;
  if (what==LFO1_DEPTH)          lfo1_depth=val;
  if (what==PITCHBEND_DEPTH)     pb_depth=val;
  if (what==PITCHBEND_SPEED)     pb_freq=val;

   if (what==FILTER1_CUTOFF)      
     cutoff=val;

   if (what==FILTER1_RESONANCE)         
     resonance=val;
}



const char * LgptsamplerMachine::getMachineParamCharStar(int machineParam,int paramValue)
{
  static const char * txt_onetwoseven[]=
    {
      "0","1","2","3","4","5","6","7","8","9",
      "10","11","12","13","14","15","16","17","18","19",
      "20","21","22","23","24","25","26","27","28","29",
      "30","31","32","33","34","35","36","37","38","39",
      "40","41","42","43","44","45","46","47","48","49",
      "50","51","52","53","54","55","56","57","58","59",
      "60","61","62","63","64","65","66","67","68","69",
      "70","71","72","73","74","75","76","77","78","79",
      "80","81","82","83","84","85","86","87","88","89",
      "90","91","92","93","94","95","96","97","98","99",
      "100","101","102","103","104","105","106","107","108","109",
      "110","111","112","113","114","115","116","117","118","119",
      "120","121","122","123","124","125","126","127"
    };
      
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

  switch (machineParam)
    {
    case OSC1_TYPE:
      return txt_onetwoseven[this->checkI(OSC1_TYPE,paramValue)];
    case OSC2_TYPE:
      return str_osc[this->checkI(OSC2_TYPE,paramValue)];;
     
    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    }
  return str_null;
}




void LgptsamplerMachine::reset()
{
  sample_num=0;
  last_sample=0;
}


Sint32 LgptsamplerMachine::tick()
{
  //  return 0;
  float  f_in;
  float  f_out;
  Sint16 s_in;
  Sint16 s_out;
  int        i;

  if (index>=SAM | index<0)
    index=0;

  if (index==0)
    {
      //SI.Render(channel,buffer,SAM/2,true);
      SI->Render(channel,buffer,SAM/2,true);
      i=0;
      for (i=0;i<SAM;i++)
	buffer[i]=(fp2i(buffer[i])>>2);
      
      for (i=0;i<SAM;i++)
	buffer16[i]=buffer[i];
    }
  s_out=buffer16[index];
  
  index++;
  return s_out;
}
